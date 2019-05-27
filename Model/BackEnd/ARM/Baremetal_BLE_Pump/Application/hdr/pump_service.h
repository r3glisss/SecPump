/*
 * pump_service.h
 *
 *  Created on: Nov 15, 2018
 *      Author: ghada
 */

#ifndef APPLICATION_USER_PUMP_SERVICE_H_
#define APPLICATION_USER_PUMP_SERVICE_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
// //#include "bluenrg_gap.h"
// #include "string.h"
// #include "hci_const.h"
// #include "bluenrg_hal_aci.h"
// #include "bluenrg_aci_const.h"
// #include "hci.h"
// #include "hci_le.h"
// #include "sm.h"
// #include <stdlib.h>

#include "hal_types.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap.h"
#include "string.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "hci_const.h"
//#include "gp_timer.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_aci_const.h"
#include "hci.h"
#include "hci_le.h"
#include "hal.h"
#include "sm.h"
#include "debug.h"

#include <stdlib.h>
/** @addtogroup Applications
 *  @{
 */

/** @addtogroup SensorDemo
 *  @{
 */

/** @addtogroup SENSOR_SERVICE
 * @{
 */

/** @addtogroup SENSOR_SERVICE_Exported_Defines
 * @{
 */
/* Exported defines ----------------------------------------------------------*/
#define IDB04A1 0
#define IDB05A1 1

/**
 * @}
 */

/** @addtogroup SENSOR_SERVICE_Exported_Types
 *  @{
 */

typedef struct {

	uint8_t name[32];				/*name from chacteristic name*/
	uint16_t initVolume; 		/*initial volume*/
	uint16_t volume;	  		/*volume from chacteristic volume*/
	uint32_t initDuration; 	/*initial duration*/
	uint32_t duration;  		/*duration get from characteristic duration*/
	uint32_t startingTime;	/*initial time when the cure has been created*/

} CureProg;
/**
 * @}
 */

/** @addtogroup SENSOR_SERVICE_Exported_Functions
 *  @{
 */


void Add_Command_Service(void);

tBleStatus UpdateName(uint8_t * name);

tBleStatus UpdateVolume(uint16_t vol);

tBleStatus UpdateDuration(uint32_t dur);

void Add_Check_Service(void);

void Add_Dis_Service(void);

void Read_Request_CB(uint16_t handle);

void setConnectable(void);

void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle);

void GAP_DisconnectionComplete_CB(void);
//void       user_notify(void * pData);
void HCI_Event_CB(void *pckt);

void Attribute_Modified_CB(uint16_t handle, uint8_t data_length,uint8_t *att_data);

void Parse(uint8_t * packet, uint8_t MODE); // parse the data and call the required function (create, modify,delete)

void PacketArbitrer(uint8_t * name, uint8_t * volume, uint8_t * duration, uint8_t MODE);

uint8_t CreateCure(uint8_t * name, uint8_t * volume, uint8_t * duration);

uint8_t ModifyCure(uint8_t * volume, uint8_t * duration);

uint8_t DeleteCure();

uint8_t MatchName(uint8_t * name);

void RunCure();

uint32_t GetDuration(uint32_t currentTime);

uint8_t UpdateCureValues(uint32_t cureDuration);

uint8_t UpdateRestingVolume(uint32_t cureDuration);

uint8_t UpdateRestingDuration(uint32_t cureDuration);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_PUMP_SERVICE_H_ */
