#pragma once
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string.h>
#include <vector>
#include <algorithm>

class tcp_server
{
private:
    std::string IP;
    uint16_t PORT;

    int epollfd,serverfd;

    uint16_t conn_num;

    struct sockaddr_in cliaddr, servaddr;

    struct epoll_event server_event;

    bool enable_flag=false;

    
public:
    tcp_server(const char* IP,uint16_t PORT,uint16_t conn_num);
    ~tcp_server();

    int tcp_enable();

    int data_deal(const std::vector<uint8_t>& recvbuf,std::vector<uint8_t>& sendbuf, bool& send_yes );

private:
    int addfd(int epollfd, int fd,bool enable_et);//(Level Triggered)水平触发&&(Edge Triggered)边缘触发
    int modfd(int epollfd, int fd,bool enable_et);
    // int LT_deal(struct epoll_event *events, int count, int epollfd, int serverfd);
    int ET_deal(struct epoll_event *events, int count, int epollfd, int serverfd);
};

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
        std::cout<<"1\n";
        int count = epoll_wait(this->epollfd, client_event, this->conn_num, -1);//永久阻塞
        std::cout<<"count="<<count<<" serverfd:"<<this->serverfd<<" client_event:"<<client_event[0].data.fd<<std::endl;

        if(count < 0)
        {
            std::cerr<<"epoll_wait fail\n";
            break;
        }

        this->ET_deal(client_event,count,this->epollfd,this->serverfd);
    }
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

            std::string conn_ip= inet_ntoa(cliaddr.sin_addr);
            uint16_t conn_port=ntohs(cliaddr.sin_port);
            std::cout<<conn_ip<<":"<<conn_port<<std::endl;
            //将clientfd添加到epoll_wait
            this->addfd(epollfd,clientfd,true);

        }
        /* 8.2 处理客户端发来的消息 */
        else if (events[i].events & EPOLLIN)
        {
            std::cout<<"message\n";
            uint8_t buf[1024];
            std::vector<uint8_t> buf_tatol;

            /* 边缘触发，循环读取缓冲区中内容，直到errno == EAGAIN */
            while (true)
            {
                bool done=false;//关闭fd
                bool over=false;//接收成功

                memset(buf, 0, 1024);

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
                int ret_number=recv(socket_temp,buf,1024,0);


                    //1. =0 连接关闭
                if(ret_number==0)
                {
                    done=true;
                }

                //2. <0 出错 对非阻塞socket而言，EAGAIN不是一种错误。在VxWorks和Windows上，EAGAIN的名字叫做EWOULDBLOCK。
                if(ret_number<0){
                    //如果读取不到数据，返回一个EAGIN错误
                    if((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                        done=true;
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
                        buf_tatol.push_back(buf[i]);
                    }
                    std::cout<<"ret_number:"<<ret_number<<std::endl;
                }



                if(over)
                {
                    std::vector<uint8_t> sendbuf;
                    bool send_yes=false;
                    //deal
                    this->data_deal(buf_tatol,sendbuf,send_yes);
                    if(send_yes)
                    {
                        int sendbuf_size=sendbuf.size();
                        if(sendbuf_size>0)
                        {
                            //new
                            uint8_t* sendbuf2=new uint8_t[sendbuf_size];
                            //copy
                            std::copy_n(sendbuf.begin(),sendbuf_size,sendbuf2);
                            //send
                            send(socket_temp,sendbuf2,sendbuf_size,0);
                            //delete
                            delete [] sendbuf2;
                        }
                        
                    }
                }
                /* 关闭fd */
                if(done){close(socket_temp);}
            }
        }
    }
}

 int tcp_server::data_deal(const std::vector<uint8_t>& recvbuf,std::vector<uint8_t>& sendbuf, bool& send_yes )
 {
     send_yes=true;
     sendbuf.push_back(254);
     sendbuf.push_back(45);
     return 0;
 }

