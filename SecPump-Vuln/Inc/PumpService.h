// Copyright (C) 2019 LCIS Laboratory - Cyril Bresch
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, in version 3.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// This program is part of the SecPump @https://github.com/r3glisss/SecPump

/******************************************************************************
  * @file    PumpService.h 
  * @author  CL
  * @version V1.0.0
  * @date    04-July-2014
  * @brief   
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
  
/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef _PUMP_SERVICE_H_
#define _PUMP_SERVICE_H_

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "bluenrg_gap.h"
#include "string.h"
#include "hci_const.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_aci_const.h"   
#include "hci.h"
#include "hci_le.h"
#include "sm.h"
#include "main.h"


/** @addtogroup Applications
 *  @{
 */

/** @addtogroup PumpDemo
 *  @{
 */
 
/** @addtogroup PUMP_SERVICE 
 * @{
 */

/** @addtogroup PUMP_SERVICE_Exported_Defines 
 * @{
 */
/* Exported defines ----------------------------------------------------------*/   
#define IDB04A1 0
#define IDB05A1 1

/**
 * @}
 */

/** @addtogroup PUMP_SERVICE_Exported_Types
 *  @{
 */
typedef int i32_t;

/** @addtogroup PUMP_SERVICE_Exported_Functions
 *  @{
 */

tBleStatus Add_Pump_Service(void);

void Read_Request_CB(uint16_t handle);
void Attribute_Modified_CB(uint16_t handle, uint8_t data_length, uint8_t *att_data);
void setConnectable(void);
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle);
void GAP_DisconnectionComplete_CB(void);
void user_notify(void * pData);

void ProcessModeReq(uint8_t * att_data);
void ProcessBolusReq(uint8_t * att_data);
void ProcessVulnReq(uint8_t * att_data);

/// malicious vulnerable function
void MaliciousMemCpy(void *dest, void *src, size_t n); 

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

#ifdef __cplusplus
}
#endif

#endif /* _PUMP_SERVICE_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

