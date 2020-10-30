/**
  ******************************************************************************
  * @file           : stm32f4xx_nucleo_bus.h
  * @brief          : header file for the BSP BUS IO driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32F4XX_NUCLEO_BUS_H
#define STM32F4XX_NUCLEO_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_conf.h"
#include "stm32f4xx_nucleo_errno.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4XX_NUCLEO
  * @{
  */

/** @defgroup STM32F4XX_NUCLEO_BUS STM32F4XX_NUCLEO BUS
  * @{
  */

/** @defgroup STM32F4XX_NUCLEO_BUS_Exported_Constants STM32F4XX_NUCLEO BUS Exported Constants
  * @{
  */

#define BUS_SPI1_INSTANCE SPI1
#define BUS_SPI1_MISO_GPIO_PIN GPIO_PIN_6
#define BUS_SPI1_MISO_GPIO_AF GPIO_AF5_SPI1
#define BUS_SPI1_MISO_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_MISO_GPIO_PORT GPIOA
#define BUS_SPI1_MISO_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_SPI1_MOSI_GPIO_PIN GPIO_PIN_7
#define BUS_SPI1_MOSI_GPIO_AF GPIO_AF5_SPI1
#define BUS_SPI1_MOSI_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_MOSI_GPIO_PORT GPIOA
#define BUS_SPI1_MOSI_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define BUS_SPI1_SCK_GPIO_PIN GPIO_PIN_3
#define BUS_SPI1_SCK_GPIO_AF GPIO_AF5_SPI1
#define BUS_SPI1_SCK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_SPI1_SCK_GPIO_PORT GPIOB
#define BUS_SPI1_SCK_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()
#ifndef BUS_SPI1_POLL_TIMEOUT
  #define BUS_SPI1_POLL_TIMEOUT                   0x1000U
#endif
/* SPI1 Baud rate in bps  */
#ifndef BUS_SPI1_BAUDRATE   
   #define BUS_SPI1_BAUDRATE   10000000U /* baud rate of SPIn = 10 Mbps*/
#endif

/**
  * @}
  */

/** @defgroup STM32F4XX_NUCLEO_BUS_Private_Types STM32F4XX_NUCLEO BUS Private types
  * @{
  */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U) 
typedef struct
{
  pSPI_CallbackTypeDef  pMspInitCb;
  pSPI_CallbackTypeDef  pMspDeInitCb;
}BSP_SPI_Cb_t;
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1U) */
/**
  * @}
  */
  
/** @defgroup STM32F4XX_NUCLEO_LOW_LEVEL_Exported_Variables LOW LEVEL Exported Constants
  * @{
  */

extern SPI_HandleTypeDef hspi1;

/**
  * @}
  */

/** @addtogroup STM32F4XX_NUCLEO_BUS_Exported_Functions
  * @{
  */    

/* BUS IO driver over SPI Peripheral */
HAL_StatusTypeDef MX_SPI1_Init(SPI_HandleTypeDef* hspi);
int32_t BSP_SPI1_Init(void);
int32_t BSP_SPI1_DeInit(void);
int32_t BSP_SPI1_Send(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI1_Recv(uint8_t *pData, uint16_t Length);
int32_t BSP_SPI1_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t Length);
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
int32_t BSP_SPI1_RegisterDefaultMspCallbacks (void);
int32_t BSP_SPI1_RegisterMspCallbacks (BSP_SPI_Cb_t *Callbacks);
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1U) */

int32_t BSP_GetTick(void);

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

#endif /* STM32F4XX_NUCLEO_BUS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
