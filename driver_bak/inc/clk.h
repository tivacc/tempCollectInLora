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
/** \file clk.h
 **
 ** Headerfile for CLK functions
 **  
 **
 ** History:
 **   - 2017-05-02   Ronnie     First Version
 **
 ******************************************************************************/

#ifndef __CLK_H__
#define __CLK_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32l110.h"

#ifdef __cplusplus
extern "C"
{
#endif


/**
 *******************************************************************************
 ** function prototypes.
 ******************************************************************************/

/*******************************************************************************
 * Global definitions
 ******************************************************************************/
#define CLK_RCH_VAL             (4*1000*1000)      ///< internal high cr frequency
#define CLK_RCL_VAL             (38500)             ///< internal low cr frequency
#define CLK_XTL_VAL             (32768)             ///< external low cr frequency
#ifndef CLK_XTH_VAL
#define CLK_XTH_VAL             (32*1000*1000)      ///< external high cr frequency
#endif
/**
 *******************************************************************************
 ** \brief clk input source enum
 **
 ** \note
 ******************************************************************************/
typedef enum en_clk_source
{
    ClkRCH  = 0u,               ///< Internal High CR Clock Oscillator
    ClkXTH  = 1u,               ///< External High CR Clock Oscillator
    ClkRCL  = 2u,               ///< Internal Low CR Clock Oscillator
    ClkXTL = 3u,                ///< external low CR Clock Oscillator
}en_clk_source_t;

/**
 *******************************************************************************
 ** \brief clock division value enum
 ******************************************************************************/
typedef enum en_clk_div
{
    ClkDiv1 = 1,                ///< no div
    ClkDiv2 = 2,                ///< div 2
    ClkDiv4 = 4,
    ClkDiv8 = 8,
    ClkDiv16 = 16,
    ClkDiv32 = 32,
    ClkDiv64 = 64,
    ClkDiv128 = 128,
}en_clk_div_t;

/**
 *******************************************************************************
 ** \brief clock cycle value enum
 ******************************************************************************/
typedef enum en_clk_cycle
{
    ClkCycle4,
    ClkCycle16,
    ClkCycle64,
    ClkCycle256,
    ClkCycle1024,
    ClkCycle4096,
    ClkCycle16384,
}en_clk_cycle_t;

/**
 *******************************************************************************
 ** \brief clock frequency enum
 ** 
 ******************************************************************************/
typedef enum en_clk_freq
{
    ClkFreq32_8K,
    ClkFreq38_4K,
    ClkFreq4Mhz,
    ClkFreq6Mhz,
    ClkFreq8Mhz,
    ClkFreq16Mhz,
    ClkFreq24Mhz,
    ClkFreq32Mhz,
    ClkFreq12Mhz,
    ClkFreq20Mhz,
}en_clk_freq_t;

/**
 *******************************************************************************
 ** \brief clock module function setting event enumeration
 **
 ** \note  
 ******************************************************************************/
typedef enum en_clk_func
{
    ClkFuncWkupRCH,
    ClkFuncXTHEn,                       ///< enable XTH input Pin
    ClkFuncXTLEn,                       ///< enable XTL input Pin
    ClkFuncXTLAWSON,                    ///< enable XTL always on if enabled
    ClkFuncFaultEn,                     ///< enable clk failure detect.
    ClkFuncRtcLPWEn,                    ///< enable RTC lowpower mode.
    ClkFuncLockUpEn,                    ///< enable reset when PC fail.
    ClkFuncRstPinIOEn,                  ///< reset pin as IO
    ClkFuncSwdPinIOEn,                  ///< swd pin as IO.
}en_clk_func_t;


typedef enum en_clk_peripheral_gate
{
    ClkPeripheralUart0      = 0u,
    ClkPeripheralUart1      = 1u,
    ClkPeripheralLpUart     = 2u,
    ClkPeripheralI2c        = 4u,
    ClkPeripheralSpi        = 6u,
    ClkPeripheralBt         = 8u,
    ClkPeripheralLpTim      = 9u,
    ClkPeripheralAdt        = 10u,
    ClkPeripheralPca        = 14u,
    ClkPeripheralWdt        = 15u,
    ClkPeripheralAdcBgr     = 16u,
    ClkPeripheralVcLvd      = 17u,
    ClkPeripheralRtc        = 20u,
    ClkPeripheralClkTrim    = 21u,
    ClkPeripheralTick       = 24u,
    ClkPeripheralSWD        = 25u,
    ClkPeripheralCrc        = 26u,
    ClkPeripheralGpio       = 28u,
    ClkPeripheralRam        = 30u,
    ClkPeripheralFlash      = 31u,
}en_clk_peripheral_gate_t;

///< \todo  need to delete??
typedef enum
{
    ClkDBGTim0,
    ClkDBGTim1,
    ClkDBGTim2,
    ClkDBGLpTim,
    ClkDBGTim4,
    ClkDBGTim5,
    ClkDBGTim6,
    ClkDBGTPca,
    ClkDBGTWdt,
    ClkDBGTRtc,
    ClkDBGTTick,
}en_clk_debug_t;


typedef struct
{
    en_clk_source_t     enClk;      //XTL  -> XTL
                                    //RCL  -> 38.5k
                                    //RCH  -> hclk/8
                                    //XTH  -> XTH
    boolean_t           bNoRef;     //TRUE  :  use hclk 
                                    //FALSE :  use reference clock
    uint32_t            u32LoadVal; 
}stc_clk_systickcfg_t;


typedef struct
{
    en_clk_source_t enClkSrc;
    en_clk_div_t    enHClkDiv;
    en_clk_div_t    enPClkDiv;
}stc_clk_config_t;
/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
//clk init/deinit
en_result_t Clk_Init(stc_clk_config_t *pstcCfg);
en_result_t Clk_DeInit(void);

//clk source basic function
en_result_t Clk_Enable(en_clk_source_t enSource, boolean_t bFlag);
en_result_t Clk_SetSource(en_clk_source_t enSource);
boolean_t Clk_GetClkRdy(en_clk_source_t enSource);
en_result_t Clk_SetXTH_StableTime(en_clk_cycle_t enCycle);
en_result_t Clk_SetRCL_StableTime(en_clk_cycle_t enCycle);
en_result_t Clk_SetXTL_StableTime(en_clk_cycle_t enCycle);

//clk switch advanced function
en_result_t Clk_SwitchTo(en_clk_source_t enSource);

en_result_t Clk_SetRCHFreq(en_clk_freq_t enVal);
en_result_t Clk_SetRCLFreq(en_clk_freq_t enVal);

//clk frequency function
en_result_t Clk_SetHClkDiv(uint8_t u8Div);
en_result_t Clk_SetPClkDiv(uint8_t u8Div);

uint32_t Clk_GetHClkFreq(void);
uint32_t Clk_GetPClkFreq(void);

//peripheral function
en_result_t Clk_SetPeripheralGate(en_clk_peripheral_gate_t enPeripheral,
                                  boolean_t bFlag);
boolean_t Clk_GetPeripheralGate(en_clk_peripheral_gate_t enPeripheral);


//clock function enable/disable
en_result_t Clk_SetFunc(en_clk_func_t enFunc, boolean_t bFlag);

en_result_t Clk_SysTickConfig(stc_clk_systickcfg_t *pstcCfg);
en_result_t Clk_DebugClk(en_clk_debug_t enPeri, boolean_t bFlag);

en_result_t Clk_SetRTCAdjustClkFreq(uint32_t u32val);
//@} // ClkGroup

#ifdef __cplusplus
#endif


#endif /* __CLK_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


