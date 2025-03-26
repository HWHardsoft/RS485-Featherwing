
//===================================================================================//
/**
 * ModbusRTU_Server.ino
 * 
 * demo for RS485 Featherwing by Zihatec, 2025 
 *
 * The Modbus server can be addressed from a PC via a Modbus simulator and USB-RS485 adapter, 
 * for example. The LED on the Feather microcontroller board can be controlled via COIL register 0. 
 * The LED status and the count of an internal counter can be retrieved by querying the 
 * holding register....
 *
 * This code was tested with the following Featherboards:
 *  Sparkfun ThingPlus 32 C
 *  Sparkfun ThingPlus RS2040
 *  Adafruit Feather Huzzah32
 *  Adafruit Feather M0 Express
 *  Adafruit Feather ESP32-S3
 *
 * watch this video for demonstration: https://www.youtube.com/shorts/rvGP3tobp28
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

const int ledPin = LED_BUILTIN;
int counter = 0;

// Declare the RS485 interface here with a hardware serial port.
// auto send/receive: Dip Switch  S1: ON - OFF - OFF - ON
RS485Class RS485 (Serial1, -1, -1, -1); // (Serial Port, DE, RE, TX) 

// Create a Modbus RTU node instance with the RS485 interface.
CSE_ModbusRTU modbusRTU (&RS485, 0x01, "modbusRTU-0x01"); // (RS-485 Port, Device Address, Device Name)

// Create a Modbus RTU server instance with the Modbus RTU node.
CSE_ModbusRTU_Server modbusRTUServer (modbusRTU, "modbusRTUServer"); // (CSE_ModbusRTU, Server Name)

//===================================================================================//

void setup() {
  // Initialize the default serial port for debug messages
  Serial.begin (115200);
  delay (1000);
  Serial.println ("CSE_ModbusRTU - Modbus RTU Server LED");

  // Initialize the RS485 interface. If you are initializing the RS485 interface
  // manually, then the parameter can be empty.
  RS485.begin (9600,SERIAL_8N1);

  // Initialize the Modbus RTU server
  modbusRTUServer.begin();

  // Configure the LED
  pinMode (ledPin, OUTPUT);
  digitalWrite (ledPin, LOW);


  // Configure four coils starting at address 0x00
  // build-in LED is coil 0x00
   modbusRTUServer.configureCoils (0x00, 4);

  // Configure four holding registers
  // status of build-in LED is holding register  0x00 
  // status of counter is holding register  0x01
   modbusRTUServer.configureHoldingRegisters (0x00, 4);

}

//===================================================================================//

void loop() {
  // increment counter
  counter++;

  // holding registers
  // save counter value
  modbusRTUServer.writeHoldingRegister(0x01,counter);
  
  // Poll for Modbus RTU requests
  int requestReceived = modbusRTUServer.poll(); 

  // coils
  if ((requestReceived != -1) && (requestReceived < 0x80)) {
    Serial.println ("Request received");

    // Read the current value of the coil
    int coilValue = modbusRTUServer.readCoil (0x00);
    modbusRTUServer.writeHoldingRegister(0x00,coilValue); // LED status in holding register

    // set led according coilValue  
    if (coilValue == 1) {
      // Coil value set, turn LED on
      digitalWrite (ledPin, HIGH);
      
    } else if (coilValue == 0) {
      // Coil value clear, turn LED off
      digitalWrite (ledPin, LOW);
    } else {
      Serial.println ("Error reading coil");
    }  
      
  }
}

//===================================================================================//
