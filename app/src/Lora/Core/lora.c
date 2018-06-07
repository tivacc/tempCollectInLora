 /*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: LoRaMac classA device implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis, Gregory Cristian and Wael Guibene
*/
/******************************************************************************
  * @file    lora.c
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    01-June-2017
  * @brief   lora API to drive the lora state Machine
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "LoRaMac.h"
#include "lora.h"
#include "timeService.h"
#include "Comissioning.h"
#include "initDevice.h"
#include "lpm.h"

/*!
 * LoRaWAN ETSI duty cycle control enable/disable
 *
 * \remark Please note that ETSI mandates duty cycled transmissions. Use only for test purposes
 */
#define LORAWAN_DUTYCYCLE_ON                        true

static uint8_t DevEui[] = LORAWAN_DEVICE_EUI;

static uint8_t NwkSKey[] = LORAWAN_NWKSKEY;
static uint8_t AppSKey[] = LORAWAN_APPSKEY;

static uint32_t DevAddr = LORAWAN_DEVICE_ADDRESS;


/*!
 * User application data buffer size
 */
#define LORAWAN_APP_DATA_BUFF_SIZE                    32
static uint8_t AppDataBuff[LORAWAN_APP_DATA_BUFF_SIZE];


/*!
 * User application data structure
 */
static lora_AppData_t AppData={ AppDataBuff,  0 ,0 };

/*!
 * Indicates if the node is sending confirmed or unconfirmed messages
 */
static FunctionalState IsTxConfirmed ;

/*!
 * Defines the LoRa parameters at Init
 */
static  LoRaParam_t* LoRaParamInit;


DeviceState_t DeviceState = DEVICE_STATE_INIT ;

static LoRaMacPrimitives_t LoRaMacPrimitives;
static LoRaMacCallback_t LoRaMacCallbacks;
static MibRequestConfirm_t mibReq;

static LoRaMainCallback_t *LoRaMainCallbacks;


/*!
 * Indicates if a new packet can be sent
 */
static bool NextTx = true;


/*!
 * \brief   Prepares the payload of the frame
 *
 * \retval  [0: frame could be send, 1: error]
 */
static bool SendFrame( void )
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;
    
    if( LoRaMacQueryTxPossible( AppData.BuffSize, &txInfo ) != LORAMAC_STATUS_OK )
    {
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate = LoRaParamInit->TxDatarate;
    }
    else
    {
        if( IsTxConfirmed == DISABLE )
        {
            mcpsReq.Type = MCPS_UNCONFIRMED;
            mcpsReq.Req.Unconfirmed.fPort = AppData.Port;
            mcpsReq.Req.Unconfirmed.fBuffer = AppData.Buff;
            mcpsReq.Req.Unconfirmed.fBufferSize = AppData.BuffSize;
            mcpsReq.Req.Unconfirmed.Datarate = LoRaParamInit->TxDatarate;
        }
        else
        {
            mcpsReq.Type = MCPS_CONFIRMED;
            mcpsReq.Req.Confirmed.fPort = AppData.Port;
            mcpsReq.Req.Confirmed.fBuffer = AppData.Buff;
            mcpsReq.Req.Confirmed.fBufferSize = AppData.BuffSize;
            mcpsReq.Req.Confirmed.NbTrials = 8;
            mcpsReq.Req.Confirmed.Datarate = LoRaParamInit->TxDatarate;
        }
    }
    if( LoRaMacMcpsRequest( &mcpsReq ) == LORAMAC_STATUS_OK )
    {
        return false;
    }
    return true;
}

/*!
 * \brief   MCPS-Confirm event function
 *
 * \param   [IN] McpsConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void McpsConfirm( McpsConfirm_t *mcpsConfirm )
{
    if( mcpsConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
    {
        switch( mcpsConfirm->McpsRequest )
        {
            case MCPS_UNCONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                break;
            }
            case MCPS_CONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                // Check AckReceived
                // Check NbTrials
                break;
            }
            case MCPS_PROPRIETARY:
            {
                break;
            }
            default:
                break;
        }
    }
    NextTx = true;
}

/*!
 * \brief   MCPS-Indication event function
 *
 * \param   [IN] mcpsIndication - Pointer to the indication structure,
 *               containing indication attributes.
 */
static void McpsIndication( McpsIndication_t *mcpsIndication )
{
    if( mcpsIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
        return;
    }

    switch( mcpsIndication->McpsIndication )
    {
        case MCPS_UNCONFIRMED:
        {
            break;
        }
        case MCPS_CONFIRMED:
        {
            break;
        }
        case MCPS_PROPRIETARY:
        {
            break;
        }
        case MCPS_MULTICAST:
        {
            break;
        }
        default:
            break;
    }

    // Check Multicast
    // Check Port
    // Check Datarate
    // Check FramePending
    // Check Buffer
    // Check BufferSize
    // Check Rssi
    // Check Snr
    // Check RxSlot
    if( mcpsIndication->RxData == true )
    {
        switch( mcpsIndication->Port )
        {
        case 224:
      		// Check compliance test enable command (i)
            if( ( mcpsIndication->BufferSize == 4 ) &&
                ( mcpsIndication->Buffer[0] == 0x01 ) &&
                ( mcpsIndication->Buffer[1] == 0x01 ) &&
                ( mcpsIndication->Buffer[2] == 0x01 ) &&
                ( mcpsIndication->Buffer[3] == 0x01 ) )
            {
                IsTxConfirmed = DISABLE;
                AppData.Port = 224;
                AppData.BuffSize = 2;          
                
                MibRequestConfirm_t mibReq;
                mibReq.Type = MIB_ADR;
                mibReq.Param.AdrEnable = true;
                LoRaMacMibSetRequestConfirm( &mibReq );
            }
            break;
        default:
            
            AppData.Port = mcpsIndication->Port;
            AppData.BuffSize = mcpsIndication->BufferSize;
            memcpy1( AppData.Buff, mcpsIndication->Buffer, AppData.BuffSize );
            
            LoRaMainCallbacks->LoraRxData( &AppData );
            break;
        }
    }
}

/*!
 * \brief   MLME-Confirm event function
 *
 * \param   [IN] MlmeConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void MlmeConfirm( MlmeConfirm_t *mlmeConfirm )
{
    switch( mlmeConfirm->MlmeRequest )
    {
        case MLME_JOIN:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                // Status is OK, node has joined the network
                DeviceState = DEVICE_STATE_JOINED;
            }
            else
            {
                // Join was not successful. Try to join again
                DeviceState = DEVICE_STATE_JOIN;
            }
            break;
        }
        case MLME_LINK_CHECK:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                // Check DemodMargin
                // Check NbGateways
            }
            break;
        }
        default:
            break;
    }
    NextTx = true;
}
/**
 *  lora Init
 */
void lora_Init (LoRaMainCallback_t *callbacks, LoRaParam_t* LoRaParam )
{
  /* init the DeviceState*/
  DeviceState= DEVICE_STATE_INIT;
  
  /* init the Tx Duty Cycle*/
  LoRaParamInit = LoRaParam;
  
  /* init the main call backs*/
  //LoRaMainCallbacks = callbacks;
}

void lora_fsm( void)
{
#if 1
  switch( DeviceState )
  {
    case DEVICE_STATE_INIT:
    {
        LoRaMacPrimitives.MacMcpsConfirm = McpsConfirm;
        LoRaMacPrimitives.MacMcpsIndication = McpsIndication;
        LoRaMacPrimitives.MacMlmeConfirm = MlmeConfirm;
        LoRaMacCallbacks.GetBatteryLevel = LoRaMainCallbacks->BoardGetBatteryLevel;

        LoRaMacInitialization( &LoRaMacPrimitives, &LoRaMacCallbacks, LORAMAC_REGION_CN470 );

        mibReq.Type = MIB_ADR;
        mibReq.Param.AdrEnable = LoRaParamInit->AdrEnable;
        LoRaMacMibSetRequestConfirm( &mibReq );

        mibReq.Type = MIB_PUBLIC_NETWORK;
        mibReq.Param.EnablePublicNetwork = LoRaParamInit->EnablePublicNetwork;
        LoRaMacMibSetRequestConfirm( &mibReq );
                        
        mibReq.Type = MIB_DEVICE_CLASS;
        mibReq.Param.Class= LoRaParamInit->Class;
        LoRaMacMibSetRequestConfirm( &mibReq );

        DeviceState = DEVICE_STATE_JOIN;
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      mibReq.Type = MIB_NET_ID;
      mibReq.Param.NetID = LORAWAN_NETWORK_ID;
      LoRaMacMibSetRequestConfirm( &mibReq );

      mibReq.Type = MIB_DEV_ADDR;
      mibReq.Param.DevAddr = DevAddr;
      LoRaMacMibSetRequestConfirm( &mibReq );

      mibReq.Type = MIB_NWK_SKEY;
      mibReq.Param.NwkSKey = NwkSKey;
      LoRaMacMibSetRequestConfirm( &mibReq );

      mibReq.Type = MIB_APP_SKEY;
      mibReq.Param.AppSKey = AppSKey;
      LoRaMacMibSetRequestConfirm( &mibReq );

      mibReq.Type = MIB_NETWORK_JOINED;
      mibReq.Param.IsNetworkJoined = true;
      LoRaMacMibSetRequestConfirm( &mibReq );

      DeviceState = DEVICE_STATE_SEND;
      break;
    }
    case DEVICE_STATE_SEND:
    {
      if( NextTx )
      {
          AppData.Port = 1;
          AppData.Buff = "test!";
          AppData.BuffSize = strlen("test!");
          IsTxConfirmed =  ENABLE;

          NextTx = SendFrame();
      }

      DeviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      // Wake up through events
      Lpm_GotoLpmMode();
      break;
    }
    default:
    {
      DeviceState = DEVICE_STATE_INIT;
      break;
    }
  }
  #endif
}

