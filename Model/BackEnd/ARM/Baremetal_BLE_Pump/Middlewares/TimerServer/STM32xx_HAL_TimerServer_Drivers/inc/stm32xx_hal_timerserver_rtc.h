/**
  ******************************************************************************
  * @file    stm32xx_hal_timerserver_rtc.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    01-October-2014
  * @brief   Header for stm32xx_timerserver.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32XX_HAL_TIMERSERVER_RTC_H
#define __STM32XX_HAL_TIMERSERVER_RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup Middlewares
 *  @{
 */

/** @addtogroup ST
 *  @{
 */
 
/** @addtogroup TimerServer
 *  @{
 */

/** @defgroup STM32XX_HAL_TIMERSERVER_RTC
 *  @{
 */
 
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup STM32XX_HAL_TIMERSERVER_RTC_Exported_Macros
 *  @{
 */
/* Exported macros -----------------------------------------------------------*/
/**
  * @brief  Read if the RTC WakeUp Timer peripheral is enabled.
  * @param  __HANDLE__: specifies the RTC handle.
  * @retval None
  */ 
#define __HAL_TIMERSERVER_RTC_GET_WAKEUPTIMER_STATUS(__HANDLE__)	(((((__HANDLE__)->Instance->CR) & (RTC_CR_WUTE)) != RESET)? SET : RESET)

 /**
   * @brief  Write the RTC Wakeup counter.
   * @param  __HANDLE__:	specifies the RTC handle.
   * @param  __COUNTER__: 	Value of the counter to be written.
   * @retval None
   */
#define __HAL_TIMERSERVER_RTC_SET_WAKEUPCOUNTER(__HANDLE__, __COUNTER__)		((__HANDLE__)->Instance->WUTR = (__COUNTER__))

 /**
   * @brief  Read the SSR register
   * @param  __HANDLE__:	specifies the RTC handle.
   * @retval SSR register value
   */
 #define __HAL_TIMERSERVER_RTC_GET_SSR(__HANDLE__)		((__HANDLE__)->Instance->SSR)

 /**
   * @brief  Read the WUCKSEL Configuration
   * @param  __HANDLE__:	specifies the RTC handle.
   * @retval WUCKSEL value
   */
 #define __HAL_TIMERSERVER_RTC_GET_WUCKSEL(__HANDLE__)		(((__HANDLE__)->Instance->CR) & RTC_CR_WUCKSEL)

 /**
   * @brief  Read the PREDIV_A Configuration
   * @param  __HANDLE__:	specifies the RTC handle.
   * @retval PREDIV_A value
   */
 #define __HAL_TIMERSERVER_RTC_GET_PREDIVA(__HANDLE__)		((((__HANDLE__)->Instance->PRER) & RTC_PRER_PREDIV_A)>>16)

 /**
   * @brief  Read the PREDIV_S Configuration
   * @param  __HANDLE__:	specifies the RTC handle.
   * @retval PREDIV_S value
   */
#ifndef CMSIS_RTC_GET_PREDIVS_MASK_CORRECTED
  #define __HAL_TIMERSERVER_RTC_GET_PREDIVS(__HANDLE__) (((__HANDLE__)->Instance->PRER) & (0x7FFF))
#else
  #define __HAL_TIMERSERVER_RTC_GET_PREDIVS(__HANDLE__) (((__HANDLE__)->Instance->PRER) & RTC_PRER_PREDIV_S)
#endif



/**
 * @}
 */
 
/* Exported functions ------------------------------------------------------- */

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

#endif /*__STM32XX_HAL_TIMERSERVER_RTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
