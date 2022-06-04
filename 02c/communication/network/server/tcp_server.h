#pragma once
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <vector>
#include <map>

struct IP_PORT_t{
    char* IP;
    uint16_t port;
};

class tcp_server
{
private:
    std::string IP;
    uint16_t PORT;

    int epollfd,serverfd;
    
    //服务器允许最大连接数量
    uint16_t conn_num;

    struct sockaddr_in cliaddr, servaddr;
    
    //存储连接服务器的<fd -- IP：port>
    std::map<uint16_t,struct IP_PORT_t> IP_POOl; 

    struct epoll_event server_event;

    bool enable_flag=false;
    

    
public:
    tcp_server(const char* IP,uint16_t PORT,uint16_t conn_num);
    ~tcp_server();

    int tcp_enable();

    int data_deal(const std::vector<uint8_t>& recvbuf,std::vector<uint8_t>& sendbuf,bool& done,bool& send_yes ,const char* IP,const uint16_t& port);

private:
    int addfd(int epollfd, int fd,bool enable_et);//(Level Triggered)水平触发&&(Edge Triggered)边缘触发
    int modfd(int epollfd, int fd,bool enable_et);
    // int LT_deal(struct epoll_event *events, int count, int epollfd, int serverfd);
    int ET_deal(struct epoll_event *events, int count, int epollfd, int serverfd);
};


