#include<unistd.h>
#include<iostream>
#include"tcp_client.h"


int main(int argc,char** argv)
{
    
    std::string IP=argv[1];
    std::string port=argv[2];
    tcp_client tcp(IP.c_str(),std::stoul(port));
    uint8_t send_buf[10]={0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa};
    uint8_t buf[10];
    int ret=tcp.tcp_short_SR(send_buf,10,buf,3);
    std::cout<<"ret="<<ret<<std::endl;
    printf("%x %x %x %x\n",buf[0],buf[1],buf[2],buf[3]);
    
    ret=tcp.tcp_short_SR(send_buf,10,buf,3);
    std::cout<<"ret="<<ret<<std::endl;
    printf("%x %x %x %x\n",buf[0],buf[1],buf[2],buf[3]);

    return 0;
}

