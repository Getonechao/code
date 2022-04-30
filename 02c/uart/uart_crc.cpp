#include "uart_raw.h"


/*
* crc src : https://github.com/whik/crc-lib-c
*
*/


uint16_t uart::crc16_usb(uint8_t *data, uint16_t length){
    uint8_t i;
    uint16_t crc = 0xffff;        // Initial value
    while(length--)
    {
        crc ^= *data++;            // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;    // crc^0xffff
}



uint16_t uart::crc16_modbus(uint8_t *data, uint16_t length){
    uint8_t i;
    uint16_t crc = 0xffff;        // Initial value
    while(length--)
    {
        crc ^= *data++;            // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005
            else
                crc = (crc >> 1);
        }
    }
    return crc;
}