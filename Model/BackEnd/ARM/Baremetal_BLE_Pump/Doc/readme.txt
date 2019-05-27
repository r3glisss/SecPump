/**
  @page BlueNRG Expansion Board for STM32 Nucleo Boards Sensor Demo Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    readme.txt  
  * @author  CL/AST
  * @version V0.0.1
  * @date    28-May-2014
  * @brief   This application contains an example which shows how implementing
  *          a proprietary Bluetooth Low Energy profile: the sensor profile.
  *          The communication is done using a STM32 Nucleo board and a Smartphone
  *          with BTLE.
  ******************************************************************************
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
  @endverbatim

  
@par Example Description 

Main function to show how to use the BlueNRG Bluetooth Low Energy 
expansion board to send data from an STM32 Nucleo board to a smartphone with
the support BLE and the "BlueNRG" app freely available on both GooglePlay and iTunes.
The URL to the iTunes for BlueNRG is http://itunes.apple.com/app/bluenrg/id705873549?uo=5
The URL to the GooglePlay is https://play.google.com/store/apps/details?id=com.st.blunrg
The source code of the "BlueNRG" app, both for iOS and Android, is freely downloadable 
from the developer website at http://software.g-maps.it/

NO SUPPORT WILL BE PROVIDED TO YOU BY STMICROELECTRONICS FOR ANY OF THE
ANDROID .apk FILES INCLUDED IN OR REFERENCED BY THIS PACKAGE.

The board will act as Server-Peripheral.
After establishing the connection:
- by pressing the USER button on the board, the cube showed by
  the app on the smartphone in the MOTION tab will rotate on the
  three axes (x,y,z).
- in the ENVIRONMENT tab of the app the temperature, pressure
  and humidity emulated values sent by the STM32 Nucleo to the smartphone are shown.
- in the OTHER tab of the app the RSSI value is shown.
For testing this sample application, a smartphone (acting as Central device) with the
"BlueNRG" app running is needed.

This application shows also how to add new GATT services and characteristics.
Setting the NEW_SERVICES define to 1 (in $PATH_TO_THIS_APPLICATION\Inc\sensor_service.h),
two new services are enabled:
  - the Time Server service, which has two characteristics
      - seconds, a read only characteristic which exposes the number of seconds
        passed since system boot
      - minutes, a characteristic which exposes the number of minutes passed
        since system boot. This characteristic can be read by GATT server, and a
        “notify” event is generated for this characteristic at one minute intervals.
  - the LED service, which can be used to control the state of LED2 present on the
    STM32 Nucleo board. This service has a writable “LED button characteristic”,
    which controls the state of LED2.
    When the GATT client application modifies the value of this characteristic, LED2
    is toggled.
Hence the application advertises its services and characteristics to the listening client
devices while waiting for a connection to be made.
After the connection is created by the central device, data is periodically updated. 

In this example, when the NEW_SERVICES is set to 1, it is recommended to use a PC as Central 
device and a BlueNRG USB dongle.
If you don't have the USB dongle, as an alternative, you can use another STM32 Nucleo board +
X-NUCLEO-IDB04A1/X-NUCLEO-IDB05A1 with the Virtual_COM_Port application included in this package.
Also, you need to install the BlueNRG GUI on your PC.
The BlueNRG GUI installer is provided in the STSW-BLUENRG-DK package available at 
http://www.st.com
Alternatively any Android/iOS app for the BLE diagnostic can be used.
The BlueNRG GUI can be used to initialize and configure the BlueNRG USB dongle
properly so that it can be used for discovering services and characteristics of the BLE 
application running on the STM32 Nucleo board equipped with a BlueNRG expansion board where the
SensorDemo application is running.
The PC connected to the BlueNRG USB dongle must be configured as “GAP central device”,
while the STM32 Nucleo board equipped with the BlueNRG expansion board acts as “GAP peripheral
device”.
Once the BlueNRG USB dongle is configured correctly, it can be used to scan remote
devices and send the ACI commands.
In the ACI Utilities tab on the BlueNRG GUI all the commands needed to set the 
device as "Central", scan the network, connect the Central to the Peripheral and discover 
its services and characteristics are provided.
At this point the new services that have been added through the preprocessor defined symbol 
“NEW_SERVICES” are accessible from the Central device. 



                            ------------------
                            |    PC with     | 
                            |   BlueNRG GUI  |
                            |    running     |
                            ------------------
                           / --------------- /
                          / --------------- / 
             /---------->/    ---------    /<------\ USB Mini-B to A Cable
            / In USB     ------------------         \ powering the board
           /  port                                   \
    --------------                               -----------------------
    |  BlueNRG   |--                             |   STM32 Nucleo +    |
    | USB Dongle |--                             | X-NUCLEO-IDB04A1    |
    --------------                               | or X-NUCLEO-IDB05A1 |
                                                 -----------------------


The communication is done using a vendor specific profile.

  
@par Hardware and Software environment

  - This example runs on STM32 Nucleo devices with BlueNRG STM32 expansion board
    (X-NUCLEO-IDB04A1 or X-NUCLEO-IDB05A1)
  - This example has been tested with STMicroelectronics:
    - NUCLEO-L053R8 RevC board
    - NUCLEO-F401RE RevC board
    - NUCLEO-L476RG RevC board
    and can be easily tailored to any other supported device and development board.
    This example runs also on the NUCLEO-F411RE RevC board, even if the chip could
    be not exploited at its best since the projects are configured for the
    NUCLEO-F401RE target board.

  
@par How to use it? 

In order to make the program work, you must do the following:
 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
 - Open IAR toolchain (this firmware has been successfully tested with
   Embedded Workbench V7.80.4).
   Alternatively you can use the Keil uVision toolchain (this firmware
   has been successfully tested with V5.22) or the System Workbench for
   STM32 (this firmware has been successfully tested with Version 1.12.0).
 - Rebuild all files and load your image into target memory.
 - Run the example.
 - Alternatively, you can download the pre-built binaries in "Binary" 
   folder included in the distributed package.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
