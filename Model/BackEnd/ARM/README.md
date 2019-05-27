# FreeRTOS Pump

This port has been design to be run from a linux operating system: 
- Make sure that you have the STM32Cube Mx Software installer
- Make sure that the [AArch32 bare-metal compiler](https://developer.arm.com/open-source/gnu-toolchain/gnu-a/downloads?fbclid=IwAR2I_6zIP3RlRHWNmEP8ILh4RCU_YZxbl81QFU_9FZ7fBnJA82Z5OmoFixg) is in your path (arm-eabi)

Then in the root folder tape:
~~~
make
~~~

Congrat's you can now port the binary on the F7 board


# Baremetal Pump

This port has been designed to be run from a linux system. 
- Make sure that the [AArch32 bare-metal compiler](https://developer.arm.com/open-source/gnu-toolchain/gnu-a/downloads?fbclid=IwAR2I_6zIP3RlRHWNmEP8ILh4RCU_YZxbl81QFU_9FZ7fBnJA82Z5OmoFixg) is in your path (arm-eabi)

Then in the F401RE folder tape:
~~~
make
~~~

Congrat's you can now port the binary on the F4 board + BlueNRG extension board.

