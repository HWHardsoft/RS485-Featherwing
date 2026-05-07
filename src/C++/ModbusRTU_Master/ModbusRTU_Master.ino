/**
 * ModbusRTU_Master.ino
 * 
 * Modbus master (client) demo for RS485 Feather Wing by Zihatec, 2026 
 *
 * This example demonstrates the use of a Modbus RTU master to control the outputs  
 * (coil registers) of an Modbus slave and read the Holding registers of this slave to control an LED 
 *
 * DIP switch setting:
 *  S1: OFF - ON - ON - OFF
 *  S2: ON
 *  S4: ON - ON - ON - ON
 * 
 * 
 * This example demonstrates  use the ModbusRTUMaster library (https://github.com/CMB27/ModbusRTUMaster) by  C. M. Bulliner
 * 
 * 
 */ 

#include <ModbusRTUMaster.h>
#include "boards.h"

// You can change the baud, config, and unit id values if you like.
// Just make sure they match the settings you use in ModbusRTUSlaveExample.
// Note, the config value will be ignored when using SoftwareSerial.
// SoftwareSerial only supports SERIAL_8N1.
#define MODBUS_BAUD 9600
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_UNIT_ID 1


#define LED_PIN LED_BUILTIN
#define BUTTON_PIN 12
//#define BUTTON_PIN 18  // Thing Plus RP2040

ModbusRTUMaster modbus(MODBUS_SERIAL, -1);

const uint8_t numCoils = 2;
const uint8_t numDiscreteInputs = 2;
const uint8_t numHoldingRegisters = 2;
const uint8_t numInputRegisters = 2;

bool coils[numCoils];
bool discreteInputs[numDiscreteInputs];
uint16_t holdingRegisters[numHoldingRegisters];
uint16_t inputRegisters[numInputRegisters];

unsigned long transactionCounter = 0;
unsigned long errorCounter = 0;

const char* errorStrings[] = {
  "success",
  "invalid id",
  "invalid buffer",
  "invalid quantity",
  "response timeout",
  "frame error",
  "crc error",
  "unknown comm error",
  "unexpected id",
  "exception response",
  "unexpected function code",
  "unexpected response length",
  "unexpected byte count",
  "unexpected address",
  "unexpected value",
  "unexpected quantity"
};



void setup() {
  // define gpio pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
 

  // serial interface for debugging 
  #if !defined(NO_DEBUG_SERIAL) 
  Serial.begin(115200);
  #endif


  // serial interface for MODBUS
  #ifndef SOFTWARE_SERIAL
    MODBUS_SERIAL.begin(MODBUS_BAUD, MODBUS_CONFIG);
    modbus.begin(MODBUS_BAUD, MODBUS_CONFIG);
  #else
    MODBUS_SERIAL.begin(MODBUS_BAUD);
    modbus.begin(MODBUS_BAUD);
  #endif

}

void loop() {

  // set coil 0 according status of button on master
  coils[0] = !digitalRead(BUTTON_PIN);
  
  uint8_t error;

  //error = modbus.writeMultipleHoldingRegisters(MODBUS_UNIT_ID, 0, holdingRegisters, numHoldingRegisters);
  //printLog(MODBUS_UNIT_ID, 16, 0, numHoldingRegisters, error);
  
  
  error = modbus.readHoldingRegisters(MODBUS_UNIT_ID, 0, holdingRegisters, numHoldingRegisters);
  printLog(MODBUS_UNIT_ID, 16, 0, numHoldingRegisters, error);

  // show holding register of slave
  for(int i1 = 0; i1 <  numHoldingRegisters; i1++) {
    #if !defined(NO_DEBUG_SERIAL)
    Serial.print("Holding Register "); 
    Serial.print(i1);
    Serial.print(": ");
    Serial.println(holdingRegisters[i1],HEX);
    #endif
  }
  


  error = modbus.writeMultipleCoils(MODBUS_UNIT_ID, 0, coils, numCoils);
  printLog(MODBUS_UNIT_ID, 15, 0, numHoldingRegisters, error);

  error = modbus.readInputRegisters(MODBUS_UNIT_ID, 0, inputRegisters, numInputRegisters);
  printLog(MODBUS_UNIT_ID, 4, 0, numHoldingRegisters, error);

  error = modbus.readDiscreteInputs(MODBUS_UNIT_ID, 0, discreteInputs, numDiscreteInputs);
  printLog(MODBUS_UNIT_ID, 2, 0, numHoldingRegisters, error);

  // control LED according status of button (input register 0) of slave
  digitalWrite(LED_PIN, discreteInputs[0]); 

}



void printLog(uint8_t unitId, uint8_t functionCode, uint16_t startingAddress, uint16_t quantity, uint8_t error) {
  #if !defined(NO_DEBUG_SERIAL)
  transactionCounter++;
  if (error) errorCounter++;
  char string[128];
  sprintf(string, "%ld %ld %02X %02X %04X %04X %s", transactionCounter, errorCounter, unitId, functionCode, startingAddress, quantity, errorStrings[error]);
  Serial.print(string);
  if (error == MODBUS_RTU_MASTER_EXCEPTION_RESPONSE) {
    sprintf(string, ": %02X", modbus.getExceptionResponse());
    Serial.print(string);
  }
  Serial.println();
  #endif
}
