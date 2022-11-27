## QP-Tools-Tests

### Configuration

Dowload QPC bundle from https://www.state-machine.com/ (This will include QP/C, QP/C++ and QTools). Or you can download separatadely from github https://github.com/QuantumLeaps


After that include QPC path to environment: ```export QPC="/home/user/qp-linux_7.1.0/qp/qpc```.
Also add QTOOLS as follows: ```export QTOOLS="/home/user/qp-linux_7.1.0/qp/qtools``` and ```export PATH="/home/user/qp-linux_7.1.0/qp/qtools/bin:$PATH"```

Lastly, it necessary to build qspy tools into qp-linux_7.1.0/qp/qtools/bin folder.
* ```cd ~/qp-linux_7.1.0/qp/qtools/qspy/posix/```
* ```make```

This will automatically build qspy into qp/qtools/bin folder.

The last tool needed is qview. You can install via ```pip install qview```, or you can use as a standalone file that is located ```cd ~/qp-linux_7.1.0/qp/qtools/qview/```.

Qview requires python tkinter: ```sudo apt-get install py


## Extras
sudo apt-get install python3-pil python3-pil.imagetk
pip install playsound
pip install pynput
pip install inputs
pip install bluezero


## Env Vars
export ARM_GCC_PATH="/home/marco/gcc-arm-none-eabi-10.3-2021.10/bin"
export CUBE_PATH="/home/marco/STM32CubeMX"
export CUBE_PROGRAMMER_PATH="/home/marco/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin"
export QPC="/home/marco/qp-linux_7.1.0/qp/qpc"
export QTOOLS="/home/marco/qp-linux_7.1.0/qp/qtools"


# Cube configs
Remove the following interrupts from auto code generate
PendSV_Handler
NMI_Handler
SysTickhandler

# QM License
The state machine was built using QMActive scheme. So it is necessary to use a paid license. In this case we are using the free trial license locatede on qp-linux_7.0.2/qp/qpc/LICENSES/QM-EVAL-QPC.qlc.

More information on: https://state-machine.com/qm/sm.html#sm_class
