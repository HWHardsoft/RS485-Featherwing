
//===================================================================================//
/**
 * ModbusRTU_Client.ino
 * 
 * demo for RS485 Featherwing by Zihatec, 2025 
 *
 * This example demonstrates the use of a Modbus RTU client to control the outputs 
 * (coil registers) of an Waveshare RTU relay unit
 *
 * This code was tested with the following Featherboards:
 *  Sparkfun ThingPlus 32 C
 *  Sparkfun ThingPlus RS2040
 *  Adafruit Feather Huzzah32
 *  Adafruit Feather M0 Express
 *  Adafruit Feather ESP32-S3
 *
 * watch this video for demonstration: https://www.youtube.com/shorts/1c11LD4pH0A
 * 
 * Original code by Vishnu Mohanan (@vishnumaiea)
 * GitHub Repository: https://github.com/CIRCUITSTATE/CSE_ModbusRTU
 * 
 * 
 */
//===================================================================================//

#include <CSE_ArduinoRS485.h>
#include <CSE_ModbusRTU.h>

//===================================================================================//


#define PORT_RS485          Serial1 // The hardware serial port for the RS-485 interface

//===================================================================================//

// Declare the RS485 interface here with a hardware serial port.
// auto send/receive: Dip Switch  S1: ON - OFF - OFF - ON
RS485Class RS485 (Serial1, -1, -1, -1); // (Serial Port, DE, RE, TX)

// Create a Modbus RTU node instance with the RS485 interface.
CSE_ModbusRTU modbusRTU (&RS485, 0x02, "Waveshare RTU Relay"); // (RS-485 Port, Device Address, Device Name)

// Create a Modbus RTU server instance with the Modbus RTU node.
CSE_ModbusRTU_Client modbusRTUClient (modbusRTU, "modbusRTUClient"); // (CSE_ModbusRTU, Client Name)

//===================================================================================//

void setup() {
  // Initialize the default serial port for debug messages
  Serial.begin (115200);
  delay (1000);
  Serial.println ("CSE_ModbusRTU - Modbus RTU Client LED");


  // Initialize the RS485 interface. If you are initializing the RS485 interface
  // manually, then the parameter can be empty.
  RS485.begin(9600, SERIAL_8N1);

  // Initialize the Modbus RTU client.
  modbusRTUClient.begin();
  modbusRTUClient.setServerAddress (0x01); // Set the server address to 0x01
}

//===================================================================================//

void loop() {
  // turn all 8 relays ON
  for(int i1 = 0; i1 < 6; i1++){
    Serial.print("Turning Relay "); Serial.print(i1);

    if (modbusRTUClient.writeCoil (i1, 0xFF00) == -1) { // Turn the Relay on
      Serial.println (" on failed.");
    } else {
      Serial.println (" on success.");
    }
    delay (200);

  }

 // turn all 8 relays OFF
  for(int i1 = 0; i1 < 6; i1++){
    Serial.print("Turning Relay "); Serial.print(i1);

    if (modbusRTUClient.writeCoil (i1, 0x0000) == -1) { // Turn the Relay off
      Serial.println (" off failed.");
    } else {
      Serial.println (" off success.");
    }
    delay (200);

  }

}

//===================================================================================//
