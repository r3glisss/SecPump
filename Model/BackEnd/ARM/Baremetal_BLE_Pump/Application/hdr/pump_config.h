/*
 * pump_config.h
 *
 *  Created on: Nov 15, 2018
 *      Author: ghada
 */

#ifndef APPLICATION_USER_PUMP_CONFIG_H_
#define APPLICATION_USER_PUMP_CONFIG_H_


/** BLE parameters **/

/* Pump Name*/
#define PUMP_DEVICE_NAME "devname" //device name characteristic in generic access service
/*the advertising name of the Pump
 * @note put a tab before the name chosen to be accepted *
 * @Maximum_size = 23 bytes
 */
#define PUMP_ADVERTIZE_NAME "	VSPM"


/* BLE MODE:
 * CODE_PIN_ENABLED = 1 => Connection with password ( Passkey Pairing with fixed Pin stored in EEPROM)
 * CODE_PIN_ENABLED = 0 => Connection without password ( Justworks Pairing )
 * */
#define CODE_PIN_ENABLED 0
#define CLEAR_SECURITY_DATABASE 0

/* DIS Service parametrs */
#define MANUFUCTURER_NAME "Manuf_Name"
#define MODEL_NUMBER "xxxx"
#define SERIAL_NUMBER "xxxx"
#define HARDWARE_REVISION "xxxxxx"
#define FIRMWARE_REVISION "xxxxxx"
#define SOFTWARE_REVISION "xxxxxx"
#define SYSTEM_ID "40-24"



#endif /* APPLICATION_USER_PUMP_CONFIG_H_ */
