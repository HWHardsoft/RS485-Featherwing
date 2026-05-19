#
# Modbus RTU Slave (server) micropython demo code for Zihatec RS485 Wing
#
# by Zihatec GmbH, 2026
#
 

from machine import UART
import time

# Configure UART for MODBUS RTU
# uart = UART(0, baudrate=9600)  	# Sparkfun Thing Plus RP2040
uart = UART(2, baudrate=9600)		# Sparkfun Thing Plus ESP32 Wroom / Huzzah 32



# MODBUS Slave Configuration
SLAVE_ADDRESS = 1

# Register Memory
coils = [False] * 100           # Coils (0-99)
discrete_inputs = [False] * 100  # Discrete Inputs (0-99)
holding_registers = [0] * 100    # Holding Registers (0-99)
input_registers = [0] * 100      # Input Registers (0-99)

def calculate_crc(data):
    """Calculate CRC16 for MODBUS RTU"""
    crc = 0xFFFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 0x0001:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc

def verify_crc(data):
    """Verify CRC of received message"""
    if len(data) < 4:
        return False
    received_crc = data[-2] | (data[-1] << 8)
    calculated_crc = calculate_crc(data[:-2])
    return received_crc == calculated_crc

def send_response(response_data):
    """Send response with CRC"""
    crc = calculate_crc(response_data)
    response_data.append(crc & 0xFF)
    response_data.append((crc >> 8) & 0xFF)
    uart.write(bytes(response_data))
    print(f"Response: {[hex(b) for b in response_data]}")

def send_exception(slave_addr, function_code, exception_code):
    """Send MODBUS Exception"""
    response = bytearray([
        slave_addr,
        function_code | 0x80,
        exception_code
    ])
    send_response(response)

def handle_read_coils(slave_addr, data):
    """Function Code 01: Read Coils"""
    if len(data) < 8:
        return
    
    start_addr = (data[2] << 8) | data[3]
    quantity = (data[4] << 8) | data[5]
    
    if quantity < 1 or quantity > 2000 or start_addr + quantity > len(coils):
        send_exception(slave_addr, 0x01, 0x02)  # Illegal data address
        return
    
    # Pack coils into bytes
    byte_count = (quantity + 7) // 8
    coil_bytes = []
    
    for i in range(byte_count):
        byte_val = 0
        for bit in range(8):
            coil_idx = start_addr + i * 8 + bit
            if coil_idx < start_addr + quantity:
                if coils[coil_idx]:
                    byte_val |= (1 << bit)
        coil_bytes.append(byte_val)
    
    response = bytearray([slave_addr, 0x01, byte_count] + coil_bytes)
    send_response(response)

def handle_read_discrete_inputs(slave_addr, data):
    """Function Code 02: Read Discrete Inputs"""
    if len(data) < 8:
        return
    
    start_addr = (data[2] << 8) | data[3]
    quantity = (data[4] << 8) | data[5]
    
    if quantity < 1 or quantity > 2000 or start_addr + quantity > len(discrete_inputs):
        send_exception(slave_addr, 0x02, 0x02)
        return
    
    byte_count = (quantity + 7) // 8
    input_bytes = []
    
    for i in range(byte_count):
        byte_val = 0
        for bit in range(8):
            input_idx = start_addr + i * 8 + bit
            if input_idx < start_addr + quantity:
                if discrete_inputs[input_idx]:
                    byte_val |= (1 << bit)
        input_bytes.append(byte_val)
    
    response = bytearray([slave_addr, 0x02, byte_count] + input_bytes)
    send_response(response)

def handle_read_holding_registers(slave_addr, data):
    """Function Code 03: Read Holding Registers"""
    if len(data) < 8:
        return
    
    start_addr = (data[2] << 8) | data[3]
    quantity = (data[4] << 8) | data[5]
    
    if quantity < 1 or quantity > 125 or start_addr + quantity > len(holding_registers):
        send_exception(slave_addr, 0x03, 0x02)
        return
    
    byte_count = quantity * 2
    register_bytes = []
    
    for i in range(quantity):
        reg_val = holding_registers[start_addr + i]
        register_bytes.append((reg_val >> 8) & 0xFF)
        register_bytes.append(reg_val & 0xFF)
    
    response = bytearray([slave_addr, 0x03, byte_count] + register_bytes)
    send_response(response)

def handle_read_input_registers(slave_addr, data):
    """Function Code 04: Read Input Registers"""
    if len(data) < 8:
        return
    
    start_addr = (data[2] << 8) | data[3]
    quantity = (data[4] << 8) | data[5]
    
    if quantity < 1 or quantity > 125 or start_addr + quantity > len(input_registers):
        send_exception(slave_addr, 0x04, 0x02)
        return
    
    byte_count = quantity * 2
    register_bytes = []
    
    for i in range(quantity):
        reg_val = input_registers[start_addr + i]
        register_bytes.append((reg_val >> 8) & 0xFF)
        register_bytes.append(reg_val & 0xFF)
    
    response = bytearray([slave_addr, 0x04, byte_count] + register_bytes)
    send_response(response)

def handle_write_single_coil(slave_addr, data):
    """Function Code 05: Write Single Coil"""
    if len(data) < 8:
        return
    
    coil_addr = (data[2] << 8) | data[3]
    coil_value = (data[4] << 8) | data[5]
    
    if coil_addr >= len(coils):
        send_exception(slave_addr, 0x05, 0x02)
        return
    
    if coil_value == 0xFF00:
        coils[coil_addr] = True
        print(f"Coil {coil_addr} = ON")
    elif coil_value == 0x0000:
        coils[coil_addr] = False
        print(f"Coil {coil_addr} = OFF")
    else:
        send_exception(slave_addr, 0x05, 0x03)  # Illegal data value
        return
    
    # Echo back
    response = bytearray(data[:-2])
    send_response(response)

def handle_write_single_register(slave_addr, data):
    """Function Code 06: Write Single Register"""
    if len(data) < 8:
        return
    
    reg_addr = (data[2] << 8) | data[3]
    reg_value = (data[4] << 8) | data[5]
    
    if reg_addr >= len(holding_registers):
        send_exception(slave_addr, 0x06, 0x02)
        return
    
    holding_registers[reg_addr] = reg_value
    print(f"Holding Register {reg_addr} = {reg_value}")
    
    # Echo back
    response = bytearray(data[:-2])
    send_response(response)

def handle_write_multiple_coils(slave_addr, data):
    """Function Code 15: Write Multiple Coils"""
    if len(data) < 10:
        return
    
    start_addr = (data[2] << 8) | data[3]
    quantity = (data[4] << 8) | data[5]
    byte_count = data[6]
    
    if start_addr + quantity > len(coils):
        send_exception(slave_addr, 0x0F, 0x02)
        return
    
    for i in range(quantity):
        byte_idx = 7 + (i // 8)
        bit_idx = i % 8
        if byte_idx < len(data) - 2:
            coils[start_addr + i] = bool(data[byte_idx] & (1 << bit_idx))
    
    print(f"Coils {start_addr}-{start_addr+quantity-1} written")
    
    response = bytearray([slave_addr, 0x0F, data[2], data[3], data[4], data[5]])
    send_response(response)

def handle_write_multiple_registers(slave_addr, data):
    """Function Code 16: Write Multiple Registers"""
    if len(data) < 11:
        return
    
    start_addr = (data[2] << 8) | data[3]
    quantity = (data[4] << 8) | data[5]
    byte_count = data[6]
    
    if start_addr + quantity > len(holding_registers):
        send_exception(slave_addr, 0x10, 0x02)
        return
    
    for i in range(quantity):
        high_byte = data[7 + i * 2]
        low_byte = data[8 + i * 2]
        holding_registers[start_addr + i] = (high_byte << 8) | low_byte
    
    print(f"Holding Registers {start_addr}-{start_addr+quantity-1} written")
    
    response = bytearray([slave_addr, 0x10, data[2], data[3], data[4], data[5]])
    send_response(response)

def process_modbus_request(data):
    """Process incoming MODBUS request"""
    if len(data) < 4:
        return
    
    if not verify_crc(data):
        print("CRC error - request ignored")
        return
    
    slave_addr = data[0]
    
    # Only respond to own address
    if slave_addr != SLAVE_ADDRESS:
        return
    
    function_code = data[1]
    print(f"\nRequest received: Slave={slave_addr}, FC={hex(function_code)}")
    
    # Process function code
    if function_code == 0x01:
        handle_read_coils(slave_addr, data)
    elif function_code == 0x02:
        handle_read_discrete_inputs(slave_addr, data)
    elif function_code == 0x03:
        handle_read_holding_registers(slave_addr, data)
    elif function_code == 0x04:
        handle_read_input_registers(slave_addr, data)
    elif function_code == 0x05:
        handle_write_single_coil(slave_addr, data)
    elif function_code == 0x06:
        handle_write_single_register(slave_addr, data)
    elif function_code == 0x0F:
        handle_write_multiple_coils(slave_addr, data)
    elif function_code == 0x10:
        handle_write_multiple_registers(slave_addr, data)
    else:
        send_exception(slave_addr, function_code, 0x01)  # Illegal function

# Initialization - set example values
print("MODBUS RTU Slave - Address 1")
print("-" * 40)

# Example values
holding_registers[0] = 1234
holding_registers[1] = 5678
holding_registers[2] = 100
input_registers[0] = 42
input_registers[1] = 99
discrete_inputs[0] = True
discrete_inputs[1] = True

print("Initialized:")
print(f"  Holding Reg 0-2: {holding_registers[0:3]}")
print(f"  Input Reg 0-1: {input_registers[0:2]}")
print(f"  Discrete Inputs 0-1: {discrete_inputs[0:2]}")
print("\nWaiting for requests...\n")

# Main loop
counter = 0
while True:
    # Read UART data
    data = uart.read(256)
    
    
    if data:
        process_modbus_request(bytearray(data))
    
    # Simulate sensor data (optional)
    counter += 1
    if counter >= 1000:
        counter = 0
        input_registers[0] = (input_registers[0] + 1) % 100
    
    time.sleep(0.50)