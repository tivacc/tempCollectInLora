/*******************************************************************************
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
/** \file gpio.h
 **
 ** GPIO driver
 ** @link GPIO Group Some description @endlink
 **
 **   - 2017-05-04
 **
 ******************************************************************************/

#ifndef __GPIO_H__
#define __GPIO_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32l110.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup GPIO Group
 **
 **
 ******************************************************************************/
//@{
#define GPIO_GPSZ       (0x40)

/*******************************************************************************
 * Global type definitions
 ******************************************************************************/
typedef enum
{
    GpioDirOut,
    GpioDirIn,
}en_gpio_dir_t;

typedef enum
{
    GpioIrqHigh     = 1 << 0,
    GpioIrqLow      = 1 << 1,
    GpioIrqRising   = 1 << 2,
    GpioIrqFalling  = 1 << 3,
}en_gpio_irqtype_t;

typedef enum
{
    GpioCtrlIESEL,              ///< >0:    deep sleep
                                ///< =0:    active sleep
    GpioCtrlIRPOL,              ///< >0:    reverse output
                                ///< =0:    normal output
    GpioCtrlHclkEn,             ///< >0:    output enable
                                ///< =0     output disable
    GpioCtrlPclkEn,             ///< >0:    output enable
                                ///< =0     output disable
    GpioCtrlHclkSel,            ///< div value
    GpioCtrlPclkSel,            ///< div value
}en_gpio_ctrlext_t;


typedef enum
{
    GpioCtrl1SrcP35 = 0x1,
    GpioCtrl1SrcP36 = 0x2,
    GpioCtrl1SrcP01 = 0x3,
    GpioCtrl1SrcP02 = 0x4,
    GpioCtrl1SrcP03 = 0x5,
    GpioCtrl1SrcP15 = 0x6,
    GpioCtrl1SrcP14 = 0x7,
    GpioCtrl1SrcP23 = 0x8,
    GpioCtrl1SrcP24 = 0x9,
    GpioCtrl1SrcP25 = 0xA,
    GpioCtrl1SrcP26 = 0xB,
    GpioCtrl1SrcP27 = 0xC,
    GpioCtrl1SrcP31 = 0xD,
    GpioCtrl1SrcP32 = 0xE,
    GpioCtrl1SrcP33 = 0xF,

}en_gpio_ctrl1src_t;




typedef enum
{
    GpioSignalCAP4,
    GpioSignalCAP3,
    GpioSignalCAP2,
    GpioSignalCAP1,
    GpioSignalCAP0,
    GpioSignalTm6A2,
    GpioSignalTm6A1,
    GpioSignalTm6A0,
    GpioSignalTm6B2,
    GpioSignalTm6B1,
    GpioSignalTm6B0,
    GpioSignalTm3,
    GpioSignalTm2,
    GpioSignalTm1,
    GpioSignalTm0,
}en_gpio_target_t;


typedef enum
{
    GpioTMxIn = 0,
    GpioPcaCapxIn = 0,
    GpioUart0RxdIn,
    GpioUart1RxdIn,
    GpioUart2RxdIn,
}en_gpio_timex_in_t;


/*******************************************************************************
 * Global definitions
 ******************************************************************************/

en_result_t Gpio_SetAnalog(uint8_t u8Port, uint8_t u8Pin, boolean_t bFlag);

en_result_t Gpio_InitIO(uint8_t u8Port, uint8_t u8Pin,
                        en_gpio_dir_t enDir);

en_result_t Gpio_InitIOExt(uint8_t u8Port, uint8_t u8Pin,
                        en_gpio_dir_t enDir,
                        boolean_t bPullup,
                        boolean_t bPulldown,
                        boolean_t bOdr,
                        boolean_t bDrive);

void Gpio_SetIO(uint8_t u8Port, uint8_t u8Pin,boolean_t bVal);
boolean_t Gpio_GetIO(uint8_t u8Port, uint8_t u8Pin);

en_result_t Gpio_EnableIrq(uint8_t u8Port,
                           uint8_t u8Pin,
                           en_gpio_irqtype_t enType);

en_result_t Gpio_DisableIrq(uint8_t u8Port,
                            uint8_t u8Pin,
                            en_gpio_irqtype_t enType);

boolean_t Gpio_GetIrqStat(uint8_t u8Port,uint8_t u8Pin);
en_result_t Gpio_ClearIrq(uint8_t u8Port,uint8_t u8Pin);


en_result_t Gpio_SetCtrlExt(en_gpio_ctrlext_t enFunc, uint8_t u8val);
en_result_t Gpio_SetSsnSel(en_gpio_ctrl1src_t enSrc);
en_result_t Gpio_SetExtClkSel(en_gpio_ctrl1src_t enSrc);
en_result_t Gpio_SetSignalSource(en_gpio_target_t enTarget, en_gpio_timex_in_t enType);

/*
    pin function configure macro
*/
#define Gpio_SetFunc_UART0RX_P01()          do{\
                                                M0P_GPIO->P0ADS_f.P01 = 0;\
                                                M0P_GPIO->P01_SEL_f.SEL = 1;\
                                                M0P_GPIO->P0DIR_f.P01 = 1;\
                                            }while(0);

#define Gpio_SetFunc_I2CDAT_P01()           do{\
                                                M0P_GPIO->P0ADS_f.P01 = 0;\
                                                M0P_GPIO->P01_SEL_f.SEL = 2;\
                                                M0P_GPIO->P0DIR_f.P01 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART1TX_P01()          do{\
                                                M0P_GPIO->P0ADS_f.P01 = 0;\
                                                M0P_GPIO->P01_SEL_f.SEL = 3;\
                                                M0P_GPIO->P0DIR_f.P01 = 0;\
                                            }while(0);

#define Gpio_SetFunc_TM0TOGGLE_P01()        do{\
                                                M0P_GPIO->P0ADS_f.P01 = 0;\
                                                M0P_GPIO->P01_SEL_f.SEL = 4;\
                                                M0P_GPIO->P0DIR_f.P01 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IB1_P01()              do{\
                                                M0P_GPIO->P0ADS_f.P01 = 0;\
                                                M0P_GPIO->P01_SEL_f.SEL = 5;\
                                                M0P_GPIO->P0DIR_f.P01 = 0;\
                                            }while(0);

#define Gpio_SetFunc_SPICLK_P01()           do{\
                                                M0P_GPIO->P0ADS_f.P01 = 0;\
                                                M0P_GPIO->P01_SEL_f.SEL = 6;\
                                                M0P_GPIO->P0DIR_f.P01 = 0;\
                                            }while(0);

#define Gpio_SetFunc_EXT2_P01()             do{\
                                                M0P_GPIO->P0ADS_f.P01 = 0;\
                                                M0P_GPIO->P01_SEL_f.SEL = 7;\
                                                M0P_GPIO->P0DIR_f.P01 = 1;\
                                            }while(0);

#define Gpio_SetFunc_UART0TX_P02()          do{\
                                                M0P_GPIO->P0ADS_f.P02 = 0;\
                                                M0P_GPIO->P02_SEL_f.SEL = 1;\
                                                M0P_GPIO->P0DIR_f.P02 = 0;\
                                            }while(0);

#define Gpio_SetFunc_I2CCLK_P02()           do{\
                                                M0P_GPIO->P0ADS_f.P02 = 0;\
                                                M0P_GPIO->P02_SEL_f.SEL = 2;\
                                                M0P_GPIO->P0DIR_f.P02 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART1TX_P02()          do{\
                                                M0P_GPIO->P0ADS_f.P02 = 0;\
                                                M0P_GPIO->P02_SEL_f.SEL = 3;\
                                                M0P_GPIO->P0DIR_f.P02 = 0;\
                                            }while(0);

#define Gpio_SetFunc_TM0TOGGLEB_P02()       do{\
                                                M0P_GPIO->P0ADS_f.P02 = 0;\
                                                M0P_GPIO->P02_SEL_f.SEL = 4;\
                                                M0P_GPIO->P0DIR_f.P02 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IA2_P02()              do{\
                                                M0P_GPIO->P0ADS_f.P02 = 0;\
                                                M0P_GPIO->P02_SEL_f.SEL = 5;\
                                                M0P_GPIO->P0DIR_f.P02 = 0;\
                                            }while(0);

#define Gpio_SetFunc_SPICS_P02()            do{\
                                                M0P_GPIO->P0ADS_f.P02 = 0;\
                                                M0P_GPIO->P02_SEL_f.SEL = 6;\
                                                M0P_GPIO->P0DIR_f.P02 = 0;\
                                            }while(0);

#define Gpio_SetFunc_GATE2_P02()            do{\
                                                M0P_GPIO->P0ADS_f.P02 = 0;\
                                                M0P_GPIO->P02_SEL_f.SEL = 7;\
                                                M0P_GPIO->P0DIR_f.P02 = 1;\
                                            }while(0);

#define Gpio_SetFunc_CAP3_P03()             do{\
                                                M0P_GPIO->P0ADS_f.P03 = 0;\
                                                M0P_GPIO->P03_SEL_f.SEL = 1;\
                                                M0P_GPIO->P0DIR_f.P03 = 1;\
                                            }while(0);

#define Gpio_SetFunc_SPICS_P03()            do{\
                                                M0P_GPIO->P0ADS_f.P03 = 0;\
                                                M0P_GPIO->P03_SEL_f.SEL = 2;\
                                                M0P_GPIO->P0DIR_f.P03 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IB2_P03()              do{\
                                                M0P_GPIO->P0ADS_f.P03 = 0;\
                                                M0P_GPIO->P03_SEL_f.SEL = 3;\
                                                M0P_GPIO->P0DIR_f.P03 = 0;\
                                            }while(0);

#define Gpio_SetFunc_EXT3_P03()             do{\
                                                M0P_GPIO->P0ADS_f.P03 = 0;\
                                                M0P_GPIO->P03_SEL_f.SEL = 4;\
                                                M0P_GPIO->P0DIR_f.P03 = 1;\
                                            }while(0);

#define Gpio_SetFunc_RTC1HZ_P03()           do{\
                                                M0P_GPIO->P0ADS_f.P03 = 0;\
                                                M0P_GPIO->P03_SEL_f.SEL = 5;\
                                                M0P_GPIO->P0DIR_f.P03 = 0;\
                                            }while(0);

#define Gpio_SetFunc_PCAECI_P03()           do{\
                                                M0P_GPIO->P0ADS_f.P03 = 0;\
                                                M0P_GPIO->P03_SEL_f.SEL = 6;\
                                                M0P_GPIO->P0DIR_f.P03 = 0;\
                                            }while(0);

#define Gpio_SetFunc_VC1OUT_P03()           do{\
                                                M0P_GPIO->P0ADS_f.P03 = 0;\
                                                M0P_GPIO->P03_SEL_f.SEL = 7;\
                                                M0P_GPIO->P0DIR_f.P03 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CMP3_P03()             do{\
                                                M0P_GPIO->P0ADS_f.P03 = 0;\
                                                M0P_GPIO->P03_SEL_f.SEL = 1;\
                                                M0P_GPIO->P0DIR_f.P03 = 0;\
                                            }while(0);

#define Gpio_SetFunc_I2CCLK_P14()           do{\
                                                M0P_GPIO->P1ADS_f.P14 = 0;\
                                                M0P_GPIO->P14_SEL_f.SEL = 1;\
                                                M0P_GPIO->P1DIR_f.P14 = 0;\
                                            }while(0);

#define Gpio_SetFunc_TM2TOGGLEB_P14()       do{\
                                                M0P_GPIO->P1ADS_f.P14 = 0;\
                                                M0P_GPIO->P14_SEL_f.SEL = 2;\
                                                M0P_GPIO->P1DIR_f.P14 = 0;\
                                            }while(0);

#define Gpio_SetFunc_PCAECI_P14()           do{\
                                                M0P_GPIO->P1ADS_f.P14 = 0;\
                                                M0P_GPIO->P14_SEL_f.SEL = 3;\
                                                M0P_GPIO->P1DIR_f.P14 = 0;\
                                            }while(0);

#define Gpio_SetFunc_ADCRDY_P14()           do{\
                                                M0P_GPIO->P1ADS_f.P14 = 0;\
                                                M0P_GPIO->P14_SEL_f.SEL = 4;\
                                                M0P_GPIO->P1DIR_f.P14 = 0;\
                                            }while(0);

#define Gpio_SetFunc_SPICS_P14()            do{\
                                                M0P_GPIO->P1ADS_f.P14 = 0;\
                                                M0P_GPIO->P14_SEL_f.SEL = 5;\
                                                M0P_GPIO->P1DIR_f.P14 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART0TX_P14()          do{\
                                                M0P_GPIO->P1ADS_f.P14 = 0;\
                                                M0P_GPIO->P14_SEL_f.SEL = 6;\
                                                M0P_GPIO->P1DIR_f.P14 = 0;\
                                            }while(0);

#define Gpio_SetFunc_I2CDAT_P15()           do{\
                                                M0P_GPIO->P1ADS_f.P15 = 0;\
                                                M0P_GPIO->P15_SEL_f.SEL = 1;\
                                                M0P_GPIO->P1DIR_f.P15 = 0;\
                                            }while(0);

#define Gpio_SetFunc_TM2TOGGLE_P15()        do{\
                                                M0P_GPIO->P1ADS_f.P15 = 0;\
                                                M0P_GPIO->P15_SEL_f.SEL = 2;\
                                                M0P_GPIO->P1DIR_f.P15 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IB0_P15()              do{\
                                                M0P_GPIO->P1ADS_f.P15 = 0;\
                                                M0P_GPIO->P15_SEL_f.SEL = 3;\
                                                M0P_GPIO->P1DIR_f.P15 = 0;\
                                            }while(0);

#define Gpio_SetFunc_GATE3_P15()            do{\
                                                M0P_GPIO->P1ADS_f.P15 = 0;\
                                                M0P_GPIO->P15_SEL_f.SEL = 4;\
                                                M0P_GPIO->P1DIR_f.P15 = 1;\
                                            }while(0);

#define Gpio_SetFunc_SPICLK_P15()           do{\
                                                M0P_GPIO->P1ADS_f.P15 = 0;\
                                                M0P_GPIO->P15_SEL_f.SEL = 5;\
                                                M0P_GPIO->P1DIR_f.P15 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART0RX_P15()          do{\
                                                M0P_GPIO->P1ADS_f.P15 = 0;\
                                                M0P_GPIO->P15_SEL_f.SEL = 6;\
                                                M0P_GPIO->P1DIR_f.P15 = 1;\
                                            }while(0);

#define Gpio_SetFunc_LVDOUT_P15()           do{\
                                                M0P_GPIO->P1ADS_f.P15 = 0;\
                                                M0P_GPIO->P15_SEL_f.SEL = 7;\
                                                M0P_GPIO->P1DIR_f.P15 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IA2_P23()              do{\
                                                M0P_GPIO->P2ADS_f.P23 = 0;\
                                                M0P_GPIO->P23_SEL_f.SEL = 1;\
                                                M0P_GPIO->P2DIR_f.P23 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IB0_P23()              do{\
                                                M0P_GPIO->P2ADS_f.P23 = 0;\
                                                M0P_GPIO->P23_SEL_f.SEL = 2;\
                                                M0P_GPIO->P2DIR_f.P23 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IA0_P23()              do{\
                                                M0P_GPIO->P2ADS_f.P23 = 0;\
                                                M0P_GPIO->P23_SEL_f.SEL = 3;\
                                                M0P_GPIO->P2DIR_f.P23 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CAP0_P23()             do{\
                                                M0P_GPIO->P2ADS_f.P23 = 0;\
                                                M0P_GPIO->P23_SEL_f.SEL = 4;\
                                                M0P_GPIO->P2DIR_f.P23 = 1;\
                                            }while(0);

#define Gpio_SetFunc_SPIMISO_P23()          do{\
                                                M0P_GPIO->P2ADS_f.P23 = 0;\
                                                M0P_GPIO->P23_SEL_f.SEL = 5;\
                                                M0P_GPIO->P2DIR_f.P23 = 1;\
                                            }while(0);

#define Gpio_SetFunc_UART1TX_P23()          do{\
                                                M0P_GPIO->P2ADS_f.P23 = 0;\
                                                M0P_GPIO->P23_SEL_f.SEL = 6;\
                                                M0P_GPIO->P2DIR_f.P23 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IROUT_P23(x)           do{\
                                                M0P_GPIO->P2ADS_f.P23 = 0;\
                                                M0P_GPIO->P23_SEL_f.SEL = 7;\
                                                M0P_GPIO->P2DIR_f.P23 = 0;\
                                                M0P_GPIO->CTRL1_f.IR_POL = (x);\
                                            }while(0);

#define Gpio_SetFunc_CMP0_P23()             do{\
                                                M0P_GPIO->P2ADS_f.P23 = 0;\
                                                M0P_GPIO->P23_SEL_f.SEL = 1;\
                                                M0P_GPIO->P2DIR_f.P23 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IB0_P24()              do{\
                                                M0P_GPIO->P2ADS_f.P24 = 0;\
                                                M0P_GPIO->P24_SEL_f.SEL = 1;\
                                                M0P_GPIO->P2DIR_f.P24 = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_IB1_P24()              do{\
                                                M0P_GPIO->P2ADS_f.P24 = 0;\
                                                M0P_GPIO->P24_SEL_f.SEL = 2;\
                                                M0P_GPIO->P2DIR_f.P24 = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_HCLKOUT_P24()          do{\
                                                M0P_GPIO->P2ADS_f.P24 = 0;\
                                                M0P_GPIO->P24_SEL_f.SEL = 3;\
                                                M0P_GPIO->P2DIR_f.P24 = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 1;\
                                            }while(0);

#define Gpio_SetFunc_CAP1_P24()             do{\
                                                M0P_GPIO->P2ADS_f.P24 = 0;\
                                                M0P_GPIO->P24_SEL_f.SEL = 4;\
                                                M0P_GPIO->P2DIR_f.P24 = 1;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_SPIMOSI_P24()          do{\
                                                M0P_GPIO->P2ADS_f.P24 = 0;\
                                                M0P_GPIO->P24_SEL_f.SEL = 5;\
                                                M0P_GPIO->P2DIR_f.P24 = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART1RX_P24()          do{\
                                                M0P_GPIO->P2ADS_f.P24 = 0;\
                                                M0P_GPIO->P24_SEL_f.SEL = 6;\
                                                M0P_GPIO->P2DIR_f.P24 = 1;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_VC2OUT_P24()           do{\
                                                M0P_GPIO->P2ADS_f.P24 = 0;\
                                                M0P_GPIO->P24_SEL_f.SEL = 7;\
                                                M0P_GPIO->P2DIR_f.P24 = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_CMP1_P24()             do{\
                                                M0P_GPIO->P2ADS_f.P24 = 0;\
                                                M0P_GPIO->P24_SEL_f.SEL = 1;\
                                                M0P_GPIO->P2DIR_f.P24 = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_SPICLK_P25()           do{\
                                                M0P_GPIO->P2ADS_f.P25 = 0;\
                                                M0P_GPIO->P25_SEL_f.SEL = 1;\
                                                M0P_GPIO->P2DIR_f.P25 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CAP0_P25()             do{\
                                                M0P_GPIO->P2ADS_f.P25 = 0;\
                                                M0P_GPIO->P25_SEL_f.SEL = 2;\
                                                M0P_GPIO->P2DIR_f.P25 = 1;\
                                            }while(0);

#define Gpio_SetFunc_IA1_P25()              do{\
                                                M0P_GPIO->P2ADS_f.P25 = 0;\
                                                M0P_GPIO->P25_SEL_f.SEL = 3;\
                                                M0P_GPIO->P2DIR_f.P25 = 0;\
                                            }while(0);

#define Gpio_SetFunc_LVDOUT_P25()           do{\
                                                M0P_GPIO->P2ADS_f.P25 = 0;\
                                                M0P_GPIO->P25_SEL_f.SEL = 4;\
                                                M0P_GPIO->P2DIR_f.P25 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART2RX_P25()          do{\
                                                M0P_GPIO->P2ADS_f.P25 = 0;\
                                                M0P_GPIO->P25_SEL_f.SEL = 5;\
                                                M0P_GPIO->P2DIR_f.P25 = 1;\
                                            }while(0);

#define Gpio_SetFunc_I2CDAT_P25()           do{\
                                                M0P_GPIO->P2ADS_f.P25 = 0;\
                                                M0P_GPIO->P25_SEL_f.SEL = 6;\
                                                M0P_GPIO->P2DIR_f.P25 = 0;\
                                            }while(0);

#define Gpio_SetFunc_GATE1_P25()            do{\
                                                M0P_GPIO->P2ADS_f.P25 = 0;\
                                                M0P_GPIO->P25_SEL_f.SEL = 7;\
                                                M0P_GPIO->P2DIR_f.P25 = 1;\
                                            }while(0);

#define Gpio_SetFunc_CMP0_P25()             do{\
                                                M0P_GPIO->P2ADS_f.P25 = 0;\
                                                M0P_GPIO->P25_SEL_f.SEL = 1;\
                                                M0P_GPIO->P2DIR_f.P25 = 0;\
                                            }while(0);

#define Gpio_SetFunc_SPIMOSI_P26()          do{\
                                                M0P_GPIO->P2ADS_f.P26 = 0;\
                                                M0P_GPIO->P26_SEL_f.SEL = 1;\
                                                M0P_GPIO->P2DIR_f.P26 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IA0_P26()              do{\
                                                M0P_GPIO->P2ADS_f.P26 = 0;\
                                                M0P_GPIO->P26_SEL_f.SEL = 2;\
                                                M0P_GPIO->P2DIR_f.P26 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IB1_P26()              do{\
                                                M0P_GPIO->P2ADS_f.P26 = 0;\
                                                M0P_GPIO->P26_SEL_f.SEL = 3;\
                                                M0P_GPIO->P2DIR_f.P26 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CAP2_P26()             do{\
                                                M0P_GPIO->P2ADS_f.P26 = 0;\
                                                M0P_GPIO->P26_SEL_f.SEL = 4;\
                                                M0P_GPIO->P2DIR_f.P26 = 1;\
                                            }while(0);

#define Gpio_SetFunc_UART2TX_P26()          do{\
                                                M0P_GPIO->P2ADS_f.P26 = 0;\
                                                M0P_GPIO->P26_SEL_f.SEL = 5;\
                                                M0P_GPIO->P2DIR_f.P26 = 0;\
                                            }while(0);

#define Gpio_SetFunc_I2CCLK_P26()           do{\
                                                M0P_GPIO->P2ADS_f.P26 = 0;\
                                                M0P_GPIO->P26_SEL_f.SEL = 6;\
                                                M0P_GPIO->P2DIR_f.P26 = 0;\
                                            }while(0);

#define Gpio_SetFunc_EXT1_P26()             do{\
                                                M0P_GPIO->P2ADS_f.P26 = 0;\
                                                M0P_GPIO->P26_SEL_f.SEL = 7;\
                                                M0P_GPIO->P2DIR_f.P26 = 1;\
                                            }while(0);

#define Gpio_SetFunc_CMP2_P26()             do{\
                                                M0P_GPIO->P2ADS_f.P26 = 0;\
                                                M0P_GPIO->P26_SEL_f.SEL = 1;\
                                                M0P_GPIO->P2DIR_f.P26 = 0;\
                                            }while(0);

#define Gpio_SetFunc_SPIMISO_P27()          do{\
                                                M0P_GPIO->P2ADS_f.P27 = 0;\
                                                M0P_GPIO->P27_SEL_f.SEL = 1;\
                                                M0P_GPIO->P2DIR_f.P27 = 1;\
                                            }while(0);

#define Gpio_SetFunc_IA1_P27()              do{\
                                                M0P_GPIO->P2ADS_f.P27 = 0;\
                                                M0P_GPIO->P27_SEL_f.SEL = 2;\
                                                M0P_GPIO->P2DIR_f.P27 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IA2_P27()              do{\
                                                M0P_GPIO->P2ADS_f.P27 = 0;\
                                                M0P_GPIO->P27_SEL_f.SEL = 3;\
                                                M0P_GPIO->P2DIR_f.P27 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CAP3_P27()             do{\
                                                M0P_GPIO->P2ADS_f.P27 = 0;\
                                                M0P_GPIO->P27_SEL_f.SEL = 4;\
                                                M0P_GPIO->P2DIR_f.P27 = 1;\
                                            }while(0);

#define Gpio_SetFunc_UART0RX_P27()          do{\
                                                M0P_GPIO->P2ADS_f.P27 = 0;\
                                                M0P_GPIO->P27_SEL_f.SEL = 5;\
                                                M0P_GPIO->P2DIR_f.P27 = 1;\
                                            }while(0);

#define Gpio_SetFunc_IRC24MOUT_P27()        do{\
                                                M0P_GPIO->P2ADS_f.P27 = 0;\
                                                M0P_GPIO->P27_SEL_f.SEL = 6;\
                                                M0P_GPIO->P2DIR_f.P27 = 0;\
                                            }while(0);

#define Gpio_SetFunc_X32MOUT_P27()          do{\
                                                M0P_GPIO->P2ADS_f.P27 = 0;\
                                                M0P_GPIO->P27_SEL_f.SEL = 7;\
                                                M0P_GPIO->P2DIR_f.P27 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CMP3_P27()             do{\
                                                M0P_GPIO->P2ADS_f.P27 = 0;\
                                                M0P_GPIO->P27_SEL_f.SEL = 1;\
                                                M0P_GPIO->P2DIR_f.P27 = 0;\
                                            }while(0);

#define Gpio_SetFunc_TM3TOGGLE_P31()        do{\
                                                M0P_GPIO->P3ADS_f.P31 = 0;\
                                                M0P_GPIO->P31_SEL_f.SEL = 1;\
                                                M0P_GPIO->P3DIR_f.P31 = 0;\
                                                M0P_GPIO->CTRL1_f.PCLK_EN = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_PCAECI_P31()           do{\
                                                M0P_GPIO->P3ADS_f.P31 = 0;\
                                                M0P_GPIO->P31_SEL_f.SEL = 2;\
                                                M0P_GPIO->P3DIR_f.P31 = 0;\
                                                M0P_GPIO->CTRL1_f.PCLK_EN = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_PCLKOUT_P31()          do{\
                                                M0P_GPIO->P3ADS_f.P31 = 0;\
                                                M0P_GPIO->P31_SEL_f.SEL = 3;\
                                                M0P_GPIO->P3DIR_f.P31 = 0;\
                                                M0P_GPIO->CTRL1_f.PCLK_EN = 1;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_VC1OUT_P31()           do{\
                                                M0P_GPIO->P3ADS_f.P31 = 0;\
                                                M0P_GPIO->P31_SEL_f.SEL = 4;\
                                                M0P_GPIO->P3DIR_f.P31 = 0;\
                                                M0P_GPIO->CTRL1_f.PCLK_EN = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART0TX_P31()          do{\
                                                M0P_GPIO->P3ADS_f.P31 = 0;\
                                                M0P_GPIO->P31_SEL_f.SEL = 5;\
                                                M0P_GPIO->P3DIR_f.P31 = 0;\
                                                M0P_GPIO->CTRL1_f.PCLK_EN = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_IRC38KOUT_P31()        do{\
                                                M0P_GPIO->P3ADS_f.P31 = 0;\
                                                M0P_GPIO->P31_SEL_f.SEL = 6;\
                                                M0P_GPIO->P3DIR_f.P31 = 0;\
                                                M0P_GPIO->CTRL1_f.PCLK_EN = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 0;\
                                            }while(0);

#define Gpio_SetFunc_HCLKOUT_P31()          do{\
                                                M0P_GPIO->P3ADS_f.P31 = 0;\
                                                M0P_GPIO->P31_SEL_f.SEL = 7;\
                                                M0P_GPIO->P3DIR_f.P31 = 0;\
                                                M0P_GPIO->CTRL1_f.PCLK_EN = 0;\
                                                M0P_GPIO->CTRL1_f.HCLK_EN = 1;\
                                            }while(0);

#define Gpio_SetFunc_TM3TOGGLEB_P32()       do{\
                                                M0P_GPIO->P3ADS_f.P32 = 0;\
                                                M0P_GPIO->P32_SEL_f.SEL = 1;\
                                                M0P_GPIO->P3DIR_f.P32 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CAP2_P32()             do{\
                                                M0P_GPIO->P3ADS_f.P32 = 0;\
                                                M0P_GPIO->P32_SEL_f.SEL = 2;\
                                                M0P_GPIO->P3DIR_f.P32 = 1;\
                                            }while(0);

#define Gpio_SetFunc_IB2_P32()              do{\
                                                M0P_GPIO->P3ADS_f.P32 = 0;\
                                                M0P_GPIO->P32_SEL_f.SEL = 3;\
                                                M0P_GPIO->P3DIR_f.P32 = 0;\
                                            }while(0);

#define Gpio_SetFunc_VC2OUT_P32()           do{\
                                                M0P_GPIO->P3ADS_f.P32 = 0;\
                                                M0P_GPIO->P32_SEL_f.SEL = 4;\
                                                M0P_GPIO->P3DIR_f.P32 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART1TX_P32()          do{\
                                                M0P_GPIO->P3ADS_f.P32 = 0;\
                                                M0P_GPIO->P32_SEL_f.SEL = 5;\
                                                M0P_GPIO->P3DIR_f.P32 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CAP4_P32()             do{\
                                                M0P_GPIO->P3ADS_f.P32 = 0;\
                                                M0P_GPIO->P32_SEL_f.SEL = 6;\
                                                M0P_GPIO->P3DIR_f.P32 = 1;\
                                            }while(0);

#define Gpio_SetFunc_RTC1HZ_P32()           do{\
                                                M0P_GPIO->P3ADS_f.P32 = 0;\
                                                M0P_GPIO->P32_SEL_f.SEL = 7;\
                                                M0P_GPIO->P3DIR_f.P32 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CMP2_P32()             do{\
                                                M0P_GPIO->P3ADS_f.P32 = 0;\
                                                M0P_GPIO->P32_SEL_f.SEL = 1;\
                                                M0P_GPIO->P3DIR_f.P32 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CMP4_P32()             do{\
                                                M0P_GPIO->P3ADS_f.P32 = 0;\
                                                M0P_GPIO->P32_SEL_f.SEL = 2;\
                                                M0P_GPIO->P3DIR_f.P32 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART2RX_P33()          do{\
                                                M0P_GPIO->P3ADS_f.P33 = 0;\
                                                M0P_GPIO->P33_SEL_f.SEL = 1;\
                                                M0P_GPIO->P3DIR_f.P33 = 1;\
                                            }while(0);

#define Gpio_SetFunc_CAP1_P33()             do{\
                                                M0P_GPIO->P3ADS_f.P33 = 0;\
                                                M0P_GPIO->P33_SEL_f.SEL = 2;\
                                                M0P_GPIO->P3DIR_f.P33 = 1;\
                                            }while(0);

#define Gpio_SetFunc_IB1_P33()              do{\
                                                M0P_GPIO->P3ADS_f.P33 = 0;\
                                                M0P_GPIO->P33_SEL_f.SEL = 3;\
                                                M0P_GPIO->P3DIR_f.P33 = 0;\
                                            }while(0);

#define Gpio_SetFunc_PCAECI_P33()           do{\
                                                M0P_GPIO->P3ADS_f.P33 = 0;\
                                                M0P_GPIO->P33_SEL_f.SEL = 4;\
                                                M0P_GPIO->P3DIR_f.P33 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART1RX_P33()          do{\
                                                M0P_GPIO->P3ADS_f.P33 = 0;\
                                                M0P_GPIO->P33_SEL_f.SEL = 5;\
                                                M0P_GPIO->P3DIR_f.P33 = 1;\
                                            }while(0);

#define Gpio_SetFunc_X32KOUT_P33()          do{\
                                                M0P_GPIO->P3ADS_f.P33 = 0;\
                                                M0P_GPIO->P33_SEL_f.SEL = 6;\
                                                M0P_GPIO->P3DIR_f.P33 = 0;\
                                            }while(0);

#define Gpio_SetFunc_TM1TOGGLEB_P33()       do{\
                                                M0P_GPIO->P3ADS_f.P33 = 0;\
                                                M0P_GPIO->P33_SEL_f.SEL = 7;\
                                                M0P_GPIO->P3DIR_f.P33 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CMP1_P33()             do{\
                                                M0P_GPIO->P3ADS_f.P33 = 0;\
                                                M0P_GPIO->P33_SEL_f.SEL = 1;\
                                                M0P_GPIO->P3DIR_f.P33 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CAP0_P34()             do{\
                                                M0P_GPIO->P3ADS_f.P34 = 0;\
                                                M0P_GPIO->P34_SEL_f.SEL = 1;\
                                                M0P_GPIO->P3DIR_f.P34 = 1;\
                                            }while(0);

#define Gpio_SetFunc_UART2TX_P34()          do{\
                                                M0P_GPIO->P3ADS_f.P34 = 0;\
                                                M0P_GPIO->P34_SEL_f.SEL = 2;\
                                                M0P_GPIO->P3DIR_f.P34 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IA1_P34()              do{\
                                                M0P_GPIO->P3ADS_f.P34 = 0;\
                                                M0P_GPIO->P34_SEL_f.SEL = 3;\
                                                M0P_GPIO->P3DIR_f.P34 = 0;\
                                            }while(0);

#define Gpio_SetFunc_EXT0_P34()             do{\
                                                M0P_GPIO->P3ADS_f.P34 = 0;\
                                                M0P_GPIO->P34_SEL_f.SEL = 4;\
                                                M0P_GPIO->P3DIR_f.P34 = 1;\
                                            }while(0);

#define Gpio_SetFunc_IA0_P34()              do{\
                                                M0P_GPIO->P3ADS_f.P34 = 0;\
                                                M0P_GPIO->P34_SEL_f.SEL = 5;\
                                                M0P_GPIO->P3DIR_f.P34 = 0;\
                                            }while(0);

#define Gpio_SetFunc_RTC1HZ_P34()           do{\
                                                M0P_GPIO->P3ADS_f.P34 = 0;\
                                                M0P_GPIO->P34_SEL_f.SEL = 6;\
                                                M0P_GPIO->P3DIR_f.P34 = 0;\
                                            }while(0);

#define Gpio_SetFunc_TM1TOGGLE_P34()        do{\
                                                M0P_GPIO->P3ADS_f.P34 = 0;\
                                                M0P_GPIO->P34_SEL_f.SEL = 7;\
                                                M0P_GPIO->P3DIR_f.P34 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CMP0_P34()             do{\
                                                M0P_GPIO->P3ADS_f.P34 = 0;\
                                                M0P_GPIO->P34_SEL_f.SEL = 1;\
                                                M0P_GPIO->P3DIR_f.P34 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART1TX_P35()          do{\
                                                M0P_GPIO->P3ADS_f.P35 = 0;\
                                                M0P_GPIO->P35_SEL_f.SEL = 1;\
                                                M0P_GPIO->P3DIR_f.P35 = 0;\
                                            }while(0);

#define Gpio_SetFunc_IB2_P35()              do{\
                                                M0P_GPIO->P3ADS_f.P35 = 0;\
                                                M0P_GPIO->P35_SEL_f.SEL = 2;\
                                                M0P_GPIO->P3DIR_f.P35 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART0TX_P35()          do{\
                                                M0P_GPIO->P3ADS_f.P35 = 0;\
                                                M0P_GPIO->P35_SEL_f.SEL = 3;\
                                                M0P_GPIO->P3DIR_f.P35 = 0;\
                                                M0P_GPIO->P3PU_f.P35 = 1;\
                                            }while(0);

#define Gpio_SetFunc_GATE0_P35()            do{\
                                                M0P_GPIO->P3ADS_f.P35 = 0;\
                                                M0P_GPIO->P35_SEL_f.SEL = 4;\
                                                M0P_GPIO->P3DIR_f.P35 = 1;\
                                            }while(0);

#define Gpio_SetFunc_IB0_P35()              do{\
                                                M0P_GPIO->P3ADS_f.P35 = 0;\
                                                M0P_GPIO->P35_SEL_f.SEL = 5;\
                                                M0P_GPIO->P3DIR_f.P35 = 0;\
                                            }while(0);

#define Gpio_SetFunc_SPIMISO_P35()          do{\
                                                M0P_GPIO->P3ADS_f.P35 = 0;\
                                                M0P_GPIO->P35_SEL_f.SEL = 6;\
                                                M0P_GPIO->P3DIR_f.P35 = 1;\
                                            }while(0);

#define Gpio_SetFunc_I2CDAT_P35()           do{\
                                                M0P_GPIO->P3ADS_f.P35 = 0;\
                                                M0P_GPIO->P35_SEL_f.SEL = 7;\
                                                M0P_GPIO->P3DIR_f.P35 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART1RX_P36()          do{\
                                                M0P_GPIO->P3ADS_f.P36 = 0;\
                                                M0P_GPIO->P36_SEL_f.SEL = 1;\
                                                M0P_GPIO->P3DIR_f.P36 = 1;\
                                                M0P_GPIO->P3PD_f.P36 = 1;\
                                            }while(0);

#define Gpio_SetFunc_IA2_P36()              do{\
                                                M0P_GPIO->P3ADS_f.P36 = 0;\
                                                M0P_GPIO->P36_SEL_f.SEL = 2;\
                                                M0P_GPIO->P3DIR_f.P36 = 0;\
                                            }while(0);

#define Gpio_SetFunc_UART0RX_P36()          do{\
                                                M0P_GPIO->P3ADS_f.P36 = 0;\
                                                M0P_GPIO->P36_SEL_f.SEL = 3;\
                                                M0P_GPIO->P3DIR_f.P36 = 1;\
                                            }while(0);

#define Gpio_SetFunc_CAP4_P36()             do{\
                                                M0P_GPIO->P3ADS_f.P36 = 0;\
                                                M0P_GPIO->P36_SEL_f.SEL = 4;\
                                                M0P_GPIO->P3DIR_f.P36 = 1;\
                                            }while(0);

#define Gpio_SetFunc_IA1_P36()              do{\
                                                M0P_GPIO->P3ADS_f.P36 = 0;\
                                                M0P_GPIO->P36_SEL_f.SEL = 5;\
                                                M0P_GPIO->P3DIR_f.P36 = 0;\
                                            }while(0);

#define Gpio_SetFunc_SPIMOSI_P36()          do{\
                                                M0P_GPIO->P3ADS_f.P36 = 0;\
                                                M0P_GPIO->P36_SEL_f.SEL = 6;\
                                                M0P_GPIO->P3DIR_f.P36 = 0;\
                                            }while(0);

#define Gpio_SetFunc_I2CCLK_P36()           do{\
                                                M0P_GPIO->P3ADS_f.P36 = 0;\
                                                M0P_GPIO->P36_SEL_f.SEL = 7;\
                                                M0P_GPIO->P3DIR_f.P36 = 0;\
                                            }while(0);

#define Gpio_SetFunc_CMP4_P36()             do{\
                                                M0P_GPIO->P3ADS_f.P36 = 0;\
                                                M0P_GPIO->P36_SEL_f.SEL = 1;\
                                                M0P_GPIO->P3DIR_f.P36 = 0;\
                                            }while(0);
/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/******************************************************************************
  Global function prototypes (definition in C source)
*******************************************************************************/


//@} // GPIO Group

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H__ */
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

