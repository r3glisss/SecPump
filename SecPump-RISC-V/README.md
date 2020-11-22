# SecPump RISC-V :syringe:

## Get started
This sub-folder contains three folders: "Scripts" which contains the PC interface with the pump. "FPGA" which contains the prebuilt "bitstream" and "MCS" files for the Arty A7-35T Digilent FPGA. "Freedom-e-sdk" is a clone of the SiFive toolchain. It contains the source of the Pump in "software/secpump".

### Requirements
Read the documentation concerning the regular SecPump version first, read the documentation concerning the regular SecPump version first, be aware that you have been reminded twice to read the documentation.

To be able to run the SecPump model on the Arty A7-35T Digilent FPGA. You should:
- Build the bitstream of the RISC-V processor that may run on the FPGA. Alternatively, you can use the pre-build images in the "FPGA" folder
- Compile the SecPump software and flash the FPGA
- Launch the simulation script in the "Scripts" folder and observe SecPump in action on RISC-V

### Building the bitstream

SecPump is based on RocketChip, to build the core and run it on the FPGA, you should follow the regular flow provided by [SiFive](https://github.com/sifive/freedom). Also, the tutorial provided by [Digilent](https://github.com/sifive/freedom) might help to get you the proper environment and the bitstream/MCS file. Please read and understand both tutorials before doing anything. Keep in mind that to further flash a program on the FPGA you will also need an Olimex cable!

### Compiling the SecPump software and flashing the FPGA

Assuming you were able to flash the FPGA and properly connect the Olimex cable to the latter. To flash the SecPump software on the RISC-V board enter the following commands:

~~~bash
cd Freedom-e-sdk
# You can adjust the following command according to the version you want
make PROGRAM=secpump TARGET=sifive-hifive1 CONFIGURATION=release upload
~~~

The FPGA is supposed to be flashed with the SecPump software. You can check it through the serial interface:
~~~bash
screen /dev/ttyUSB1 57600 # check UART peripheral first
~~~

If everything went well you be able to observe the Firmware booting:
~~~bash
==== SECPUMP RISC-V INIT ====
~~~

At this point, the pump is waiting to be connected to the insulin kinetic Simulator (based on the extended Bergman's model). This simulator uses the UART to communicate with the standby pump. Therefore, the UART console must first be closed:
~~~bash
ctrl+a+k # screen command
~~~

### Running a simulation

**WARNING: Always reset the pump before running a simulation !** 

To connect the simulator with the pump everything operates as the ARM version of the system:
~~~bash
cd Scripts
python Sec-Interface-AutoSim.py
~~~

A 24-hour simulation is launched (displayed below). The diabetic blood glucose is modeled by the python script and the RISC-V-based pump regulates it with insulin using an integrated PID.

<p align="center">
    <img src="https://github.com/r3glisss/SecPump/blob/master/Img/Sec-Vanilla.gif">
</p>

### Todo: pushing the manual mode script
