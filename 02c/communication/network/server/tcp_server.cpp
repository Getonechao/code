#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string.h>
#include <algorithm>
#include <map>
#include "tcp_server.h"



tcp_server::tcp_server(const char* IP,uint16_t PORT,uint16_t conn_num):IP(IP),PORT(PORT),conn_num(conn_num)
{
}

tcp_server::~tcp_server()
{
}

int tcp_server::tcp_enable(){

    /* 1.服务器socket */
    servaddr.sin_family = PF_INET;
    servaddr.sin_port = htons(this->PORT);
    servaddr.sin_addr.s_addr = inet_addr(this->IP.c_str());
    this->serverfd = socket(PF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(this->serverfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt));

    /*2. bind*/
    int ret_bind = bind(this->serverfd , (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(-1==ret_bind){std::cout<<"[err]:bind failed\n"; return -1;}

    /*3.listen */
    int ret_listen = listen(this->serverfd , this->conn_num);
    if(-1==ret_listen){std::cout<<"[err]:listen failed\n"; return -1;}


    /*4. epoll_create*/
    this->epollfd = epoll_create(this->conn_num);

    /* 5. epoll_ctl */
    this->addfd(this->epollfd,this->serverfd,true);//边缘触发

    /* 6. client event 数组 */
    struct epoll_event client_event[this->conn_num];

    /* 7. while */
    while(1)
    {
        std::cout<<"\n*******************"<<std::endl;
        /* 8.epoll_wait */
        std::cout<<"while(1) begin.....\n";
        int count = epoll_wait(this->epollfd, client_event, this->conn_num, -1);//永久阻塞
        std::cout<<" the  number  of  file  descriptors  ready : "<<count<<std::endl;
    
        std::cout<<"\n    server fd:"<<this->serverfd<<std::endl;
        for (size_t i = 0; i < count; i++)
        {
            std::cout<<"            -------> connect client fd:"<<client_event[i].data.fd<<std::endl;
        }
        std::cout<<"\n"<<std::endl;

        //epoll_wait执行失败，错误处理
        if(count < 0)
        {
            std::cerr<<"epoll_wait fail "<<std::endl;
            switch(errno)
            {
                case EBADF:
                    {
                        std::cout<<"epoll_wait err: epfd is not a valid file descriptor."<<std::endl;
                    }break;
                case EFAULT:
                    {
                        std::cout<<"epoll_wait err: The memory area pointed to by events is not accessible with write permissions"<<std::endl;
                    }break;
                case EINTR:
                    {
                        std::cout<<"epoll_wait err: The call was interrupted by a signal handler before either (1) any of the requested events occurred or (2) the timeout expired;"<<std::endl;
                    }break;
                case EINVAL:
                    {
                        std::cout<<"epoll_wait err: epfd is not an epoll file descriptor, or maxevents is less than or equal to zero."<<std::endl;
                    }break;
            }
            break;
        }

        this->ET_deal(client_event,count,this->epollfd,this->serverfd);
    }
    return 0;
}

int tcp_server::addfd(int epollfd, int fd,bool enable_et)
{
    struct epoll_event event;
	event.data.fd = fd;
	event.events  = EPOLLIN|EPOLLRDHUP|EPOLLHUP|EPOLLERR|EPOLLOUT;//(Level Triggered)水平触发
	if(enable_et){
		event.events |= EPOLLET;//(Edge Triggered)边缘触发
	}
    int ret_epoll_ctl = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD)| O_NONBLOCK);//非阻塞

    return 0;
}

int tcp_server::modfd(int epollfd, int fd,bool enable_et)
{
    struct epoll_event event;
	event.data.fd = fd;
	event.events  = EPOLLIN;//(Level Triggered)水平触发
	if(enable_et){
		event.events |= EPOLLET;//(Edge Triggered)边缘触发
	}
    int ret_epoll_ctl = epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD)| O_NONBLOCK);//非阻塞

    return 0;
}

int tcp_server::ET_deal(struct epoll_event *events, int count, int epollfd, int serverfd)
{
    std::cout<<"有如下事件发生："<<std::hex<<events->events<<std::endl;
    /* 8.事件触发 */
    for (size_t i = 0; i < count; i++)
    {
        int socket_temp=events[i].data.fd;
        
        /* 8.1 连接事件 */
        if(socket_temp==this->serverfd)
        {
            std::cout<<"connect.........\n";
            socklen_t client_addrLength= sizeof(struct sockaddr_in);
            int clientfd = accept( serverfd, ( struct sockaddr* )&cliaddr, &client_addrLength);

            //将clientfd添加到epoll_wait
            this->addfd(epollfd,clientfd,true);

            //存到IP_POOL
            struct IP_PORT_t IP_PORT{
                .IP= inet_ntoa(cliaddr.sin_addr),
                .port=ntohs(cliaddr.sin_port)
            } ;
            this->IP_POOl[clientfd]=IP_PORT;

        }
        /* 8.2  0x01  EPOLLIN 处理客户端发来的消息  EPOLLRDNORM + EPOLLRDBAND
                      触发条件：
                        1.有数据来临，读事件
                        2.客户端正常close()关闭
                        3.新建建立连接时
         */
        else if (events[i].events & EPOLLIN)
        {   
            std::cout<<"EPOLLIN"<<std::endl;
            //recv buf
            uint8_t recvbuf_temp[1024];
            std::vector<uint8_t> recvbuf;
            // ip:port
            // std::string conn_ip= inet_ntoa(cliaddr.sin_addr);
            // uint16_t conn_port=ntohs(cliaddr.sin_port);
            // std::cout<<"fd "<<events[i].data.fd<<std::endl;
            // std::cout<<"u64 "<<events[i].data.u64<<std::endl;
            // std::cout<<"u32 "<<events[i].data.u32<<std::endl;
            // std::cout<<*events[i].data.ptr<<std::endl;
            /* 边缘触发，循环读取缓冲区中内容，直到errno == EAGAIN */
            while (true)
            {
                bool done=false;//关闭fd
                bool over=false;//接收成功

                memset(recvbuf_temp, 0, 1024);

                /*
                *返回值:
                *       1.成功执行时，返回接收到的字节数。
                *       2.另一端已关闭则返回0。
                *       3.失败返回-1，errno被设为以下的某个值 ：
                *                                        EAGAIN：套接字已标记为非阻塞，而接收操作被阻塞或者接收超时 
                *                                        EBADF：sock不是有效的描述词 
                *                                        ECONNREFUSE：远程主机阻绝网络连接 
                *                                        EFAULT：内存空间访问出错 
                *                                        EINTR：操作被信号中断 
                *                                        EINVAL：参数无效 
                *                                        ENOMEM：内存不足 
                *                                        ENOTCONN：与面向连接关联的套接字尚未被连接上 
                *                                        ENOTSOCK：sock索引的不是套接字 当返回值是0时，为正常关闭连接；
                */
                int ret_number=recv(socket_temp,recvbuf_temp,1024,0);


                //1. =0 连接关闭
                if(ret_number==0)
                {

                    done=true;
                }

                //2. <0 出错 查看errno 对非阻塞socket而言，EAGAIN不是一种错误。在VxWorks和Windows上，EAGAIN的名字叫做EWOULDBLOCK。
                if(ret_number<0){
                    //如果数据读取完毕，没有数据可读，返回一个EAGIN错误
                    switch (errno)
                    {

                        //11	/* Try again */ 在一些较老的unxi系统，也会使用EWOULDBLOCK   
                    case EAGAIN:
                        {
                            over=true;//recv接受完成
                        }
                        break;
                        //  4	/* Interrupted system call */
                    case EINTR:
                        {
                            done=true;//系统调用中断错误
                            std::cout<<"recv err:"<<strerror(errno)<<std::endl;
                        }
                    default:
                        {
                            
                            std::cout<<"recv err:"<<strerror(errno)<<std::endl;
                        }
                        break;
                    }

                }

                //3. >0 接收数据
                if(ret_number>0)
                { 
                    /* 汇总 */
                    for (size_t i = 0; i < ret_number; i++)
                    {
                        recvbuf.push_back(recvbuf_temp[i]);
                    }
                    std::cout<<"ret_number:"<<ret_number<<std::endl;
                }


                //4. 处理.....
                if(over)
                {
                    std::vector<uint8_t> sendbuf;
                    bool send_yes=false;
                    //根据fd查询已经连接的IP：port
                    auto it=this->IP_POOl.find(socket_temp);
                    const char* ip_cur;
                    uint16_t port_cur;
                    if(it!=this->IP_POOl.end())
                    {
                       ip_cur=it->second.IP;
                       port_cur=it->second.port;

                    }else{
                        ip_cur="127.0.0.1";
                        port_cur=0;
                    }
                                          
                    /********************************* deal ************************************/
                        this->data_deal(recvbuf,sendbuf,done,send_yes,ip_cur,port_cur);
                    /***************************************************************************/


                    if(send_yes)
                    {
                        int sendbuf_size=sendbuf.size();
                        //如果有要发送的数据
                        if(sendbuf_size>0)
                        {
                            //new
                            uint8_t* sendbuf_temp=new uint8_t[sendbuf_size];
                            //copy
                            std::copy_n(sendbuf.begin(),sendbuf_size,sendbuf_temp);
                            //send
                            int ret_send=send(socket_temp,sendbuf_temp,sendbuf_size,0);
                            //delete
                            delete [] sendbuf_temp;
                            //break
                            break;
                        }else{break;}
                        
                    }else{ break;}
                }
                /* client connect 关闭fd */
                if(done){
                    
                    close(socket_temp); 
                    std::cout<<"close fd="<<socket_temp<<"\n"<<std::endl;
                    //删除IP_POOL中键fd：socket_temp
                    auto it = IP_POOl.find(socket_temp);
                    if(it!=IP_POOl.end())
                    {
                        std::cout<<"erase IP_POOL\n"<<std::endl;
                        this->IP_POOl.erase(it);
                    }
                    break;
                }

            }
        }
      
        /* 8.2  0x002 EPOLLPRI 带外数据 高优先级数据可读 即TCP协议报文中的URG  eg：ctrl + c 一般不用
                    触发条件：
                        1.带外数据，当 read ()能够读取紧急数据的时候
        */
        if (events[i].events & EPOLLPRI)
        {

        }
      
        /* 8.2  0x004 EPOLLOUT 
                    触发条件：
                        1.ET模式下，连接建立后，回复数据时，触发一次
                        1.有数据发出，写事件
        */
        if (events[i].events & EPOLLOUT)
        {
        //     std::cout<<"EPOLLOUT thing"<<std::endl;
        //    char* buf="hello world";
        //     send(events[i].data.fd,buf,strlen(buf),0);
        }
      
        /* 8.2  0x008 EPOLLERR 服务器出错，触发
                    触发条件：
                        1.向已经断开的socket写或者读，会发生EPollErr。(即客户端异常断开，如宕机)，需要服务器主动做"超时检查";

         */
        if (events[i].events & EPOLLERR)
        {
            std::cout<<"EPOLLERR thing "<<std::endl;
        }
     
        /* 8.2  0x010 EPOLLHUP 表示读写都关闭 */
        if (events[i].events & EPOLLHUP)
        {
            std::cout<<"EPOLLHUP thing "<<std::endl;
        }
     
        /* 8.2  0x040 EPOLLRDNORM 普通数据可读
        
         */
        if (events[i].events & EPOLLRDNORM)
        {

        }
    
        /* 8.2  0x080 EPOLLRDBAND 优先级带数据可读
        */
        if (events[i].events & EPOLLRDBAND)
        {

        }
     
        /* 8.2  0x100 EPOLLWRNORM 普通数据可写 */
        if (events[i].events & EPOLLWRNORM)
        {

        }
    
        /* 8.2  0x200 EPOLLWRBAND 优先级带数据可写*/
        if (events[i].events & EPOLLWRBAND)
        {

        }
    
        /* 8.2  0x400 EPOLLMSG */
        if (events[i].events & EPOLLMSG)
        {

        }

        /* 8.2  0x2000 EPOLLRDHUP 对端断开连接  读关闭 注意与EPOLLHUP的区别
                    触发条件：
                        1.对端断开连接,
                        2.表示的是对方读挂起，可能对方还可以写数据，所以当触发该事件的时候可能网络中还有数据未处理，这在和客户端通信的时候丢掉几个包不处理是没有问题的.
        */
        if (events[i].events & EPOLLRDHUP)
        {
            std::cout<<"EPOLLRDHUP thing "<<std::endl;
        }
     
        /* 8.2 1u << 28 EPOLLEXCLUSIVE  (since Linux 4.5)避免发生惊群现象
        
         */
        if (events[i].events & EPOLLEXCLUSIVE)
        {

        }
   
        /* 8.2 1u << 29 EPOLLWAKEUP (since Linux 3.5)
        */
        if (events[i].events & EPOLLWAKEUP)
        {

        }
     
        /* 8.2 1u << 30 EPOLLONESHOT 只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里*/
        if (events[i].events & EPOLLONESHOT)
        {

        }

        // /* 8.2 1u << 31 EPOLLET 边缘触发 用来设置水平或边缘触发模式 */
        // if (events[i].events & EPOLLET)
        // {
        //     std::cout<<"EPOLLET"<<std::endl;
        // }
    }
    return 0;
}

 int tcp_server::data_deal(const std::vector<uint8_t>& recvbuf,std::vector<uint8_t>& sendbuf,bool& done,bool& send_yes ,const char* IP,const uint16_t& port)
 {
    
     for (auto &&i : recvbuf)
     {
         std::cout<<std::hex<<+i;
     }
     std::cout<<std::endl;
     sendbuf.push_back(245);
     
     /* 是否开启回复 */
    if(true)
    {
        send_yes=true;
    }else{
        send_yes=false;
    }
     return 0;
 }
