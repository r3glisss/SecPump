# SecPump :syringe: 

SecPump is an open wireless insulin pump system workbench that models the insulin kinetics based on the modified Bergman's minimal model. The system workbench is primarily tailored for security assessments and countermeasures development against the numerous security flaws related to wearable medical devices. The platform is open-source and free from commercials constraints making it a suitable target to perform both hardware and software security demonstrations. SecPump aims at being straightforward to use, it only requires an inexpensive Nucleo F446-RE STM32 board with the BlueNRG extension.

<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/SecPump_Overview.JPG">
</p>

## Context

SecPump is a research project designed in collaboration with the [LCIS](https://lcis.grenoble-inp.fr/le-laboratoire), the [University of Arizona](https://www.arizona.edu/), and the [SERENE-IoT project](http://serene.minalogic.net/). SERENE-IoT aims at contributing to developing high quality connected care services and diagnostic tools based on Advanced Smart Health-Care IoT Devices. This platform intends to provide a framework for security evaluation, tailored for countermeasures development against the numerous security flaws related to medical devices. 

## Why an open-platform?

Wireless insulin pumps are considered life-critical embedded systems. While these devices are promising innovation, there are vulnerable to a wide range of cyber-attacks. Unfortunately, due to the lack of openness to research and commercial constraints, security researchers cannot freely demonstrate security issues on these devices. Besides, the security countermeasures assessments can be difficult without an open-model. Here comes SecPump! The repository provides: 

- An abstract representation of a wireless wearable insulin pump that models the blood glucose regulation within the human body. The open-source code base of the pump can be extended and improved for various demonstrations including cyber-physical system design, automation (PID regulation), security, and safety. 

- A software security demonstration based on advanced memory corruption exploitation. This demonstration exploits an intentional stack-based buffer overflow bug introduced in a vulnerable BLE characteristic. The bug allows an attacker to maliciously modify the functioning of the cyber-physical system at run-time. Such an attack, here, raises the amount of insulin injected over time.

## General description

SecPump is a wireless insulin pump and insulin kinetics simulator. The vanilla version of the system workbench (version without any security issue) requires an STM32 board with the BlueNRG extension and a computer or a Raspberry Pi running Python. The computer/Raspberry Pi models the human body kinetics over time based on Bergman's differential equations. In other words, it models how blood glucose reacts based on the insulin level in the human body. In our case, the computer/Raspberry Pi models the blood glucose of a diabetic. As diabetic does not produce insulin, the latter should be injected thanks to an artificial pancreas such as an insulin pump. The STM32 is only the insulin pump of the whole system. To represent the physical link between a theoretical diabetic and the wireless pump, the STM32 is connected via the serial port to the computer/Raspberry Pi. Over time, the computer/Rasperry Pi sends the amount of glucose of the diabetic to the STM32 that computes the amount of insulin to inject accordingly. The amount of insulin to inject by the pump is computed thanks to an integrated PID. SecPump is then a closed-loop regulation system. As a note, SecPump can also be programmed as an open-loop system when the amount of insulin to inject can be manually configured using a BLE interface (smartphone or BLE python script).

## Get started

The repository contains three folders: "Scripts" contains the human body simulation scripts, the PC interface with the pump, and a BLE communication script. "SecPump-Vanilla" contains the model of the functional pump, and finally, "SecPump-Security-Demo" is a deliberately vulnerable modified version of the pump.

### Requirements

To run the vanilla system it is mandatory to have a Nucleo F446-RE STM32 board with the BlueNRG extension, a Linux computer with a BLE connection (if possible). 

To replay the attack it is preferable to have two computers available both based on Linux, one of which should include a functional BLE stack.

SecPump requires an [ARM-based bare-metal cross compiler](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads?fbclid=IwAR2I_6zIP3RlRHWNmEP8ILh4RCU_YZxbl81QFU_9FZ7fBnJA82Z5OmoFixg). Make sure the latter is in your path.

You can grab it using the following commands:
~~~bash
wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
tar xfj gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2
echo "export PATH=\$PATH:$(pwd)"\/gcc-arm-none-eabi-8-2018-q4-major-linux\/bin >> ~/.bashrc
source ~/.bashrc
~~~

Install the python dependencies, it is recommanded to use a dedicated new python environment: 

~~~bash
pip install -r Scripts/requirements.txt
~~~

## SecPump Vanilla

First of all, you have to get into the right folder:
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

At this point, the pump is waiting to be connected to the insulin kinetic Simulator (based on the extended Bergman's model). This simulator uses the UART to communicate with the standby pump. Therefore, the UART console must first be closed:
~~~bash
ctrl+A+k # screen command
~~~

Then, you can connect the simulator with the pump:
~~~bash
cd Scripts
python Sec-Interface.py
~~~

A 24-hour simulation is launched (displayed below). The diabetic blood glucose is modeled by the python script and the STM32-based pump regulates it with insulin using an integrated PID.

<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/Sec-Vanilla.gif">
</p>

**WARNING: Always reset the pump before running a simulation !**

The pump can be switched to "MANUAL MODE" for insulin step regulation. The configuration change is achieve using BLE. 

There are two options to make this change:

**(Option 1):** Do it from a smartphone and an application such as "BLE Scanner" to connect to a BLE device.

With "BLE Scanner" connect to SecPump:
<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/BLE-Notifier-11.jpg">
</p>

Click on "W" for the first BLE characteristic, and write "1" in text. This will change the pump mode to "MANUAL". (Write 0 to change back to AUTO, or simply reset the pump):
<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/BLE-Notifier-21.jpg">
</p>


Click on "W" for the second BLE characteristic, and write the insulin step to inject. (ex: 15):
<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/BLE-Notifier-22.jpg">
</p>

By relaunching a simulation you should get the following display:

<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/Sec-Vanilla-Manual.gif">
</p>

**(Option 2, Requires BLE Stack on Linux):** Connect to SecPump using the provided python script:

~~~bash
cd Scripts
python BlueCmd.py
~~~

Once the connection is established, the script allows us to change the operating mode and to program insulin injection steps.

~~~
[*] Scanning For Device
[*] Discovered device f8:1d:78:63:1d:b6
[*] Discovered device 65:55:21:14:e1:2b
[*] Discovered device 02:80:e1:00:34:12
Device 65:55:21:14:e1:2b (random), RSSI=-71 dB
  Flags = 1a
  Tx Power = 0c
  Manufacturer = 4c0010051118028ff6
Device f8:1d:78:63:1d:b6 (public), RSSI=-96 dB
  Flags = 05
  Complete 16b Services = 0000fff0-0000-1000-8000-00805f9b34fb,0000ffe5-0000-1000-8000-00805f9b34fb,0000ffe0-0000-1000-8000-00805f9b34fb
Device 02:80:e1:00:34:12 (public), RSSI=-71 dB
  Flags = 06
  Tx Power = fe
  Complete Local Name = SecPump
[+] Found Medical Pump
[*] Connecting to Device...
[*] Discovering Services...
Service <uuid=Generic Attribute handleStart=1 handleEnd=4>
Service <uuid=Generic Access handleStart=5 handleEnd=11>
Service <uuid=42821a40-e477-11e2-82d0-0002a5d5c51b handleStart=12 handleEnd=18>
[*] Accessing Pump Service
[*] Getting MODE characteristic
[*] Getting BOLUS characteristic

(1) MODE
(2) BOLUS
        Choice?1
Msg >1

(1) MODE
(2) BOLUS
        Choice?15
~~~

### Cyber-physical device research

The "Script" folder also contains a python file named "Model-Sim". This file allows us to make a simulation of the whole system (SecPump+Insulin kinetics regulation) without the STM32. This script allows for instance to adapt the PID coefficients of the pump without having to reflash the STM32 and re-setup the whole system.

## SecPump Security Demonstration

To replay the attack, you have to flash the STM32 with the vulnerable firmware. The latter suffers from an intentional buffer overflow in a new vulnerable characteristic called "VULNERABLE".

To perform the demo it is recommended to have two computers. One is connected to SecPump and displays the pump functioning while the other one has a native Linux with a function BLE stack to perform the remote attack.

While this repo provides the access to the vulnerable code, if you recompile the vulnerable firmware the provided attack won't probably work without adapting the provided exploit.

So, simply flash the STM32 with the pre-compiled provided vulnerable firmware, on the first computer. 

~~~ bash
cd SecPump-Vuln/Vulnerable-Firmware
./Flash_Vuln.sh
~~~

As the SecPump-Vanilla version, launch the simulation script.

~~~bash
cd Scripts
python Sec-Interface.py
~~~

While the pump is now regulating the blood glucose, it is time to launch the remote attack that exploits the buffer overflow vulnerability in the firmware.

On your second computer, disjoined from the main computer, launch the following command:

~~~
cd Scripts
python Exploit.py
~~~

The exploit script automatically finds the wireless pump and performs the payload injection on the vulnerable BLE characteristic:

<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/Xploit.gif">
</p>

After a couple of seconds, the amount of insulin injected by the pump should raise considerably being lethal for the patient:

<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/Sec-Vuln.gif">
</p>

## License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by   the Free Software Foundation, in version 3. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

## Attribution

If used for research, please cite the SecPump publication: https://ieeexplore.ieee.org/document/9031704

C. Bresch, D. Hély, S. Chollet, and R. Lysecky, *“SecPump: A Connected Open Source Infusion Pump for Security Research Purposes,”* IEEE Embedded Systems Letters, 2020.

~~~Latex
@article{bresch2020secpump,
  title={SecPump: A Connected Open Source Infusion Pump for Security Research Purposes},
  author={Bresch, Cyril and H{\'e}ly, David and Chollet, St{\'e}phanie and Lysecky, Roman},
  journal={IEEE Embedded Systems Letters},
  year={2020},
  publisher={IEEE}
}
~~~

## Acknowledgements

This work is carried out under the SERENE-IoT project, a project labeled within the framework of PENTA, the EUREKA Cluster for Application and Technology Research in Europe on NanoElectronics.

This work is supported by the French National Research Agency in the framework of the “investissement d’avenir” program (ANR-16-IDEX-02)

This research was partially supported by the National Science Foundation under Grant CNS-1615891.

## Contact

cyrbresch[at]gmail[dot]com
