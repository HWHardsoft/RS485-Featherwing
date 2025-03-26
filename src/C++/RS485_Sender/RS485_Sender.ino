/*
  RS-485 Sender

  This sketch periodically sends a string over the RS-485 interface

  Modified for use with RS485 Feather Wing by Zihatec GmbH

  Originally created by Sandeep Mistry, on 4 July 2018.
  Modified by @vishnumaiea for CIRCUITSTATE Electronics (@circuitstate).

  Source: https://github.com/CIRCUITSTATE/CSE_ArduinoRS485
*/


#include <CSE_ArduinoRS485.h>


int counter = 0;

// Recommended initialization for activated automatic send/receive switching
// S1: OFF - ON - ON - OFF 
RS485Class RS485 (Serial1, -1, -1, -1); // DE, RE, TX

// alternative initialization for software controlled send/receive switching 
// S1: OFF - ON - OFF - ON
//RS485Class RS485 (Serial1, 12, -1, -1); // DE, RE, TX  (Huzzah32, Sparkfun ThingPlus 32 C, Feather M0 ... )
//RS485Class RS485 (Serial1, 17, -1, -1); // DE, RE, TX  (Sparkfun ThingPlus RP2040)



void setup() {
  // Initialize the RS485 interface
  RS485.begin (9600);
}

void loop() {
  RS485.beginTransmission();
  RS485.print ("Hello ");
  RS485.println (counter);
  RS485.endTransmission();

  counter++;

  delay (1000);
}
