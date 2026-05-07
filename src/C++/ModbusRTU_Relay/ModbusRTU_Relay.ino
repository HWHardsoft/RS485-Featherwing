/**
 * ModbusRTU_Relay.ino
 * 
 * Modbus master (client) demo for RS485 Feather Wing by Zihatec, 2026 
 *
 * This example demonstrates how to control the outputs of a modbus relay unit (Waveshare Modus RTU Relay)  
 * via RS485 Wing and Feather board 
 *
 * DIP switch setting:
 *  S1: OFF - ON - ON - OFF
 *  S2: ON
 *  S4: ON - ON - ON - ON
 * 
 * 
 * This example demonstrates use the ModbusRTUMaster library (https://github.com/CMB27/ModbusRTUMaster) by  C. M. Bulliner
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


ModbusRTUMaster modbus(MODBUS_SERIAL, -1);

const uint8_t numCoils = 8;


bool coils[numCoils];

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
  uint8_t error;

  // turn all 8 relays ON
  for(int i1 = 0; i1 < 8; i1++){
    coils[i1] = 1;
    error = modbus.writeMultipleCoils(MODBUS_UNIT_ID, 0, coils, numCoils);
    printLog(MODBUS_UNIT_ID, 15, 0, 8, error);
    delay(200);
  
  }

  // turn all 8 relays OFF
  for(int i1 = 0; i1 < 8; i1++){
    coils[i1] = 0;
    error = modbus.writeMultipleCoils(MODBUS_UNIT_ID, 0, coils, numCoils);
    printLog(MODBUS_UNIT_ID, 15, 0, 8, error);
    delay(200);
  
  }
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
