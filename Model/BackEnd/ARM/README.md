# FreeRTOS Pump

This port has been design to be run from a linux operating system: 
- Make sure that you have the STM32Cube Mx Software installer
- Make sure that the [AArch32 bare-metal compiler](https://developer.arm.com/open-source/gnu-toolchain/gnu-a/downloads?fbclid=IwAR2I_6zIP3RlRHWNmEP8ILh4RCU_YZxbl81QFU_9FZ7fBnJA82Z5OmoFixg) is in your path (arm-eabi)

You can grab it using this command:
~~~
wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
tar xfj gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
echo "export PATH=\$PATH:$(pwd)"\/gcc-arm-none-eabi-8-2018-q4-major-linux\/bin >> ~/.bashrc
source ~/.bashrc
~~~

Then :
~~~
cd Baremetal_BLE_Pump/F401RE
make
~~~

Congrat's you can now port the binary on the F7 board


# Baremetal Pump

This port has been designed to be run from a linux system. 
- Make sure that the [AArch32 bare-metal compiler](https://developer.arm.com/open-source/gnu-toolchain/gnu-a/downloads?fbclid=IwAR2I_6zIP3RlRHWNmEP8ILh4RCU_YZxbl81QFU_9FZ7fBnJA82Z5OmoFixg) is in your path (arm-eabi)

You can grab it using this command:
~~~
wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
tar xfj gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
echo "export PATH=\$PATH:$(pwd)"\/gcc-arm-none-eabi-8-2018-q4-major-linux\/bin >> ~/.bashrc
source ~/.bashrc
~~~

Then:
~~~
cd FreeRTOS_Pump/F401RE
make
~~~

Congrat's you can now port the binary on the F4 board + BlueNRG extension board.
