# RISCV port

This port has been design to be run from a linux operating system Ubuntu 16-LTS. The RISC-V port of the SecPump is based on the SiFive's E300 Freedom platform. In order to setup the plateform, you first need to carefully follow the [Freedom E300 Arty FPGA Dev Kit](https://github.com/sifive/freedom) installation guide. 

Once installed and the bitstream generated for the Arty 7 FPGA, you need to build the SecPump application.Assuming the RISC-V compiler is in your path:
~~~
make clean
make
~~~

Congrat's you can now upload the software on the Arty Board. 
