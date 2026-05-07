# RS485-Featherwing
Here you will find various demo programs in C++ (Arduino) or Python for our [RS422 / RS485 Feather Wing](https://www.hwhardsoft.de/english/projects/rs485-featherwing/)

![RS485 Featherwing](https://cdn.hackaday.io/images/2570431742991238794.jpg)

A detailed [tutorial](https://www.hackster.io/hartmut-wendt/modbus-rtu-with-feather-boards-56500f) on using the RS485 Wing for Modbus can be found at Hackster.io.


## Arduino MODBUS RTU

Since Arduino's own MODBUS libraries do not support either the ESP32 or the RS485 Wing, I tested various other libraries and came across the [ModbusRTUMaster](https://github.com/CMB27/ModbusRTUMaster) and [ModbusRTUSlave](https://github.com/CMB27/ModbusRTUSlave) libraries by C. M. Bulliner. Both libraries can be installed directly via the Arduino IDE. Please install with all dependencies. 

![Library installation](https://github.com/HWHardsoft/MODBUS-RTU-with-Arduino-RS485-shield/blob/main/assets/install_cbs_modbus_libs.png)

I have provided three code examples for MODBUS RTU. For the two programmes, ModbusRTU_Master and ModbusRTU_Slave, a pushbutton must be connected between pin 12 and GND in each case. You can then have two boards communicate with each other. One acts as the slave (server) and the other as the master (client). If you press the pushbutton on one board, the built-in LED on the other Feather board lights up.

The third example programme shows how to control an external slave (in this case, a Waveshare Modbus RTU relay) using a Feather board as the master.  


## Circuit Python MODBUS RTU
Please copy the files from the lib folder to the lib subfolder on your CircuitPython device. Replace the code.py file in the CircuitPython device with the new code.py. 

(based on pyCom Modbus lib) 


## Micro Python MODBUS RTU
Please copy the files from the uModBus folder to the uModBus subfolder on your MicroPython device. Copy the main.py file in the directory of your device. 

(based on pyCom Modbus lib) 



## License

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
