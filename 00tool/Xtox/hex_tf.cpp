#include "hex_tf.h"
#include<stdint.h>

// uint16_t uint16_t2bits(uint16_t hex){
//     switch (hex)
//     {
//     case 0:
//         return 0x0000;
//         break;
//     case 1:
//         return 0x0001;
//         break;
//     case 2:
//         return 0x0002;
//         break;
//     case 3:
//         return 0x0004;
//         break;
//     case 4:
//         return 0x0008;
//         break;
//     case 5:
//         return 0x0010;
//         break;
//     case 6:
//         return 0x0020;
//         break;
//     case 7:
//         return 0x0040;
//         break;   
//     case 8:
//         return 0x0080;
//         break;
//     case 9:
//         return 0x0100;
//         break;
//     case 10:
//         return 0x0200;
//         break;
//     case 11:
//         return 0x0400;
//         break;     
//     case 12:
//         return 0x0800;
//         break;
//     case 13:
//         return 0x1000;
//         break;  
//     case 14:
//         return 0x2000;
//         break;
//     case 15:
//         return 0x4000;
//         break; 
//     case 16:
//         return 0x8000;
//         break;
//     default:
//         printf("[err]:hex to bit failed!!!\n");
//         break;
//     }
// }


/********************联合体**************************/
union tf_2byte
{
    // uint8_t   my_array[2];
    uint16_t  my_uint16;
    int16_t   my_int16;
};


union tf_4byte
{
    // uint8_t   my_array[4];
    uint32_t  my_uint32;
    int32_t   my_int32;
    float     my_float32;
};


union tf_8byte
{
    // uint8_t  my_array[8];
    uint64_t my_uint64;
    int64_t  my_int64;
    double   my_float64;
};


/***********************************************/
/* A to B 大小端 */



/* uint8数组-->float32*/
float hex_to_float32(uint8_t* array,const int mode)
{
    union tf_4byte ret;

    switch (mode)
    {
    case 2:
        {
             ret.my_uint32=(*(array)<<24)|(*(array+1)<<16)|(*(array+2)<<8)|(*(array+3)<<0);
        }
        break;
    case 1:
        {
             ret.my_uint32=(*(array+3)<<24)|(*(array+2)<<16)|(*(array+1)<<8)|(*(array)<<0);
        }
        break;
    default:
        break;
    }
    return ret.my_float32;
}

/*float32 --> uint8数组*/
void float32_to_hex( float number,uint8_t* array,uint16_t length,const int mode)
{
    union tf_4byte ret;
    ret.my_float32=number;
    switch (mode)
    {
    case 2:
        {
            *(array+0)=ret.my_uint32>>24;
            *(array+1)=ret.my_uint32>>16;
            *(array+2)=ret.my_uint32>>8;
            *(array+3)=ret.my_uint32>>0;
        }   
        break;
    case 1:
        {
            *(array+0)=ret.my_uint32>>0;
            *(array+1)=ret.my_uint32>>8;
            *(array+2)=ret.my_uint32>>16;
            *(array+3)=ret.my_uint32>>24;
        }
        break;
    
    default:
        break;
    }

}


/* uint8数组-->float64*/
double hex_to_float64(uint8_t* array,const int mode)
{
    union tf_8byte ret;

    switch (mode)
    {
    case 2:
        {
            ret.my_uint64=(((uint64_t)*(array+0))<<56)|(((uint64_t)*(array+1))<<48)|(((uint64_t)*(array+2))<<40)|(((uint64_t)*(array+3))<<32)
                           |(((uint64_t)*(array+4))<<24)|(((uint64_t)*(array+5))<<16)|(((uint64_t)*(array+6)<<8))|(((uint64_t)*(array+7))<<0);
        }
        break;
    case 1:
        {
            ret.my_uint64=(((uint64_t)*(array+7))<<56)|(((uint64_t)*(array+6))<<48)|(((uint64_t)*(array+5))<<40)|(((uint64_t)*(array+4))<<32)
                          |(((uint64_t)*(array+3))<<24)|(((uint64_t)*(array+2))<<16)|(((uint64_t)*(array+1))<<8)|(((uint64_t)*(array+0))<<0);
        }
        break;
    default:
        break;
    }
    return  ret.my_float64;
}

/*float64 --> uint8数组*/
void float64_to_hex( double number,uint8_t* array,uint16_t length,const int mode)
{
    union tf_8byte ret;
    ret.my_float64=number;
    switch (mode)
    {
    case 2:
        {
            *(array+0)=ret.my_uint64>>56;
            *(array+1)=ret.my_uint64>>48;
            *(array+2)=ret.my_uint64>>40;
            *(array+3)=ret.my_uint64>>32;
            *(array+4)=ret.my_uint64>>24;
            *(array+5)=ret.my_uint64>>16;
            *(array+6)=ret.my_uint64>>8;
            *(array+7)=ret.my_uint64>>0;
        }   
        break;
    case 1:
        {
            *(array+0)=ret.my_uint64>>0;
            *(array+1)=ret.my_uint64>>8;
            *(array+2)=ret.my_uint64>>16;
            *(array+3)=ret.my_uint64>>24;
            *(array+4)=ret.my_uint64>>32;
            *(array+5)=ret.my_uint64>>40;
            *(array+6)=ret.my_uint64>>48;
            *(array+7)=ret.my_uint64>>56;
        }
        break;
    
    default:
        break;
    }

}


/* uint8-->uint16 */
uint16_t hex_to_uint16(uint8_t* array,const int mode)
{
     union tf_2byte ret;
    switch (mode)
    {
    case 2:
        {
            ret.my_uint16=(*(array+0)<<8)|(*(array+1));
        }
        break;
    case 1:
        {
            ret.my_uint16=(*(array+1)<<8)|(*(array+0));
        }
        break;
    default:
        break;
    }
    return ret.my_uint16;
}


/*uint16 --> uint8*/
void uint16_to_hex( uint16_t number,uint8_t* array,uint16_t length,const int mode)
{
    union tf_2byte ret;
    ret.my_uint16=number;
    switch (mode)
    {
    case 2:
        {
           *(array+0)=number>>8;
           *(array+1)=number>>0;
        }
        break;
    case 1:
        {
            *(array+0)=number>>0;
            *(array+1)=number>>8;
        }
        break;
    default:
        break;
    }

}


/* uint8-->uint32 */
uint32_t hex_to_uint32(uint8_t* array,const int mode)
{
     union tf_4byte ret;
    switch (mode)
    {
    case 2:
        {
            ret.my_uint32=((uint32_t)*(array+0)<<24)|((uint32_t)*(array+1)<<16)|((uint32_t)*(array+2)<<8)|((uint32_t)*(array+3));
        }
        break;
    case 1:
        {
            ret.my_uint32=((uint32_t)*(array+3)<<24)|((uint32_t)*(array+2)<<16)|((uint32_t)*(array+1)<<8)|((uint32_t)*(array+0));
        }
        break;
    default:
        break;
    }
    return ret.my_uint32;
}


/*uint32 --> uint8*/
void uint32_to_hex( uint32_t number,uint8_t* array,uint16_t length,const int mode)
{
    union tf_4byte ret;
    ret.my_uint32=number;
    switch (mode)
    {
        case 2:
            {
                *(array+0)=ret.my_uint32>>24;
                *(array+1)=ret.my_uint32>>16;
                *(array+2)=ret.my_uint32>>8;
                *(array+3)=ret.my_uint32>>0;
            }
            break;
        case 1:
            {
                *(array+0)=ret.my_uint32>>0;
                *(array+1)=ret.my_uint32>>8;
                *(array+2)=ret.my_uint32>>16;
                *(array+3)=ret.my_uint32>>24;
            }
            break;
        default:
            break;
    }

}



/* uint8-->uint64 */
uint64_t hex_to_uint64(uint8_t* array,const int mode)
{
     union tf_8byte ret;
    switch (mode)
    {
    case 2:
        {
            ret.my_uint64=(((uint64_t)*(array+0))<<56)|(((uint64_t)*(array+1))<<48)|(((uint64_t)*(array+2))<<40)|(((uint64_t)*(array+3))<<32)
                        |(((uint64_t)*(array+4))<<24)|(((uint64_t)*(array+5))<<16)|(((uint64_t)*(array+6))<<8)|(((uint64_t)*(array+7))<<0);
        }
        break;
    case 1:
        {
           ret.my_uint64=(((uint64_t)*(array+7))<<56)|(((uint64_t)*(array+6))<<48)|(((uint64_t)*(array+5))<<40)|(((uint64_t)*(array+4))<<32)
                        |(((uint64_t)*(array+3))<<24)|(((uint64_t)*(array+2))<<16)|(((uint64_t)*(array+1))<<8)|(((uint64_t)*(array+0))<<0);
        }
        break;
    default:
        break;
    }
    return ret.my_uint64;
}


/*uint64 --> uint8*/
void uint64_to_hex( uint64_t number,uint8_t* array,uint16_t length,const int mode)
{
    union tf_8byte ret;
    ret.my_uint64=number;
    switch (mode)
    {
        case 2:
            {
                *(array+0)=ret.my_uint64>>56;
                *(array+1)=ret.my_uint64>>48;
                *(array+2)=ret.my_uint64>>40;
                *(array+3)=ret.my_uint64>>32;
                *(array+4)=ret.my_uint64>>24;
                *(array+5)=ret.my_uint64>>16;
                *(array+6)=ret.my_uint64>>8;
                *(array+7)=ret.my_uint64>>0;
            }
            break;
        case 1:
            {
                *(array+0)=ret.my_uint64>>0;
                *(array+1)=ret.my_uint64>>8;
                *(array+2)=ret.my_uint64>>16;
                *(array+3)=ret.my_uint64>>24;
                *(array+4)=ret.my_uint64>>32;
                *(array+5)=ret.my_uint64>>40;
                *(array+6)=ret.my_uint64>>48;
                *(array+7)=ret.my_uint64>>56;
            }
            break;
        default:
            break;
    }

}



/* uint8-->int16 */
int16_t hex_to_int16(uint8_t* array,const int mode)
{
    union tf_2byte ret;
    switch (mode)
    {
    case 2:
        {
            ret.my_uint16=(*(array+0)<<8)|(*(array+1));
        }
        break;
    case 1:
        {
            ret.my_uint16=(*(array+1)<<8)|(*(array+0));
        }
        break;
    default:
        break;
    }
    return ret.my_int16;
}


/*int16 --> uint8*/
void int16_to_hex( int16_t number,uint8_t* array,uint16_t length,const int mode)
{
    union tf_2byte ret;
    ret.my_int16=number;
    switch (mode)
    {
    case 2:
        {
           *(array+0)=ret.my_int16>>8;
           *(array+1)=ret.my_int16>>0;
        }
        break;
    case 1:
        {
            *(array+0)=ret.my_int16>>0;
            *(array+1)=ret.my_int16>>8;
        }
        break;
    default:
        break;
    }

}



/* uint8-->int32 */
int32_t hex_to_int32(uint8_t* array,const int mode)
{
    union tf_4byte ret;
    switch (mode)
    {
    case 2:
        {
            ret.my_int32=(*(array+0)<<24)|(*(array+1)<<16)|(*(array+2)<<8)|(*(array+3));
        }
        break;
    case 1:
        {
            ret.my_int32=(*(array+3)<<24)|(*(array+2)<<16)|(*(array+1)<<8)|(*(array+0));
        }
        break;
    default:
        break;
    }
    return ret.my_int32;
}


/*int32 --> uint8*/
void int32_to_hex( int32_t number,uint8_t* array,uint16_t length,const int mode)
{
    union tf_4byte ret;
    ret.my_int32=number;
    switch (mode)
    {
        case 2:
            {
                *(array+0)= ret.my_int32>>24;
                *(array+1)= ret.my_int32>>16;
                *(array+2)= ret.my_int32>>8;
                *(array+3)= ret.my_int32>>0;
            }
            break;
        case 1:
            {
                *(array+0)= ret.my_int32>>0;
                *(array+1)= ret.my_int32>>8;
                *(array+2)= ret.my_int32>>16;
                *(array+3)= ret.my_int32>>24;
            }
            break;
        default:
            break;
    }

}

/* uint8-->int64 */
int64_t hex_to_int64(uint8_t* array,const int mode)
{
     union tf_8byte ret;
    switch (mode)
    {
    case 2:
        {
            ret.my_int64=(((uint64_t)*(array+0))<<56)|(((uint64_t)*(array+1))<<48)|(((uint64_t)*(array+2))<<40)|(((uint64_t)*(array+3))<<32)
                        |(((uint64_t)*(array+4))<<24)|(((uint64_t)*(array+5))<<16)|(((uint64_t)*(array+6))<<8)|(((uint64_t)*(array+7))<<0);
        }
        break;
    case 1:
        {
           ret.my_int64=(((uint64_t)*(array+7))<<56)|(((uint64_t)*(array+6))<<48)|(((uint64_t)*(array+5))<<40)|(((uint64_t)*(array+4))<<32)
                        |(((uint64_t)*(array+3))<<24)|(((uint64_t)*(array+2))<<16)|(((uint64_t)*(array+1))<<8)|(((uint64_t)*(array+0))<<0);
        }
        break;
    default:
        break;
    }
    return ret.my_int64;
}


/*uint64 --> uint8*/
void int64_to_hex( int64_t number,uint8_t* array,uint16_t length,const int mode)
{
    union tf_8byte ret;
    ret.my_int64=number;
    switch (mode)
    {
        case 2:
            {
                *(array+0)=ret.my_int64>>56;
                *(array+1)=ret.my_int64>>48;
                *(array+2)=ret.my_int64>>40;
                *(array+3)=ret.my_int64>>32;
                *(array+4)=ret.my_int64>>24;
                *(array+5)=ret.my_int64>>16;
                *(array+6)=ret.my_int64>>8;
                *(array+7)=ret.my_int64>>0;
            }
            break;
        case 1:
            {
                *(array+0)=ret.my_int64>>0;
                *(array+1)=ret.my_int64>>8;
                *(array+2)=ret.my_int64>>16;
                *(array+3)=ret.my_int64>>24;
                *(array+4)=ret.my_int64>>32;
                *(array+5)=ret.my_int64>>40;
                *(array+6)=ret.my_int64>>48;
                *(array+7)=ret.my_int64>>56;
            }
            break;
        default:
            break;
    }

}
