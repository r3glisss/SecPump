# The SecPump Project 

Welcome to the SecPump project!
Documentation is under construction, however all applications are already deployable and functional. 

<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/VSPM.PNG">
</p>

## Context

The secPump is an open platform modelling a simple infusion pump using Bluetooth Low Energy technology. The SecPump has been designed within the [Serene IoT project](http://serene.minalogic.net/) which aims at contributing to develop high quality connected care services and diagnosis tools based on Advanced Smart Health-Care IoT Devices.  This platform intends to provide a framework for security evaluation, tailored for countermeasures development against the numerous security flaws related to medical devices. 

## Why an open platform ?

Medical infusion pumps are considered as life critical embedded systems. While these devices are promising in the medical area, there are known to be poor in security and strongly vulnerable to many attack vectors. Most of the time basic features such as integrity, authenticity, privacy and safety are not respected. If such a lack of security exists it is due to the difficulty to find an open source model of such a critical system. In addition, the evaluation of security features in these devices can be really difficult to expose and evaluate without a good demonstrator. Here comes the VSPM! The objectives of our platform are threefold: 

- Establishing and providing a set of documents of good security/safety practices for the development of a life critical application. Our software architecture document is also open source and provides all the details related to the medical pump.

- Providing an abstract representation of the software used in a typical infusion Pump. This open source base can be extended and improved for specific implementation/demonstration. 

- Providing an open-source medical platform that can be used to demonstrate and expose the exploitation of hardware and software vulnerabilities without attacking a real device. For example, at the hardware level, our technology uses encryption algorithms such as AES. It is therefore possible to carry out side channel attacks to leak cryptographic keys.  

## How to get started ?

As explain above the VSPM is a complete open source cyber physical system that models the dosage and performance of a drug administered to a patient in real time. To start playing with the application may be aware of the architecture of this repo:

- The Docs folder contains all the specifications of the demonstrator. You may also find a complete security analysis of the demonstrator using the eBIOS method. (ANSII)
- The Model folder contains the implementation of the embedded application for both RISCV and ARM architecture. You may also find a way to launch the VPSM interface on your host computer. Obviously, the subfolders provide some README to guide in the installation of the demonstrator.

## Acknoledgements

This work is carried out under the SERENE-IoT project, a project labelled within the framework of PENTA, the EUREKA cluster for Application and Technology Research in Europe on NanoElectronics.

This research was partially supported by the National Science Foundation under Grant CNS-1615890.
