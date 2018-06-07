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
/** \file lpuart.h
 **
 ** Headerfile for LPUART functions
 **  
 **
 ** History:
 **   - 2017-05-11   Cathy     First Version
 **
 *****************************************************************************/

#ifndef __LPUART_H__
#define __LPUART_H__

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
 ** \lpuart 的sclk分频选择
 ******************************************************************************/
 
typedef enum en_lpuart_sclkprs
{
    LPUartDiv128 = 0u,      
    LPUartDiv64  = 1u, 
    LPUartDiv32  = 2u,
    LPUartDiv16  = 3u,
    LPUartDiv8   = 4u,
    LPUartDiv4   = 5u,
    LPUartDiv2   = 6u,
    LPUartDiv1   = 7u,
} en_lpuart_sclkprs_t;
/**
 ******************************************************************************
 ** \lpuart 的sclk时钟源选择
 ******************************************************************************/

typedef enum en_lpuart_sclk_sel
{
    LPUart_Pclk   = 0u,
	LPUart_Pclk_1 = 1u,
	LPUart_Xtl    = 2u,//外部低速晶振
	LPUart_Rcl    = 3u,//内部低速晶振
} en_lpuart_sclksel_t;
/**
 ******************************************************************************
 ** \lpuart 时钟相关配置
 ******************************************************************************/
typedef struct stc_lpuart_sclk_sel
{
	en_lpuart_sclkprs_t enSclk_Prs;
	en_lpuart_sclksel_t enSclk_sel;
}stc_lpuart_sclk_sel_t;
/**
 ******************************************************************************
 ** \lpuart 的正常工作模式、多主机工作模式选择
 ******************************************************************************/

typedef enum en_lpuart_multimode
{
    LPUartNormal = 0u,         
    LPUartMulti  = 1u,          
} en_lpuart_multimode_t;
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
 ** \lpuart 的工作模式及从机地址及地址掩码
 ******************************************************************************/

typedef struct stc_lpuart_multimode
{
	en_lpuart_multimode_t enMulti_mode;
	uint8_t 			u8SlaveAddr;
	uint8_t				u8SaddEn;
}stc_lpuart_multimode_t;

/**
 ******************************************************************************
 ** \lpuart 正常工作模式or低功耗工作模式配置
 *****************************************************************************/
typedef enum en_lpuart_lpmode
{
    LPUartNoLPMode = 0u,
    LPUartLPMode = 1u,
}en_lpuart_lpmode_t;

/**
 ******************************************************************************
 ** \lpuart 的4种工作模式配置
 ******************************************************************************/	
typedef enum en_lpuart_mode
{
    LPUartMode0 = 0u,         
    LPUartMode1 = 1u,
    LPUartMode2 = 2u,
    LPUartMode3 = 3u,
} en_lpuart_mode_t;
/**
 ******************************************************************************
 ** \lpuart 模式配置结构
 ******************************************************************************/
typedef struct stc_lpuart_mode
{
  en_lpuart_lpmode_t enLpMode;
  en_lpuart_mode_t   enMode;
}stc_lpuart_mode_t;
/**
 ******************************************************************************
 ** \lpuart 的发送或接收功能使能
 ******************************************************************************/

typedef enum en_lpuart_func
{
    LPUartTx = 0u,       
    LPUartRx = 1u,
}en_lpuart_func_t;
/**
 ******************************************************************************
 ** \lpuart 的发送或接收中断使能
 ******************************************************************************/

typedef enum en_lpuart_irq_sel
{
    LPUartTxIrq       = 0u,         
    LPUartRxIrq       = 1u,  
    LPUartTxShiftNull = 2u,
}en_lpuart_irq_sel_t;
/**
 ******************************************************************************
 ** \lpuart 的发送或接收中断结构
 ******************************************************************************/
typedef struct stc_lpuart_irq_en
{
    boolean_t bTxIrq;          ///< UART TX interrupt
    boolean_t bRxIrq;          ///< UART RX interrupt
}stc_lpuart_irq_en_t;
/**
 ******************************************************************************
 ** \lpuart 的中断处理函数接口
 ******************************************************************************/

typedef struct stc_lpuart_irq_cb
{
    func_ptr_t pfnTxIrqCb;      
    func_ptr_t pfnRxErrIrqCb;   
    func_ptr_t pfnRxIrqCb;     
}stc_lpuart_irq_cb_t,stc_lpuart_intern_cb_t;
/**
 ******************************************************************************
 ** \lpuart 的状态位
 ******************************************************************************/

typedef enum en_lpuart_status
{
    LPUartRFRAMEError    	= 0u,   
    LPUartTxIdle          = 1u,   
    LPUartRxFull          = 2u,   
    LPUartTxEmpty         = 3u,  
   
}en_lpuart_status_t;
/**
 ******************************************************************************
 ** \uart 通道波特率配置
 ******************************************************************************/
typedef struct stc_lpuart_baud_config
{
    uint8_t u8LpMode;
	uint8_t u8Mode;
	uint8_t u8Dbaud;
	uint32_t u32Baud;
}stc_lpuart_baud_config_t;
/**
 ******************************************************************************
 ** \lpuart 的整体配置结构体
 ******************************************************************************/

typedef struct stc_lpuart_config
{
	stc_lpuart_sclk_sel_t* pstcLpuart_clk;
    stc_lpuart_mode_t*   	pstcRunMode;    
    stc_lpuart_multimode_t* pstcMultiMode;     
    stc_lpuart_irq_cb_t* 	pstcIrqCb;          
    boolean_t         		bTouchNvic;           
} stc_lpuart_config_t;
//中断相关函数
en_result_t LPUart_EnableIrq(en_lpuart_irq_sel_t enIrqSel);
en_result_t LPUart_DisableIrq(en_lpuart_irq_sel_t enIntSel);
//void LPUart_IrqHandler(void);

// LPuart初始化总函数
en_result_t LPUart_Init(stc_lpuart_config_t* pstcConfig);
en_result_t LPUart_DeInit(boolean_t bTouchNvic);

//lpuart时钟及分频设置
en_result_t LPUart_SelSclk(en_lpuart_sclksel_t enClk);
en_result_t LPUart_SelSclkPrs(en_lpuart_sclkprs_t enPrs);

// lpuart波特率设置
// 波特率设置
uint16_t LPUart_SetBaudRate(uint32_t u32pclk,en_lpuart_sclkprs_t enprs,stc_lpuart_baud_config_t* pstcBaud);
uint32_t LPUart_GetBaudRate(void);

//lpuart 4种工作模式设置及多主机模式设置
en_result_t LPUart_SetMode(stc_lpuart_mode_t* pstcMode);
en_result_t LPUart_SetMultiMode(stc_lpuart_multimode_t* pstcMultiConfig);

//多主机模式的从机地址及地址掩码设置
en_result_t LPUart_SetMMDOrCk(en_uart_mmdorck_t enTb8);
en_result_t LPUart_SetSaddr(uint8_t u8Addr);
en_result_t LPUart_SetSaddrEn(uint8_t u8Addren);

//发送接收使能处理
en_result_t LPUart_EnableFunc(en_lpuart_func_t enFunc);
en_result_t LPUart_DisableFunc(en_lpuart_func_t enFunc);
// 获取清除及读写相关状态
boolean_t LPUart_GetStatus(en_lpuart_status_t enStatus);
en_result_t LPUart_ClrStatus(en_lpuart_status_t enStatus);
// 读取及发送数据处理函数
en_result_t LPUart_SendData(uint32_t u32Data);
uint32_t LPUart_ReceiveData(void);

#endif 


