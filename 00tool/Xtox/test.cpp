 #include "hex_tf.h"
#include<iostream>
#include<vector>

int main()
{   
    // 1-1  uint8-->float32
    {
        //10.501 big
        // uint8_t array[4]={0x41,0x28,0x04,0x18};
        //10.501 small
        // uint8_t array[4]={0x18,0x04,0x28,0x41};

        std::vector<uint8_t> array={0x41,0x28,0x04,0x18};
        float ret= hex_to_float32(array.data(),2);
        std::cout<<ret<<std::endl;
    }

    //1-2 float32-->uint8
    // {
    //     uint8_t array[4];
    //     float number=10.501;
    //     float32_to_hex(number,array,4,2);
    //     for (size_t i = 0; i < 4; i++)
    //     {
    //         printf(" %x \n",*(array+i));
    //     }
    // }

    // 2-1  uint8-->float32
    //  {
    //     // 40250083126E978D
    //     //10.501 big
    //     // uint8_t array[8]={0x40,0x25,0x00,0x83,0x12,0x6E,0x97,0x8D};
    //     //10.501 small
    //     uint8_t array[8]={0x8D,0x97,0x6E,0x12,0x83,0x00,0x25,0x40};
    //     double ret= hex_to_float64(array,1);
    //     // printf("ret = %lf",ret);
    //     std::cout<<ret<<std::endl;
    // }
    
    // 2-2  float32-->uint8
    // {
    //     double number=10.501;
    //     uint8_t array[8];
    //     float64_to_hex(number,array,4,2);
    //     for (size_t i = 0; i < 8; i++)
    //     {
    //         printf(" %x \n",*(array+i));
    //     }
    // }

    //3-1-1 uint8-->uint16
    // {
    //     //大端 10000
    //     // uint8_t array[8]={0x27,0x10};

    //     //小端 10000
    //     uint8_t array[8]={0x10,0x27};

    //     uint16_t ret= hex_to_uint16(array,1); 
    //     std::cout<<ret<<std::endl;  
    // }


    //3-1-2 uint16-->uint8
    // {
    //     uint16_t number=10000;
    //     uint8_t array[8];
    //     uint16_to_hex(number,array,4,2);
    //     for (size_t i = 0; i < 2; i++)
    //     {
    //          printf(" %x \n",*(array+i));
    //     }
        
    // }
    
    //3-2-1 uint8-->uint32 64
    // {
    //     //大端 1,000,000,000   3b9aca00
    //     // uint8_t array[8]={0x3b,0x9a,0xca,0x00};

    //     //小端 1,000,000,000   3b9aca00
    //     uint8_t array[8]={0x00,0xca,0x9a,0x3b};

    //     uint32_t ret= hex_to_uint32(array,1); 
    //     std::cout<<ret<<std::endl;  
    // }

    //3-2-2 uint32-->uint8
    // {
    //     uint32_t number=1000000000;
    //     uint8_t array[8];
    //     uint32_to_hex(number,array,4,1);
    //     for (size_t i = 0; i < 4; i++)
    //     {
    //          printf(" %x ",*(array+i));
    //     }
        
    // }

    //3-3-1 uint8-->uint64
    // {
    //     //大端 10,000,000,000,000,000,000  8ac7230489e80000
    //     //  uint8_t array[8]={0x8a,0xc7,0x23,0x04,0x89,0xe8,0x00,0x00};

    //     //小端 
    //     uint8_t array[8]={0x00,0x00,0xe8,0x89,0x04,0x23,0xc7,0x8a};

    //     uint64_t ret= hex_to_uint64(array,1); 
    //     std::cout<<ret<<std::endl;  
    // }

    //3-3-2 uint64-->uint8
    // {
    //     uint64_t number=10000000000000000000;
    //     uint8_t array[8];
    //     uint64_to_hex(number,array,8,2);
    //     for (size_t i = 0; i < 8; i++)
    //     {
    //          printf(" %x ",*(array+i));
    //     }
    // }

    //4-1-1 uint8-->int16
    // {
    //     //大端 -15536 c350
    //     // uint8_t array[8]={0xc3,0x50};

    //     //小端 
    //     uint8_t array[8]={0x50,0xc3};

    //     int16_t ret= hex_to_int16(array,1); 
    //     std::cout<<ret<<std::endl;  
    // }
    
    //4-1-2 int64-->uint8
    // {
    //     int16_t number=-15536;
    //     uint8_t array[8];
    //     int16_to_hex(number,array,2,1);
    //     for (size_t i = 0; i < 2; i++)
    //     {
    //          printf(" %x ",*(array+i));
    //     }
    // }

    //4-2-1 uint8-->int32
    // {
    //     //大端 -939224096    c8 04 93 e0
    //     // uint8_t array[8]={0xc8,0x04,0x93,0xe0};

    //     //小端 
    //     uint8_t array[8]={0xe0,0x93,0x04,0xc8};

    //     int32_t ret= hex_to_int32(array,1); 
    //     std::cout<<ret<<std::endl;  
    // }

    //4-2-2 int32-->uint8
    // {
    //     int32_t number=-939224096;
    //     uint8_t array[8];
    //     int32_to_hex(number,array,2,2);
    //     for (size_t i = 0; i < 4; i++)
    //     {
    //          printf(" %x ",*(array+i));
    //     }
    // }


    return 0;
}