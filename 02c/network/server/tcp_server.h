#pragma once
#include<iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

class tcp_server
{
private:
    std::string IP;
    uint16_t PORT;

    int epollfd,serverfd,clientfd;

    uint16_t conn_num;

    struct sockaddr_in cliaddr, servaddr;

    struct epoll_event server_event, client_event[1024];

    bool enable_flag=false;

    
public:
    tcp_server(const char* IP,uint16_t PORT,uint16_t conn_num);
    ~tcp_server();

    int tcp_enable();
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
    server_event.events= EPOLLIN;
    server_event.data.fd=this->serverfd;
    int ret_epoll_ctl = epoll_ctl(epollfd, EPOLL_CTL_ADD, serverfd, &server_event);
    fcntl(serverfd, F_SETFL, fcntl(serverfd, F_GETFD, 0)| O_NONBLOCK);

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
                struct epoll_event conn_event; 
                conn_event.events= EPOLLIN;
                conn_event.data.fd=this->serverfd;
                int ret_epoll_ctl = epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &conn_event);
                fcntl(clientfd, F_SETFL, fcntl(clientfd, F_GETFD, 0)| O_NONBLOCK);

            }
            
        }
        



    }
}
