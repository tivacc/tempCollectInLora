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
 **\uartͨ��ѡ��
 ******************************************************************************/

typedef enum en_uart_channel
{
    UARTCH0 = 0u,
    UARTCH1 = 1u,
}en_uart_channel_t;


/**
 ******************************************************************************
 ** \uart������ģʽ��ַ֡/����֡������żУ��
 ******************************************************************************/
typedef enum en_uart_mmdorck
{
    DataOrEven 	 = 0u,     //������ģʽʱ������֡TB8��� ,�Ƕ�����ģʽżУ��   
    AddrOrOdd    = 1u,          
}en_uart_mmdorck_t;

/**
 ******************************************************************************
 ** \uart������ģʽ����
 ******************************************************************************/
typedef enum en_uart_multimode
{
    UartNormal = 0u,         //��������ģʽ
    UartMulti  = 1u,         //����������ģʽ
}en_uart_multimode_t;
/**
 ******************************************************************************
 ** \uart������ģʽ���ӻ���ַ�͵�ַ��������
 ******************************************************************************/

typedef struct stc_uart_multimode
{
	en_uart_multimode_t enMulti_mode;
	uint8_t 			u8SlaveAddr;//�ӻ���ַ
	uint8_t				u8SaddEn;//�Ӽ���ַ����
}stc_uart_multimode_t;

/**
 ******************************************************************************
 ** \uart ���ֹ���ģʽѡ��
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
 ** \uart ���ͽ��չ���ʹ��
 ******************************************************************************/
typedef enum en_uart_func
{
    UartTx = 0u,      //mode0ģʽ����TX    
    UartRx = 1u,  //��mode0ģʽ����RX and TX ,mode0ģʽ����RX  
}en_uart_func_t;
/**
 ******************************************************************************
 ** \uart���ͽ����ж�ʹ��
 ******************************************************************************/
typedef enum en_uart_irq_sel
{
    UartTxIrq       = 0u,          
    UartRxIrq       = 1u,          
}en_uart_irq_sel_t;
/**
 ******************************************************************************
 ** \uart���ͽ����жϽṹ
 ******************************************************************************/
typedef struct stc_uart_irq_en
{
    boolean_t bTxIrq;          ///< UART TX interrupt
    boolean_t bRxIrq;          ///< UART RX interrupt
}stc_uart_irq_en_t;
/**
 ******************************************************************************
 ** \uart���ͽ����жϴ������ӿ�
 ******************************************************************************/

typedef struct stc_uart_irq_cb
{
    func_ptr_t pfnTxIrqCb;      
    func_ptr_t pfnRxErrIrqCb;  
    func_ptr_t pfnRxIrqCb;      
}stc_uart_irq_cb_t;
/**
 ******************************************************************************
 ** \uart ״̬��־λ
 ******************************************************************************/
typedef enum en_uart_status
{
    UartRFRAMEError    	= 0u,  
    UartRxFull          = 1u,   
    UartTxEmpty         = 2u,   
   
}en_uart_status_t;
/**
 ******************************************************************************
 ** \uart ͨ����ַ���жϺ�����ַ�ṹ
 ******************************************************************************/
typedef struct stc_uart_instance_data
{   
    uint32_t               u32Idx;               ///< channel index
    M0P_UART_TypeDef       *pstcInstance;        ///< pointer to registers of an instance
    stc_uart_irq_cb_t      stcUartInternIrqCb;   ///< Uart internal interrupt callback function
} stc_uart_instance_data_t;
/**
 ******************************************************************************
 ** \uart ͨ������������
 ******************************************************************************/
typedef struct stc_uart_baud_config
{
	uint8_t u8Mode;//���ֹ���ģʽ
	uint8_t u8Dbaud;//�Ƿ�˫��������
	uint32_t u32Baud;//������
}stc_uart_baud_config_t;
/**
 ******************************************************************************
 ** \uart ��������
 ******************************************************************************/

typedef struct stc_uart_config
{
    en_uart_mode_t   	enRunMode;    
    stc_uart_multimode_t* pstcMultiMode; 
   // en_uart_mmdorck_t    enMmdOrCk;
    stc_uart_irq_cb_t*  pstcIrqCb;           
    boolean_t           bTouchNvic;         
} stc_uart_config_t;
//�ж�������ú���
en_result_t Uart_EnableIrq(uint8_t u8Idx,
                           en_uart_irq_sel_t enIrqSel);
en_result_t Uart_DisableIrq(uint8_t u8Idx,
                            en_uart_irq_sel_t enIntSel);
//void Uart_IrqHandler(uint8_t u8Idx);
// �ܳ�ʼ������
en_result_t Uart_Init(uint8_t u8Idx, 
                      stc_uart_config_t* pstcConfig);
en_result_t Uart_DeInit(uint8_t u8Idx, boolean_t bTouchNvic);
//����ģʽ���á�������ģʽ�´ӻ���ַ�͵�ַ��������
en_result_t Uart_SetMode(uint8_t u8Idx,en_uart_mode_t enMode);
en_result_t Uart_SetMultiMode(uint8_t u8Idx,stc_uart_multimode_t* pstcMultiConfig);
en_result_t Uart_SetMMDOrCk(uint8_t u8Idx,en_uart_mmdorck_t enTb8);
en_result_t Uart_SetSaddr(uint8_t u8Idx,uint8_t u8Addr);
en_result_t Uart_SetSaddrEn(uint8_t u8Idx,uint8_t u8Addren);
boolean_t Uart_GetRb8(uint8_t u8Idx);

// ����������
uint16_t Uart_SetBaudRate(uint8_t u8Idx,uint32_t u32pclk,stc_uart_baud_config_t* pstcBaud);
                             
// ���ͻ����ʹ�ܺͽ�ֹ
en_result_t Uart_EnableFunc(uint8_t u8Idx, en_uart_func_t enFunc);
en_result_t Uart_DisableFunc(uint8_t u8Idx, en_uart_func_t enFunc);
//״̬λ�Ļ�ȡ�����
boolean_t Uart_GetStatus(uint8_t u8Idx,en_uart_status_t enStatus);
en_result_t Uart_ClrStatus(uint8_t u8Idx,en_uart_status_t enStatus);
//���ݲ�ѯ��ʽ���շ�����
//en_result_t  Uart_MultiSendFirstData(uint8_t U8Addr);
en_result_t Uart_SendData(uint8_t u8Idx, uint32_t u32Data);
uint32_t Uart_ReceiveData(uint8_t u8Idx);


#ifdef __cplusplus
#endif

#endif /* __UART_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/



