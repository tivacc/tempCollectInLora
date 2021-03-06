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
/** \file spi.h
 **
 ** Headerfile for SPI functions
 **  
 **
 ** History:
 **   - 2017-05-17  1.0  Devi     First Version
 **
 *****************************************************************************/
#ifndef __SPI_H__
#define __SPI_H__

/******************************************************************************
 * Include files
 *****************************************************************************/
#include "ddl.h"
#include "interrupts_hc32l110.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//@{

/******************************************************************************
 * Global type definitions
 *****************************************************************************/
/**
 ******************************************************************************
 ** \brief SPI 功能使能设置
 ******************************************************************************/ 
typedef enum en_spi_en
{
    SpiEn     = 1u,  
    SpiDis    = 0u,  
}en_spi_en_t;
/**
 ******************************************************************************
 ** \brief SPI 模式配置
 ******************************************************************************/ 
typedef enum en_spi_mode
{
    SpiMaster = 1u,  
    SpiSlave  = 0u,  
}en_spi_mode_t;
/**
 ******************************************************************************
 ** \brief SPI 时钟极性设置
 ******************************************************************************/ 
typedef enum en_spi_cpol
{
    Spicpollow   = 0u,  // 极性为低
    Spicpolhigh  = 1u,  // 极性为高
}en_spi_cpol_t;
/**
 ******************************************************************************
 ** \brief SPI 时钟相位设置
 ******************************************************************************/ 
typedef enum en_spi_cpha
{
    Spicphafirst   = 0u,  //第一边沿采样
    Spicphasecond  = 1u,  //第二边沿采样
}en_spi_cpha_t;
/**
 ******************************************************************************
 ** \brief SPI 时钟分频配置
 *****************************************************************************/
typedef enum en_spi_clk_div
{
    SpiClkDiv2   = 0u,          
    SpiClkDiv4   = 1u,          
    SpiClkDiv8   = 2u,          
    SpiClkDiv16  = 3u,          
    SpiClkDiv32  = 4u,          
    SpiClkDiv64  = 5u,          
    SpiClkDiv128 = 6u,          
}en_spi_clk_div_t;

/**
 ******************************************************************************
 ** \brief SPI 片选脚电平选择
 *****************************************************************************/
typedef enum en_spi_cspin
{
    SpiCsLow  = 0u,            
    SpiCsHigh = 1u,            
}en_spi_cspin_t;

/**
 ******************************************************************************
 ** \brief SPI 状态
 *****************************************************************************/
typedef enum en_spi_status
{
    SpiIf              = 0x80,   
    SpiWcol            = 0x40,   
    SpiSserr           = 0x20,   
    SpiMdf             = 0x10,   
}en_spi_status_t;
/**
 ******************************************************************************
 ** \brief SPI 总体配置结构体
 *****************************************************************************/
typedef struct stc_spi_config
{
    boolean_t           bMasterMode;       ///< Master or slave mode
    uint8_t             u8BaudRate;      ///< Baud rate (bps)
    boolean_t           bCPOL;           ///< FALSE: CPOL High, 
                                         ///< TRUE:  CPOL Low
    boolean_t           bCPHA;           ///< FALSE: cpha first
                                         ///< TRUE:  cpha second
    boolean_t           bIrqEn;          ///< Pointer to SPI interrupt enable or disable
    boolean_t           bCsmode;         ///< Pointer to SPI CS mode
    func_ptr_t          pfnIrqCb;        ///< SPI  interrupt callback function

} stc_spi_config_t;

//SPI 中断
void Spi_IRQHandler(void);

//SPI 获取状态      
boolean_t Spi_GetStatus(en_spi_status_t enStatus);

//SPI初始化函数
en_result_t Spi_Init(stc_spi_config_t* pstcConfig);
//SPI关闭函数
en_result_t Spi_DeInit(void);
//SPI 配置主发送的电平
void Spi_SetCS(boolean_t bFlag);
//SPI 发送数据
en_result_t Spi_SendData(uint8_t u8Data);
//SPI 接收数据
uint8_t Spi_ReceiveData(void);

//@} // Spi Group

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/

