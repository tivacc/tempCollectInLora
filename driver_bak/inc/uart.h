/******************************************************************************
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
/*****************************************************************************/
/** \file uart.h
 **
 ** Headerfile for UART functions
 **  
 **
 ** History:
 **   - 2017-05-10   Cathy     First Version
 **
 *****************************************************************************/

#ifndef __UART_H__
#define __UART_H__
/*****************************************************************************
 * Include files
 *****************************************************************************/
#include "ddl.h"
#include "interrupts_hc32l110.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
 /**
 ******************************************************************************
 **\uart通道选择
 ******************************************************************************/

typedef enum en_uart_channel
{
    UARTCH0 = 0u,
    UARTCH1 = 1u,
}en_uart_channel_t;


/**
 ******************************************************************************
 ** \uart多主机模式地址帧/数据帧或者奇偶校验
 ******************************************************************************/
typedef enum en_uart_mmdorck
{
    DataOrEven 	 = 0u,     //多主机模式时，数据帧TB8标记 ,非多主机模式偶校验   
    AddrOrOdd    = 1u,          
}en_uart_mmdorck_t;

/**
 ******************************************************************************
 ** \uart多主机模式配置
 ******************************************************************************/
typedef enum en_uart_multimode
{
    UartNormal = 0u,         //正常工作模式
    UartMulti  = 1u,         //多主机工作模式
}en_uart_multimode_t;
/**
 ******************************************************************************
 ** \uart多主机模式及从机地址和地址掩码配置
 ******************************************************************************/

typedef struct stc_uart_multimode
{
	en_uart_multimode_t enMulti_mode;
	uint8_t 			u8SlaveAddr;//从机地址
	uint8_t				u8SaddEn;//从及地址掩码
}stc_uart_multimode_t;

/**
 ******************************************************************************
 ** \uart 四种工作模式选择
 ******************************************************************************/

typedef enum en_uart_mode
{
    UartMode0 = 0u,         
    UartMode1 = 1u,
    UartMode2 = 2u,
    UartMode3 = 3u,
} en_uart_mode_t;
/**
 ******************************************************************************
 ** \uart 发送接收功能使能
 ******************************************************************************/
typedef enum en_uart_func
{
    UartTx = 0u,      //mode0模式代表TX    
    UartRx = 1u,  //非mode0模式代表RX and TX ,mode0模式代表RX  
}en_uart_func_t;
/**
 ******************************************************************************
 ** \uart发送接收中断使能
 ******************************************************************************/
typedef enum en_uart_irq_sel
{
    UartTxIrq       = 0u,          
    UartRxIrq       = 1u,          
}en_uart_irq_sel_t;
/**
 ******************************************************************************
 ** \uart发送接收中断结构
 ******************************************************************************/
typedef struct stc_uart_irq_en
{
    boolean_t bTxIrq;          ///< UART TX interrupt
    boolean_t bRxIrq;          ///< UART RX interrupt
}stc_uart_irq_en_t;
/**
 ******************************************************************************
 ** \uart发送接收中断处理函数接口
 ******************************************************************************/

typedef struct stc_uart_irq_cb
{
    func_ptr_t pfnTxIrqCb;      
    func_ptr_t pfnRxErrIrqCb;  
    func_ptr_t pfnRxIrqCb;      
}stc_uart_irq_cb_t;
/**
 ******************************************************************************
 ** \uart 状态标志位
 ******************************************************************************/
typedef enum en_uart_status
{
    UartRFRAMEError    	= 0u,  
    UartRxFull          = 1u,   
    UartTxEmpty         = 2u,   
   
}en_uart_status_t;
/**
 ******************************************************************************
 ** \uart 通道地址及中断函数地址结构
 ******************************************************************************/
typedef struct stc_uart_instance_data
{   
    uint32_t               u32Idx;               ///< channel index
    M0P_UART_TypeDef       *pstcInstance;        ///< pointer to registers of an instance
    stc_uart_irq_cb_t      stcUartInternIrqCb;   ///< Uart internal interrupt callback function
} stc_uart_instance_data_t;
/**
 ******************************************************************************
 ** \uart 通道波特率配置
 ******************************************************************************/
typedef struct stc_uart_baud_config
{
	uint8_t u8Mode;//四种工作模式
	uint8_t u8Dbaud;//是否双倍波特率
	uint32_t u32Baud;//波特率
}stc_uart_baud_config_t;
/**
 ******************************************************************************
 ** \uart 总体配置
 ******************************************************************************/

typedef struct stc_uart_config
{
    en_uart_mode_t   	enRunMode;    
    stc_uart_multimode_t* pstcMultiMode; 
   // en_uart_mmdorck_t    enMmdOrCk;
    stc_uart_irq_cb_t*  pstcIrqCb;           
    boolean_t           bTouchNvic;         
} stc_uart_config_t;
//中断相关设置函数
en_result_t Uart_EnableIrq(uint8_t u8Idx,
                           en_uart_irq_sel_t enIrqSel);
en_result_t Uart_DisableIrq(uint8_t u8Idx,
                            en_uart_irq_sel_t enIntSel);
//void Uart_IrqHandler(uint8_t u8Idx);
// 总初始化处理
en_result_t Uart_Init(uint8_t u8Idx, 
                      stc_uart_config_t* pstcConfig);
en_result_t Uart_DeInit(uint8_t u8Idx, boolean_t bTouchNvic);
//工作模式配置、多主机模式下从机地址和地址掩码设置
en_result_t Uart_SetMode(uint8_t u8Idx,en_uart_mode_t enMode);
en_result_t Uart_SetMultiMode(uint8_t u8Idx,stc_uart_multimode_t* pstcMultiConfig);
en_result_t Uart_SetMMDOrCk(uint8_t u8Idx,en_uart_mmdorck_t enTb8);
en_result_t Uart_SetSaddr(uint8_t u8Idx,uint8_t u8Addr);
en_result_t Uart_SetSaddrEn(uint8_t u8Idx,uint8_t u8Addren);
boolean_t Uart_GetRb8(uint8_t u8Idx);

// 波特率设置
uint16_t Uart_SetBaudRate(uint8_t u8Idx,uint32_t u32pclk,stc_uart_baud_config_t* pstcBaud);
                             
// 发送或接收使能和禁止
en_result_t Uart_EnableFunc(uint8_t u8Idx, en_uart_func_t enFunc);
en_result_t Uart_DisableFunc(uint8_t u8Idx, en_uart_func_t enFunc);
//状态位的获取和清除
boolean_t Uart_GetStatus(uint8_t u8Idx,en_uart_status_t enStatus);
en_result_t Uart_ClrStatus(uint8_t u8Idx,en_uart_status_t enStatus);
//数据查询方式的收发操作
//en_result_t  Uart_MultiSendFirstData(uint8_t U8Addr);
en_result_t Uart_SendData(uint8_t u8Idx, uint32_t u32Data);
uint32_t Uart_ReceiveData(uint8_t u8Idx);


#ifdef __cplusplus
#endif

#endif /* __UART_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/



