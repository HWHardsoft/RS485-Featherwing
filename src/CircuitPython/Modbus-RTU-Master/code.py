#
# Modbus RTU Master (client) demo code for Zihatec RS485 Wing
#
#
# based on pycom library:
#
# This software is licensed under the GNU GPL version 3 or any
# later version, with permitted additional terms. For more information
# see the Pycom Licence v1.0 document supplied with this file, or
# available at https://www.pycom.io/opensource/licensing
#

import board
import busio
import time
from microcontroller import Pin
from uModBusSerial import uModBusSerial


output_value = 0xFF00

######################### RTU SERIAL MODBUS #########################
modbus_obj = uModBusSerial(baudrate=9600)
#if the serial modbus requires a ctrl pin, you can set it like this:
#modbus_obj = uModBusSerial(baudrate=9600, ctrl_pin=board.D22)

while True:
    try:
        # ######################## READ COILS #########################
        slave_addr = 0x01
        starting_address = 0x00
        coil_quantity = 8

        coil_status = modbus_obj.read_coils(slave_addr, starting_address, coil_quantity)
        print('Coil status: ' + ' '.join('{:d}'.format(x) for x in coil_status))


        # ###################### READ INPUT REGISTERS ##################
        #slave_addr=0x01
        #starting_address=0x10
        #register_quantity=10
        #signed=True

        #register_value = modbus_obj.read_input_registers(slave_addr, starting_address, register_quantity, signed)
        #print('Input register value: ' + ' '.join('{:d}'.format(x) for x in register_value))
    
    
        # ##################### READ DISCRETE INPUTS ##################
        #slave_addr=0x01
        #starting_address=0x0
        #input_quantity=10

        #input_status = modbus_obj.read_discrete_inputs(slave_addr, starting_address, input_quantity)
        #print('Input status: ' + ' '.join('{:d}'.format(x) for x in input_status))


        # ##################### READ HOLDING REGISTERS ##################
        # slave_addr=0x01 
        # starting_address=0x1
        # register_quantity=1
        # signed=True

        # holding_registers = modbus_obj.read_holding_registers(slave_addr, starting_address, register_quantity, signed)
        # print('Input status: ' + ' '.join('{:d}'.format(x) for x in holding_registers))


        # ##################### WRITE SINGLE REGISTER ##################
        #slave_addr = 0x01
        #register_address = 18
        #register_value = 55
        #signed = True

        #return_flag = modbus_obj.write_single_register(slave_addr, register_address, register_value, signed)
        #output_flag = 'Success' if return_flag else 'Failure'
        #print('Writing single coil status: ' + output_flag)


        # ##################### WRITE MULIPLE COILS ##################
        #slave_addr=0x01
        #starting_address=0x10
        #output_values=[1,1,1,0,0,1,1,1,0,0,1,1,1]

        #return_flag = modbus_obj.write_multiple_coils(slave_addr, starting_address, output_values)
        #output_flag = 'Success' if return_flag else 'Failure'
        #print('Writing multiple coil status: ' + output_flag)


        # ##################### WRITE SINGLE COIL ##################
        slave_addr=0x01
        output_address=0x00
        if output_value == 0xFF00:
            output_value = 0
        else:
            output_value = 0xFF00

        return_flag = modbus_obj.write_single_coil(slave_addr, output_address, output_value)
        output_flag = 'Success' if return_flag else 'Failure'
        print('Writing single coil status: ' + output_flag)


        # ##################### WRITE MULIPLE COILS ##################
        #slave_addr=0x0A
        #starting_address=0x00
        #output_values=[1,1,1,0,0,1,1,1,0,0,1,1,1]

        #return_flag = modbus_obj.write_multiple_coils(slave_addr, starting_address, output_values)
        #output_flag = 'Success' if return_flag else 'Failure'
        #print('Writing multiple coil status: ' + output_flag)

    except KeyboardInterrupt:
        print("Programm beendet")
        break
    except Exception as e:
        print("Error:", e)
        time.sleep(1)