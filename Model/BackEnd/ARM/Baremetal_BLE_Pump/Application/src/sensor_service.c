/**
  ******************************************************************************
  * @file    sensor_service.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    04-July-2014
  * @brief   Add a sample service using a vendor specific profile.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "sensor_service.h"

/** @addtogroup X-CUBE-BLE1_Applications
 *  @{
 */

/** @addtogroup SensorDemo
 *  @{
 */
 
/** @defgroup SENSOR_SERVICE
 * @{
 */

/** @defgroup SENSOR_SERVICE_Private_Variables
 * @{
 */
/* Private variables ---------------------------------------------------------*/
volatile int connected = FALSE;
volatile uint8_t set_connectable = 1;
volatile uint16_t connection_handle = 0;
volatile uint8_t notification_enabled = FALSE;
volatile Sensor_t sensor = {0};
uint16_t sampleServHandle, TXCharHandle, RXCharHandle;
uint16_t aioServHandle, digitalCharHandle, analogCharHandle;

/**
 * @}
 */

/** @defgroup SENSOR_SERVICE_Private_Macros
 * @{
 */
/* Private macros ------------------------------------------------------------*/
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

#define COPY_AIO_SERVICE_UUID(uuid_struct)  COPY_UUID_128(uuid_struct,0x00,0x00,0x18,0x15, 0x00,0x00, 0x10,0x00, 0x80,0x00, 0x00,0x80,0x5f,0x9b,0x34,0xfb)
#define COPY_DIGITAL_UUID(uuid_struct)   	  COPY_UUID_128(uuid_struct,0x00,0x00,0x2A,0x56, 0x00,0x00, 0x10,0x00, 0x80,0x00, 0x00,0x80,0x5f,0x9b,0x34,0xfb)
#define COPY_ANALOG_UUID(uuid_struct)       COPY_UUID_128(uuid_struct,0x00,0x00,0x2A,0x58, 0x00,0x00, 0x10,0x00, 0x80,0x00, 0x00,0x80,0x5f,0x9b,0x34,0xfb)
#define COPY_AGGREGATE_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x2A,0x5A, 0x00,0x00, 0x10,0x00, 0x80,0x00, 0x00,0x80,0x5f,0x9b,0x34,0xfb)

/* Store Value into a buffer in Little Endian Format */
#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )
/**
 * @}
 */

/** @defgroup SENSOR_SERVICE_Exported_Functions 
 * @{
 */ 
/**
 * @brief  Add an accelerometer service using a vendor specific profile.
 *
 * @param  None
 * @retval tBleStatus Status
 */
tBleStatus Add_Aio_Service(void)
{
  tBleStatus ret;

  uint8_t uuid[16];
  
  COPY_AIO_SERVICE_UUID(uuid);
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 7,
                          &aioServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  COPY_DIGITAL_UUID(uuid);
  ret =  aci_gatt_add_char(aioServHandle, UUID_TYPE_128, uuid, 6,
                           CHAR_PROP_READ, ATTR_PERMISSION_NONE,
			   0,
                           16, 0, &digitalCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  COPY_ANALOG_UUID(uuid);
  ret =  aci_gatt_add_char(aioServHandle, UUID_TYPE_128, uuid, 6,
                           CHAR_PROP_READ|CHAR_PROP_NOTIFY,  ATTR_PERMISSION_NONE,
                           GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                           16, 0, &analogCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  PRINTF("Service AIO added. Handle 0x%04X, Digital Charac handle: 0x%04X, Acc Charac handle: 0x%04X\r\n",aioServHandle, digitalCharHandle, analogCharHandle);
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding AIO service.\r\n");
  return BLE_STATUS_ERROR ;
    
}

/**
 * @brief  Send a notification for a digital characteristic.
 *
 * @param  None
 * @retval tBleStatus Status
 */
tBleStatus Digital_Notify(void)
{  
  static uint8_t val = 0;
  tBleStatus ret;
  PRINTF("Digital_Notify\r\n") ;	
  val = ~val;	
  ret = aci_gatt_update_char_value(aioServHandle, digitalCharHandle, 0, 1,
                                   &val);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating digital characteristic.\r\n") ;
    return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;	
}

/**
 * @brief  Update acceleration characteristic value.
 *
 * @param  Structure containing acceleration value in mg
 * @retval Status
 */
tBleStatus Aio_Update(Sensor_t *data)
{  
  tBleStatus ret;    
  uint8_t buff[2];
    
  STORE_LE_16(buff,data->data);
	
  ret = aci_gatt_update_char_value(aioServHandle, analogCharHandle, 0, 2, buff);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating analog characteristic.\r\n") ;
    return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;	
}

/**
 * @brief  Puts the device in connectable mode.
 *         If you want to specify a UUID list in the advertising data, those data can
 *         be specified as a parameter in aci_gap_set_discoverable().
 *         For manufacture data, aci_gap_update_adv_data must be called.
 * @param  None 
 * @retval None
 */
/* Ex.:
 *
 *  tBleStatus ret;    
 *  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','l','u','e','N','R','G'};    
 *  const uint8_t serviceUUIDList[] = {AD_TYPE_16_BIT_SERV_UUID,0x34,0x12};    
 *  const uint8_t manuf_data[] = {4, AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 0x05, 0x02, 0x01};
 *  
 *  ret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
 *                                 8, local_name, 3, serviceUUIDList, 0, 0);    
 *  ret = aci_gap_update_adv_data(5, manuf_data);
 *
 */
void setConnectable(void)
{  
  tBleStatus ret;
  
  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','l','u','e','N','R','G'};
  
  /* disable scan response */
  hci_le_set_scan_resp_data(0,NULL);
  PRINTF("General Discoverable Mode.\r\n");
  
  ret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
                                 sizeof(local_name), local_name, 0, NULL, 0, 0);
  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("Error while setting discoverable mode (%d)\r\n", ret);    
  }  
}

/**
 * @brief  This function is called when there is a LE Connection Complete event.
 * @param  uint8_t Address of peer device
 * @param  uint16_t Connection handle
 * @retval None
 */
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle)
{  
  connected = TRUE;
  connection_handle = handle;
  
  PRINTF("Connected to device:");
  for(int i = 5; i > 0; i--){
    PRINTF("%02X-", addr[i]);
  }
  PRINTF("%02X\r\n", addr[0]);
}

/**
 * @brief  This function is called when the peer device gets disconnected.
 * @param  None 
 * @retval None
 */
void GAP_DisconnectionComplete_CB(void)
{
  connected = FALSE;
  PRINTF("Disconnected\r\n");
  /* Make the device connectable again. */
  set_connectable = TRUE;
  notification_enabled = FALSE;
}

/**
 * @brief  Read request callback.
 * @param  uint16_t Handle of the attribute
 * @retval None
 */
void Read_Request_CB(uint16_t handle)
{  
  if(handle == analogCharHandle + 1){
    Aio_Update((Sensor_t*)&sensor);
  }  
  
  //EXIT:
  if(connection_handle != 0)
    aci_gatt_allow_read(connection_handle);
}

/**
 * @brief  Callback processing the ACI events.
 * @note   Inside this function each event must be identified and correctly
 *         parsed.
 * @param  void* Pointer to the ACI packet
 * @retval None
 */
void HCI_Event_CB(void *pckt)
{
  hci_uart_pckt *hci_pckt = pckt;
  /* obtain event packet */
  hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;
  
  if(hci_pckt->type != HCI_EVENT_PKT)
    return;
  
  switch(event_pckt->evt){
    
  case EVT_DISCONN_COMPLETE:
    {
      GAP_DisconnectionComplete_CB();
    }
    break;
    
  case EVT_LE_META_EVENT:
    {
      evt_le_meta_event *evt = (void *)event_pckt->data;
      
      switch(evt->subevent){
      case EVT_LE_CONN_COMPLETE:
        {
          evt_le_connection_complete *cc = (void *)evt->data;
          GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
        }
        break;
      }
    }
    break;
    
  case EVT_VENDOR:
    {
      evt_blue_aci *blue_evt = (void*)event_pckt->data;
      switch(blue_evt->ecode){

      case EVT_BLUE_GATT_READ_PERMIT_REQ:
        {
          evt_gatt_read_permit_req *pr = (void*)blue_evt->data;                    
          Read_Request_CB(pr->attr_handle);                    
        }
        break;
      }
    }
    break;
  }    
}

/**
 * @}
 */
 
/**
 * @}
 */

/**
 * @}
 */

 /**
 * @}
 */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
