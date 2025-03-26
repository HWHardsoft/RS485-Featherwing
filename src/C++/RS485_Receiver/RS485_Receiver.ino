/*
  RS-485 Receiver.

  This sketch prints any data received from the RS-485 interface to the Serial port.

  Modified for use with RS485 Feather Wing by Zihatec GmbH

  Originally created by Sandeep Mistry, on 4 July 2018.
  Modified by @vishnumaiea for CIRCUITSTATE Electronics (@circuitstate).

  Source: https://github.com/CIRCUITSTATE/CSE_ArduinoRS485
*/

#include <CSE_ArduinoRS485.h>

// Recommended initialization for activated automatic send/receive switching
// S1: OFF - ON - ON - OFF 
RS485Class RS485 (Serial1, -1, -1, -1); // DE, RE, TX

// alternative initialization for software controlled send/receive switching (Huzzah32)
// S1: OFF - ON - OFF - ON
//RS485Class RS485 (Serial1, 12, -1, -1); // DE, RE, TX

void setup() {
  Serial.begin (9600);
  while (!Serial);

  RS485.begin (9600);

  // Enable reception. Can be disabled with: RS485.noReceive();
  RS485.receive();
}

void loop() {
  if (RS485.available()) {
    Serial.write (RS485.read());
  }
}

