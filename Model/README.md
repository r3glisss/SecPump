# Folder's content

This folder contains different subfolders for the infusion pump port:

 - The __BackEnd__ folder contains the port of the pump on both RISCV and ARM ISA. 
    - Porting the pump on the __ARM ISA__ requires having a F401-RE STM32 board + BlueNRG extension for the BLE/Baremetal implementation or the STM32F767Z for the FreeRTOS port. Be aware that the port on Freertos only communicates with UART
    - Porting the pump on the __RISCV ISA__ requires having an Artix 7 fpga board or the HiFive 1 Freedom board from Sifive.
    - The __BackEnd-Logic__ subfolder contains a portable code of the logic that runs inside the medical pump. This small code can be easily ported on a different board than those proposed on this repository


 - The __FrontEnd__ folder contains the pump programming interface running on Linux operating system.
    - The UART_Interface subfolder is the pump programming interface associated to the FreeRTOS port. This interface is used to communicate with the pump using the UART.
    - The BLE-FrontEnd subfolder contains a command line interface associated to the ARM BLE/Baremetal port of the medical pump. While the pump is communicating in BLE with the command line interface it is also possible to use putty in UART to display all the forensic logs in real time. 
    - The Front-End Logic subfolder contains a blank interface with no specific communication protocol. Therefore it is possible to fill this Front-End with the communication protocol of your choice. 