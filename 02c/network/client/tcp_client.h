#pragma once
#include <netinet/in.h>

class tcp_client
{
private:
    std::string IP;
    uint16_t port;

    int socket_fd;//网络socket句柄
    bool socket_fd_flag=false;//socket是否存在
    
    struct sockaddr_in serv_addr;
    bool connect_flag=false;//连接是否存在
public:
    tcp_client(const char* IP,const uint16_t port);
    ~tcp_client();
    void tcp_socket();
    bool tcp_connect(const char* IP,const uint16_t port);
    int  tcp_send(const uint8_t* buf,uint16_t buf_length);
    int  tcp_recv(uint8_t* buf,uint16_t buf_length);
    void tcp_close();
public:
    int tcp_short_SR(const uint8_t* send_buf,uint16_t send_buf_length,uint8_t* recv_buf,uint16_t recv_buf_length);
private:
    

};

