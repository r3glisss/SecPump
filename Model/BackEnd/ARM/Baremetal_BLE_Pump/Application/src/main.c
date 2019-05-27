/**
  ******************************************************************************
  * @file    main.c
  * @author  CL
  * @version V1.0.0
  * @date    04-July-2014
  * @brief   This application contains an example which shows how implementing
  *          a proprietary Bluetooth Low Energy profile: the sensor profile.
  *          The communication is done using a Nucleo board and a Smartphone
  *          with BTLE.
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

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "pump_service.h"
#include "pump_config.h"
#include "osal.h"
#include "sensor_service.h"
#include "debug.h"
#include "stm32_bluenrg_ble.h"
#include "bluenrg_utils.h"




/** @addtogroup X-CUBE-BLE1_Applications
 *  @{
 */

/** @defgroup SensorDemo
 *  @{
 */

/** @defgroup MAIN
 * @{
 */

/** @defgroup MAIN_Private_Defines
 * @{
 */
/* Private defines -----------------------------------------------------------*/
#define BDADDR_SIZE 6

/**
 * @}
 */

/* Private macros ------------------------------------------------------------*/

/** @defgroup MAIN_Private_Variables
 * @{
 */
/* Private variables ---------------------------------------------------------*/
extern volatile uint8_t set_connectable;
extern volatile int connected;
//extern volatile int     connected;
//extern CureProg cure;
uint8_t bnrg_expansion_board = IDB04A1; /* at startup, suppose the X-NUCLEO-IDB04A1 is used */
UART_HandleTypeDef UartHandle;
extern int debug;
/**
 * @}
 */

/** @defgroup MAIN_Private_Function_Prototypes
 * @{
 */
/* Private function prototypes -----------------------------------------------*/
void User_Process(Sensor_t* sensor);
tBleStatus Digital_Notify(void);
int __io_putchar(int ch);
void init_ble_general_mode(void); //#TODO to move to pump.h
extern void (*state_fn)(void);
/**
 * @}
 */

/**
 * @brief  Main function to show how to use the BlueNRG Bluetooth Low Energy
 *         expansion board to send data from a Nucleo board to a smartphone
 *         with the support BLE and the "BlueNRG" app freely available on both
 *         GooglePlay and iTunes.
 *         The URL to the iTunes for the "BlueNRG" app is
 *         http://itunes.apple.com/app/bluenrg/id705873549?uo=5
 *         The URL to the GooglePlay is
 *         https://play.google.com/store/apps/details?id=com.st.bluenrg
 *         The source code of the "BlueNRG" app, both for iOS and Android, is
 *         freely downloadable from the developer website at
 *         http://software.g-maps.it/
 *         The board will act as Server-Peripheral.
 *
 *         After connection has been established:
 *          - by pressing the USER button on the board, the cube showed by
 *            the app on the smartphone will rotate.
 *
 *         The communication is done using a vendor specific profile.
 *
 * @param  None
 * @retval None
 */
int main(void)
{

  /* STM32Cube HAL library initialization:
   *  - Configure the Flash prefetch, Flash preread and Buffer caches
   *  - Systick timer is configured by default as source of time base, but user
   *    can eventually implement his proper time base source (a general purpose
   *    timer for example or other time source), keeping in mind that Time base
   *    duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
   *    handled in milliseconds basis.
   *  - Low Level Initialization
   */
  HAL_Init();

  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /* Configure the User Button in GPIO Mode */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

  /* Configure the system clock */
  SystemClock_Config();

  UartHandle.Instance        = USART2;
  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_8;

  HAL_UART_Init(&UartHandle);
	// We check if a code PIN appearing mode is chosen or not (see the CODE_PIN_ENABLED constant in meersens_config.h)
		if(CODE_PIN_ENABLED == 0)
		{
			init_ble_general_mode();  		// BLE initialization without code PIN
		}
		else
		{
			//init_ble_Passkey();				// BLE initialization with code PIN
		}

		// We check if we have to clear the Security Database (white list...) (see the CLEAR_SECURITY_DATABASE constant in meersens_config.h)
		//if(CLEAR_SECURITY_DATABASE) clear_data_Base();

  while(1)
  {
    if (set_connectable) {
		setConnectable();
		set_connectable = FALSE;
	}
	// in place of  hci_user_evt_proc();
    HCI_Process();
    //drug simulation
  if(state_fn == NULL){
	  //do nothing
  //  printf("simulation not running\r\n");
	   BSP_LED_Toggle(LED2);
	   HAL_Delay(500);
  }else{
    printf("simulation running\n\r");
	  (*state_fn)();
  }

  }
}

/** #TODO to remove not used
 * @brief  Process user input (i.e. pressing the USER button on Nucleo board)
 *         and send the updated acceleration data to the remote client.
 *
 * @param  Sensor_t* sensor
 * @retval None
 */
void User_Process(Sensor_t* sensor)
{
  if(set_connectable){
    setConnectable();
    set_connectable = FALSE;
  }

  /* Check if the user has pushed the button #TODO t remove */
  if(BSP_PB_GetState(BUTTON_KEY) != RESET) // not woking...
  {
     HAL_Delay(500);
     Digital_Notify();
  }
  HAL_Delay(1000);

  BSP_LED_Toggle(LED2); //used for debugging (BSP_LED_Init() above must be also enabled)

  if(connected)
  {
    /* Update acceleration data */
    sensor->data += 1;
    PRINTF("Aio: %d\r\n", sensor->data);
    Aio_Update(sensor);
  }
}

int __io_putchar(int ch)
{
  HAL_UART_Transmit(&UartHandle, (uint8_t *) &ch, 1, 0xFFFF);
  return ch;
}


void init_ble_general_mode(void) {

	/* Initialization BLE ending */
/* USER CODE END X_CUBE_BLE1_Init_PreTreatment */

/* Initialize the peripherals and the BLE Stack */
  const char *name = PUMP_DEVICE_NAME;
  uint8_t SERVER_BDADDR[] = {0x12, 0x34, 0x00, 0xE1, 0x80, 0x03};
  uint8_t bdaddr[BDADDR_SIZE];
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;

  uint8_t  hwVersion;
  uint16_t fwVersion;
  int ret;

  /* Initialize the BlueNRG SPI driver  #TODO to remmove if ble works*/
  BNRG_SPI_Init();

  /* Initialize the BlueNRG HCI */
  HCI_Init();

  /* Reset BlueNRG hardware */
  BlueNRG_RST();

  /* get the BlueNRG HW and FW versions */
  getBlueNRGVersion(&hwVersion, &fwVersion);

  /*
   * Reset BlueNRG again otherwise we won't
   * be able to change its MAC address.
   * aci_hal_write_config_data() must be the first
   * command after reset otherwise it will fail.
   */
  hci_reset();

  HAL_Delay(100);

  if(debug) PRINTF("HWver %d\nFWver %d\n", hwVersion, fwVersion);

  if (hwVersion > 0x30) { /* X-NUCLEO-IDB05A1 expansion board is used */
    bnrg_expansion_board = IDB05A1;
    /*
     * Change the MAC address to avoid issues with Android cache:
     * if different boards have the same MAC address, Android
     * applications unless you restart Bluetooth on tablet/phone
     */
    SERVER_BDADDR[5] = 0x02;
  }
  /* The Nucleo board must be configured as SERVER */
  Osal_MemCpy(bdaddr, SERVER_BDADDR, sizeof(SERVER_BDADDR));


  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                                  CONFIG_DATA_PUBADDR_LEN,
                                  bdaddr);
  if (ret) {
    if(debug) printf("Setting BD_ADDR failed.\n\r");
  }

  ret = aci_gatt_init();
  if(ret){
    if(debug) printf("GATT_Init failed.\n\r");
  }

  if (bnrg_expansion_board == IDB05A1) {
    ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  }
  else {
    ret = aci_gap_init_IDB04A1(GAP_PERIPHERAL_ROLE_IDB04A1, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  }

  if (ret != BLE_STATUS_SUCCESS) {
    if(debug) printf("GAP_Init failed.\n");
  }

  ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0,
                                   strlen(name), (uint8_t *)name);

  if (ret) {
    if(debug) printf("aci_gatt_update_char_value failed.\n");
    while(1);
  }

  ret = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
                                     OOB_AUTH_DATA_ABSENT,
                                     NULL,
                                     7,
                                     16,
                                     USE_FIXED_PIN_FOR_PAIRING,
                                     123456,
                                     BONDING);
  if (ret == BLE_STATUS_SUCCESS) {
    if(debug) printf("BLE Stack Initialized.\n");
  }

  if(debug) printf("SERVER: BLE Stack Initialized\n");

  //add GATT Databse
  //Add_Dis_Service();
    Add_Check_Service();
  Add_Command_Service();



  /* Set output power level */
  ret = aci_hal_set_tx_power_level(1,4);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
