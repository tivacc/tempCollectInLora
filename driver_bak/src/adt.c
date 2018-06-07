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
/** \file adt.c
 **
 ** Low Voltage Detect driver API.
 ** @link Lvd Group Some description @endlink
 **
 **   - 2017-06-16 LiuHL    First Version
 **
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "adt.h"

/**
 ******************************************************************************
 ** \addtogroup Lvd Group
 ******************************************************************************/
//@{

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')        *
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static func_ptr_t    pfnAdtIrqCbk[3][16];

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 *****************************************************************************/

/**
 * \brief   AdtEnableNvic
 *          使能NVIC中ADT中断
 *
 * \param   [in]  enIrqn  中断号
 *
 * \retval  无
 */
static void AdtEnableNvic(IRQn_Type enIrqn)
{
    NVIC_ClearPendingIRQ(enIrqn);
    NVIC_EnableIRQ(enIrqn);
    NVIC_SetPriority(enIrqn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 * \brief   AdtDisableNvic
 *          除能NVIC中ADT中断
 *
 * \param   [in]  enIrqn  中断号
 *
 * \retval  无
 */
static void AdtDisableNvic(IRQn_Type enIrqn)
{
    NVIC_ClearPendingIRQ(enIrqn);
    NVIC_DisableIRQ(enIrqn);
    NVIC_SetPriority(enIrqn, DDL_IRQ_LEVEL_DEFAULT);
}

/**
 * \brief   Adt_IrqHandler
 *          ADC中断服务程序
 *
 * \param   [in]  u8Param  未使用
 *
 * \retval  无
 */
void Adt_IRQHandler(uint8_t u8Param)
{
    uint8_t u8Adt = u8Param - 4;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*u8Adt);

    if (TRUE == pstcM0PAdt->INTFR_f.CMAF)
    {
        if (NULL != pfnAdtIrqCbk[u8Adt][AdtCMAIrq])
        {
            pfnAdtIrqCbk[u8Adt][AdtCMAIrq]();
        }
        pstcM0PAdt->INTCR = ~(1<<AdtCMAIrq);
    }

    if (TRUE == pstcM0PAdt->INTFR_f.CMBF)
    {
        if (NULL != pfnAdtIrqCbk[u8Adt][AdtCMBIrq])
        {
            pfnAdtIrqCbk[u8Adt][AdtCMBIrq]();
        }
        pstcM0PAdt->INTCR = ~(1<<AdtCMBIrq);
    }

    if (TRUE == pstcM0PAdt->INTFR_f.CMCF)
    {
        if (NULL != pfnAdtIrqCbk[u8Adt][AdtCMCIrq])
        {
            pfnAdtIrqCbk[u8Adt][AdtCMCIrq]();
        }
        pstcM0PAdt->INTCR = ~(1<<AdtCMCIrq);
    }

    if (TRUE == pstcM0PAdt->INTFR_f.CMDF)
    {
        if (NULL != pfnAdtIrqCbk[u8Adt][AdtCMDIrq])
        {
            pfnAdtIrqCbk[u8Adt][AdtCMDIrq]();
        }
        pstcM0PAdt->INTCR = ~(1<<AdtCMDIrq);
    }

    if (TRUE == pstcM0PAdt->INTFR_f.OVFF)
    {
        if (NULL != pfnAdtIrqCbk[u8Adt][AdtOVFIrq])
        {
            pfnAdtIrqCbk[u8Adt][AdtOVFIrq]();
        }
        pstcM0PAdt->INTCR = ~(1<<AdtOVFIrq);
    }

    if (TRUE == pstcM0PAdt->INTFR_f.UDFF)
    {
        if (NULL != pfnAdtIrqCbk[u8Adt][AdtUDFIrq])
        {
            pfnAdtIrqCbk[u8Adt][AdtUDFIrq]();
        }
        pstcM0PAdt->INTCR = ~(1<<AdtUDFIrq);
    }

    if (TRUE == pstcM0PAdt->INTFR_f.DTEF)
    {
        if (NULL != pfnAdtIrqCbk[u8Adt][AdtDTEIrq])
        {
            pfnAdtIrqCbk[u8Adt][AdtDTEIrq]();
        }
        pstcM0PAdt->INTCR = ~(1<<AdtDTEIrq);
    }

    if (TRUE == pstcM0PAdt->INTFR_f.SAMLF)
    {
        if (NULL != pfnAdtIrqCbk[u8Adt][AdtSAMLIrq])
        {
            pfnAdtIrqCbk[u8Adt][AdtSAMLIrq]();
        }
        pstcM0PAdt->INTCR = ~(1<<AdtSAMLIrq);
    }

    if (TRUE == pstcM0PAdt->INTFR_f.SAMHF)
    {
        if (NULL != pfnAdtIrqCbk[u8Adt][AdtSAMHIrq])
        {
            pfnAdtIrqCbk[u8Adt][AdtSAMHIrq]();
        }
        pstcM0PAdt->INTCR = ~(1<<AdtSAMHIrq);
    }
}

/**
 * \brief   Adt_ConfigIrq
 *          配置中断
 *
 * \param   [in]  enAdtUnit     ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  enAdtIrq      中断类型
 ×          [in]  bEn           中断使能
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_ConfigIrq(en_adt_unit_t enAdtUnit,
                          en_adt_irq_type_t enAdtIrq,
                          boolean_t bEn,
                          func_ptr_t pfnIrqCalbak)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pfnAdtIrqCbk[enAdtUnit][enAdtIrq] = pfnIrqCalbak;

    u32Val = pstcM0PAdt->ICONR;
    if (bEn)
    {
        u32Val |= 1<<enAdtIrq;
    }
    else
    {
        u32Val &= ~(1<<enAdtIrq);
    }
    pstcM0PAdt->ICONR = u32Val;
    return Ok;
}

/**
 * \brief   Adt_GetIrqFlag
 *          获取中断标志
 *
 * \param   [in]  enAdtUnit     ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  enAdtIrq      中断类型
 ×          [in]  pbFlag        中断标志指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_GetIrqFlag(en_adt_unit_t enAdtUnit,
                           en_adt_irq_type_t enAdtIrq,
                           boolean_t* pbFlag)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    u32Val = pstcM0PAdt->INTFR;
    *pbFlag = (u32Val>>enAdtIrq) & 0x1;

    return Ok;
}

/**
 * \brief   Adt_ClearIrqFlag
 *          清除中断标志
 *
 * \param   [in]  enAdtUnit     ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  enAdtIrq      中断类型
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_ClearIrqFlag(en_adt_unit_t enAdtUnit,
                             en_adt_irq_type_t enAdtIrq)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->INTCR = ~(1<<enAdtIrq);
    return Ok;
}


/**
 * \brief   Adt_ConfigHwCntUp
 *          配置硬件递加事件
 *
 * \param   [in]  enAdtUnit     ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  enAdtHwCntUp  硬件递加事件
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ConfigHwCntUp(en_adt_unit_t enAdtUnit, en_adt_hw_cnt_t enAdtHwCntUp)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (AdtHwCntMax <= enAdtHwCntUp))
    {
        return ErrorInvalidParameter;
    }

    u32Val = pstcM0PAdt->HCUPR;
    pstcM0PAdt->HCUPR = u32Val | (1<<enAdtHwCntUp);
    return Ok;
}

/**
 * \brief   Adt_ClearHwCntUp
 *          清除硬件递加事件
 *
 * \param   [in]  enAdtUnit     ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ClearHwCntUp(en_adt_unit_t enAdtUnit)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }
    pstcM0PAdt->HCUPR = 0;
    return Ok;
}


/**
 * \brief   Adt_ConfigHwCntDwn
 *          配置硬件递减事件
 *
 * \param   [in]  enAdtUnit     ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  enAdtHwCntDwn 硬件递减事件
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ConfigHwCntDwn(en_adt_unit_t enAdtUnit, en_adt_hw_cnt_t enAdtHwCntDwn)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (AdtHwCntMax <= enAdtHwCntDwn))
    {
        return ErrorInvalidParameter;
    }

    u32Val = pstcM0PAdt->HCDOR;
    pstcM0PAdt->HCDOR = u32Val | (1<<enAdtHwCntDwn);
    return Ok;
}

/**
 * \brief   Adt_ClearHwCntDwn
 *          清除硬件递减事件
 *
 * \param   [in]  enAdtUnit     ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ClearHwCntDwn(en_adt_unit_t enAdtUnit)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->HCDOR = 0;
    return Ok;
}

/**
 * \brief   Adt_ConfigHwStart
 *          配置硬件启动事件
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  enAdtHwStart      硬件启动事件
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ConfigHwStart(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwStart)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (AdtHwTrigEnd <= enAdtHwStart))
    {
        return ErrorInvalidParameter;
    }
    u32Val = pstcM0PAdt->HSTAR;
    pstcM0PAdt->HSTAR = u32Val | (1<<enAdtHwStart);
    return Ok;
}

/**
 * \brief   Adt_ClearHwStart
 *          清除硬件启动事件
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ClearHwStart(en_adt_unit_t enAdtUnit)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->HSTAR = 0;
    return Ok;
}

/**
 * \brief   Adt_EnableHwStart
 *          使能硬件启动
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_EnableHwStart(en_adt_unit_t enAdtUnit)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    u32Val = pstcM0PAdt->HSTAR;
    pstcM0PAdt->HSTAR = u32Val | (1u<<31);
    return Ok;
}

/**
 * \brief   Adt_DisableHwStart
 *          除能硬件启动
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_DisableHwStart(en_adt_unit_t enAdtUnit)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    u32Val = pstcM0PAdt->HSTAR;
    pstcM0PAdt->HSTAR = u32Val & 0x7FFFFFFF;
    return Ok;
}

/**
 * \brief   Adt_ConfigHwStop
 *          配置硬件停止事件
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  enAdtHwStop       硬件停止事件
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ConfigHwStop(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwStop)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (AdtHwTrigEnd <= enAdtHwStop))
    {
        return ErrorInvalidParameter;
    }
    u32Val = pstcM0PAdt->HSTPR;
    pstcM0PAdt->HSTPR = u32Val | (1<<enAdtHwStop);
    return Ok;
}

/**
 * \brief   Adt_ClearHwStop
 *          清除硬件停止事件
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ClearHwStop(en_adt_unit_t enAdtUnit)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->HSTPR = 0;
    return Ok;
}

/**
 * \brief   Adt_EnableHwStop
 *          使能硬件停止
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_EnableHwStop(en_adt_unit_t enAdtUnit)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    u32Val = pstcM0PAdt->HSTPR;
    pstcM0PAdt->HSTPR = u32Val | (1u<<31);
    return Ok;
}

/**
 * \brief   Adt_DisableHwStop
 *          除能硬件停止
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_DisableHwStop(en_adt_unit_t enAdtUnit)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    u32Val = pstcM0PAdt->HSTPR;
    pstcM0PAdt->HSTPR = u32Val & 0x7FFFFFFF;
    return Ok;
}

/**
 * \brief   Adt_ConfigHwClear
 *          配置硬件清零事件
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  enAdtHwClear      硬件清零事件
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ConfigHwClear(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwClear)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (AdtHwTrigEnd <= enAdtHwClear))
    {
        return ErrorInvalidParameter;
    }
    u32Val = pstcM0PAdt->HCELR & (1u<<31);
    pstcM0PAdt->HCELR = u32Val | (1u<<enAdtHwClear);
    return Ok;
}

/**
 * \brief   Adt_ClearHwClear
 *          清除硬件清零事件
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ClearHwClear(en_adt_unit_t enAdtUnit)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->HCELR = 0;
    return Ok;
}

/**
 * \brief   Adt_EnableHwClear
 *          使能硬件清零
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_EnableHwClear(en_adt_unit_t enAdtUnit)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    u32Val = pstcM0PAdt->HCELR;
    pstcM0PAdt->HCELR = u32Val | (1u<<31);
    return Ok;
}

/**
 * \brief   Adt_DisableHwClear
 *          除能硬件清零
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_DisableHwClear(en_adt_unit_t enAdtUnit)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    u32Val = pstcM0PAdt->HCELR;
    pstcM0PAdt->HCELR = u32Val & 0x7FFFFFFF;
    return Ok;
}

/**
 * \brief   Adt_ConfigHwCaptureA
 *          配置硬件捕获A事件
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  enAdtHwCaptureA   硬件捕获A时间选择
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ConfigHwCaptureA(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwCaptureA)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (AdtHwTrigEnd <= enAdtHwCaptureA))
    {
        return ErrorInvalidParameter;
    }

    u32Val = pstcM0PAdt->HCPAR;
    pstcM0PAdt->HCPAR = u32Val | (1<<enAdtHwCaptureA);
    pstcM0PAdt->PCONR_f.CAPCA = 1;
    return Ok;
}

/**
 * \brief   Adt_ClearHwCaptureA
 *          清除硬件捕获A事件
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ClearHwCaptureA(en_adt_unit_t enAdtUnit)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->HCPAR = 0;
    return Ok;
}

/**
 * \brief   Adt_ConfigHwCaptureB
 *          配置硬件捕获B事件
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  enAdtHwCaptureB   硬件捕获B时间选择
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ConfigHwCaptureB(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwCaptureB)
{
    uint32_t u32Val;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (AdtHwTrigEnd <= enAdtHwCaptureB))
    {
        return ErrorInvalidParameter;
    }

    u32Val = pstcM0PAdt->HCPBR;
    pstcM0PAdt->HCPBR =  u32Val | (1<<enAdtHwCaptureB);
    pstcM0PAdt->PCONR_f.CAPCB = 1;
    return Ok;
}

/**
 * \brief   Adt_ClearHwCaptureB
 *          清除硬件捕获B事件
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_ClearHwCaptureB(en_adt_unit_t enAdtUnit)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->HCPBR = 0;
    return Ok;
}

/**
 * \brief   Adt_SwSyncStart
 *          软件同步开始
 *
 * \param   [in]  pstcAdtSwSyncStart    软件同步开始指针
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_SwSyncStart(stc_adt_sw_sync_t* pstcAdtSwSyncStart)
{
    uint32_t u32Val = 0;

    if (NULL == pstcAdtSwSyncStart)
    {
        return ErrorInvalidParameter;
    }

    if (pstcAdtSwSyncStart->bAdTim4)
    {
        u32Val |= 0x1;
    }
    if (pstcAdtSwSyncStart->bAdTim5)
    {
        u32Val |= 0x2;
    }
    if (pstcAdtSwSyncStart->bAdTim6)
    {
        u32Val |= 0x4;
    }

    M0P_ADT0->SSTAR = u32Val;
    return Ok;
}

/**
 * \brief   Adt_SwSyncStop
 *          软件同步停止
 *
 * \param   [in]  pstcAdtSwSyncStop     软件同步停止指针
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_SwSyncStop(stc_adt_sw_sync_t* pstcAdtSwSyncStop)
{
    uint32_t u32Val = 0;

    if (NULL == pstcAdtSwSyncStop)
    {
        return ErrorInvalidParameter;
    }

    if (pstcAdtSwSyncStop->bAdTim4)
    {
        u32Val |= 0x1;
    }
    if (pstcAdtSwSyncStop->bAdTim5)
    {
        u32Val |= 0x2;
    }
    if (pstcAdtSwSyncStop->bAdTim6)
    {
        u32Val |= 0x4;
    }

    M0P_ADT0->SSTPR = u32Val;
    return Ok;
}

/**
 * \brief   Adt_SwSyncClear
 *          软件同步清零
 *
 * \param   [in]  pstcAdtSwSyncClear    软件同步清零指针
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_SwSyncClear(stc_adt_sw_sync_t* pstcAdtSwSyncClear)
{
    uint32_t u32Val = 0;

    if (NULL == pstcAdtSwSyncClear)
    {
        return ErrorInvalidParameter;
    }

    if (pstcAdtSwSyncClear->bAdTim4)
    {
        u32Val |= 0x1;
    }
    if (pstcAdtSwSyncClear->bAdTim5)
    {
        u32Val |= 0x2;
    }
    if (pstcAdtSwSyncClear->bAdTim6)
    {
        u32Val |= 0x4;
    }

    M0P_ADT0->SCLRR = u32Val;
    return Ok;
}

/**
 * \brief   Adt_GetSwSyncState
 *          获取软件同步运行状态
 *
 * \param   [in]  pstcAdtSwSyncState    ADV Timer软件同步运行状态指针
 *
 * \retval  en_result_t    Ok:  设置成功
 *                         ErrorInvalidParameter:  无效参数
 */
en_result_t Adt_GetSwSyncState(stc_adt_sw_sync_t* pstcAdtSwSyncState)
{
    if (NULL == pstcAdtSwSyncState)
    {
        return ErrorInvalidParameter;
    }

    if (M0P_ADT0->SSTAR & 0x1)
    {
        pstcAdtSwSyncState->bAdTim4 = TRUE;
    }
    else
    {
        pstcAdtSwSyncState->bAdTim4 = FALSE;
    }
    if (M0P_ADT0->SSTAR & 0x2)
    {
        pstcAdtSwSyncState->bAdTim5 = TRUE;
    }
    else
    {
        pstcAdtSwSyncState->bAdTim5 = FALSE;
    }
    if (M0P_ADT0->SSTAR & 0x4)
    {
        pstcAdtSwSyncState->bAdTim6 = TRUE;
    }
    else
    {
        pstcAdtSwSyncState->bAdTim6 = FALSE;
    }
    return Ok;
}

/**
 * \brief   Adt_AosTrigConfig
 *          AOS触发配置
 *
 * \param   [in]  pstcAdtAosTrigCfg 触发配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_AosTrigConfig(stc_adt_aos_trig_cfg_t* pstcAdtAosTrigCfg)
{
    if (NULL == pstcAdtAosTrigCfg)
    {
        return ErrorInvalidParameter;
    }

    M0P_ADT0->ITRIG_f.IAOS0S = pstcAdtAosTrigCfg->enAos0TrigSrc;
    M0P_ADT0->ITRIG_f.IAOS1S = pstcAdtAosTrigCfg->enAos1TrigSrc;
    M0P_ADT0->ITRIG_f.IAOS2S = pstcAdtAosTrigCfg->enAos2TrigSrc;
    M0P_ADT0->ITRIG_f.IAOS3S = pstcAdtAosTrigCfg->enAos3TrigSrc;
    return Ok;
}

/**
 * \brief   Adt_IrqTrigConfig
 *          中断触发配置
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
                  pstcAdtIrqTrigCfg 触发配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_IrqTrigConfig(en_adt_unit_t enAdtUnit,
                              stc_adt_irq_trig_cfg_t* pstcAdtIrqTrigCfg)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (NULL == pstcAdtIrqTrigCfg))
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->CTRLR_f.UDFE = pstcAdtIrqTrigCfg->bAdtUnderFlowTrigEn;
    pstcM0PAdt->CTRLR_f.OVFE = pstcAdtIrqTrigCfg->bAdtOverFlowTrigEn;
    pstcM0PAdt->CTRLR_f.CMDE = pstcAdtIrqTrigCfg->bAdtCntMatchDTrigEn;
    pstcM0PAdt->CTRLR_f.CMCE = pstcAdtIrqTrigCfg->bAdtCntMatchCTrigEn;
    pstcM0PAdt->CTRLR_f.CMBE = pstcAdtIrqTrigCfg->bAdtCntMatchBTrigEn;
    pstcM0PAdt->CTRLR_f.CMAE = pstcAdtIrqTrigCfg->bAdtCntMatchATrigEn;
    return Ok;
}

/**
 * \brief   Adt_PortTrigConfig
 *          端口触发配置
 *
 * \param   [in]  enAdtTrigPort         触发端口
 ×          [in]  pstcAdtPortTrigCfg    触发配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_PortTrigConfig(en_adt_trig_port_t enAdtTrigPort,
                               stc_adt_port_trig_cfg_t* pstcAdtPortTrigCfg)
{
    if (NULL == pstcAdtPortTrigCfg)
    {
        return ErrorInvalidParameter;
    }

    switch (enAdtTrigPort)
    {
        case AdtTrigA:
            M0P_ADT0->TTRIG_f.TRIGAS = pstcAdtPortTrigCfg->enTrigSrc;
            M0P_ADT0->FCONR_f.NOFIENTA = pstcAdtPortTrigCfg->bFltEn;
            M0P_ADT0->FCONR_f.NOFICKTA = pstcAdtPortTrigCfg->enFltClk;
            break;

        case AdtTrigB:
            M0P_ADT0->TTRIG_f.TRIGBS = pstcAdtPortTrigCfg->enTrigSrc;
            M0P_ADT0->FCONR_f.NOFIENTB = pstcAdtPortTrigCfg->bFltEn;
            M0P_ADT0->FCONR_f.NOFICKTB = pstcAdtPortTrigCfg->enFltClk;
            break;

        case AdtTrigC:
            M0P_ADT0->TTRIG_f.TRIGCS = pstcAdtPortTrigCfg->enTrigSrc;
            M0P_ADT0->FCONR_f.NOFIENTC = pstcAdtPortTrigCfg->bFltEn;
            M0P_ADT0->FCONR_f.NOFICKTC = pstcAdtPortTrigCfg->enFltClk;
            break;

        case AdtTrigD:
            M0P_ADT0->TTRIG_f.TRIGDS = pstcAdtPortTrigCfg->enTrigSrc;
            M0P_ADT0->FCONR_f.NOFIENTD = pstcAdtPortTrigCfg->bFltEn;
            M0P_ADT0->FCONR_f.NOFICKTD = pstcAdtPortTrigCfg->enFltClk;
            break;

        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 * \brief   Adt_TIMxXPortConfig
 *          TIMxX端口配置
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *          [in]  enAdtTIMxXPort    TIMxX端口
 ×          [in]  pstcAdtTIMxXCfg   TIMxX端口配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_TIMxXPortConfig(en_adt_unit_t enAdtUnit,
                                en_adt_TIMxX_port_t enAdtTIMxXPort,
                                stc_adt_TIMxX_port_cfg_t* pstcAdtTIMxXCfg)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (NULL == pstcAdtTIMxXCfg))
    {
        return ErrorInvalidParameter;
    }

    switch (enAdtTIMxXPort)
    {
        case AdtTIMxA:
            pstcM0PAdt->PCONR_f.CAPCA = pstcAdtTIMxXCfg->enCap;
            pstcM0PAdt->PCONR_f.STACA = pstcAdtTIMxXCfg->enStaOut;
            pstcM0PAdt->PCONR_f.STPCA = pstcAdtTIMxXCfg->enStpOut;
            pstcM0PAdt->PCONR_f.STASTPSA = pstcAdtTIMxXCfg->enStaStp;
            pstcM0PAdt->PCONR_f.CMPCA = pstcAdtTIMxXCfg->enCmpc;
            pstcM0PAdt->PCONR_f.PERCA = pstcAdtTIMxXCfg->enPerc;
            pstcM0PAdt->PCONR_f.OUTENA = pstcAdtTIMxXCfg->bOutEn;
            pstcM0PAdt->PCONR_f.DISSELA = pstcAdtTIMxXCfg->enDisSel;
            pstcM0PAdt->PCONR_f.DISVALA = pstcAdtTIMxXCfg->enDisVal;
            pstcM0PAdt->FCONR_f.NOFIENGA = pstcAdtTIMxXCfg->bFltEn;
            pstcM0PAdt->FCONR_f.NOFICKGA = pstcAdtTIMxXCfg->enFltClk;
            break;

        case AdtTIMxB:
            pstcM0PAdt->PCONR_f.CAPCB = pstcAdtTIMxXCfg->enCap;
            pstcM0PAdt->PCONR_f.STACB = pstcAdtTIMxXCfg->enStaOut;
            pstcM0PAdt->PCONR_f.STPCB = pstcAdtTIMxXCfg->enStpOut;
            pstcM0PAdt->PCONR_f.STASTPSB = pstcAdtTIMxXCfg->enStaStp;
            pstcM0PAdt->PCONR_f.CMPCB = pstcAdtTIMxXCfg->enCmpc;
            pstcM0PAdt->PCONR_f.PERCB = pstcAdtTIMxXCfg->enPerc;
            pstcM0PAdt->PCONR_f.OUTENB = pstcAdtTIMxXCfg->bOutEn;
            pstcM0PAdt->PCONR_f.DISSELB = pstcAdtTIMxXCfg->enDisSel;
            pstcM0PAdt->PCONR_f.DISVALB = pstcAdtTIMxXCfg->enDisVal;
            pstcM0PAdt->FCONR_f.NOFIENGB = pstcAdtTIMxXCfg->bFltEn;
            pstcM0PAdt->FCONR_f.NOFICKGB = pstcAdtTIMxXCfg->enFltClk;
            break;

        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 * \brief   Adt_EnableBreakPort
 *          使能端口刹车
 *
 * \param   [in]  port              端口
 *                pstcAdtBrkPtCfg   端口刹车配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_EnableBreakPort(uint8_t port, stc_adt_break_port_cfg_t* pstcAdtBrkPtCfg)
{
    uint32_t u32Val;

    if (NULL == pstcAdtBrkPtCfg)
    {
        return ErrorInvalidParameter;
    }
    
    u32Val = M0P_ADT0->PTBKP;
    u32Val &= ~(1u<<port);
    M0P_ADT0->PTBKP = u32Val | (pstcAdtBrkPtCfg->enPol<<port);
	
	u32Val = M0P_ADT0->PRBKS;
    M0P_ADT0->PRBKS = u32Val | (1u<<port);

    return Ok;
}

/**
 * \brief   Adt_ClearBreakPort
 *          清除端口刹车
 *
 * \param   none
 *
 * \retval  none
 */
void Adt_ClearBreakPort(void)
{
    M0P_ADT0->PRBKS = 0;
}

/**
 * \brief   Adt_Disable3Cfg
 *          无效条件3配置
 *
 * \param   [in]  pstcAdtDisable3   无效条件3配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_Disable3Cfg(stc_adt_disable_3_cfg_t* pstcAdtDisable3)
{
    uint8_t i;

    if (NULL == pstcAdtDisable3)
    {
        return ErrorInvalidParameter;
    }

    Adt_ClearBreakPort();
    for (i = 0; i < 15; i++)
    {
        if (TRUE == pstcAdtDisable3->stcBrkPtCfg[i].bPortEn)
        {
            Adt_EnableBreakPort(i, &pstcAdtDisable3->stcBrkPtCfg[i]);
        }
    }

    M0P_ADT0->AOSSR_f.BFILTE = pstcAdtDisable3->bFltEn;
    M0P_ADT0->AOSSR_f.BFILTS = pstcAdtDisable3->enFltClk;
    M0P_ADT0->AOSSR_f.SOFTB = pstcAdtDisable3->bSwBrk;

    return Ok;
}

/**
 * \brief   Adt_GetPortBreakFlag
 *          获取端口刹车标志
 *
 * \param   none
 *
 * \retval  TRUE or FALSE
 */
boolean_t Adt_GetPortBreakFlag(void)
{
    return M0P_ADT0->AOSSR_f.FBRAK;
}

/**
 * \brief   Adt_ClearPortBreakFlag
 *          清除端口刹车标志
 *
 * \param   none
 *
 * \retval  none
 */
void Adt_ClearPortBreakFlag(void)
{
    M0P_ADT0->AOSCL_f.FBRAKE = 0;
}

/**
 * \brief   Adt_Disable1Cfg
 *          无效条件1配置
 *
 * \param   [in]  pstcAdtDisable1   无效条件1配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_Disable1Cfg(stc_adt_disable_1_cfg_t* pstcAdtDisable1)
{

    if (NULL == pstcAdtDisable1)
    {
        return ErrorInvalidParameter;
    }

    M0P_ADT0->AOSSR_f.SMH2 = pstcAdtDisable1->bTim6OutSH;
    M0P_ADT0->AOSSR_f.SMH1 = pstcAdtDisable1->bTim5OutSH;
    M0P_ADT0->AOSSR_f.SMH0 = pstcAdtDisable1->bTim4OutSH;
    M0P_ADT0->AOSSR_f.SML2 = pstcAdtDisable1->bTim6OutSL;
    M0P_ADT0->AOSSR_f.SML1 = pstcAdtDisable1->bTim5OutSL;
    M0P_ADT0->AOSSR_f.SML0 = pstcAdtDisable1->bTim4OutSL;

    return Ok;
}

/**
 * \brief   Adt_GetSameBreakFlag
 *          获取同高同低刹车标志
 *
 * \param   none
 *
 * \retval  TRUE or FALSE
 */
boolean_t Adt_GetSameBreakFlag(void)
{
    return M0P_ADT0->AOSSR_f.FSAME;
}

/**
 * \brief   Adt_ClearSameBreakFlag
 *          清除同高同低刹车标志
 *
 * \param   none
 *
 * \retval  none
 */
void Adt_ClearSameBreakFlag(void)
{
    M0P_ADT0->AOSCL_f.FSAME = 0;
}

/**
 * \brief   Adt_PwmDitherConfig
 *          PWM展频配置
 *
 * \param   [in]  pstcAdtPwmDitherCfg   PWM展频配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_PwmDitherConfig(stc_adt_pwm_dither_cfg_t* pstcAdtPwmDitherCfg)
{
    if (NULL == pstcAdtPwmDitherCfg)
    {
        return ErrorInvalidParameter;
    }

    M0P_ADT0->CTRLR_f.DITENS = pstcAdtPwmDitherCfg->enAdtPDType;
    M0P_ADT0->CTRLR_f.DITENB = pstcAdtPwmDitherCfg->bTimxBPDEn;
    M0P_ADT0->CTRLR_f.DITNA = pstcAdtPwmDitherCfg->bTimxAPDEn;
    return Ok;
}

/**
 * \brief   Adt_Init
 *          ADT初始化
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 * \param   [in]  pstcAdtBaseCntCfg     计数配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_Init(en_adt_unit_t enAdtUnit, stc_adt_basecnt_cfg_t* pstcAdtBaseCntCfg)
{
    int32_t i;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (NULL == pstcAdtBaseCntCfg))
    {
        return ErrorInvalidParameter;
    }

    if (AdtTriangleModeB < pstcAdtBaseCntCfg->enCntMode)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->GCONR_f.MODE = pstcAdtBaseCntCfg->enCntMode;
    pstcM0PAdt->GCONR_f.DIR = pstcAdtBaseCntCfg->enCntDir;
    pstcM0PAdt->GCONR_f.CKDIV = pstcAdtBaseCntCfg->enCntClkDiv;

    for (i = 0; i < 16; i++)
    {
        pfnAdtIrqCbk[enAdtUnit][i] = NULL;
    }

    AdtEnableNvic((IRQn_Type)((int32_t)TIM4_IRQn + (int32_t)enAdtUnit));

    return Ok;
}

/**
 * \brief   Adt_DeInit
 *          ADT Deinit
 *
 * \param   none
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_DeInit(en_adt_unit_t enAdtUnit)
{
    int32_t i;
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->GCONR_f.START = 0;

    AdtDisableNvic((IRQn_Type)((int32_t)TIM4_IRQn + (int32_t)enAdtUnit));

    for (i = 0; i < 16; i++)
    {
        pfnAdtIrqCbk[enAdtUnit][i] = NULL;
    }

    return Ok;
}

/**
 * \brief   Adt_StartCount
 *          开始计数
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_StartCount(en_adt_unit_t enAdtUnit)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->GCONR_f.START = 1;

    return Ok;
}

/**
 * \brief   Adt_StopCount
 *          停止计数
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_StopCount(en_adt_unit_t enAdtUnit)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->GCONR_f.START = 0;

    return Ok;
}

/**
 * \brief   Adt_SetCount
 *          设置计数值
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *                u16Value          计数值
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_SetCount(en_adt_unit_t enAdtUnit, uint16_t u16Value)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->CNTER_f.CNT = u16Value;
    return Ok;
}

/**
 * \brief   Adt_ClearCount
 *          清除计数值
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *                u16Value          计数值
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_ClearCount(en_adt_unit_t enAdtUnit)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->CNTER_f.CNT = 0;
    return Ok;
}

/**
 * \brief   Adt_GetCntState
 *          获取计数状态
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_GetCntState(en_adt_unit_t enAdtUnit, stc_adt_cntstate_cfg_t* pstcAdtCntState)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (NULL == pstcAdtCntState))
    {
        return ErrorInvalidParameter;
    }

    pstcAdtCntState->u16Counter = pstcM0PAdt->CNTER_f.CNT;
    pstcAdtCntState->u8ValidPeriod = pstcM0PAdt->STFLR_f.VPERNUM;
    pstcAdtCntState->bDTEF = pstcM0PAdt->STFLR_f.DTEF;
    pstcAdtCntState->bUDFF = pstcM0PAdt->STFLR_f.UDFF;
    pstcAdtCntState->bOVFF = pstcM0PAdt->STFLR_f.OVFF;
    pstcAdtCntState->bCMDF = pstcM0PAdt->STFLR_f.CMDF;
    pstcAdtCntState->bCMCF = pstcM0PAdt->STFLR_f.CMCF;
    pstcAdtCntState->bCMBF = pstcM0PAdt->STFLR_f.CMBF;
    pstcAdtCntState->bCMAF = pstcM0PAdt->STFLR_f.CMAF;

    return Ok;
}

/**
 * \brief   Adt_SetPeriod
 *          配置计数周期
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  u16Period         计数周期值
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_SetPeriod(en_adt_unit_t enAdtUnit, uint16_t u16Period)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->PERAR = u16Period;

    return Ok;
}

/**
 * \brief   Adt_SetPeriodBuf
 *          配置计数周期缓冲
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  u16PeriodBuf          计数周期缓冲值
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_SetPeriodBuf(en_adt_unit_t enAdtUnit, uint16_t u16PeriodBuf)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->PERBR = u16PeriodBuf;
    pstcM0PAdt->BCONR_f.BENP = 1u;

    return Ok;
}

/**
 * \brief   Adt_ClearPeriodBuf
 *          配置计数周期
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_ClearPeriodBuf(en_adt_unit_t enAdtUnit)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->BCONR_f.BENP = 0;
    pstcM0PAdt->PERBR = 0;

    return Ok;
}

/**
 * \brief   Adt_SetValidPeriod
 *          配置有效计数周期
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  pstcAdtValidPerCfg            有效计数周期配置指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_SetValidPeriod(en_adt_unit_t enAdtUnit,
                               stc_adt_validper_cfg_t* pstcAdtValidPerCfg)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if ((AdTIM6 < enAdtUnit) || (NULL == pstcAdtValidPerCfg))
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->VPERR_f.PCNTS = pstcAdtValidPerCfg->enValidCnt;
    pstcM0PAdt->VPERR_f.PCNTE = pstcAdtValidPerCfg->enValidCdt;
    pstcM0PAdt->VPERR_f.GEPERID = pstcAdtValidPerCfg->bPeriodD;
    pstcM0PAdt->VPERR_f.GEPERIC = pstcAdtValidPerCfg->bPeriodC;
    pstcM0PAdt->VPERR_f.GEPERIB = pstcAdtValidPerCfg->bPeriodB;
    pstcM0PAdt->VPERR_f.GEPERIA = pstcAdtValidPerCfg->bPeriodA;

    return Ok;
}

/**
 * \brief   Adt_SetCompareValue
 *          配置比较输出计数基准值
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *          [in]  enAdtCompare      比较基准
 ×          [in]  u16Compare        比较基准值
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_SetCompareValue(en_adt_unit_t enAdtUnit,
                                en_adt_compare_t enAdtCompare,
                                uint16_t u16Compare)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    if (AdtCompareA == enAdtCompare)
    {
        pstcM0PAdt->GCMAR = u16Compare;
    }
    else if (AdtCompareB == enAdtCompare)
    {
        pstcM0PAdt->GCMBR = u16Compare;
    }
    else if (AdtCompareC == enAdtCompare)
    {
        pstcM0PAdt->GCMCR = u16Compare;
    }
    else if (AdtCompareD == enAdtCompare)
    {
        pstcM0PAdt->GCMDR = u16Compare;
    }
    else
    {
        return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 * \brief   Adt_EnableValueBuf
 *          配置通用比较值/捕获值缓存传送
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *          [in]  enAdtTIMxXPort    TIMxX端口
 ×          [in]  bCompareBufEn     通用比较值缓存传送使能
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_EnableValueBuf(en_adt_unit_t enAdtUnit,
                                 en_adt_TIMxX_port_t enAdtTIMxXPort,
                                 boolean_t bCompareBufEn)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    if (AdtTIMxA == enAdtTIMxXPort)
    {
        pstcM0PAdt->BCONR_f.BENA = bCompareBufEn;
    }
    else if (AdtTIMxB == enAdtTIMxXPort)
    {
        pstcM0PAdt->BCONR_f.BENB = bCompareBufEn;
    }
    else
    {
        return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 * \brief   Adt_ClearValueBuf
 *          清除比较输出计数值/捕获值缓存
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *          [in]  enAdtTIMxXPort    TIMxX端口
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_ClearValueBuf(en_adt_unit_t enAdtUnit,
                                     en_adt_TIMxX_port_t enAdtTIMxXPort)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    if (AdtTIMxA == enAdtTIMxXPort)
    {
        pstcM0PAdt->GCMCR = 0;
        pstcM0PAdt->BCONR_f.BENA = 0;
    }
    else if (AdtTIMxB == enAdtTIMxXPort)
    {
        pstcM0PAdt->GCMDR = 0;
        pstcM0PAdt->BCONR_f.BENB = 0;
    }
    else
    {
        return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 * \brief   Adt_GetCaptureValue
 *          获取捕获值
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *          [in]  enAdtTIMxXPort    TIMxX端口
 *          [in]  pu16Capture       捕获值指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_GetCaptureValue(en_adt_unit_t enAdtUnit,
                                en_adt_TIMxX_port_t enAdtTIMxXPort,
                                uint16_t* pu16Capture)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    if (AdtTIMxA == enAdtTIMxXPort)
    {
        *pu16Capture = pstcM0PAdt->GCMAR_f.GCMA_D;
    }
    else if (AdtTIMxB == enAdtTIMxXPort)
    {
        *pu16Capture = pstcM0PAdt->GCMBR_f.GCMB_D;
    }
    else
    {
        return ErrorInvalidParameter;
    }
    return Ok;
}

/**
 * \brief   Adt_GetCaptureBuf
 *          获取捕获缓存值
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 *          [in]  enAdtTIMxXPort    TIMxX端口
 *          [in]  pu16CaptureBuf    捕获缓存值指针
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_GetCaptureBuf(en_adt_unit_t enAdtUnit,
                                en_adt_TIMxX_port_t enAdtTIMxXPort,
                                uint16_t* pu16CaptureBuf)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    if (AdtTIMxA == enAdtTIMxXPort)
    {
        *pu16CaptureBuf = pstcM0PAdt->GCMCR_f.GCMC_D;
    }
    else if (AdtTIMxB == enAdtTIMxXPort)
    {
        *pu16CaptureBuf = pstcM0PAdt->GCMDR_f.GCMD_D;
    }
    else
    {
        return ErrorInvalidParameter;
    }
    return Ok;
}

/**
 * \brief   Adt_SetDTUA
 *          设置死区时间基准值
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  u16Value          基准值
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_SetDTUA(en_adt_unit_t enAdtUnit,
                        uint16_t u16Value)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->DTUAR = u16Value;


    return Ok;
}

/**
 * \brief   Adt_SetDTDA
 *          设置死区时间基准值
 *
 * \param   [in]  enAdtUnit         ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  u16Value          基准值
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_SetDTDA(en_adt_unit_t enAdtUnit,
                        uint16_t u16Value)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->DTDAR = u16Value;


    return Ok;
}

/**
 * \brief   Adt_ConfigDT
 *          配置死区时间功能
 *
 * \param   [in]  enAdtUnit     ADV Timer通道选择（TIM4、TIM5、TIM6）
 ×          [in]  bDTEn         死区功能使能
 ×          [in]  bEqual        DTDAR的值和DTUAR的值自动相等
 *
 * \retval  en_result_t  Ok:  配置成功
 *                       ErrorInvalidParameter: 无效参数
 */
en_result_t Adt_ConfigDT(en_adt_unit_t enAdtUnit,
                         boolean_t bDTEn,
                         boolean_t bEqual)
{
    volatile M0P_ADT_TypeDef *pstcM0PAdt = (M0P_ADT_TypeDef *)((uint32_t)M0P_ADT0+0x400*enAdtUnit);

    if (AdTIM6 < enAdtUnit)
    {
        return ErrorInvalidParameter;
    }

    pstcM0PAdt->DCONR_f.DTCEN = bDTEn;
    pstcM0PAdt->DCONR_f.SEPA = bEqual;

    return Ok;
}



//@} // Lvd Group

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

