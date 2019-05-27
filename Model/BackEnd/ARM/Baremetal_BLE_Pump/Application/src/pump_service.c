#include "pump_service.h"
// #include "bluenrg_gap_aci.h"
// #include "bluenrg_gatt_aci.h"
#include "pump_config.h"
#include <sys/time.h>


/* Private variables ---------------------------------------------------------*/
__IO uint32_t connected = FALSE;
__IO uint8_t set_connectable = 1;
__IO uint16_t connection_handle = 0;
__IO uint8_t notification_enabled = FALSE;


void (*state_fn)(void)=NULL;
/*Cure init in the data section*/
CureProg cure = {"" , 0 , 0 , 0 , 0 , 0 };

int debug=1;
//int cure_exist = 0;
uint16_t commandServHandle, createCharHandle, modifyCharHandle, deleteCharHandle;
uint16_t checkServHandle, nameLogCharHandle, volumeLogCharHandle, durationLogCharHandle;
uint16_t disServHandle, manufacturerNameCharHandle, modelNumberCharHandle,
		serialNumberCharHandle, hardwareRevisionCharHandle,
		firmwareRevisionCharHandle, softwareRevisionCharHandle,
		systemIDCharHandle;

/*Private macros */

/* Store 16 bit Value into a buffer in Little Endian Format */
#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )

/* Copy 16 bit UUID into a buffer*/
#define COPY_UUID_16(uuid_struct, uuid_1, uuid_0) \
do {\
  uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; \
}while(0)

/* copy 128 bit UUID into a buffer*/
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
  uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
    uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
      uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
        uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)


/* Pump services */

void Add_Dis_Service(void) {
	uint8_t status;
	uint8_t uuid[2];
	COPY_UUID_16(uuid, 0x18, 0x0A);

	//aci_gatt_add_serv
	//status = aci_gatt_add_serv(service_uuid_type,service_uuid_16,service_type,max_attribute_records, &service_handle);
	status = aci_gatt_add_serv(UUID_TYPE_16, uuid, PRIMARY_SERVICE, 0x0F,
			&disServHandle);
	if (status != BLE_STATUS_SUCCESS) {

			if (debug)
				printf("add_serv dis fail:0x%02x\r\n", status);
	} else {

			if (debug)
				printf("add_serv DIS \r\n");
	}

	COPY_UUID_16(uuid, 0x2A, 0x29);

	//aci_gatt_add_char
	//status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_16,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	status = aci_gatt_add_char(disServHandle, UUID_TYPE_16, uuid, 0x000A,
	CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE, 0x07,
			0x01, &manufacturerNameCharHandle);
	if (status != BLE_STATUS_SUCCESS) {
		if (debug)
			printf("add_char manuf name failed:0x%02x\r\n", status);
	} else {
		if (debug)
			printf("add_char manufe name\r\n");
	}

	uint8_t uuid_62[2];

	COPY_UUID_16(uuid_62, 0x2A, 0x24);

	//aci_gatt_add_char
	//status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_16,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	status = aci_gatt_add_char(disServHandle, UUID_TYPE_16, uuid_62, 0x0004,
	CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE, 0x07,
			0x01, &modelNumberCharHandle);
	if (status != BLE_STATUS_SUCCESS) {
		if (debug)
			printf("add_char model number fail:0x%02x\r\n", status);
	} else {
		if (debug)
			printf("add_char model number\r\n");
	}

	uint8_t uuid_66[2];

	COPY_UUID_16(uuid_66, 0x2A, 0x25);

	//aci_gatt_add_char
	//status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_16,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	status = aci_gatt_add_char(disServHandle, UUID_TYPE_16, uuid_66, 0x0004,
	CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE, 0x07,
			0x01, &serialNumberCharHandle);
	if (status != BLE_STATUS_SUCCESS) {
		if (debug)
			printf("add_char fail:0x%02x\r\n", status);
	} else {
		if (debug)
			printf("add_char ser Num\r\n");
	}

	uint8_t uuid_70[2];

	COPY_UUID_16(uuid_70, 0x2A, 0x27);

	//aci_gatt_add_char
	//status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_16,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	status = aci_gatt_add_char(disServHandle, UUID_TYPE_16, uuid_70, 0x0006,
	CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE, 0x07,
			0x01, &hardwareRevisionCharHandle);
	if (status != BLE_STATUS_SUCCESS) {
		if (debug)
			printf("add_char fail :0x%02x\r\n", status);
	} else {
		if (debug)
			printf("add_char hard Rev\r\n");
	}

	uint8_t uuid_74[2];

	COPY_UUID_16(uuid_74, 0x2A, 0x26);

	//aci_gatt_add_char
	//status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_16,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	status = aci_gatt_add_char(disServHandle, UUID_TYPE_16, uuid_74, 0x0006,
	CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE, 0x07,
			0x01, &firmwareRevisionCharHandle);
	if (status != BLE_STATUS_SUCCESS) {
		if (debug)
			printf("add_char fail :0x%02x\r\n", status);
	} else {
		if (debug)
			printf("add_char firmw Rev\r\n");
	}

	uint8_t uuid_78[2];

	COPY_UUID_16(uuid_78, 0x2A, 0x28);

	//aci_gatt_add_char
	//status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_16,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	status = aci_gatt_add_char(disServHandle, UUID_TYPE_16, uuid_78, 0x0006,
	CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE, 0x07,
			0x01, &softwareRevisionCharHandle);
	if (status != BLE_STATUS_SUCCESS) {
		if (debug)
			printf("add_char fail : 0x%02x\r\n", status);
	} else {
		if (debug)
			printf("add_char soft Rev\r\n");
	}

	uint8_t uuid_82[2];

	COPY_UUID_16(uuid_82, 0x2A, 0x23);

	//aci_gatt_add_char
	//status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_16,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	status = aci_gatt_add_char(disServHandle, UUID_TYPE_16, uuid_82, 0x0008,
	CHAR_PROP_READ, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE, 0x07,
			0x00, &systemIDCharHandle);
	if (status != BLE_STATUS_SUCCESS) {
		if (debug)
			printf("add_char failed:0x%02x\r\n", status);
	} else {
		if (debug)
			printf("add_char systemID\r\n");
	}

}

void Add_Command_Service(void){


	  uint8_t status;
	  uint8_t uuid[16];


		/*Command Service Created Here*/
	  COPY_UUID_128(uuid, 0xFC,0xB4,0x30,0x12,0xCB,0xC4,0x11,0xE8,0xA8,0xD5,0xF2,0x80,0x1F,0x1B,0x9F,0xD1);

	  //aci_gatt_add_serv
	  //status = aci_gatt_add_serv(service_uuid_type,service_uuid_128,service_type,max_attribute_records, &service_handle);
	  // IMPORTANT: Please make sure the Max_Attribute_Records parameter is configured exactly with the required value.
	  status = aci_gatt_add_serv(UUID_TYPE_128,uuid,PRIMARY_SERVICE,0x07, &commandServHandle);
	  if (status != BLE_STATUS_SUCCESS) {
	    PRINTF("add command service failed:0x%02x\r\n", status);
	  }else{
	    PRINTF("add command service success\r\n");
	  }


		/*CREATE Characteristic Created Here*/
	  COPY_UUID_128(uuid, 0xFC,0xB4,0x32,0xEC,0xCB,0xC4,0x11,0xE8,0xA8,0xD5,0xF2,0x80,0x1F,0x1B,0x9F,0xD1);

	  //aci_gatt_add_char
	  //status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_128,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	  status = aci_gatt_add_char(commandServHandle,UUID_TYPE_128,uuid,0x0030/*0x002C*/,CHAR_PROP_WRITE,ATTR_PERMISSION_NONE,GATT_NOTIFY_ATTRIBUTE_WRITE,0x07,0x01, &createCharHandle);
	  if (status != BLE_STATUS_SUCCESS) {
	    PRINTF("name char failed:0x%02x\r\n", status);
	  }else{
	    PRINTF("add create characteristic SUCCESS\r\n");
	  }

		/*MODIFY Characteristic Created Here*/
	  COPY_UUID_128(uuid, 0xFC,0xB4,0x34,0x4A,0xCB,0xC4,0x11,0xE8,0xA8,0xD5,0xF2,0x80,0x1F,0x1B,0x9F,0xD1);

	  //aci_gatt_add_char
	  //status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_128,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	  status = aci_gatt_add_char(commandServHandle,UUID_TYPE_128,uuid,0x0030/*0x002C*/,CHAR_PROP_WRITE,ATTR_PERMISSION_NONE,GATT_NOTIFY_ATTRIBUTE_WRITE,0x07,0x01, &modifyCharHandle);
	  if (status != BLE_STATUS_SUCCESS) {
	    PRINTF("aci_gatt_add_char() failed:0x%02x\r\n", status);
	  }else{
	    PRINTF("add modify characteristic SUCCESS\r\n");
	  }


		/*DELETE Characteristic Created Here*/
	  COPY_UUID_128(uuid, 0xFC,0xB4,0x35,0x8A,0xCB,0xC4,0x11,0xE8,0xA8,0xD5,0xF2,0x80,0x1F,0x1B,0x9F,0xD1);

	  //aci_gatt_add_char
	  //status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_128,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	  status = aci_gatt_add_char(commandServHandle,UUID_TYPE_128,uuid,0x0030/*0x002C*/,CHAR_PROP_WRITE,ATTR_PERMISSION_NONE,GATT_NOTIFY_ATTRIBUTE_WRITE,0x07,0x01, &deleteCharHandle);
	  if (status != BLE_STATUS_SUCCESS) {
	    PRINTF("aci_gatt_add_char() failed:0x%02x\r\n", status);
	  }else{
	    PRINTF("add delete characteristic SUCCESS\r\n");
	  }
}

void Add_Check_Service(void){
	uint8_t status;
	uint8_t uuid[16];


	/*CHECK Service Created Here*/
	COPY_UUID_128(uuid, 0xFC,0xB4,0x3A,0xBC,0xCB,0xC4,0x11,0xE8,0xA8,0xD5,0xF2,0x80,0x1F,0x1B,0x9F,0xD1);
	  //aci_gatt_add_serv
	  //status = aci_gatt_add_serv(service_uuid_type,service_uuid_128,service_type,max_attribute_records, &service_handle);
	  // IMPORTANT: Please make sure the Max_Attribute_Records parameter is configured exactly with the required value.
	  status = aci_gatt_add_serv(UUID_TYPE_128,uuid,PRIMARY_SERVICE,0x0A, &checkServHandle);
	  if (status != BLE_STATUS_SUCCESS) {
	    PRINTF("aci_gatt_add_serv() failed:0x%02x\r\n", status);
	  }else{
	    PRINTF("add check service SUCCESS\r\n");}

		/*NAME Characteristic Created Here*/
	  COPY_UUID_128(uuid, 0xFC,0xB4,0x3B,0xDE,0xCB,0xC4,0x11,0xE8,0xA8,0xD5,0xF2,0x80,0x1F,0x1B,0x9F,0xD1);

	  //aci_gatt_add_char
	  //status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_128,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	  status = aci_gatt_add_char(checkServHandle,UUID_TYPE_128,uuid,0x30,0x12,ATTR_PERMISSION_NONE,GATT_NOTIFY_ATTRIBUTE_WRITE,0x07,0x00, &nameLogCharHandle);
	  if (status != BLE_STATUS_SUCCESS) {
	    PRINTF("aci_gatt_add_char() failed:0x%02x\r\n", status);
	  }else{
	    PRINTF("add name char SUCCESS\r\n");
	  }

		/*VOLUME Characteristic Created Here*/
	  COPY_UUID_128(uuid, 0xFC,0xB4,0x3D,0x0A,0xCB,0xC4,0x11,0xE8,0xA8,0xD5,0xF2,0x80,0x1F,0x1B,0x9F,0xD1);

	  //aci_gatt_add_char
	  //status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_128,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	  status = aci_gatt_add_char(checkServHandle,UUID_TYPE_128,uuid,0x4,0x12,ATTR_PERMISSION_NONE,GATT_NOTIFY_ATTRIBUTE_WRITE,0x07,0x00, &volumeLogCharHandle);
	  if (status != BLE_STATUS_SUCCESS) {
	    PRINTF("aci_gatt_add_char() failed:0x%02x\r\n", status);
	  }else{
	    PRINTF("add vol characteristic SUCCESS\r\n");
	  }

		/*DURATION Characteristic Created Here*/
	  COPY_UUID_128(uuid, 0xFC,0xB4,0x41,0x7E,0xCB,0xC4,0x11,0xE8,0xA8,0xD5,0xF2,0x80,0x1F,0x1B,0x9F,0xD1);

	  //aci_gatt_add_char
	  //status = aci_gatt_add_char(service_handle,char_uuid_type,char_uuid_128,char_value_length,char_properties,security_permissions,gatt_evt_mask,enc_key_size,is_variable, &char_handle);
	  status = aci_gatt_add_char(checkServHandle,UUID_TYPE_128,uuid,0x8,0x12,ATTR_PERMISSION_NONE,GATT_NOTIFY_ATTRIBUTE_WRITE,0x07,0x00, &durationLogCharHandle);
	  if (status != BLE_STATUS_SUCCESS) {
	    PRINTF("aci_gatt_add_char() failed:0x%02x\r\n", status);
	  }else{
	    PRINTF("add duration characteristic SUCCESS\r\n");
	  }
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

 // const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'P','U','M','P'};
  const char  local_name[] = PUMP_ADVERTIZE_NAME;
  /* disable scan response */
  hci_le_set_scan_resp_data(0,NULL);
  PRINTF("General Discoverable Mode.\n\r");

  ret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
                                 sizeof(local_name), local_name, 0, NULL, 0, 0);
  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("Error while setting discoverable mode (%d)\n\r", ret);
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
  for(uint32_t i = 5; i > 0; i--){
    PRINTF("%02X-", addr[i]);
  }
  PRINTF("%02X\n\r", addr[0]);
}

/**
 * @brief  This function is called when the peer device gets disconnected.
 * @param  None
 * @retval None
 */
void GAP_DisconnectionComplete_CB(void)
{
  connected = FALSE;
  PRINTF("Disconnected\n\r");
  /* Make the device connectable again. */
  set_connectable = TRUE;
  notification_enabled = FALSE;
}

/**
 * @brief  Read request callback.
 * @param  uint16_t Handle of the attribute
 * @retval None #TODO
 */
void Read_Request_CB(uint16_t handle)
{//  if(handle == accCharHandle + 1){
//    Acc_Update((AxesRaw_t*)&axes_data);
//  }
//  else if(handle == tempCharHandle + 1){
//    int16_t data;
//    data = 270 + ((uint64_t)rand()*15)/RAND_MAX; //sensor emulation
//    Acc_Update((AxesRaw_t*)&axes_data); //FIXME: to overcome issue on Android App
//                                        // If the user button is not pressed within
//                                        // a short time after the connection,
//                                        // a pop-up reports a "No valid characteristics found" error.
//    Temp_Update(data);
//  }
//  else if(handle == pressCharHandle + 1){
//    int32_t data;
//
//    HAL_Delay(100);
//    data = 100000 + ((uint64_t)rand()*1000)/RAND_MAX;
//    Press_Update(data);
//  }
//  else if(handle == humidityCharHandle + 1){
//    uint16_t data;
//
//    data = 450 + ((uint64_t)rand()*100)/RAND_MAX;
//
//    Humidity_Update(data);
//  }
//
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
void HCI_Event_CB(void * pData)
{
  hci_uart_pckt *hci_pckt = pData;
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
        /* //this callback is invoked when a Gatt attribute is modified (when a  client writes to a Gatt attribute ) */
        		case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED: {
        			if (debug)
        				printf("EVT_BLUE_GATT_ATTRIBUTE_MODIFIED \r\n");
        			evt_gatt_attr_modified_IDB05A1 *evt =
        					(evt_gatt_attr_modified_IDB05A1*) blue_evt->data;
        			/* Parse the attribute that has been modified and take the required action*/
        			Attribute_Modified_CB(evt->attr_handle, evt->data_length,
        					evt->att_data);
        		}
        			break;
      }
    }
    break;
  }
}

//intgration functions
void Parse(uint8_t * packet, uint8_t MODE)
{
	uint8_t name[36];
		uint8_t volume[8];
		uint8_t duration[12];
		// cyril : why those sizes ? 32+1+4+1+6=44-> Maximum_size = 48
		// volume 4 = 9999 ml = 10l
		// duration 6 : 999999 s = 11 jours

		int i;
		uint8_t * packetPtr = packet;

		for(i=0;i<32;i++, packetPtr++)
		{
			if(*(packetPtr) == '\0')
			{
				packetPtr++;
				break;
			}

			*(name + i) = *(packetPtr);
		}

		if(i == 32)
		{
			packetPtr++;
		}
		
		*(name + i) = '\0';

		for(i=0;i<4;i++, packetPtr++)
		{
			if(*(packetPtr) == '\0')
			{
				packetPtr++;
				break;
			}

			*(volume + i) = *(packetPtr);
		}

		if(i == 4)
		{
			packetPtr++;
		}

		*(volume + i) = '\0';

		for(i=0;i<6;i++, packetPtr++)
		{
			if(*(packetPtr) == '\0')
			{
				packetPtr++;
				break;
			}
			*(duration + i) = *(packetPtr);
		}

		*(duration + i) = '\0';


	if(debug)printf("[*]NAME : %s\n\r[*]VOLUME : %s\n\r[*]DURATION : %s\n\r", name, volume, duration);

		PacketArbitrer(name, volume, duration, MODE);



}

/* @brief function called when a attribute has been modified
 * @note  handle of the Attribute = charhandle +1
 *        handle of the notification char = charhandle + 2
 * @param uint16_t handle of the attribute that has been modified
 * @param uint8_t the length of data
 *
 */

void PacketArbitrer(uint8_t * name, uint8_t * volume, uint8_t * duration, uint8_t MODE)
{
	if (MODE == 'C')
	{
		printf("Creating Mode\n");
		//update simulation value
		if(!CreateCure(name, volume, duration))
		{
			printf("[!] ERROR CREATING CURE\n");
		}
	}

	else if (MODE == 'M')
	{
		if(MatchName(name))
		{
			if(!ModifyCure(volume, duration))
			{
				printf("[!] ERROR MODIFYING CURE\n");
			}
		}
		else
		{
			printf("[!] ERROR MATCHING NAME IN MODIF\n");
		}
	}

	else if (MODE == 'D')
	{
		if(MatchName(name))
		{
			if(!DeleteCure())
			{
				printf("[!] ERROR DELETING CURE\n\r");
			}
		}
		else
		{
				printf("[!] ERROR MATCHING NAME IN DEL\n\r");
		}
	}
	else
	{
			printf("[!] ERROR NO MODE IN PACKET ARBITRER\n\r");
	}
}


uint8_t CreateCure(uint8_t * name, uint8_t * volume, uint8_t * duration)
{
	strncpy(cure.name, name, 31);
	cure.initVolume = atoi(volume);
	cure.volume = atoi(volume);
	cure.initDuration = atoi(duration);
	cure.duration = atoi(duration);
	
	// REPLACE THIS BY TIME OF TIMER
	cure.startingTime = HAL_GetTick()/1000;
	printf("cure starting time after creation %d\n\r",cure.startingTime);
	printf("cure duration %d\n\r",cure.duration);

	if(cure.initVolume == 0 | cure.initDuration == 0)
	{
		printf("NULL VOLUME AND DURATION\n\r");
	}
	else
	{
		//cure_exist=1;
		//SEND INFORMATION THROUGHT BLE HERE AKA cure.volume, cure.duration, cure.name
		printf("[*]CURRENT CURE NAME : %s VOLUME : %d DURATION : %d\n\r", cure.name, cure.volume, cure.duration);
		//UpdateName(cure.name);
		//UpdateVolume(cure.volume);
		//UpdateDuration(cure.duration);
		//if(debug) printf("initial ble update of name , vol , duration done \n\r");
		state_fn = RunCure;

	}

	printf("[*] CURE CREATED\n\r");
	return 1;
}


uint8_t ModifyCure(uint8_t * volume, uint8_t * duration)
{
	cure.initVolume = atoi(volume);
	cure.volume = atoi(volume);
	cure.initDuration =	atoi(duration);
	cure.duration = atoi(duration);
	// REPLACE THIS BY TIME OF TIMER
	cure.startingTime = time(NULL);

	if(cure.initVolume == 0 | cure.initDuration == 0)
	{
		printf("NULL VOLUME AND DURATION\n");
	}
	else
	{
		state_fn = RunCure;
	}

	printf("[*] CURE MODIFIED\n");
	return 1;
}


uint8_t DeleteCure()
{
	memset(cure.name, 0x00, 32);
	cure.volume = 0;
	cure.initVolume = 0;
	cure.initDuration = 0;
	cure.duration = 0;
	cure.startingTime = 0;
	// TODO update all the char HERE

	if(debug)
	{
		printf("Delete with [*]NAME : %u\n\r[*]VOLUME : %d\n\r[*]DURATION : %d\n\r", cure.name, cure.volume, cure.duration);
 	}
	UpdateVolume(cure.volume);
 	UpdateDuration(cure.duration);
	UpdateName(cure.name);

	//cure_exist = 0;
	state_fn = NULL;

	printf("[*] CURE DELETED\n");
	return 1;
}

uint8_t MatchName(uint8_t * name)
{
	if(!strcmp(name, cure.name))
	{
		return 1;
	}
	return 0;
}


void RunCure()
{
	uint32_t currentTime = 0;
	uint32_t cureDuration = 0;
	HAL_Delay(100);

	//Curent Time in Seconde
	currentTime = HAL_GetTick()/1000;
	printf("currentTime in seconds %d\n\r", currentTime);
	cureDuration = GetDuration(currentTime);
	printf("cure simulation duration  %d\n\r",cureDuration );

	//Update Value Here	
	UpdateName(cure.name);
	UpdateVolume(cure.volume);
	UpdateDuration(cure.duration);	

	if(cureDuration >= cure.initDuration)
	{
		printf("END OF CURE\n");
		DeleteCure();
		return;
	}

	//change  simulation values
	if(UpdateCureValues(cureDuration))
	{
		printf("CURE SIMULATION UPDATED\n");

	}
}

uint32_t GetDuration(uint32_t currentTime)
{
	return (currentTime - cure.startingTime);
}

uint8_t UpdateCureValues(uint32_t cureDuration)
{
	if(UpdateRestingVolume(cureDuration))
	{
		printf("RESTING VOLUME UPDATED\n\r");
		//UpdateVolume(&cure.volume);
	}
	else
	{
		printf("ERROR VOLUME UPDATE\n\r");
	}
	if(UpdateRestingDuration(cureDuration))
	{
		printf("RESTING TIME UPDATED\n\r");
		//UpdateDuration(&cure.duration);
	}
	else
	{
		printf("ERROR TIME UPDATE\n\r");
	}
	return 1;
}

uint8_t UpdateRestingVolume(uint32_t cureDuration)
{
	cure.volume = cure.initVolume - ((cure.initVolume * cureDuration) / cure.initDuration);
	if(debug) printf("update Resting volume %u\n\r",cure.volume);
	return 1;
}

uint8_t UpdateRestingDuration(uint32_t cureDuration)
{
	cure.duration = cure.initDuration - cureDuration;
	printf("cure Resting duration  %u\n\r",cure.duration);
	return 1;
}

void Attribute_Modified_CB(uint16_t handle, uint8_t data_length,
		uint8_t *att_data) {
	/* when a create attribute is modified #TODO change the service name to create*/

	if (handle == nameLogCharHandle + 2) {
		if (debug)
			printf("notification enable for Namelog char\n\r");
	}
	else if (handle == volumeLogCharHandle +2)
	{
		if(debug)printf("notificaton enable for volume char\n\r ");
	}else if(handle == durationLogCharHandle + 2)
	{
		if(debug) printf("notification enable for duration char");

	}
	else if (handle == createCharHandle + 1) {
		/* check if cure exist #TODO*/
		//call the parse with MODE = CREATE
			//if(cure_exist == 0)
			if(state_fn == NULL)
			{
				Parse(att_data,'C');
			}else {
				printf("there is cure running, you can't create a new one !\n\r");
			}

	}else if(handle == modifyCharHandle + 1) {

		//if(cure_exist == 1)
		if(!(state_fn == NULL))
		{
			Parse(att_data,'M');
		}else {
			printf("there is no cure running, you can't modify !\n\r");
		}

	}else if(handle == deleteCharHandle + 1) {

		//if(cure_exist == 1)
		if(!(state_fn == NULL))
		{
			Parse(att_data,'D');
		}else {
			printf("there is no cure running, you can't delete!\n\r");
		}}


	/* when it's notification enabled/disabled event for a specific char */

	// #TODO complete the other characteristic
}

//update
 tBleStatus UpdateName(uint8_t *name) {
	//char *name = "insoline";
	//uint8_t *name = "ghada";
	uint8_t buff[3];
	tBleStatus ret;
	//put Data into the buffer in the required format
	ret = aci_gatt_update_char_value(checkServHandle, nameLogCharHandle, 0, strlen(name),
			(uint8_t *)name);

	if (ret != BLE_STATUS_SUCCESS) {
		if (debug)
			printf("Error while updating name char characteristic.%x\r\n", ret);
		return BLE_STATUS_ERROR;
	} else {

			printf("update name sucess\n\r\n\r");
	}
	return BLE_STATUS_SUCCESS;
}

tBleStatus UpdateVolume(uint16_t vol) {

	//char *name = "insoline";
	//uint8_t *name = "ghada";
	tBleStatus ret;
	//put Data into the buffer in the required format
	
	uint8_t sendingVolume[12] = "";
	sprintf(sendingVolume, "%d", vol);

	ret = aci_gatt_update_char_value(checkServHandle, volumeLogCharHandle, 0, 0x4,
			&sendingVolume[0]);

	if (ret != BLE_STATUS_SUCCESS) {
		if (debug)
			printf("Error while updating vol char characteristic.%x\r\n", ret);
		return BLE_STATUS_ERROR;
	} else {

			printf("update volume sucess\n\r");
	}
	return BLE_STATUS_SUCCESS;
}

tBleStatus UpdateDuration(uint32_t dur) {

	//char *name = "insoline";
	//uint8_t *name = "ghada";
	tBleStatus ret;
	//put Data into the buffer in the required format

	uint8_t sendingDuration[12] = "";
	sprintf(sendingDuration, "%d", dur);	
		
	ret = aci_gatt_update_char_value(checkServHandle, durationLogCharHandle, 0, 0x8,
			&sendingDuration[0]);

	if (ret != BLE_STATUS_SUCCESS) {
		if (debug)
			printf("Error while updating duration char characteristic.%x\r\n", ret);
		return BLE_STATUS_ERROR;
	} else {

			printf("update duration sucess\n\r");
	}
	return BLE_STATUS_SUCCESS;
}

int _gettimeofday( struct timeval *tv, void *tzvp )
{
    uint64_t t = HAL_GetTick();  // get uptime in nanoseconds
    tv->tv_sec = t / 1000000000;  // convert to seconds
    tv->tv_usec = ( t % 1000000000 ) / 1000;  // get remaining microseconds
    return 0;  // return non-zero for error
} // end _gettimeofday()
