#ifndef HEX_TF_H
#define HEX_TF_H
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

//float
float hex_to_float32(u_int8_t* array,const int mode);

void float32_to_hex( float number,uint8_t* array,uint16_t length,const int mode);

//double
double hex_to_float64(uint8_t* array,const int mode);

void float64_to_hex( double number,uint8_t* array,uint16_t length,const int mode);

//uint16 uint32 uint64
uint16_t hex_to_uint16(uint8_t* array,const int mode);

void uint16_to_hex( uint16_t number,uint8_t* array,uint16_t length,const int mode);

uint32_t hex_to_uint32(uint8_t* array,const int mode);

void uint32_to_hex( uint32_t number,uint8_t* array,uint16_t length,const int mode);

uint64_t hex_to_uint64(uint8_t* array,const int mode);

void uint64_to_hex( uint64_t number,uint8_t* array,uint16_t length,const int mode);

//int16 int32 int64

int16_t hex_to_int16(uint8_t* array,const int mode);

void int16_to_hex( int16_t number,uint8_t* array,uint16_t length,const int mode);

int32_t hex_to_int32(uint8_t* array,const int mode);

void int32_to_hex( int32_t number,uint8_t* array,uint16_t length,const int mode);

int64_t hex_to_int64(uint8_t* array,const int mode);

void int64_to_hex( int64_t number,uint8_t* array,uint16_t length,const int mode);

#endif