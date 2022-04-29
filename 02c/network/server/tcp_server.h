#pragma once
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

    struct epoll_event server_event, client_event[1024];

    bool enable_flag=false;

    
public:
    tcp_server(const char* IP,uint16_t PORT,uint16_t conn_num);
    ~tcp_server();

    int tcp_enable();

private:
    int addfd(int epollfd, int fd,bool enable_et);//(Level Triggered)水平触发&&(Edge Triggered)边缘触发
    int LT_deal(struct epoll_event *events, int count, int epollfd, int serverfd);
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
    int serverfd = socket(PF_INET, SOCK_STREAM, 0);

    /*2. bind*/
    int ret_bind = bind(serverfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    /*3.listen */
    int ret_listen = listen(serverfd, this->conn_num);

    /*4. epoll_create*/
    int ret_epoll_create = epoll_create(this->conn_num);

    /* 5. epoll_ctl */
    this->addfd(epollfd,serverfd,true);//边缘触发

    /* 6. while */
    while(1)
    {
        /* 7.epoll_wait */
        int count = epoll_wait(epollfd, client_event, conn_num, -1);//阻塞
        if(count < 0)
        {
            std::cerr<<"epoll_wait fail\n";
            break;
        }

        /* 8.事件触发 */
        for (size_t i = 0; i < count; i++)
        {
            int socket_temp=client_event[i].data.fd;
            
            /* 8.1 连接事件 */
            if(socket_temp==this->serverfd)
            {
                socklen_t client_addrLength= sizeof(struct sockaddr_in);
                int clientfd = accept( serverfd, ( struct sockaddr* )&cliaddr, &client_addrLength);

                std::string conn_ip= inet_ntoa(cliaddr.sin_addr);
                uint16_t conn_port=ntohs(cliaddr.sin_port);

                //将clientfd添加到epoll_wait
                this->addfd(epollfd,clientfd,true);

            }
            /* 8.2 处理客户端发来的消息 */
            else if ()
            {
                
            }
            
            
        }
        



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
    int ret_epoll_ctl = epoll_ctl(epollfd, EPOLL_CTL_ADD, serverfd, &server_event);
    fcntl(serverfd, F_SETFL, fcntl(serverfd, F_GETFD, 0)| O_NONBLOCK);

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
                socklen_t client_addrLength= sizeof(struct sockaddr_in);
                int clientfd = accept( serverfd, ( struct sockaddr* )&cliaddr, &client_addrLength);

                std::string conn_ip= inet_ntoa(cliaddr.sin_addr);
                uint16_t conn_port=ntohs(cliaddr.sin_port);

                //将clientfd添加到epoll_wait
                this->addfd(epollfd,clientfd,true);

            }
            /* 8.2 处理客户端发来的消息 */
            else if (events[i].events & EPOLLIN)
            {
                uint8_t buf[1024];
                std::vector<uint8_t> buf_tatol;

                /* 边缘触发，循环读取缓冲区中内容，直到errno == EAGAIN */
                while (true)
                {
                    memset(buf, 0, 1024);
                    int ret_number=recv(socket_temp,buf,1024,0);
                    if(ret_number>0)
                    { 
                        /* 汇总 */
                        std::copy_n(buf,ret_number,buf_tatol.end());
                    }else{
                        if((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                            printf("EAGAIN\n");
                            continue;
					    }

                        //deal
					    close(socket_temp);
					    break;
                    }
                }
                
                
            }
            
            
        }
}

