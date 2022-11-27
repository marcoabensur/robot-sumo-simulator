# Robot-Sumo-Simulator

This is a robot-sumo C program that runs both on a target hardware and on a PC machine. When running on a PC, we use a Python program to send inputs (e.g distance sensors) and receive outputs (e.g Motor data) from the C code.

The simulator have the following functionalities: distance sensor simulation, line sensor simulation, motor simulation, bluetooth simulation, start module simulation, buzzer simulation, button and led simulaiton, and RF radio simulation.

The code was based on QP™ Real-Time Embedded Frameworks (RTEFs) from https://www.state-machine.com/. In this makefile we give a rough view of the usage. But for a complete understanding it is recomended to read the official docs and [videos](https://www.youtube.com/@StateMachineCOM)

![](https://media.giphy.com/media/wbGX082h14XFiaeMeW/giphy.gif)

## Table of contents
* [Requirements](#requirements)
* [Getting Started](#getting-started)

## Requirements

The progrram was devolped and tested on Linux. All the used tools are also available for windows, but those were not tested, so some problems may appear on the first configuration.

Used tools:
 * [GNU Make](https://www.gnu.org/software/make/) for automated building. No particular version recommended.
 * [GNU ARM Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm) for building C source code for the STM32 hardware platform. Version 10-2020-q4-major used.
 * [Python 3](https://www.python.org/downloads/)
 * [QPC Bundle](https://www.state-machine.com/). This project was developed on version 7.1.2, so this is the recommended version. This will install QP/C, QM and Qview.


## PC configuration

Dowload QPC bundle from https://www.state-machine.com/ (This will include QP/C, QP/C++ and QTools). Or download separatadely from github https://github.com/QuantumLeaps

After that include QPC downloaded path to the environment variables editing ~/.bashrc file: 
```
export QPC="/home/user/qp-linux_7.1.2/qp/qpc
```

Also add QTOOLS as follows: 
```
export QTOOLS="/home/user/qp-linux_7.1.2/qp/qtools
```
```
export PATH="/home/user/qp-linux_7.1.2/qp/qtools/bin:$PATH"
```

Finally, it is necessary to build qspy tools into /home/user/qp-linux_7.1.2/qp/qtools/bin folder.
```
$ cd ~/qp-linux_7.1.2/qp/qtools/qspy/posix/
$ make
```

This will automatically build qspy into qp/qtools/bin folder.

The last tool needed is qview. You can install via ```pip install qview```, or use as a standalone file that is located ```cd ~/qp-linux_7.1.2/qp/qtools/qview/```.

### Qspy
The installed qspy tool, is used as a server to join a C program and a Python program.

### Qview
The installed qview tool, is used as a front-end aplication to vizualize commands received on Qspy. Also it can send command to qspy.

### Environemnt variables
Add the following environment variables to the ~/.bashrc file
```
export QPC="/home/user/qp-linux_7.1.2/qp/qpc"
export QTOOLS="/home/user/qp-linux_7.1.2/qp/qtools"
```
### Python libs
```
pip install tk
sudo apt-get install python3-pil python3-pil.imagetk
pip install pynput
pip install inputs
pip install bluezero

```

### Compilinig the code
To compile the code to run on a PC simply urn the following command:
```
make CONF=spy
```

this will build the binaries on build_spy folder.


### Running
With all the configuration done, it should be possible to run the project on a PC. 
* Open 3 terminals
* Run qspy command on one terminal
* Run the simulator on another terminal using:
```
python3 ./qview/simulator_base.py
```
* Run the C code that will connect to the simulator on another terminal:
```
./build_spy/sumo_hsm
```


## STM32 specific configuration
This project was made to work on a STM32 target as well as on a PC. So the folder structure and makefile are designes to use with STM32 microcontrollers. But one can adapt to any microcontroller.

Here I will discuss STM32 specific details.

### Environemnt variables


If the project will also be used on STM microcontrollers, also add:
```
export ARM_GCC_PATH="/home/user/gcc-arm-none-eabi-10.3-2021.10/bin"
export CUBE_PATH="/home/user/STM32CubeMX"
export CUBE_PROGRAMMER_PATH="/home/user/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin"
```
Where "CUBE_PATH" is the installed location of STM32CUbeMX tool, and "CUBE_PROGRAMMER_PATH" the cube programmer path. Those are used to flash the program, and set some configs.


### Cube configs
When buiding CubeMX projects, remove the following interrupts from auto code generate ```PendSV_Handler```, ```NMI_Handler```, ```SysTickhandler```

### From Cube to Compiled Code
On "./cube" folder we have the .ioc project. There are two folders, because this project was made for two microcontrollers. One can use those examples to change the microcontroller for a specific need.

In each one of those folders, there is the .ioc CubeMX file and all auto generated cube code.

After edditing some .ioc file with CubeMX and cliking on "generate code", run the command ```make prepare``` on the root of this directory to place everything on its correct location.

Note that the makefiles includes a "config.mk" file. It is necessary to edit this file when changing the microcontroller.

After that, we can already compile the code with make command on the root of the repository:

```
$ make
```

This will generate a compiled .elf and .hex file on "build" folder. This code can be flashed on a microcontroller using:
```
$ make flash
```


## State Machine And Code generation


## Bluetooth Simulation

# QM License
The state machine was built using QMActive scheme. So it is necessary to use a paid license when developing closed-source projects. In this case we are using the free trial license located on qp-linux_7.1.2/qp/qpc/LICENSES/QM-EVAL-QPC.qlc.

On the first time running the project, it will be necessary to update the license path on the user computer. It is possible to change on the qm interface, or by edintig line 4 on ```sumo_hsm.qm``` file:
```
<framework name="qpc" license="../../../../qp-linux_7.1.2/qp/qpc/LICENSES/QM-EVAL-QPC.qlc"/>
```

More information on: https://state-machine.com/qm/sm.html#sm_class
