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
        /* 8.epoll_wait */
        std::cout<<"while(1) begin.....\n";
        int count = epoll_wait(this->epollfd, client_event, this->conn_num, -1);//永久阻塞
        std::cout<<"count="<<count<<" serverfd:"<<this->serverfd<<" client_event:"<<client_event[0].data.fd<<std::endl;

        if(count < 0)
        {
            std::cerr<<"epoll_wait fail\n";
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
	event.events  = EPOLLIN;//(Level Triggered)水平触发
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
    
    /* 8.事件触发 */
    for (size_t i = 0; i < count; i++)
    {
        int socket_temp=events[i].data.fd;
        
        /* 8.1 连接事件 */
        if(socket_temp==this->serverfd)
        {
            std::cout<<"有事件连接\n";
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
        /* 8.2 处理客户端发来的消息 */
        else if (events[i].events & EPOLLIN)
        {   
            //recv buf
            uint8_t recvbuf_temp[1024];
            std::vector<uint8_t> recvbuf;
            // ip:port
            // std::string conn_ip= inet_ntoa(cliaddr.sin_addr);
            // uint16_t conn_port=ntohs(cliaddr.sin_port);

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

                //2. <0 出错 对非阻塞socket而言，EAGAIN不是一种错误。在VxWorks和Windows上，EAGAIN的名字叫做EWOULDBLOCK。
                if(ret_number<0){
                    //如果数据读取完毕，没有数据可读，返回一个EAGIN错误
                    if((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                        over=true;
                    }else{
                        done=true;
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
                        this->data_deal(recvbuf,sendbuf,send_yes,ip_cur,port_cur);
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
    }
    return 0;
}

 int tcp_server::data_deal(const std::vector<uint8_t>& recvbuf,std::vector<uint8_t>& sendbuf, bool& send_yes ,const char* IP,const uint16_t& port)
 {
    
     for (auto &&i : recvbuf)
     {
         std::cout<<i<<std::endl;
     }
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
