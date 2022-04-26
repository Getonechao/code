#include"uart_raw.h"
#include<unistd.h>

int main(){
    uart uart("/dev/ttyS1",115200,8,'E',1);
    if(0==uart.uart_enable())
    {
        uint8_t buf[3];
        buf[0]=0x98;
        buf[1]=0x01;
        buf[2]=0x08;
        
        while(1)
        {
            
            int ret=uart.uart_recv(buf,3);
            printf("this is %d %x %x %x\n",ret,buf[0],buf[1],buf[2]);
        }
    }
    return 0;
}