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
/******************************************************************************/
/** \file adc.h
 **
 ** Header file for AD Converter functions
 ** @link ADC Group Some description @endlink
 **
 **   - 2017-05-28 LiuHL    First Version
 **
 ******************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__


/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32l110.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//@{

/**
 ******************************************************************************
 ** \brief Global pre-processor symbols/macros ('#define')
******************************************************************************/

/**
 ******************************************************************************
 ** \brief scan channel enable bit
 *****************************************************************************/
#define ADC_SCAN_CH0_EN     (0x1)
#define ADC_SCAN_CH1_EN     (0x1<<1)
#define ADC_SCAN_CH2_EN     (0x1<<2)
#define ADC_SCAN_CH3_EN     (0x1<<3)
#define ADC_SCAN_CH4_EN     (0x1<<4)
#define ADC_SCAN_CH5_EN     (0x1<<5)
#define ADC_SCAN_CH6_EN     (0x1<<6)
#define ADC_SCAN_CH7_EN     (0x1<<7)


/**
 ******************************************************************************
 ** \brief Global type definitions
 *****************************************************************************/

 /**
 ******************************************************************************
 ** \brief ADC采样模式
 *****************************************************************************/
typedef enum en_adc_op_mode
{
    AdcNormalMode  = 0,        /*!< 单输入通道单次采样模式 */
    AdcContMode    = 1,        /*!< 单输入通道连续采样模式 */
    AdcScanMode    = 2,        /*!< 多输入通道扫描采样模式*/

} en_adc_op_mode_t;

/**
 ******************************************************************************
 ** \brief ADC时钟选择
 *****************************************************************************/
typedef enum en_adc_clk_sel
{
    AdcClkSysTDiv1  = 0,        /*!< PCLK */
    AdcClkSysTDiv2  = 1,        /*!< 1/2 PCLK */
    AdcClkSysTDiv4  = 2,        /*!< 1/4 PCLK */
    AdcClkSysTDiv8  = 3,        /*!< 1/8 PCLK */

} en_adc_clk_sel_t;

/**
 ******************************************************************************
 ** \brief ADC参考电压
 *****************************************************************************/
typedef enum en_adc_ref_vol_sel
{
    RefVolSelInBgr1p5 = 0,        /*!<内部参考电压1.5V(SPS<=200kHz)*/
    RefVolSelInBgr2p5 = 1,        /*!<内部参考电压2.5V(avdd>3V,SPS<=200kHz)*/
    RefVolSelExtern1  = 2,        /*!<外部输入(max avdd)   P3.6*/
    RefVolSelAVDD     = 3,        /*!<AVDD*/

}en_adc_ref_vol_sel_t;

/**
 ******************************************************************************
 ** \brief ADC采样通道选择
 *****************************************************************************/
typedef enum en_adc_samp_ch_sel
{
    AdcExInputCH0    =  0,        /*!<使用通道0输入P2.4*/
    AdcExInputCH1    =  1,        /*!<使用通道1输入P2.6*/
    AdcExInputCH2    =  2,        /*!<使用通道2输入P3.2*/
    AdcExInputCH3    =  3,        /*!<使用通道3输入P3.3*/
    AdcExInputCH4    =  4,        /*!<使用通道4输入P3.4*/
    AdcExInputCH5    =  5,        /*!<使用通道5输入P3.5*/
    AdcExInputCH6    =  6,        /*!<使用通道6输入P3.6*/
    AdcExInputCH7    =  7,        /*!<使用通道7输入P0.1*/
    AdcExInputCH8    =  8,        /*!<使用通道8输入P0.2*/
    AdcAVccDiV2Input =  9,        /*!<使用1/2 AVCC(必须使用输入增益)*/
    AdcAiTsInput     =  10,       /*!<使用AI_TS(必须使用输入增益)*/
    AdcVref1P2Input  =  11,       /*!<使用Vref1P2(必须使用输入增益)*/

}en_adc_samp_ch_sel_t;

/**
 ******************************************************************************
 ** \brief ADC采样时间选择
 *****************************************************************************/
typedef enum en_adc_samp_time_sel
{
    AdcSampTime3Clk  =  0,        /*!<The ADC with 3 sampling ADC CLK clock*/
    AdcSampTime1Clk  =  1,        /*!<The ADC with 1 sampling ADC CLK clock*/
    AdcSampTime6Clk  =  2,        /*!<The ADC with 6 sampling ADC CLK clock*/
    AdcSampTime9Clk  =  3,        /*!<The ADC with 9 sampling ADC CLK clock*/

}en_adc_samp_time_sel_t;

/**
 ******************************************************************************
 ** \brief ADC周边模块反射源选择
 *****************************************************************************/
typedef enum en_adc_trig_sel
{
    AdcTrigDisable    =  0,         /*!<禁用周边模块反射源*/
    AdcTrigTimer0     =  1,         /*!<选择timer0中断源，自动触发ADC采样*/
    AdcTrigTimer1     =  2,         /*!<选择timer1中断源，自动触发ADC采样*/
    AdcTrigTimer2     =  3,         /*!<选择timer2中断源，自动触发ADC采样*/
    AdcTrigLptimer    =  4,         /*!<选择lptimer中断源，自动触发ADC采样*/
    AdcTrigTimer4     =  5,         /*!<选择timer4中断源，自动触发ADC采样*/
    AdcTrigTimer5     =  6,         /*!<选择timer5中断源，自动触发ADC采样*/
    AdcTrigTimer6     =  7,         /*!<选择timer6中断源，自动触发ADC采样*/
    AdcTrigUart0      =  8,         /*!<选择uart0中断源，自动触发ADC采样*/
    AdcTrigUart1      =  9,         /*!<选择uart1中断源，自动触发ADC采样*/
    AdcTrigLpuart     =  10,        /*!<选择lpuart中断源，自动触发ADC采样*/
    AdcTrigVC1        =  11,        /*!<选择VC1中断源，自动触发ADC采样*/
    AdcTrigVC2        =  12,        /*!<选择VC2中断源，自动触发ADC采样*/
    AdcTrigRTC        =  13,        /*!<选择RTC中断源，自动触发ADC采样*/
    AdcTrigPCA        =  14,        /*!<选择PCA中断源，自动触发ADC采样*/
    AdcTrigSPI        =  15,        /*!<选择SPI中断源，自动触发ADC采样*/
    AdcTrigP01        =  16,        /*!<选择P01中断源，自动触发ADC采样*/
    AdcTrigP02        =  17,        /*!<选择P02中断源，自动触发ADC采样*/
    AdcTrigP03        =  18,        /*!<选择P03中断源，自动触发ADC采样*/
    AdcTrigP14        =  19,        /*!<选择P14中断源，自动触发ADC采样*/
    AdcTrigP15        =  20,        /*!<选择P15中断源，自动触发ADC采样*/
    AdcTrigP23        =  21,        /*!<选择P23中断源，自动触发ADC采样*/
    AdcTrigP24        =  22,        /*!<选择P24中断源，自动触发ADC采样*/
    AdcTrigP25        =  23,        /*!<选择P25中断源，自动触发ADC采样*/
    AdcTrigP26        =  24,        /*!<选择P26中断源，自动触发ADC采样*/
    AdcTrigP27        =  25,        /*!<选择P27中断源，自动触发ADC采样*/
    AdcTrigP31        =  26,        /*!<选择P31中断源，自动触发ADC采样*/
    AdcTrigP32        =  27,        /*!<选择P32中断源，自动触发ADC采样*/
    AdcTrigP33        =  28,        /*!<选择P33中断源，自动触发ADC采样*/
    AdcTrigP34        =  29,        /*!<选择P34中断源，自动触发ADC采样*/
    AdcTrigP35        =  30,        /*!<选择P35中断源，自动触发ADC采样*/
    AdcTrigP36        =  31,        /*!<选择P36中断源，自动触发ADC采样*/

}en_adc_trig_sel_t;

/**
 ******************************************************************************
 ** \brief Bias current select signal of pre amp for comparator
 *****************************************************************************/
typedef enum en_adc_pre_bias
{
    AdcPreBias10u  =  0,       /*!<10uA*/
    AdcPreBias5u   =  1,       /*!<5uA*/
    AdcPreBias15u  =  2,       /*!<15uA*/
    AdcPreBias20u  =  3,       /*!<20uA*/

}en_adc_pre_bias_t;

/**
 ******************************************************************************
 ** \brief Bias current select signal of vcm buf/ref buf/input buf
 *****************************************************************************/
typedef enum en_adc_vcm_bias
{
    AdcVcmBias10u  =  0,       /*!<10uA*/
    AdcVcmBias5u   =  1,       /*!<5uA*/
    AdcVcmBias15u  =  2,       /*!<15uA*/
    AdcVcmBias20u  =  3,       /*!<20uA*/

}en_adc_vcm_bias_t;

/**
 ******************************************************************************
 ** \brief ADC时钟delay
 *****************************************************************************/
typedef enum en_adc_clock_delay
{
    AdcClockDelayDisable  =  0,       /*!<no delay*/
    AdcClockDelay1Div8    =  1,       /*!<1/8 cycle delay*/
    AdcClockDelay2Div8    =  2,       /*!<2/8 cycle delay*/
    AdcClockDelay3Div8    =  3,       /*!<3/8 cycle delay*/
    AdcClockDelay4Div8    =  4,       /*!<4/8 cycle delay*/
    AdcClockDelay5Div8    =  5,       /*!<5/8 cycle delay*/
    AdcClockDelay6Div8    =  6,       /*!<6/8 cycle delay*/
    AdcClockDelay7Div8    =  7,       /*!<7/8 cycle delay*/

}en_adc_clock_delay_t;

/**
 ******************************************************************************
 ** \brief ADC内部1.5v 参考电压校正值
 *****************************************************************************/
typedef enum en_adc_opt_1p5_calib
{
    AdcOpt1p5Calib1p439  =  0,       /*!<1.439V*/
    AdcOpt1p5Calib1p447  =  1,       /*!<1.447V*/
    AdcOpt1p5Calib1p456  =  2,       /*!<1.456V*/
    AdcOpt1p5Calib1p464  =  3,       /*!<1.464V*/
    AdcOpt1p5Calib1p473  =  4,       /*!<1.473V*/
    AdcOpt1p5Calib1p480  =  5,       /*!<1.480V*/
    AdcOpt1p5Calib1p489  =  6,       /*!<1.489V*/
    AdcOpt1p5Calib1p497  =  7,       /*!<1.497V*/
    AdcOpt1p5Calib1p505  =  8,       /*!<1.505V*/
    AdcOpt1p5Calib1p514  =  9,       /*!<1.514V*/
    AdcOpt1p5Calib1p522  =  10,      /*!<1.522V*/
    AdcOpt1p5Calib1p530  =  11,      /*!<1.530V*/
    AdcOpt1p5Calib1p539  =  12,      /*!<1.539V*/
    AdcOpt1p5Calib1p547  =  13,      /*!<1.547V*/
    AdcOpt1p5Calib1p556  =  14,      /*!<1.556V*/
    AdcOpt1p5Calib1p564  =  15,      /*!<1.564V*/

}en_adc_opt_1p5_calib_t;

/**
 ******************************************************************************
 ** \brief ADC内部2.5v 参考电压校正值
 *****************************************************************************/
typedef enum en_adc_opt_2p5_calib
{
    AdcOpt2p5Calib2p375  =  0,       /*!<2.375V*/
    AdcOpt2p5Calib2p392  =  1,       /*!<2.392V*/
    AdcOpt2p5Calib2p409  =  2,       /*!<2.409V*/
    AdcOpt2p5Calib2p425  =  3,       /*!<2.425V*/
    AdcOpt2p5Calib2p442  =  4,       /*!<2.442V*/
    AdcOpt2p5Calib2p459  =  5,       /*!<2.459V*/
    AdcOpt2p5Calib2p475  =  6,       /*!<2.475V*/
    AdcOpt2p5Calib2p492  =  7,       /*!<2.492V*/
    AdcOpt2p5Calib2p509  =  8,       /*!<2.509V*/
    AdcOpt2p5Calib2p525  =  9,       /*!<2.525V*/
    AdcOpt2p5Calib2p542  =  10,      /*!<2.542V*/
    AdcOpt2p5Calib2p558  =  11,      /*!<2.558V*/
    AdcOpt2p5Calib2p572  =  12,      /*!<2.572V*/
    AdcOpt2p5Calib2p592  =  13,      /*!<2.592V*/
    AdcOpt2p5Calib2p608  =  14,      /*!<2.608V*/
    AdcOpt2p5Calib2p625  =  15,      /*!<2.625V*/

}en_adc_opt_2p5_calib_t;

/******************************************************************************
 * Extern type definitions ('typedef')                                        *
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief  ADC配置
 *****************************************************************************/
typedef struct stc_adc_cfg
{
    /*! ADC operation mode*/
    en_adc_op_mode_t enAdcOpMode;

    /*! ADC clock selection*/
    en_adc_clk_sel_t  enAdcClkSel;

    /*! Choose the ADC sampling time*/
    en_adc_samp_time_sel_t  enAdcSampTimeSel;

    /*! ADC reference voltage*/
    en_adc_ref_vol_sel_t    enAdcRefVolSel;

    /*! ADC bias current of pre amp for comparator*/
    en_adc_pre_bias_t enAdcPreBias;

    /*! ADC bias current of vcm buf/ref buf/input buf*/
    en_adc_vcm_bias_t enAdcVcmBias;

    /*! ADC clock delay*/
    en_adc_clock_delay_t enAdcClockDelay;

    /*! ADC input amplify enable*/
    boolean_t    bAdcInBufEn;

    /*!ADC region mode upper limit threshold*/
    uint32_t    u32AdcRegHighThd;

    /*!ADC region mode lower limit threshold*/
    uint32_t    u32AdcRegLowThd;

    /*! ADC trigger 0 selection*/
    en_adc_trig_sel_t enAdcTrig0Sel;

    /*! ADC trigger 1 selection*/
    en_adc_trig_sel_t enAdcTrig1Sel;

}stc_adc_cfg_t;

/**
 ******************************************************************************
 ** \brief  ADC单次采样模式配置
 *****************************************************************************/
typedef struct stc_adc_norm_cfg
{
    /*! ADC channel in normal mode*/
    en_adc_samp_ch_sel_t    enAdcNormModeCh;
    /*! ADC result accumulated mode enable/disable*/
    boolean_t  bAdcResultAccEn;

}stc_adc_norm_cfg_t;

/**
 ******************************************************************************
 ** \brief  ADC连续采样模式配置
 *****************************************************************************/
typedef struct stc_adc_cont_cfg
{
    /*! ADC channel in continuous mode*/
    en_adc_samp_ch_sel_t    enAdcContModeCh;
    /*! ADC sampling count*/
    uint8_t    u8AdcSampCnt;
    /*! ADC result accumulated mode enable/disable*/
    boolean_t  bAdcResultAccEn;

}stc_adc_cont_cfg_t;

/**
 ******************************************************************************
 ** \brief  ADC扫描采样模式配置
 *****************************************************************************/
typedef struct stc_adc_scan_cfg
{
    /*! ADC channels in scan mode*/
    uint8_t    u8AdcScanModeCh;
    /*! ADC sampling count*/
    uint8_t    u8AdcSampCnt;

}stc_adc_scan_cfg_t;

/**
 ******************************************************************************
 ** \brief  ADC中断配置
 *****************************************************************************/
typedef struct stc_adc_irq
{
    /*!<ADC continious IRQ*/
    boolean_t  bAdcContIrq;
    /*!ADC region IRQ*/
    boolean_t  bAdcRegIrq;
    /*!ADC higher than HT IRQ*/
    boolean_t  bAdcHhtIrq;
    /*!ADC lower than LT IRQ*/
    boolean_t  bAdcLltIrq;

}stc_adc_irq_t;

/**
 ******************************************************************************
 ** \brief  ADC中断回调函数
 *****************************************************************************/
typedef struct stc_adc_irq_calbakfn_pt
{
    /*! ADC continious IRQ callback function Pointer*/
    func_ptr_t  pfnAdcContIrq;
    /*! ADC region IRQ callback function Pointer*/
    func_ptr_t  pfnAdcRegIrq;
    /*! ADC higher than HT IRQ callback function Pointer*/
    func_ptr_t  pfnAdcHhtIrq;
    /*! ADC lower than LT IRQ callback function Pointer*/
    func_ptr_t  pfnAdcLltIrq;

}stc_adc_irq_calbakfn_pt_t;


/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

#ifndef ENABLE
        #define ENABLE TRUE             /*!< ENABLE DEFINE */
#endif

#ifndef DISABLE
        #define DISABLE FALSE           /*!< DISABLE DEFINE */
#endif

/******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/*! ADC initialization*/
en_result_t Adc_Init(stc_adc_cfg_t* pstcAdcConfig);
/*! ADC de-init*/
void Adc_DeInit(void);

/*! ADC conversion start*/
void Adc_Start(void);
/*! ADC conversion stop*/
void Adc_Stop(void);

/*! ADC conversion enable*/
void Adc_Enable(void);
/*! ADC conversion disable*/
void Adc_Disable(void);

/*! ADC normal mode configuration*/
en_result_t Adc_ConfigNormMode(stc_adc_cfg_t* pstcAdcConfig, stc_adc_norm_cfg_t* pstcAdcNormCfg);
/*! ADC continuous mode configuration*/
en_result_t Adc_ConfigContMode(stc_adc_cfg_t* pstcAdcConfig, stc_adc_cont_cfg_t* pstcAdcContCfg);
/*! ADC scan mode configuration*/
en_result_t Adc_ConfigScanMode(stc_adc_cfg_t* pstcAdcConfig, stc_adc_scan_cfg_t* pstcAdcScanCfg);

/*! ADC IRQ configuration*/
void Adc_ConfigIrq(stc_adc_irq_t* pstcAdcIrqCfg,
                   stc_adc_irq_calbakfn_pt_t* pstcAdcIrqCalbaks);
/*! ADC enable IRQ*/
void Adc_EnableIrq(stc_adc_irq_t* pstcAdcIrqCfg);
/*! ADC disable IRQ*/
void Adc_DisableIrq(void);
/*! ADC get IRQ state*/
void Adc_GetIrqState(stc_adc_irq_t* pstcAdcIrqState);
/*! ADC clear IRQ states*/
void Adc_ClrContIrqState(void);
void Adc_ClrRegIrqState(void);
void Adc_ClrHhtIrqState(void);
void Adc_ClrIrqLltState(void);

/*! 查询ADC转换状态*/
boolean_t Adc_PollBusyState(void);
/*! 获取采样值*/
en_result_t Adc_GetResult(uint16_t* pu16AdcResult);
/*! 获取累加采样值*/
en_result_t Adc_GetAccResult(uint32_t* pu32AdcAccResult);
/*! clear ADC accumulated result*/
void Adc_ClrAccResult(void);
/*! get ADC scan channel result*/
en_result_t Adc_GetScanResult(uint8_t u8Channel, uint16_t* pu16AdcResult);

en_result_t Adc_CalibOpt1p5(en_adc_opt_1p5_calib_t enAdcOpt1p5);
en_result_t Adc_CalibOpt2p5(en_adc_opt_2p5_calib_t enAdcOpt2p5);

en_result_t Adc_SetVref(en_adc_ref_vol_sel_t enAdcRefVolSel);

//@}
#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/