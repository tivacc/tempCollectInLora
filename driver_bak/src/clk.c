/******************************************************************************
*Copyright(C)2017, Huada Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Huada Semiconductor Co.,Ltd("HDSC").
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

/** \file clk.c
 **
 ** Common API of clk.
 ** @link clkGroup Some description @endlink
 **
 **   - 2017-05-04
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "clk.h"
#include <math.h>
/**
 *******************************************************************************
 ** \addtogroup Clk Common Functions
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define CLK_TIMEOUT             (1000000u)

#define IS_VALID_SRC(x)         (   ClkRCH == (x)||\
                                    ClkXTH == (x)||\
                                    ClkRCL == (x)||\
                                    ClkXTL == (x) )


#define IS_VALID_FUNC(x)        (   ClkFuncWkupRCH == (x)||\
                                    ClkFuncXTHEn == (x)||\
                                    ClkFuncXTLEn == (x)||\
                                    ClkFuncXTLAWSON == (x)||\
                                    ClkFuncFaultEn == (x)||\
                                    ClkFuncRtcLPWEn == (x)||\
                                    ClkFuncLockUpEn == (x)||\
                                    ClkFuncRstPinIOEn == (x)||\
                                    ClkFuncSwdPinIOEn == (x) )


/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
extern uint32_t SystemCoreClock;
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static void ClkUnlock(void)
{
    M0P_CLOCK->SYSCTRL2 = 0x5A5A;
    M0P_CLOCK->SYSCTRL2 = 0xA5A5;
}

static void ClkWriteDummy(void)
{
    M0P_CLOCK->SYSCTRL0_f.RESERVED11 = 0x0;
}

en_result_t Clk_SetFunc(en_clk_func_t enFunc, boolean_t bFlag)
{
    en_result_t enRet = Ok;

    ClkUnlock();
    bFlag = !!bFlag;

    switch (enFunc)
    {
        case ClkFuncWkupRCH:
            M0P_CLOCK->SYSCTRL0_f.WAKEUP_BYRCH = bFlag;
            break;
        case ClkFuncXTHEn:
            M0P_CLOCK->SYSCTRL1_f.EXTH_EN = bFlag;
            break;
        case ClkFuncXTLEn:
            M0P_CLOCK->SYSCTRL1_f.EXTL_EN = bFlag;
            break;
        case ClkFuncXTLAWSON:
            M0P_CLOCK->SYSCTRL1_f.XTL_ALWAYSON = bFlag;
            break;
        case ClkFuncFaultEn:
            M0P_CLOCK->SYSCTRL1_f.CLOCK_FT_EN = bFlag;
            break;
        case ClkFuncRtcLPWEn:
            M0P_CLOCK->SYSCTRL1_f.RTC_LPW = bFlag;
            break;
        case ClkFuncLockUpEn:
            M0P_CLOCK->SYSCTRL1_f.LOCK_EN = bFlag;
            break;
        case ClkFuncRstPinIOEn:
            M0P_CLOCK->SYSCTRL1_f.RES_UIO = bFlag;
            break;
        case ClkFuncSwdPinIOEn:
            M0P_CLOCK->SYSCTRL1_f.SWD_UIO = bFlag;
            break;
        default:
            ClkWriteDummy();
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}


/**
 * \brief   Clk_SetSource
 *          set chip clock input source.
 *
 * \param   [in]  enSource
 *
 * \retval  en_result_t     Ok: set clk done
 *                          ErrorInvalidParameter: invalid enSource
 */
en_result_t Clk_SwitchTo(en_clk_source_t enSource)
{
    en_clk_source_t ClkOld;
    en_clk_source_t ClkNew = enSource;


    ClkOld = (en_clk_source_t)M0P_CLOCK->SYSCTRL0_f.CLK_SW4_SEL;
    //set wait
    switch (ClkNew)
    {
        case ClkRCH:
            break;
        case ClkXTH:
            Clk_SetXTH_StableTime(ClkCycle16384);
            break;
        case ClkRCL:
            Clk_SetRCL_StableTime(ClkCycle256);
            break;
        case ClkXTL:
            Clk_SetXTL_StableTime(ClkCycle4096);
            break;
        default:
            return ErrorInvalidParameter;
    }

    //add flash wait
    M0P_FLASH->BYPASS = 0x5A5A;
    M0P_FLASH->BYPASS = 0xA5A5;
    if ((ClkXTH == ClkNew) && (CLK_XTH_VAL > (24 * 1000 * 1000)))
    {
        M0P_FLASH->CR_f.WAIT = TRUE;
    } else
    {
        M0P_FLASH->CR_f.WAIT = FALSE;
    }

    Clk_Enable(ClkNew, TRUE);

    while (FALSE == Clk_GetClkRdy(ClkNew));

    //switch
    Clk_SetSource(ClkNew);

    //close old
    Clk_Enable(ClkOld, FALSE);

    SystemCoreClockUpdate();



    return Ok;

}


/**
 * \brief   Clk_GetClk
 *          Get the indicated clock source frequence.
 *
 * \param   [in]
 *
 * \retval  uint32_t         the hclk clock frequency
 */
uint32_t Clk_GetHClkFreq(void)
{
    uint32_t u32Val = 0;
    uint8_t u8hDiv = 0;
    const uint8_t u8hcr_tbl[] = { 4, 8, 16, 24 };
    const uint16_t u16lcr_tbl[] = { 328, 385 };

    en_clk_source_t enSrc;

    //1. get current input source
    enSrc = (en_clk_source_t)M0P_CLOCK->SYSCTRL0_f.CLK_SW4_SEL;

    switch (enSrc)
    {
        case ClkRCH:
            u32Val = u8hcr_tbl[M0P_CLOCK->RCH_CR_f.FSEL];
            u32Val *= 1000u * 1000u;
            break;
        case ClkRCL:
            u32Val = u16lcr_tbl[M0P_CLOCK->RCL_CR_f.FSEL];
            u32Val *= 1000u;
            break;
        case ClkXTH:
            u32Val = CLK_XTH_VAL;
            break;
        case ClkXTL:
            u32Val = CLK_XTL_VAL;
            break;
    }

    u8hDiv =  (uint8_t)pow(2, M0P_CLOCK->SYSCTRL0_f.HCLK_PRS);
    u32Val = u32Val / u8hDiv;

    return u32Val;
}

uint32_t Clk_GetPClkFreq(void)
{
    uint8_t u8Div = 0;
    uint32_t u32Val = 0;

    u32Val = Clk_GetHClkFreq();
    u8Div = M0P_CLOCK->SYSCTRL0_f.PCLK_PRS;
    u8Div = (uint8_t)pow(2, u8Div);
    u32Val /= u8Div;

    return u32Val;
}


/**
 * \brief   Clk_Init
 *
 * \param   [in]  pstcCfg       pointer to clock config data
 *
 * \retval  en_result_t         Ok:     init done
 *                              ErrorInvalidParameter: pstcCfg is NULL
 *                              pstcCfg has invalid param
 */
en_result_t Clk_Init(stc_clk_config_t *pstcCfg)
{
    ASSERT(NULL != pstcCfg);
    ASSERT(pstcCfg->enHClkDiv <= ClkDiv128);
    ASSERT(pstcCfg->enPClkDiv <= ClkDiv8);

    Clk_SwitchTo(pstcCfg->enClkSrc);
    Clk_SetHClkDiv(pstcCfg->enHClkDiv);
    Clk_SetPClkDiv(pstcCfg->enPClkDiv);

    return Ok;
}

/**
 * \brief   Clk_DeInit
 *          set clock to Hcr 4Mhz.
 * \param   [in]
 *
 * \retval  en_result_t         Ok:     init done
 */
en_result_t Clk_DeInit(void)
{
    //set clock to HCR 4Mhz
    Clk_SwitchTo(ClkRCH);
    Clk_SetRCHFreq(ClkFreq4Mhz);
    Clk_SetHClkDiv(ClkDiv1);
    Clk_SetPClkDiv(ClkDiv1);
    return Ok;
}



//Clk_SetSource
en_result_t Clk_SetSource(en_clk_source_t enSource)
{
    en_result_t enRet = Ok;

    ClkUnlock();

    switch (enSource)
    {
        case ClkRCH:
        case ClkRCL:
        case ClkXTH:
        case ClkXTL:
            M0P_CLOCK->SYSCTRL0_f.CLK_SW4_SEL = enSource;
            break;
        default:
            ClkWriteDummy();
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

boolean_t Clk_GetClkRdy(en_clk_source_t enClk)
{
    boolean_t bFlag = FALSE;

    switch (enClk)
    {
        case ClkRCH:
            bFlag = M0P_CLOCK->RCH_CR_f.STABLE;
            break;
        case ClkXTH:
            bFlag = M0P_CLOCK->XTH_CR_f.STABLE;
            break;
        case ClkRCL:
            bFlag = M0P_CLOCK->RCL_CR_f.STABLE;
            break;
        case ClkXTL:
            bFlag = M0P_CLOCK->XTL_CR_f.STABLE;
            break;
        default:
            break;
    }
    return bFlag;
}

/**
 * \brief   Clk_SetHCrFreq
 *          set high cr source frequency.
 *
 * \param   [in]  enVal         high cr frequency value enumeration
 *
 * \retval  en_result_t         Ok: set done
 *                              ErrorInvalidParameter: invalid enVal
 */
en_result_t Clk_SetRCHFreq(en_clk_freq_t enVal)
{
    en_result_t enRet = Ok;

    switch (enVal)
    {
        case ClkFreq4Mhz:
            M0P_CLOCK->RCH_CR_f.FSEL = 0u;
            break;
        case ClkFreq8Mhz:
            M0P_CLOCK->RCH_CR_f.FSEL = 1u;
            break;
        case ClkFreq16Mhz:
            M0P_CLOCK->RCH_CR_f.FSEL = 2u;
            break;
        case ClkFreq24Mhz:
            M0P_CLOCK->RCH_CR_f.FSEL = 3u;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

en_result_t Clk_SetRCLFreq(en_clk_freq_t enVal)
{
    en_result_t enRet = Ok;

    switch (enVal)
    {
        case ClkFreq32_8K:
            M0P_CLOCK->RCL_CR_f.FSEL = 0u;
            break;
        case ClkFreq38_4K:
            M0P_CLOCK->RCL_CR_f.FSEL = 1u;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}


en_result_t Clk_SetHClkDiv(uint8_t u8Div)
{
    switch (u8Div)
    {
        case ClkDiv1:
        case ClkDiv2:
        case ClkDiv4:
        case ClkDiv8:
        case ClkDiv16:
        case ClkDiv32:
        case ClkDiv64:
        case ClkDiv128:
            ClkUnlock();
            u8Div = (uint8_t)(log(u8Div) / log(2));
            M0P_CLOCK->SYSCTRL0_f.HCLK_PRS = u8Div;
            break;
        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 * \brief   Clk_SetPClkDiv
 *
 * \param   [in]  u8Div
 *
 * \retval  en_result_t
 */
en_result_t Clk_SetPClkDiv(uint8_t u8Div)
{

    switch (u8Div)
    {
        case ClkDiv1:
        case ClkDiv2:
        case ClkDiv4:
        case ClkDiv8:
            ClkUnlock();
            u8Div = (uint8_t)(log(u8Div) / log(2));
            M0P_CLOCK->SYSCTRL0_f.PCLK_PRS = u8Div;
            break;
        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}


en_result_t Clk_SetXTH_StableTime(en_clk_cycle_t enCycle)
{
    en_result_t enRet = Ok;

    switch (enCycle)
    {
        case ClkCycle256:
            M0P_CLOCK->XTH_CR_f.STARTUP = 0u;
            break;
        case ClkCycle1024:
            M0P_CLOCK->XTH_CR_f.STARTUP = 1u;
            break;
        case ClkCycle4096:
            M0P_CLOCK->XTH_CR_f.STARTUP = 2u;
            break;
        case ClkCycle16384:
            M0P_CLOCK->XTH_CR_f.STARTUP = 3u;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}


en_result_t Clk_SetRCL_StableTime(en_clk_cycle_t enCycle)
{
    en_result_t enRet = Ok;

    switch (enCycle)
    {
        case ClkCycle4:
            M0P_CLOCK->RCL_CR_f.STARTUP = 0u;
            break;
        case ClkCycle16:
            M0P_CLOCK->RCL_CR_f.STARTUP = 1u;
            break;
        case ClkCycle64:
            M0P_CLOCK->RCL_CR_f.STARTUP = 2u;
            break;
        case ClkCycle256:
            M0P_CLOCK->RCL_CR_f.STARTUP = 3u;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

en_result_t Clk_SetXTL_StableTime(en_clk_cycle_t enCycle)
{
    en_result_t enRet = Ok;

    switch (enCycle)
    {
        case ClkCycle256:
            M0P_CLOCK->XTL_CR_f.STARTUP = 0u;
            break;
        case ClkCycle1024:
            M0P_CLOCK->XTL_CR_f.STARTUP = 1u;
            break;
        case ClkCycle4096:
            M0P_CLOCK->XTL_CR_f.STARTUP = 2u;
            break;
        case ClkCycle16384:
            M0P_CLOCK->XTL_CR_f.STARTUP = 3u;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

en_result_t Clk_SetXTLDrive(boolean_t  bHigh)
{
    bHigh = !!bHigh;
    M0P_CLOCK->XTL_CR_f.DRIVER = bHigh;

    return Ok;
}

/**
   * \brief   Clk_Enable
   *          enable/disalbe to a paticular clock
   * \param   [in]  enSource  clock source
   * \param   [in]  u8Flag    TRUE:       enable
   *                          FALSE:      disable
   * \retval  en_result_t     Ok:         operation done.
   *                          ErrorInvalidParameter: invalid source type
   */
en_result_t Clk_Enable(en_clk_source_t enSource, boolean_t bFlag)
{
    en_result_t enRet = Ok;

    ClkUnlock();
    //force to 0/1
    bFlag = !!bFlag;

    switch (enSource)
    {
        case ClkXTL:
            M0P_GPIO->P1ADS_f.P14 = bFlag;
            M0P_GPIO->P1ADS_f.P15 = bFlag;
            M0P_CLOCK->XTL_CR_f.DRIVER = 0xF;
            M0P_CLOCK->SYSCTRL0_f.XTL_EN = bFlag;
            break;

        case ClkRCL:
            M0P_CLOCK->SYSCTRL0_f.RCL_EN = bFlag;
            break;

        case ClkXTH:
            M0P_GPIO->P0ADS_f.P01 = bFlag;
            M0P_GPIO->P0ADS_f.P02 = bFlag;
            M0P_CLOCK->XTH_CR_f.DRIVER = 0xF;
            M0P_CLOCK->SYSCTRL0_f.XTH_EN = bFlag;
            break;

        case ClkRCH:
            M0P_CLOCK->SYSCTRL0_f.RCH_EN = bFlag;
            break;

        default:
            ClkWriteDummy();
            enRet = ErrorInvalidParameter;
            break;
    }

    return enRet;
}

/**
 * \brief   Clk_SetPeripheralGate
 *
 * \param   [in]  enPeripheral
 * \param   [in]  bFlag
 *
 * \retval  en_result_t
 */
en_result_t Clk_SetPeripheralGate(en_clk_peripheral_gate_t enPeripheral,
                                  boolean_t bFlag)
{
    en_result_t enRet = Ok;

    bFlag = !!bFlag;
    setBit(&M0P_CLOCK->PERI_CLKEN, enPeripheral, bFlag);
    return enRet;
}

boolean_t Clk_GetPeripheralGate(en_clk_peripheral_gate_t enPeripheral)
{
    return getBit(&M0P_CLOCK->PERI_CLKEN, enPeripheral);
}


/**
 * \brief   Clk_RtcAdjust
 * \param   [in]
 *
 * \retval  en_result_t
 */
en_result_t Clk_SetRTCAdjustClkFreq(uint32_t u32val)
{
    //en_result_t enRet = Ok;

    ClkUnlock();

    if (u32val >= (32 * 1000 * 1000))
    {
        M0P_CLOCK->SYSCTRL1_f.RTC_FREQ_ADJUST = 7;
    } else if (u32val >= (24 * 1000 * 1000))
    {
        M0P_CLOCK->SYSCTRL1_f.RTC_FREQ_ADJUST = 6;
    } else if (u32val >= (20 * 1000 * 1000))
    {
        M0P_CLOCK->SYSCTRL1_f.RTC_FREQ_ADJUST = 5;
    } else if (u32val >= (16 * 1000 * 1000))
    {
        M0P_CLOCK->SYSCTRL1_f.RTC_FREQ_ADJUST = 4;
    } else if (u32val >= (12 * 1000 * 1000))
    {
        M0P_CLOCK->SYSCTRL1_f.RTC_FREQ_ADJUST = 3;
    } else if (u32val >= (8 * 1000 * 1000))
    {
        M0P_CLOCK->SYSCTRL1_f.RTC_FREQ_ADJUST = 2;
    } else if (u32val >= (6 * 1000 * 1000))
    {
        M0P_CLOCK->SYSCTRL1_f.RTC_FREQ_ADJUST = 1;
    } else if (u32val >= (4 * 1000 * 1000))
    {
        M0P_CLOCK->SYSCTRL1_f.RTC_FREQ_ADJUST = 0;
    } else
    {
        ClkWriteDummy();
        return ErrorInvalidParameter;
    }

    return Ok;
}


en_result_t Clk_SysTickConfig(stc_clk_systickcfg_t *pstcCfg)
{
    if (NULL == pstcCfg)
    {
        return ErrorInvalidParameter;
    }

    M0P_CLOCK->PERI_CLKEN_f.TICK = 1;
    switch (pstcCfg->enClk)
    {
        case ClkRCH:
            M0P_CLOCK->SYSTICK_CR_f.CLK_SEL = 0x2;
            break;
        case ClkRCL:
            M0P_CLOCK->SYSTICK_CR_f.CLK_SEL = 0x1;
            break;
        case ClkXTH:
            M0P_CLOCK->SYSTICK_CR_f.CLK_SEL = 0x3;
            break;
        case ClkXTL:
            M0P_CLOCK->SYSTICK_CR_f.CLK_SEL = 0x0;
            break;
        default:
            return ErrorInvalidParameter;
    }

    M0P_CLOCK->SYSTICK_CR_f.NOREF = pstcCfg->bNoRef;

    SysTick->LOAD  = (uint32_t)(pstcCfg->u32LoadVal - 1UL);                         /* set reload register */
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);                /* set Priority for Systick Interrupt */
    SysTick->VAL   = 0UL;                                                           /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_ENABLE_Msk;
    return Ok;
}


en_result_t Clk_DebugClk(en_clk_debug_t enPeri, boolean_t bFlag)
{
    switch (enPeri)
    {
        case ClkDBGTim0:
        case ClkDBGTim1:
        case ClkDBGTim2:
        case ClkDBGLpTim:
        case ClkDBGTim4:
        case ClkDBGTim5:
        case ClkDBGTim6:
        case ClkDBGTPca:
        case ClkDBGTWdt:
        case ClkDBGTRtc:
        case ClkDBGTTick:
            setBit(&M0P_CLOCK->DEBUG_ACTIVE, 1 << enPeri, bFlag);
            break;
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}

//@} // Clk Functions

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
