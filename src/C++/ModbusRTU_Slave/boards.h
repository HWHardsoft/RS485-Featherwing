#ifndef MODBUS_BOARDS_H
#define MODBUS_BOARDS_H


#if defined(ESP8266)
  //Adafruit Feather Huzzah ESP8266 
  //please remove the RS485 Wing for firmware upload! 
  #define MODBUS_SERIAL Serial
  #define NO_DEBUG_SERIAL
  
#elif defined(__AVR__)  
  #define MODBUS_SERIAL Serial
  #define NO_DEBUG_SERIAL

#elif defined(NRF52_SERIES) 
  // for example nRF52 chips
  #include <Adafruit_TinyUSB.h> // for Serial
  #include <SoftwareSerial.h>
  #define SOFTWARE_SERIAL
  SoftwareSerial mySerial(1, 0); // RX, TX
  #define MODBUS_SERIAL mySerial

#else
  
  // On the majority of Feather boards, the HardwareSerial port on pins 0 and 1 is Serial1
  #define MODBUS_SERIAL Serial1
  
#endif




 
#endif

