#include"tcp_client.h"

#include<iostream>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <string.h>
#include <unistd.h>

/* 构造函数 */
tcp_client::tcp_client(const char* IP,const uint16_t port):IP(IP),port(port)
{
    //1.获取socket句柄
    this->tcp_socket();
    //2.connect
    this->tcp_connect(IP,port);
}

/* 析构函数 */
tcp_client::~tcp_client()
{
    /* 释放资源 */
    if(this->socket_fd_flag)
    {
        close(this->socket_fd);   
    }
}
/* 1.socket */
void tcp_client::tcp_socket()
{
    this->socket_fd=socket(AF_INET,SOCK_STREAM,0);
    this->socket_fd_flag=true;
}

/* 2.connect */
bool tcp_client::tcp_connect(const char* IP,const uint16_t port)
{
    if(this->IP!=IP){this->IP=IP;}
    if(this->port!=port){this->port=port;};

    memset(&serv_addr, 0, sizeof(this->serv_addr));
    this->serv_addr.sin_family = AF_INET;  //使用IPv4地址
    this->serv_addr.sin_addr.s_addr = inet_addr(this->IP.c_str());  //具体的IP地址
    this->serv_addr.sin_port = htons(this->port);

    std::cout<<"[note]:wait connecting...\n";
    if(connect(this->socket_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==0)
    {
        std::cout<<"[note]:connect success\n";
        this->connect_flag=true;
        return true;
    }else{
        std::cout<<"[note]: connect failed!!!"<<std::endl;
        this->connect_flag=false;
        return false;
    }
}
/* 3.send */
int tcp_client::tcp_send(const uint8_t* buf,uint16_t buf_length)
{   
    if(this->connect_flag==true)
    {
        int ret=send(socket_fd,buf,buf_length,0);
        return ret;
    }else{
        std::cerr<<"[err]: now,send,no connecting......\n";
        return -2;
    }
  
}
/* 4. recv*/
int tcp_client::tcp_recv(uint8_t* buf,uint16_t buf_length){
    if(this->connect_flag==true)
    {
        int ret=recv(this->socket_fd, buf, buf_length,0);
        return ret;
    }else{
        std::cerr<<"[err]: now,recv,no connecting......\n";
        return -2;
    }
}
/*5.close*/
void tcp_client::tcp_close()
{
    close(this->socket_fd);
    this->connect_flag=false;
    this->socket_fd_flag=false;
    std::cout<<"[note]:connect close\n";
}



/* 短连接 发送与接收(请求与回复)*/
int  tcp_client::tcp_short_SR(const uint8_t* send_buf,uint16_t send_buf_length,uint8_t* recv_buf,uint16_t recv_buf_length)
{
    //1.判断fd是否还存在
    if(!this->socket_fd_flag)
    {
        this->socket_fd=socket(AF_INET,SOCK_STREAM,0);
        this->socket_fd_flag=true;
    }

    //2.判断连接是否还存在
    if(!this->connect_flag)
    {
        if(this->tcp_connect(this->IP.c_str(),this->port)==false)
        {
            //连接失败
            return -1;
        }
    }

    //3.发送数据
    if(this->tcp_send(send_buf,send_buf_length)==-1){return -2;}

    //4.接收数据
    int ret=this->tcp_recv(recv_buf,recv_buf_length);
    if(ret==-1){return -3;}

    //5.关闭fd
    this->tcp_close();

    return ret;
}