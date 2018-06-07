/*************************************************************************************
* Copyright (C) 2017, Huada Semiconductor Co.,Ltd All rights reserved.    
*
* This software is owned and published by: 
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC 
* components. This software is licensed by HDSC to be adapted only 
* for use in systems utilizing HDSC components. HDSC shall not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein. HDSC is providing this software "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the software.  
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS), 
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING, 
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED 
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED 
* WARRANTY OF NONINFRINGEMENT.  
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, 
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT 
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, 
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR 
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, 
* SAVINGS OR PROFITS, 
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED 
* FROM, THE SOFTWARE.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Disclaimer and Copyright notice must be 
* included with each copy of this software, whether used in part or whole, 
* at all times.                        
*/
/******************************************************************************/
/** \file I2C.c
 **
 ** WDT function driver API.
 ** @link SampleGroup Some description @endlink
 **
 **   - 2017-06-13  1.0  CJ First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "i2c.h"
/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static func_ptr_t pfnI2ctCallback = NULL;
/**
 ******************************************************************************
 ** \brief  I2C���ò��������üĴ���
 **
 ** \param [in] u8Tm ����������ֵ
 **
 ** \retval enRet �ɹ���ʧ��
 **
 ******************************************************************************/
 en_result_t I2C_SetBaud(uint8_t u8Tm)
 {
	 en_result_t enRet = Error;
     M0P_I2C->TM = u8Tm;
     enRet = Ok;
     return enRet;
 }
 /**
 ******************************************************************************
 ** \brief  I2C����������غ���
 **
 ** \param [in] enFunc���ܲ���
 **
 ** \retval enRet �ɹ���ʧ��
 **
 ******************************************************************************/
en_result_t I2C_SetFunc(en_i2c_func_t enFunc)
{
    en_result_t enRet = Error;
    switch(enFunc)
    {
        case I2cMode_En:
            M0P_I2C->CR_f.ENS = 1;
            break;
        case I2cStart_En:
            M0P_I2C->CR_f.STA = 1;
            break;
        case I2cStop_En:
            M0P_I2C->CR_f.STO = 1;
            break;
        case I2cAck_En:
            M0P_I2C->CR_f.AA = 1;
            break;
        case I2cHlm_En:
            M0P_I2C->CR_f.H1M = 1;
            break;
        case I2cBaud_En:
            M0P_I2C->TMRUN = 0x01;
            break;
        default:
            return ErrorInvalidParameter;
            break;
    }
    enRet = Ok;
    return enRet;
}
 /**
 ******************************************************************************
 ** \brief  I2C���������غ���
 **
 ** \param [in] enFunc���ܲ���
 **
 ** \retval enRet �ɹ���ʧ��
 **
 ******************************************************************************/
 en_result_t I2C_ClearFunc(en_i2c_func_t enFunc)
 {
    en_result_t enRet = Error;
    switch(enFunc)
    {
        case I2cMode_En:
            M0P_I2C->CR_f.ENS = 0;
            break;
        case I2cStart_En:
            M0P_I2C->CR_f.STA = 0;
            break;
        case I2cStop_En:
            M0P_I2C->CR_f.STO = 0;
            break;
        case I2cAck_En:
            M0P_I2C->CR_f.AA = 0;
            break;
        case I2cHlm_En:
            M0P_I2C->CR_f.H1M = 0;
            break;
        case I2cBaud_En:
            M0P_I2C->TMRUN = 0x00;
            break;
        default:
            return ErrorInvalidParameter;
            break;
    }
    enRet = Ok;
    return enRet; 
 }
 /**
 ******************************************************************************
 ** \brief  I2C��ȡ�жϱ�Ǻ���
 **
 ** \param ��
 **
 ** \retval bIrq�жϱ��
 **
 ******************************************************************************/
boolean_t I2C_GetIrq(void)
{
    boolean_t bIrq = FALSE;
    bIrq = M0P_I2C->CR_f.SI;
    return bIrq;  
}
/**
 ******************************************************************************
 ** \brief  I2C����жϱ�Ǻ���
 **
 ** \param ��
 **
 ** \retval bIrq�жϱ��
 **
 ******************************************************************************/
en_result_t I2C_ClearIrq(void)
{
    en_result_t enRet = Error;
    M0P_I2C->CR_f.SI = 0;
    enRet = Ok;
    return enRet; 
}
 /**
 ******************************************************************************
 ** \brief  I2C��ȡ���״̬
 **
 ** \param ��
 **
 ** \retval I2C״̬
 **
 ******************************************************************************/
uint8_t I2C_GetState(void)
{
  uint8_t u8State = 0;
  u8State = M0P_I2C->STAT;
  return u8State;
}
/**
 ******************************************************************************
 ** \brief  I2Cд�ӻ���ַ����
 **
 ** \param u8SlaveAddr�ӻ���ַ
 **
 ** \retval I2Cд�ɹ����״̬
 **
 ******************************************************************************/
 en_result_t I2C_WriteSlaveAddr(stc_i2c_addr_t stcSlaveAddr)
{
    en_result_t enRet = Error;
    M0P_I2C->ADDR_f.ADDR = stcSlaveAddr.Addr;
    M0P_I2C->ADDR_f.GC = stcSlaveAddr.Gc;
    enRet = Ok;
    return enRet;     
}
/**
 ******************************************************************************
 ** \brief  �ֽ�д�ӻ�����
 **
 ** \param u8Dataд����
 **
 ** \retval д�����Ƿ�ɹ�
 **
 ******************************************************************************/
en_result_t I2C_WriteByte(uint8_t u8Data)
{
    en_result_t enRet = Error;
    M0P_I2C->DATA = u8Data;
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  �ֽڶ��ӻ�����
 **
 ** \param ��
 **
 ** \retval ��ȡ����
 **
 ******************************************************************************/
uint8_t I2C_ReadByte(void)
{
    uint8_t u8Data = 0;
    u8Data = M0P_I2C->DATA;
    return u8Data;
}
 /**
 ******************************************************************************
 ** \brief  �������ͺ���
 **
 ** \param u8Addr�ӻ��ڴ��ַ��pu8Dataд���ݣ�u32Lenд���ݳ���
 **
 ** \retval д�����Ƿ�ɹ�
 **
 ******************************************************************************/
en_result_t I2C_MasterWriteData(uint8_t u8Addr,uint8_t *pu8Data,uint32_t u32Len)
{
    en_result_t enRet = Error;
    uint8_t u8i,u8State;
    
    I2C_SetFunc(I2cStart_En);
    
    while(0 == I2C_GetIrq());
    u8State = I2C_GetState();
    while(0x08 != u8State);//��ʼ�źŷ��ͳɹ�
    I2C_ClearFunc(I2cStart_En);
    I2C_WriteByte(I2c_SlaveWt);//�ӻ���ַ����OK
    I2C_ClearIrq();
    
    while(0 == I2C_GetIrq());
    u8State = I2C_GetState();
    while(0x18 != u8State);
    I2C_WriteByte(u8Addr);
    I2C_ClearIrq();
    
    while(0 == I2C_GetIrq());
    u8State = I2C_GetState();
    while(0x28 != u8State); 
    for(u8i = 0;u8i<u32Len;u8i++)
    {
        I2C_WriteByte(pu8Data[u8i]);
        I2C_ClearIrq();
        while(0 == I2C_GetIrq());
        u8State = I2C_GetState();
        while(0x28 != u8State); 
        
    }
    if(u32Len == u8i)
    {
        I2C_SetFunc(I2cStop_En);
        I2C_ClearIrq();
    }
    enRet = Ok;
    return enRet;
}
 /**
 ******************************************************************************
 ** \brief  �ӻ����ͺ���
 **
 ** \param pu8Data�������ݻ��棬u32Len�������ݳ���
 **
 ** \retval ���������Ƿ�ɹ�
 **
 ******************************************************************************/
 en_result_t I2C_SlaveWriteData(uint8_t *pu8Data,uint32_t *u32Len)
 {
    //en_result_t enRet = Error;
    uint8_t u8i=0,u8State;
    //
    while(1)
    {

        while(0 == I2C_GetIrq());
        u8State = I2C_GetState();
        switch(u8State)
        {
            case 0xA8:
            case 0xB0:
                I2C_WriteByte(pu8Data[u8i++]);
                I2C_ClearIrq();
                break;
            case 0xB8: 
            case 0xC8:               
                I2C_WriteByte(pu8Data[u8i++]);
                I2C_ClearIrq();                 
                break;
            
            case 0xF8:
                I2C_ClearIrq();
                *u32Len = u8i;
                break;
             //case 0xC0:   
            default:
                I2C_ClearIrq();
                return ErrorInvalidParameter; 
        }
        u8State = I2C_GetState();
    } 
 }
 /**
 ******************************************************************************
 ** \brief  �ӻ����պ���
 **
 ** \param pu8Data�������ݴ�Ż��棬u32Len��������ָ��
 **
 ** \retval ���������Ƿ�ɹ�
 **
 ******************************************************************************/
en_result_t I2C_SlaveReadData(uint8_t *pu8Data,uint32_t *u32Len)
{
#if 1
    //en_result_t enRet = Error;
    uint8_t u8i=0,u8State;
    while(0 == I2C_GetIrq());
    while(1)
   {
        while(0 == I2C_GetIrq());
        u8State = I2C_GetState();
        switch(u8State)
        {
            case 0x60:
            case 0x68:
            case 0x70:
            case 0x78:
              I2C_ClearIrq();
              break;
            case 0x80:
            case 0x90:               
                pu8Data[u8i++] = I2C_ReadByte();
                I2C_ClearIrq();                 
                break;
            case 0x88:
            case 0x98:
                return Error;
            case 0xA0:
                I2C_ClearIrq();
                *u32Len = u8i;
                return Ok;
            default:
                return ErrorInvalidParameter; 
        }
   }
#endif
    
} 

/**
 ******************************************************************************
 ** \brief  �������պ���
 **
 ** \param u8Addr�ӻ��ڴ��ַ��pu8Data�����ݴ�Ż��棬u32Len�����ݳ���
 **
 ** \retval �������Ƿ�ɹ�
 **
 ******************************************************************************/
 en_result_t I2C_MasterReadData(uint8_t *pu8Data,uint8_t u8Addr,uint32_t u32Len)
{
    en_result_t enRet = Error;
    uint8_t u8i,u8State;
    
    I2C_SetFunc(I2cStart_En);
    
    while(0 == I2C_GetIrq());
    u8State = I2C_GetState();
    while(0x08 != u8State);//��ʼ�źŷ��ͳɹ�
    I2C_ClearFunc(I2cStart_En);
    
#if 1 //���ݴӻ�����E2����MCUģ��I2C�����д���
    I2C_WriteByte(I2c_SlaveWt);//�ӻ���ַ����OK
    I2C_ClearIrq();

    while(0 == I2C_GetIrq());
    u8State = I2C_GetState();
    while(0x18 != u8State);
    I2C_WriteByte(u8Addr);
    I2C_ClearIrq();
    
    while(0 == I2C_GetIrq());
    u8State = I2C_GetState();
    while(0x28 != u8State);  
    I2C_SetFunc(I2cStart_En);
    I2C_ClearIrq();
 
    while(0 == I2C_GetIrq());
    u8State = I2C_GetState();
    while(0x10 != u8State);//��ʼ�źŷ��ͳɹ�

    I2C_ClearFunc(I2cStart_En);
#endif
    I2C_WriteByte(I2c_SlaveRd);//�ӻ���ַ����OK
    I2C_ClearIrq();
    
    while(0 == I2C_GetIrq());
    u8State = I2C_GetState();
    while(0x40 != u8State);
    
    I2C_SetFunc(I2cAck_En);
    
    I2C_ClearIrq();
    while(0 == I2C_GetIrq());
    for(u8i=0;u8i<u32Len;u8i++)
    {
      pu8Data[u8i] = I2C_ReadByte();
      I2C_ClearIrq();
      while(0 == I2C_GetIrq());
      u8State = I2C_GetState();
      while(0x50 != u8State);
    }
    I2C_ClearFunc(I2cAck_En);
    I2C_SetFunc(I2cStop_En);
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  I2Cģ���ʼ��
 **
 ** \param stcI2CCfg��ʼ�����ýṹ��
 **
 ** \retval ��ʼ���Ƿ�ɹ�
 **
 ******************************************************************************/
en_result_t I2C_Init(stc_i2c_config_t stcI2CCfg)
{
   en_result_t enRet = Error;
   enRet = I2C_SetFunc(stcI2CCfg.enFunc);
   enRet = I2C_SetBaud(stcI2CCfg.u8Tm);
   enRet = I2C_WriteSlaveAddr(stcI2CCfg.stcSlaveAddr);
   pfnI2ctCallback = stcI2CCfg.pfnI2cCb;
   if(TRUE == stcI2CCfg.bTouchNvic)
   {
        EnableNvic(I2C_IRQn,DDL_IRQ_LEVEL_DEFAULT,TRUE);
   }  
   return enRet;
}
/**
 ******************************************************************************
 ** \brief  I2Cģ��رճ�ʼ��
 **
 ** \param ��
 **
 ** \retval �����Ƿ�ɹ�
 **
 ******************************************************************************/
 en_result_t I2C_DeInit(void)
 {
     en_result_t enRet = Error;
	 M0P_I2C->CR = 0x00;
     enRet = Ok;
     return enRet;
 }
 /**
 ******************************************************************************
 ** \brief  I2Cģ���жϴ�����
 **
 ** \param u8Param ������
 **
 ** \retval  ��
 **
 ******************************************************************************/
void I2c_IRQHandler(uint8_t u8Param)
{
    pfnI2ctCallback();
}