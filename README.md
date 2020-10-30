
# SecPump :syringe: 

Welcome to SecPump!

SecPump is an open wireless insulin pump system workbench that models the insulin kinetics based on the modified Bergman's minimal model. The system workbench is primarily tailored for security assessments and countermeasures development against the numerous security flaws related to wearable medical devices. The platform is open-source and free from commercials constraints making it a suitable target to perform both hardware/software security demonstrations. SecPump aims at being straightforward to use, it only requires an inexpensive Nucleo F401-RE STM32 board with its BlueNRG extension.

<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/Sec-Vanilla.gif">
</p>

## Context

SecPump is a research project designed in collaboration with the [LCIS](https://lcis.grenoble-inp.fr/le-laboratoire), the [University of Arizona](https://www.arizona.edu/), and the [SERENE IoT project](http://serene.minalogic.net/). SERENE IoT aims at contributing to developing high quality connected care services and diagnostic tools based on Advanced Smart Health-Care IoT Devices. This platform intends to provide a framework for security evaluation, tailored for countermeasures development against the numerous security flaws related to medical devices. 

## Why an open-platform?

Wireless insulin pumps are considered life critical embedded systems. While these devices are promising innovation, there are known to be poor in security and vulnerable to various cyber-attacks [1]. Unfortunately, due to the lack of openess to research, and commercial constraints, security researchers cannot freely demonstrate security issues on these devices. Besides, the security countermeasures assessments can be difficult without an open-model. Here comes the SecPump! The repository provides: 

- An abstract representation of a wireless wearable insulin pump that models the blood glucose regulation within the human body. The open source code base of the pump can be extended, and improved for various demonstrations including cyber-physical system design, automation (PID regulation),security, and safety. 

- A software security demonstration based on advanced memory corruption exploitation. This demonstration exploits an intentional stack-based buffer overflow bug introduced in a vulnerable BLE characteristic. The bug allow an attacker to maliciously modify the functionning of the cyber-physical system at run-time. Such attack here raise the amount of insulin injected over the time.

## General description

## Get started

The repository contains three folders: "Scripts" contains the human body simulation scripts, the PC interface with the pump and a BLE communication script. "SecPump-Vanilla" contains the model of the functional pump, and finally, "SecPump-Security-Demo" is a deliberately vulnerable modified version of the pump.

### Requirements
To run the vanilla system it is mandatory to have a Nucleo F401-RE STM32 board with its BlueNRG extension, a Linux computer with a BLE connection (if possible). To replay the attack it is preferable to have two computers available both based on Linux, one of which should includes a functionnal BLE stack.

SecPump requires an [ARM-based baremetal cross compiler](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads?fbclid=IwAR2I_6zIP3RlRHWNmEP8ILh4RCU_YZxbl81QFU_9FZ7fBnJA82Z5OmoFixg). Make sure the latter is in your path.

You can grab it using the following commands:
~~~bash
wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
tar xfj gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
echo "export PATH=\$PATH:$(pwd)"\/gcc-arm-none-eabi-8-2018-q4-major-linux\/bin >> ~/.bashrc
source ~/.bashrc
~~~

## SecPump Vanilla

First of all you have to get into the right folder:
~~~bash
cd SecPump-Vanilla
~~~

Connect the STM32 to your computer. Then, compile the firmware:
~~~bash
make clean all
~~~

Open a new terminal to observe the output of the UART from the board:
~~~bash
screen /dev/ttyACM0 115200 # check UART device first
~~~

Flash the STM32:
~~~bash
./Flash.sh
~~~

If everything went well, you should observe this output on your UART terminal:
~~~
==== SECPUMP INIT====
Hardware version: 49
Firmware version: 1813

[*] BLE Stack Initialized.
[*] SERVER: BLE Stack Initialized.
[+] SecPump Service Created. Handle 0x000C
[+] MODE Charac handle: 0x000D
[+] BOLUS Charac handle: 0x0010
[*] SecPump service added successfully.
[*] General Discoverable Mode.
~~~

At this point the pump is waiting to be connected to the insulin kinetic Simulator (based on the extended Bergman's model). This simulator uses the UART to communicate with the standby pump. Therefore, the UART console must first be closed:
~~~bash
ctrl+A+k # screen command
~~~

Then, you can connect the simulator with the pump:
~~~bash
cd Scripts
python Sec-Interface.py
~~~

A 24-hour simulation is launched (displayed bellow). The diabetic blood glucose is modeled by the python script and the STM32-based pump regulates it with insulin using an integrated PID.

<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/Sec-Vanilla.gif">
</p>

WARNING: Always reset the pump before running a simulation !

The pump can be switched to "MANUAL MODE" for insulin step regulation. The configuration change is achieve using BLE. 

There are two options to make this change:

(Option 1) Do it from a smartphone and an application such as "BLE Scanner" to connect to a BLE device.

In "BLE Scanner" connect to SecPump:
<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/BLE-Notifier-11.jpg">
</p>

Click on "W" for the first BLE characteristic, and write "1" in text. This will change the pump mode to "MANUAL". (Write 0 to change back to AUTO, or simply reset the pump):
<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/BLE-Notifier-21.jpg">
</p>


Click on "W" for the second BLE characteristic, and write the insulin step to inject. (ex: 15):
<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/BLE-Notifier-21.jpg">
</p>

By relaunching a simulation you should get the following display:
<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/Sec-Vanilla-Manual.gif">
</p>

(Option 2) (Requires BLE Stack on Linux) Connect to SecPump using the provided python script:
~~~bash
cd Scripts
python BlueCmd.py
~~~



## SecPump Security Demonstration



## Acknowledgements

This work is carried out under the SERENE-IoT project, a project labelled within the framework of PENTA, the EUREKA cluster for Application and Technology Research in Europe on NanoElectronics.

This research was partially supported by the National Science Foundation under Grant CNS-1615890.


