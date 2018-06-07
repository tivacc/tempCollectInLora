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
/** \file adc.c
 **
 ** ADC driver API.
 ** @link A Group Some description @endlink
 **
 **   - 2017-05-28 LiuHL    First Version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "adc.h"

/**
 ******************************************************************************
 ** \addtogroup Driver Group
 ******************************************************************************/
//@{

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/


/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void AdcEnableNvic(IRQn_Type enIrqn);
static void AdcDisableNvic(IRQn_Type enIrqn);

/******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static stc_adc_irq_calbakfn_pt_t stcAdcIrqCalbaks = {NULL, NULL, NULL, NULL};

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 *****************************************************************************/


/**
 * \brief   AdcEnableNvic
 *          使能NVIC中ADC中断
 *
 * \param   [in]  enIrqn  中断号
 *
 * \retval  无
 */
static void AdcEnableNvic(IRQn_Type enIrqn)
{
    NVIC_ClearPendingIRQ(enIrqn);
    NVIC_EnableIRQ(enIrqn);
    NVIC_SetPriority(enIrqn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 * \brief   AdcDisableNvic
 *          除能NVIC中ADC中断
 *
 * \param   [in]  enIrqn  中断号
 *
 * \retval  无
 */
static void AdcDisableNvic(IRQn_Type enIrqn)
{
    NVIC_ClearPendingIRQ(enIrqn);
    NVIC_DisableIRQ(enIrqn);
    NVIC_SetPriority(enIrqn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 * \brief   Adc_IrqHandler
 *          ADC中断服务程序
 *
 * \param   [in]  u8Param  未使用
 *
 * \retval  无
 */
void Adc_IRQHandler(uint8_t u8Param)
{
    if (TRUE == M0P_ADC->IFR_f.REG_INTF)
    {
        if (NULL != stcAdcIrqCalbaks.pfnAdcRegIrq)
        {
            stcAdcIrqCalbaks.pfnAdcRegIrq();
        }
        M0P_ADC->ICLR_f.REG_INTC = 0u;
    }

    if (TRUE == M0P_ADC->IFR_f.HHT_INTF)
    {
        if (NULL != stcAdcIrqCalbaks.pfnAdcHhtIrq)
        {
            stcAdcIrqCalbaks.pfnAdcHhtIrq();
        }
        M0P_ADC->ICLR_f.HHT_INTC = 0u;
    }

    if (TRUE == M0P_ADC->IFR_f.LLT_INTF)
    {
        if (NULL != stcAdcIrqCalbaks.pfnAdcLltIrq)
        {
            stcAdcIrqCalbaks.pfnAdcLltIrq();
        }
        M0P_ADC->ICLR_f.LLT_INTC = 0u;
    }

    if (TRUE == M0P_ADC->IFR_f.CONT_INTF)
    {
        if (NULL != stcAdcIrqCalbaks.pfnAdcContIrq)
        {
            stcAdcIrqCalbaks.pfnAdcContIrq();
        }
        M0P_ADC->ICLR_f.CONT_INTC = 0u;
    }
}

/**
 * \brief   Adc_ConfigIrq
 *          配置ADC中断
 *
 * \param   [in]  pstcAdcIrqCfg  ADC中断配置指针
 ×          [in]  pstcAdcIrqCalbaks  ADC中断回调函数指针
 *
 * \retval  none
 */
void Adc_ConfigIrq(stc_adc_irq_t* pstcAdcIrqCfg,
                   stc_adc_irq_calbakfn_pt_t* pstcAdcIrqCalbaks)
{
    if (TRUE == pstcAdcIrqCfg->bAdcContIrq)
    {
        if (NULL != pstcAdcIrqCalbaks->pfnAdcContIrq)
        {
            stcAdcIrqCalbaks.pfnAdcContIrq = pstcAdcIrqCalbaks->pfnAdcContIrq;
        }
    }

    if (TRUE == pstcAdcIrqCfg->bAdcRegIrq)
    {
        if (NULL != pstcAdcIrqCalbaks->pfnAdcRegIrq)
        {
            stcAdcIrqCalbaks.pfnAdcRegIrq = pstcAdcIrqCalbaks->pfnAdcRegIrq;
        }
    }

    if (TRUE == pstcAdcIrqCfg->bAdcHhtIrq)
    {
        if (NULL != pstcAdcIrqCalbaks->pfnAdcHhtIrq)
        {
            stcAdcIrqCalbaks.pfnAdcHhtIrq = pstcAdcIrqCalbaks->pfnAdcHhtIrq;
        }
    }

    if (TRUE == pstcAdcIrqCfg->bAdcLltIrq)
    {
        if (NULL != pstcAdcIrqCalbaks->pfnAdcLltIrq)
        {
            stcAdcIrqCalbaks.pfnAdcLltIrq = pstcAdcIrqCalbaks->pfnAdcLltIrq;
        }
    }
}

/**
 * \brief   Adc_GetIrqState
 *          获取ADC中断状态
 *
 * \param   [in]  pstcAdcIrqState  ADC中断状态指针
 *
 * \retval  none
 */
void Adc_GetIrqState(stc_adc_irq_t* pstcAdcIrqState)
{
    pstcAdcIrqState->bAdcContIrq = M0P_ADC->IFR_f.CONT_INTF;
    pstcAdcIrqState->bAdcRegIrq = M0P_ADC->IFR_f.REG_INTF;
    pstcAdcIrqState->bAdcHhtIrq = M0P_ADC->IFR_f.HHT_INTF;
    pstcAdcIrqState->bAdcLltIrq = M0P_ADC->IFR_f.LLT_INTF;
}

/**
 * \brief   Adc_ClrContIrqState
 *          清除ADC CONT中断状态
 *
 * \param   none
 *
 * \retval  none
 */
void Adc_ClrContIrqState(void)
{
    M0P_ADC->ICLR = ~(1<<3);
}

/**
 * \brief   Adc_ClrRegIrqState
 *          清除ADC REG中断状态
 *
 * \param   none
 *
 * \retval  none
 */
void Adc_ClrRegIrqState(void)
{
    M0P_ADC->ICLR = ~(1<<2);
}

/**
 * \brief   Adc_ClrHhtIrqState
 *          清除ADC HHT中断状态
 *
 * \param   none
 *
 * \retval  none
 */
void Adc_ClrHhtIrqState(void)
{
    M0P_ADC->ICLR = ~(1<<1);
}

/**
 * \brief   Adc_ClrLltIrqState
 *          清除ADC LLT中断状态
 *
 * \param   none
 *
 * \retval  none
 */
void Adc_ClrIrqLltState(void)
{
    M0P_ADC->ICLR = ~1;
}


/**
 * \brief   Adc_EnableIrq
 *          ADC中断使能
 *
 * \param   [in]  pstcAdcIrqCfg  ADC中断配置
 *
 * \retval  none
 */
void Adc_EnableIrq(stc_adc_irq_t* pstcAdcIrqCfg)
{
    if (TRUE == pstcAdcIrqCfg->bAdcContIrq)
    {
        M0P_ADC->CR0_f.IE = 1u;
    }
	else
	{
		M0P_ADC->CR0_f.IE = 0u;
	}

    if (TRUE == pstcAdcIrqCfg->bAdcRegIrq)
    {
        M0P_ADC->CR1_f.REG_IE = 1u;
    }
	else
	{
        M0P_ADC->CR1_f.REG_IE = 0u;
    }

    if (TRUE == pstcAdcIrqCfg->bAdcHhtIrq)
    {
        M0P_ADC->CR1_f.HHT_IE = 1u;
    }
	else
	{
        M0P_ADC->CR1_f.HHT_IE = 0u;
    }

    if (TRUE == pstcAdcIrqCfg->bAdcLltIrq)
    {
        M0P_ADC->CR1_f.LLT_IE = 1u;
    }
	else
    {
        M0P_ADC->CR1_f.LLT_IE = 0u;
    }

    AdcEnableNvic(ADC_IRQn);
}

/**
 * \brief   Adc_DisableIrq
 *          ADC中断除能
 *
 * \param   none
 *
 * \retval  none
 */
void Adc_DisableIrq(void)
{
    uint32_t u32Cr1;
    
    M0P_ADC->CR0_f.IE = 0u;
    
    u32Cr1 = M0P_ADC->CR1 | (1<<15);    
    u32Cr1 &= ~((1<<12)|(1<<13)|(1<<14));
    M0P_ADC->CR1 = u32Cr1;
	//M0P_ADC->CR1_f.REG_IE = 0u;
	//M0P_ADC->CR1_f.HHT_IE = 0u;
	//M0P_ADC->CR1_f.LLT_IE = 0u;
	AdcDisableNvic(ADC_IRQn);
}

/**
 * \brief   Adc_Init
 *          ADC初始化
 *
 * \param   [in]  pstcAdcConfig  ADC配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adc_Init(stc_adc_cfg_t* pstcAdcConfig)
{
    if (NULL == pstcAdcConfig)
    {
        return ErrorInvalidParameter;
    }

    M0P_ADC->CR0_f.CLKSEL = pstcAdcConfig->enAdcClkSel;
    M0P_ADC->CR0_f.SAM = pstcAdcConfig->enAdcSampTimeSel;
    M0P_ADC->CR0_f.SREF = pstcAdcConfig->enAdcRefVolSel;
    M0P_ADC->CR3_f.PRE_S = pstcAdcConfig->enAdcPreBias;
    M0P_ADC->CR3_f.VCM_S = pstcAdcConfig->enAdcVcmBias;
    M0P_ADC->CR3_f.CLK_DELAY = pstcAdcConfig->enAdcClockDelay;
    M0P_ADC->CR0_f.BUFEN = pstcAdcConfig->bAdcInBufEn;
    M0P_ADC->HT_f.RESULT_HIGH = pstcAdcConfig->u32AdcRegHighThd;
    M0P_ADC->LT_f.RESULT_LOW = pstcAdcConfig->u32AdcRegLowThd;
    M0P_ADC->CR1_f.TRIGS0 = pstcAdcConfig->enAdcTrig0Sel;
    M0P_ADC->CR1_f.TRIGS1 = pstcAdcConfig->enAdcTrig1Sel;
	M0P_ADC->CR0_f.IE = 0u;
	M0P_ADC->CR1_f.REG_IE = 0u;
	M0P_ADC->CR1_f.HHT_IE = 0u;
	M0P_ADC->CR1_f.LLT_IE = 0u;

    return Ok;
}

/**
 * \brief   Adc_DeInit
 *          ADC Deinit
 *
 * \param   none
 *
 * \retval  none
 */
void Adc_DeInit(void)
{
    AdcDisableNvic(ADC_IRQn);

    M0P_ADC->CR0_f.ADCEN = 0u;

    M0P_ADC->CR0_f.IE = 0u;
    M0P_ADC->CR1_f.REG_IE = 0u;
    M0P_ADC->CR1_f.HHT_IE = 0u;
    M0P_ADC->CR1_f.LLT_IE = 0u;
    M0P_ADC->ICLR_f.CONT_INTC = 0u;
    M0P_ADC->ICLR_f.REG_INTC = 0u;
    M0P_ADC->ICLR_f.HHT_INTC = 0u;
    M0P_ADC->ICLR_f.LLT_INTC = 0u;

    M0P_ADC->CR0_f.CLKSEL = 0u;
    M0P_ADC->CR0_f.SAM = 0x2u;
    M0P_ADC->CR0_f.SREF = 0x3u;
    M0P_ADC->CR0_f.SEL = 0xFu;
    M0P_ADC->CR3_f.PRE_S = 0u;
    M0P_ADC->CR3_f.VCM_S = 0u;
    M0P_ADC->CR3_f.CLK_DELAY = 0u;
    M0P_ADC->CR0_f.BUFEN = 0u;
    M0P_ADC->HT_f.RESULT_HIGH = 0xFFFu;
    M0P_ADC->LT_f.RESULT_LOW = 0u;
    M0P_ADC->CR1_f.TRIGS0 = 0u;
    M0P_ADC->CR1_f.TRIGS1 = 0u;
}

/**
 * \brief   Adc_Start
 *          ADC转换开始
 *
 * \param   none
 *
 * \retval  none
 */

void Adc_Start(void)
{
    M0P_ADC->ICLR_f.CONT_INTC = 0u;
    M0P_ADC->CR0_f.STATERST = 1u;
	M0P_ADC->CR0_f.START = 1u;
}

/**
 * \brief   Adc_Stop
 *          ADC转换停止
 *
 * \param   none
 *
 * \retval  none
 */
void Adc_Stop(void)
{
    M0P_ADC->CR0_f.START = 0u;
}

/**
 * \brief   Adc_Enable
 *          ADC使能
 *
 * \param   none
 *
 * \retval  none
 */
void Adc_Enable(void)
{
    M0P_ADC->CR0_f.ADCEN = 1u;
}

/**
 * \brief   Adc_Disable
 *          ADC除能
 *
 * \param   none
 *
 * \retval  none
 */
void Adc_Disable(void)
{
    M0P_ADC->CR0_f.ADCEN = 0u;
}

/**
 * \brief   Adc_ConfigNormMode
 *          配置普通转换模式
 *
 * \param   [in]  pstcAdcConfig   ADC配置指针
 *          [in]  pstcAdcNormCfg  普通转换模式配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adc_ConfigNormMode(stc_adc_cfg_t* pstcAdcConfig, stc_adc_norm_cfg_t* pstcAdcNormCfg)
{
    if (NULL == pstcAdcNormCfg || NULL == pstcAdcConfig)
    {
        return ErrorInvalidParameter;
    }

    pstcAdcConfig->enAdcOpMode = AdcNormalMode;
    M0P_ADC->CR1_f.CT = 0u;
    M0P_ADC->CR0_f.SEL = pstcAdcNormCfg->enAdcNormModeCh;
    M0P_ADC->CR1_f.RACC_EN = pstcAdcNormCfg->bAdcResultAccEn;

    return Ok;
}

/**
 * \brief   Adc_ConfigContMode
 *          配置连续转换模式
 *
 * \param   [in]  pstcAdcConfig   ADC配置指针
 *          [in]  pstcAdcNormCfg  连续转换模式配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adc_ConfigContMode(stc_adc_cfg_t* pstcAdcConfig, stc_adc_cont_cfg_t* pstcAdcContCfg)
{
    if (NULL == pstcAdcContCfg || NULL == pstcAdcConfig)
    {
        return ErrorInvalidParameter;
    }

    pstcAdcConfig->enAdcOpMode = AdcContMode;
    M0P_ADC->CR1_f.CT = 1u;
    M0P_ADC->CR0_f.SEL = pstcAdcContCfg->enAdcContModeCh;
    M0P_ADC->CR1_f.RACC_EN = pstcAdcContCfg->bAdcResultAccEn;
    if (pstcAdcContCfg->bAdcResultAccEn)
    {
        M0P_ADC->CR1_f.RACC_CLR = 1u;
    }
	M0P_ADC->CR2 = 1<<pstcAdcContCfg->enAdcContModeCh;
    M0P_ADC->CR2_f.ADCCNT = pstcAdcContCfg->u8AdcSampCnt;

    return Ok;
}

/**
 * \brief   Adc_ConfigScanMode
 *          配置扫描转换模式
 *
 * \param   [in]  pstcAdcConfig   ADC配置指针
 *          [in]  pstcAdcNormCfg  扫描转换模式配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adc_ConfigScanMode(stc_adc_cfg_t* pstcAdcConfig, stc_adc_scan_cfg_t* pstcAdcScanCfg)
{
    if (NULL == pstcAdcScanCfg || NULL == pstcAdcConfig)
    {
        return ErrorInvalidParameter;
    }

    pstcAdcConfig->enAdcOpMode = AdcScanMode;
    M0P_ADC->CR1_f.CT = 1u;
    M0P_ADC->CR2 = pstcAdcScanCfg->u8AdcScanModeCh;
    M0P_ADC->CR2_f.ADCCNT = pstcAdcScanCfg->u8AdcSampCnt;

    return Ok;
}

/**
 * \brief   Adc_PollBusyState
 *          查询ADC转换状态
 *
 * \param   none
 *
 * \retval  boolean_t  TRUE:  ADC转换中
 *                     FALSE: ADC转换完成
 */
boolean_t Adc_PollBusyState(void)
{
    return M0P_ADC->CR0_f.START;
}

/**
 * \brief   Adc_GetResult
 *          获取采样值
 *
 * \param   [out] pu16AdcResult  采样值指针
 *
 * \retval  en_result_t  Ok:  成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adc_GetResult(uint16_t* pu16AdcResult)
{
    if (NULL == pu16AdcResult)
    {
        return ErrorInvalidParameter;
    }

    *pu16AdcResult = M0P_ADC->RESULT_f.RESULT;

    return Ok;
}

/**
 * \brief   Adc_GetAccResult
 *          获取累加采样值
 *
 * \param   [out] pu32AdcAccResult  累加采样值指针
 *
 * \retval  en_result_t  Ok:  成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adc_GetAccResult(uint32_t* pu32AdcAccResult)
{
    if (NULL == pu32AdcAccResult)
    {
        return ErrorInvalidParameter;
    }

    *pu32AdcAccResult = M0P_ADC->RESULT_ACC_f.RESULT_ACC;

    return Ok;
}

/**
 * \brief   Adc_ClrAccResult
 *          清零累加采样值
 *
 * \param   none
 *
 * \retval  none
 */
void Adc_ClrAccResult(void)
{
    M0P_ADC->CR1_f.RACC_CLR = 0u;
}

/**
 * \brief   Adc_GetScanResult
 *          获取扫描采样模式下指定通道的采样值
 *
 * \param   [in]  u8Channel  通道号
 ×          [out] pu16AdcResult  采样值指针
 *
 * \retval  en_result_t  Ok:  成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adc_GetScanResult(uint8_t u8Channel, uint16_t* pu16AdcResult)
{
    if (NULL == pu16AdcResult)
    {
        return ErrorInvalidParameter;
    }

    switch (u8Channel)
    {
        case AdcExInputCH0:
            *pu16AdcResult = M0P_ADC->RESULT0_f.RESULT0;
            break;
        case AdcExInputCH1:
            *pu16AdcResult = M0P_ADC->RESULT1_f.RESULT1;
            break;
        case AdcExInputCH2:
            *pu16AdcResult = M0P_ADC->RESULT2_f.RESULT2;
            break;
        case AdcExInputCH3:
            *pu16AdcResult = M0P_ADC->RESULT3_f.RESULT3;
            break;
        case AdcExInputCH4:
            *pu16AdcResult = M0P_ADC->RESULT4_f.RESULT4;
            break;
        case AdcExInputCH5:
            *pu16AdcResult = M0P_ADC->RESULT5_f.RESULT5;
            break;
        case AdcExInputCH6:
            *pu16AdcResult = M0P_ADC->RESULT6_f.RESULT6;
            break;
        case AdcExInputCH7:
            *pu16AdcResult = M0P_ADC->RESULT7_f.RESULT7;
            break;
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}

/**
 * \brief   Adc_CalibOpt1p5
 *          设置内部1.5v 参考电压校正值
 *
 * \param   [in]  enAdcOpt1p5  内部1.5v 参考电压校正值
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adc_CalibOpt1p5(en_adc_opt_1p5_calib_t enAdcOpt1p5)
{
    if (enAdcOpt1p5 > AdcOpt1p5Calib1p564)
        return ErrorInvalidParameter;

    M0P_ADC->CR3_f.OPT1_5 = enAdcOpt1p5;
    return Ok;
}

/**
 * \brief   Adc_CalibOpt2p5
 *          设置内部2.5v 参考电压校正值
 *
 * \param   [in]  enAdcOpt2p5  内部2.5v 参考电压校正值
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adc_CalibOpt2p5(en_adc_opt_2p5_calib_t enAdcOpt2p5)
{
    if (enAdcOpt2p5 > AdcOpt2p5Calib2p625)
    {
        return ErrorInvalidParameter;
    }

    M0P_ADC->CR3_f.OPT2_5 = enAdcOpt2p5;
    return Ok;
}

/**
 * \brief   Adc_SetVref
 *          设置ADC参考电压
 *
 * \param   [in]  enAdcRefVolSel  ADC参考电压
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adc_SetVref(en_adc_ref_vol_sel_t enAdcRefVolSel)
{
	if (enAdcRefVolSel > RefVolSelAVDD)
    {
        return ErrorInvalidParameter;
    }

    M0P_ADC->CR0_f.SREF = enAdcRefVolSel;
    return Ok;
}

//@} // ADC Group


/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

