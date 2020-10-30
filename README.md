
# SecPump :syringe: 

Welcome to SecPump!

SecPump is an open wireless insulin pump system workbench that models the insulin kinetics based on the modified Bergman's minimal model. The system workbench is primarily tailored for security assessments and countermeasures development against the numerous security flaws related to wearable medical devices. The platform is open-source and free from commercials constraints making it a suitable target to perform both hardware/software security demonstrations. SecPump aims at being straightforward to use, it only requires an inexpensive Nucleo F401-RE STM32 board with the BlueNRG extension.

<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/VSPM.PNG">
</p>

## Context

SecPump is a research project designed in collaboration with the [LCIS](https://lcis.grenoble-inp.fr/le-laboratoire), the [University of Arizona](https://www.arizona.edu/), and the [SERENE IoT project](http://serene.minalogic.net/). SERENE IoT aims at contributing to developing high quality connected care services and diagnostic tools based on Advanced Smart Health-Care IoT Devices. This platform intends to provide a framework for security evaluation, tailored for countermeasures development against the numerous security flaws related to medical devices. 

## Why an open-platform?

Wireless insulin pumps are considered life critical embedded systems. While these devices are promising innovation, there are known to be poor in security and vulnerable to various cyber-attacks [1]. Unfortunately, due to the lack of openess to research, and commercial constraints, security researchers cannot freely demonstrate security issues on these devices. Besides, the security countermeasures assessments can be difficult without an open-model. Here comes the SecPump! The repo provides: 

- An abstract representation of a wireless wearable insulin pump that models the blood glucose regulation within the human body. The open source code base of the pump can be extended, and improved for various demonstrations including cyber-physical system design, automation (PID regulation),security, and safety. 

- A software security demonstration based on advanced memory corruption exploitation. This demonstration exploits an intentional stack-based buffer overflow bug introduced in a vulnerable BLE characteristic. The bug allow an attacker to maliciously modify the functionning of the cyber-physical system at run-time. Such attack here raise the amount of insulin injected over the time.

## Get started



## Acknowledgements

This work is carried out under the SERENE-IoT project, a project labelled within the framework of PENTA, the EUREKA cluster for Application and Technology Research in Europe on NanoElectronics.

This research was partially supported by the National Science Foundation under Grant CNS-1615890.


