#pragma once
#include <iostream>
#include <termios.h>
class uart
{
private:
    int fd;
    std::string dev;
    uint32_t baudrate;//波特率
    uint8_t data_bits;//数据位
    char parity;//奇偶校验位
    uint8_t stop_bits;//停止位
    int flow_ctrl;//软硬控制
    struct termios new_termios;
    struct termios old_termios;
    bool enable_flag=false;
    int MIN=0;
    int TIME=0;
public:
    uart(const char* dev,uint32_t baudrate,uint8_t data_bits,char parity,uint8_t stop_bits);
    ~uart();
    int uart_enable();
    int uart_send(const __uint8_t* buf,__uint32_t length);
    int uart_recv( __uint8_t* buf,__uint32_t length);
    void uart_close();

    int set_cc(int MIN,int TIME);
    void print_setting();
    
    //CRC校验
public:
    uint16_t crc16_modbus(uint8_t *data, uint16_t length);
    uint16_t crc16_usb(uint8_t *data, uint16_t length);

private:
    int setAttr(int fd,uint32_t baudrate,uint8_t databits,char parity, uint8_t stopbits);
    int baud_adapt(int baudrate);
};
