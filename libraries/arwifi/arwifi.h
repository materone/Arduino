/*
 arwifi.h - A simple esp8266 wifi lib.
  Tony
  Chufan Studio
  20141028
*/

#ifndef arwifi_h
#define arwifi_h

#include <Arduino.h>
#include <string.h>
#include <AltSoftSerial.h>

// Define
#define VERSION 001

class arwifi {
private:
   char* sid;
   char* passwd;
public:
	 AltSoftSerial serWifi;
   arwifi();
   arwifi(char* sid,char* passwd);
   void begin(int band);
   void setTimeout(unsigned long ms);
   boolean connect(char * url);
   boolean connect(char * host, char * port, int mux);
   void disconnect(int mux);   
   boolean joinAP(char* sid,char* passwd);
   boolean quitAP();
   boolean connected();
   void stop();
   boolean available();
   uint8_t read();
   boolean write(uint8_t* buf, uint16_t length,int mux);
   String waitData(char * Tag1,char * Tag2,char * Tag3,char * Tag4);
   void getWifiInfo();
};


#endif
