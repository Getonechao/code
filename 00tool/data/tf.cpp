#include "tf.h"

u_int16_t uint16_t2bits(u_int16_t hex){
    switch (hex)
    {
    case 0:
        return 0x0000;
        break;
    case 1:
        return 0x0001;
        break;
    case 2:
        return 0x0002;
        break;
    case 3:
        return 0x0004;
        break;
    case 4:
        return 0x0008;
        break;
    case 5:
        return 0x0010;
        break;
    case 6:
        return 0x0020;
        break;
    case 7:
        return 0x0040;
        break;   
    case 8:
        return 0x0080;
        break;
    case 9:
        return 0x0100;
        break;
    case 10:
        return 0x0200;
        break;
    case 11:
        return 0x0400;
        break;     
    case 12:
        return 0x0800;
        break;
    case 13:
        return 0x1000;
        break;  
    case 14:
        return 0x2000;
        break;
    case 15:
        return 0x4000;
        break; 
    case 16:
        return 0x8000;
        break;
    default:
        printf("[err]:hex to bit failed!!!\n");
        break;
    }
}