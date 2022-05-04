#include <iostream>

#include "mb.h"
#include "mbport.h"
#define REG_INPUT_START 1000    /* 输入寄存器开始地址 */
#define REG_INPUT_NREGS 4     /* usRegInputBuf数组大小 */
#define REG_HOLDING_START 2000  /* 保持寄存器开始地址 */
#define REG_HOLDING_NREGS 130  /* usRegHoldingBuf数组大小 */

int main()
{
    /*RTUm模式 从机地址 uart 波特率 奇偶无校验 */
    eMBInit( MB_RTU, 0x05,1,115200, MB_PAR_NONE );

    if(eMBEnable( )== MB_ENOERR)
    {  
        for( ;; )
        {
            // Call the main polling loop of the Modbus protocol stack.
            if( eMBPoll(  ) != MB_ENOERR )
            break;
        }
        std::cerr<<"err: eMBPoll failed!";
    }
    else
    {
        std::cerr<<"err: eMBEnable failed!";
    }
    return 0;
}




/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS]={0x1,0x2,0x3,0x4}; //输入寄存器数组
static USHORT   usRegHoldingStart = REG_HOLDING_START;
static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];//保持寄存器数组



/* 输入寄存器 */
eMBErrorCode  eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress,USHORT usNRegs )
{
    
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    std::cout<<"enter\n";

    /**
     * 满足条件：
     *  1. 主机请求地址大于REG_INPUT_START
     *  2. 主机请求数据量不超过 usRegInputBuf数组大小
     */
    if( ( usAddress >= REG_INPUT_START )
    && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/* 保持寄存器 */
eMBErrorCode   eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress,USHORT usNRegs, eMBRegisterMode eMode )
{

}
/* 线圈寄存器 */
eMBErrorCode   eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress,USHORT usNCoils, eMBRegisterMode eMode )
{

}
/* 离散输入寄存器 */
eMBErrorCode    eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress,USHORT usNDiscrete )
{

}