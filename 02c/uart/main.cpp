#include"uart_raw.h"
#include<unistd.h>
#include<string.h>
#include<signal.h>
void sig_catch(int signum)
{
    std::cout<<"close fd\n";
    exit(0);
}

int main(){

    signal(SIGINT, sig_catch);     //捕捉SIGINT的信号函数
    uart uart("/dev/ttyS1",115200,8,'N',1);
    if(0==uart.uart_enable())
    {
        /*1. send测试 */
        // uint8_t buf[256];
        // for(int i=0;i<256;i++)
        // {
        //     buf[i]=i;
        // }
        // while(1)
        // {
        //     sleep(1);
        //     int ret=uart.uart_send(buf,256);
        // }

        /*2. read测试 */
        uint8_t buf[256];
        while(1)
        {
            sleep(1);
             memset(buf,0,256);
            int ret=uart.uart_recv(buf,256);
            if(ret>0){
                for(int i=0;i<256;i++)
                {
                    printf("%x ",buf[i]);
                }
                printf("\n");
            }
       
        }
       
    }
    return 0;
}