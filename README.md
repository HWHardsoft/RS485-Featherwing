# RS485-Featherwing
Here you will find various demo programs in C++ (Arduino) or Python for our [RS422 / RS485 Feather Wing](https://www.hwhardsoft.de/english/projects/rs485-featherwing/)

![RS485 Featherwing](https://cdn.hackaday.io/images/2570431742991238794.jpg)

A detailed [tutorial](https://www.hackster.io/hartmut-wendt/modbus-rtu-with-feather-boards-56500f) on using the RS485 Wing for Modbus can be found at Hackster.io.


## Arduino MODBUS RTU

These examples use the libraries:
- [CSE_ArduinoRS485](https://github.com/CIRCUITSTATE/CSE_ArduinoRS485)
- [CSE-ModbusRTU](https://github.com/CIRCUITSTATE/CSE_ModbusRTU)

from Vishnu Mohanan (CIRCUITSTATE Electronics LLP).




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
