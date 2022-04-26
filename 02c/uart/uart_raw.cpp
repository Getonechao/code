
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "uart_raw.h"
/*
* uart(dev,baudrate,databits,parity,stop_bits)
*/
uart::uart(const char* dev,uint32_t baudrate,uint8_t data_bits,char parity,uint8_t stop_bits):dev(dev),baudrate(baudrate),data_bits(data_bits),parity(parity),stop_bits(stop_bits)
{
    
}

uart::~uart()
{
    this->uart_close();
}

/*
*1.打开串口，获取fd
*2.设置串口属性
*3.检查串口属性是否符合要求
*/
int uart::uart_enable()
{

    /* 1. 检查属性是否符合要求 */
    if(baud_adapt(this->baudrate)==-1)
    {
        std::cout<<"[err]:baudrate is not an exportion\n";
        return -1;
    }
    if(this->data_bits!=5&&this->data_bits!=6&&this->data_bits!=7&&this->data_bits!=8)
    {
        std::cout<<"[err]:data bits is not an exportion\n";
        return -1;
    }
    if(this->parity!='N'&&this->parity!='n'
     &&this->parity!='E'&&this->parity!='e'
     &&this->parity!='O'&&this->parity!='o')
    {
        std::cout<<"[err]: parity is not an exportion\n";
        return -1;
    }else{
        if(this->parity=='n'){this->parity='N';}
        if(this->parity=='e'){this->parity='E';}
        if(this->parity=='o'){this->parity='O';}
    }
    if(this->stop_bits!=1&&this->stop_bits!=2)
    {
        std::cout<<"[err]: stop bits is not an exportion\n";
        return -1;
    }

    /* 2. 打开串口,获取fd */
    this->fd=open(this->dev.c_str(), O_RDWR | O_NOCTTY);

    if(this->fd==-1)
    {
        std::cout<<"[err]: open dev[] failed,and can't get a fd !!!\n";
        return -1;
    }

    /* 3.设置串口属性 */
    int ret=setAttr(this->fd,this->baudrate,this->data_bits,this->parity,this->stop_bits);
    
    if(ret==-1)
    {
        std::cout<<"[err]: set attr failed !!!\n";
        return -1;
    }
    /* 4.设置使能标志位为true */
    this->enable_flag=true;
    this->print_setting();
    return 0;
   
}

void uart::uart_close(){
    
    /* 如果串口被使能 */
    if(this->enable_flag==true)
    {
        close(this->fd);
        this->enable_flag=false;
    }
}

int uart::setAttr(int fd,uint32_t baudrate,uint8_t databits,char parity, uint8_t stopbits)
{
   
    /*0. 保存原有的termios*/
    tcgetattr(this->fd, &old_termios);

    /*1. 设置波特率 */
    new_termios.c_cflag&=~CBAUD;//波特率掩码(000000010017 /*  baud speed mask)
    if(cfsetispeed(&(new_termios),baud_adapt(baudrate))<0)
    {
        std::cerr<<"[err]:function cfsetispeed()--set "<< baudrate<<" input baudrate failed!!!\n";
        this->uart_close();
        return -1;
    }
    if(cfsetospeed(&(new_termios),baud_adapt(baudrate))<0)
    {
        std::cerr<<"[err]:function cfsetospeed()--set "<< baudrate<<" output baudrate failed!!!\n";
        this->uart_close();
        return -1;
    }

    /* 2.设置数据位 */
    new_termios.c_cflag&=~CSIZE;//屏蔽其他标志
    switch (databits)//设置数据位
    {
    case 5:
        new_termios.c_cflag|=CS5;
        break;
    case 6:
        new_termios.c_cflag|=CS6;
        break;
    case 7:
        new_termios.c_cflag|=CS7;
        break;
    case 8:
        new_termios.c_cflag|=CS8;
        break;
    default:
        this->uart_close();
        std::cerr<<"[err]:set databits failed!!!\n";
        return -1;
        break;
    }
    
    /*3. 设置奇偶检验 */

    switch (parity)
    {
    case 'N'://无
        new_termios.c_iflag &= ~(INPCK | ISTRIP);//关闭输入奇偶检验检查
        new_termios.c_cflag &= ~PARENB;//关闭奇偶位
        break;
    case 'E'://奇
        new_termios.c_iflag |= (INPCK | ISTRIP);//开启输入奇偶检验检查
        new_termios.c_cflag |= PARENB;//开启奇偶位
        new_termios.c_cflag &= ~PARODD;//关闭偶位
        break;
    case 'O'://偶
        new_termios.c_iflag |= (INPCK | ISTRIP);//开启输入奇偶检验检查
        new_termios.c_cflag|= PARENB;//开启奇偶位
        new_termios.c_cflag |= PARODD;//开启偶位
        break;
    default:
        this->uart_close();
        std::cerr<<"[err]:set parity failed!!!\n";
        return -1;
        break;
    }

    /*4. 设置停止位 */
    switch (stopbits)
    {
    case 1:
        new_termios.c_cflag &= ~CSTOPB;
        break;
    case 2:
        new_termios.c_cflag |= CSTOPB;//每个字符使用2bit停止位，否者就使用一个
        break;
    default:
        this->uart_close();
        break;
    }

    /*5. 设置非规范模式(即raw模式)  */
    new_termios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);;
    new_termios.c_cc[VMIN]=0;
    new_termios.c_cc[VTIME]=1;

    /*6.other*/
    new_termios.c_oflag &=~ OPOST; /* Raw output */
    new_termios.c_iflag &= ~(IXON | IXOFF | IXANY);
    
    /*7.设置生效 */
    if(tcsetattr(this->fd,TCSANOW,&(new_termios))<0)
    {
        std::cerr<<"[err]:function tcsetattr()--set attr failed!!!\n";
        this->uart_close();
        return -1;
    }

  return 0;
}

int uart::baud_adapt(int baudrate){
    int ret=-1;
    switch (baudrate)
    {
    case 0:
        ret=B0;
        break;
    case 50:
        ret=B50;
        break;
    case 75:
        ret=B75;
        break;
    case 110:
        ret=B110;
        break;
    case 134:
        ret=B134;
        break;
    case 150:
        ret=B150;
        break;
    case 200:
        ret=B200;
        break;
    case 300:
        ret=B300;
        break;
    case 600:
        ret=B600;
        break;  
    case 1200:
        ret=B1200;
        break;  
    case 1800:
        ret=B1800;
        break;
    case 2400:
        ret=B2400;
        break;  
    case 4800:
        ret=B4800;
        break;
    case 9600:
        ret=B9600;
        break;
    case 19200:
        ret=B19200;
        break; 
    case 38400:
        ret=B38400;
        break;
    case 57600:
        ret=B57600;
        break;
    case 115200:
        ret=B115200;
        break;
    case 230400:
        ret=B230400;
        break;
    case 460800:
        ret=B460800;
        break;
    case 500000:
        ret=B500000;
        break;
    case 576000:
        ret=B576000;
        break;
    case 921600:
        ret=B921600;
        break; 
    case 1000000:
        ret=B1000000;
        break;  
    case 1152000:
        ret=B1152000;
        break;
    case 1500000:
        ret=B1500000;
        break;
    case 2000000:
        ret=B2000000;
        break;
    case 2500000:
        ret=B2500000;
        break;
    case 3000000:
        ret=B3000000;
        break; 
    case 3500000:
        ret=B3500000;
        break;
    case 4000000:
        ret=B4000000;
        break;                                     
    default:
        ret=-1;
        break;
    }
    return ret;
}

int uart::uart_send(const __uint8_t* buf,__uint32_t length)
{
    if(false==this->enable_flag)
    {
        std::cerr<<"[err]:please make your uart class enale\n";
        return -1;
    }
    //刷新输出队列
    tcflush(this->fd,TCOFLUSH);
    int ret=write(this->fd,buf,length);
    return ret;
}

int uart::uart_recv(__uint8_t* buf,__uint32_t length)
{
    if(false==this->enable_flag)
    {
        std::cerr<<"[err]:please make your uart class enale\n";
        return -1;
    }
    //刷新输入队列
    // tcflush(this->fd,TCIFLUSH);
    int ret=read(this->fd, buf, length);
    return ret;
}

/*
* 打印配置信息
*/
void uart::print_setting()
{
    std::cout<<this->dev<<" "
             <<this->baudrate<<" "
             <<+this->data_bits<<" "
             <<this->parity<<" "
             <<+this->stop_bits<<std::endl;
}