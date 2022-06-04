#include<iostream>
#include "tcp_server.h"

int main()
{
    tcp_server TCP("127.0.0.1",9001,1024);
    TCP.tcp_enable();
  
}