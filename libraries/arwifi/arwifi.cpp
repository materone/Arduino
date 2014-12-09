/*
 arwif.cpp - A simple esp8266 wifi lib.
  Tony
  Chufan Studio
  20141028
*/

#include "arwifi.h"
#include <string.h>
#include <AltSoftSerial.h>

#define  ledPin  13

//define var
AltSoftSerial serWifi;
String data;
char c;
unsigned long tStart, timeInterval = 10000UL, timeFree = 0UL, timeLast = 0UL;

/**
*  
*/
arwifi::arwifi() {
}

/**
*  
*/
arwifi::arwifi(char* sid, char* passwd) {
	begin(9600);
  quitAP();
  joinAP(sid, passwd);
}

/**
*  
*/
void arwifi::begin(int band){
	serWifi.begin(band);
}


/**
*  
*/
void arwifi::setTimeout(unsigned long ms){
  timeInterval = ms;
}

/**
*  
*/
boolean arwifi::connect(char * url) {
  boolean rc = false;
  return rc;
}

/**
*  
*/
boolean arwifi::connect(char * host, char * port, int mux) {
  boolean rc = false;
  String ret;
  serWifi.println("AT+CIPMUX=1");
  Serial.println(waitData("OK", "ERROR", "", ""));
  serWifi.print("AT+CIPSTART=");
  serWifi.print(mux);
  serWifi.print(",\"TCP\",\"");
  serWifi.print(host);
  serWifi.print("\",");
  serWifi.println(port);
  Serial.println(ret = waitData("OK", "ERROR", "Linked", "CONNECT"));
  if (ret.indexOf("OK") != -1) {
        rc = true;
  }
  return rc;
}

/**
*  
*/
void arwifi::disconnect(int mux) {
	serWifi.println("AT+CIPCLOSE=1");
  	Serial.println(waitData("OK", "ERROR", "Unlink", ""));
}

/**
*  
*/
boolean arwifi::joinAP(char* sid, char* passwd) {
  Serial.println("arwifi::joinAP");
  boolean rc = false;
  String ret = "";
  for (int i = 0; i < 3 ; i++) {
    serWifi.println("AT+CIFSR");
    ret = waitData("OK", "", "", "");
    if (ret.indexOf("0.0.0.0") != -1) {
      serWifi.print("AT+CWJAP=\"");
      serWifi.print(sid);
      serWifi.print("\",\"");
      serWifi.print(passwd);
      serWifi.println("\"");
      ret = waitData("OK", "ERROR", "", "");
      if (ret.indexOf("OK") != -1) {      	
  			Serial.println("Joined arwifi::joinAP");
        rc = true;
        break;
      }
    } else {
      rc = true;
      break;
    }
    delay(1000);
  }

  return rc;
}

/**
*  
*/
boolean arwifi::quitAP() {
  Serial.println("Begin arwifi::quitAP");
  boolean rc = false;
  serWifi.println("AT+CWQAP");
  String s = waitData("OK", "ERROR", "", "");
  if (s.indexOf("OK") != -1) {
  Serial.println("End arwifi::quitAP");
    rc = true;
  }
  return rc;
}

/**
*  
*/
boolean arwifi::connected() {
  boolean rc = false;
  serWifi.println("AT+CIPSTATUS");
  String ret = waitData("OK", "ERROR", "", "");
  if(ret.indexOf("STATUS:3")!=-1) rc = true;
  return rc;
}

/**
*
*/
void stop(){
  disconnect();
}

/**
*  
*/
boolean arwifi::available() {
  //boolean rc = false;
  return serWifi.available();
}

/**
*Read a byte from wifi 
*/
uint8_t arwifi::read() {
  uint8_t rc = false;
  if(serWifi.available()){
    rc = serWifi.read();
  }
  return rc;
}

/**
*  
*/
boolean arwifi::write(uint8_t* buf, uint16_t length,int mux) {
  boolean rc = false;
  serWifi.print("AT+CIPSEND=");
  serWifi.print(mux);
  serWifi.print(",");
  serWifi.println(length);
  String ret = waitData(">","OK", "ERROR", "");
  if(ret.indexOf(">")!=-1) {
    for(int i = 0 ; i < length ; i++){    
      serWifi.write(buf[i]);
      //Serial.print(i);
      //Serial.print("-->");
      //Serial.println(buf[i],DEC);
    }
    String ret = waitData("SEND OK", "ERROR", "", "");
    Serial.println(ret);
    if(ret.indexOf("SEND OK")!=-1) rc = true;
  }
  return rc;
}

/**
*  
*/
String arwifi::waitData(char * Tag1, char * Tag2 = "", char * Tag3 = "", char * Tag4 = "") {
  String ret = "";
  boolean rcvData = false;
  timeLast = millis();
  int cnt = 0;
  while (1)
  {
    if (serWifi.available()) {
      data = "";
      rcvData = true;
      while (serWifi.available()) {
        c = char(serWifi.read());
        if (cnt++ < 100) {
          data += c;
        }
        Serial.print(c);
        //delay(1);
      }
      Serial.println();
      Serial.print("=== ");
      Serial.print(cnt);
      Serial.println(" ===");
      //Serial.println(data);
      if (cnt > 100) {
        Serial.println("====Trunked====");
      }
      ret += data;
      cnt = 0;
    }
    timeFree = millis();
    if ((timeFree > timeLast) && (timeFree - timeLast) > timeInterval) break;

    if (!rcvData) {
      continue;
    } else {
      rcvData = false;
    }
    Serial.print("+++");
    Serial.println(ret);

    //找到任何一个标识符即退出。
    if ((Tag1 != "") && (ret.indexOf(Tag1) != -1)) break;
    if ((Tag2 != "") && (ret.indexOf(Tag2) != -1)) break;
    if ((Tag3 != "") && (ret.indexOf(Tag3) != -1)) break;
    if ((Tag4 != "") && (ret.indexOf(Tag4) != -1)) break;
  }
  return ret;
}

/**
*  
*/
void arwifi::getWifiInfo() {
  if (serWifi.available()) {
    digitalWrite(ledPin, HIGH);
    data = "";
    Serial.print("WiFi:") ;
    while (serWifi.available()) {
      c = char(serWifi.read());
      data += c;
     // Serial.write(c);
      delay(1);
    }
    Serial.print(data);
    digitalWrite(ledPin, LOW);
  }

  if (Serial.available()) {
    digitalWrite(ledPin, HIGH);
    data = "";
    Serial.print("PC:") ;
    while (Serial.available()) {
      c = char(Serial.read());
      data += c;
      //serWifi.write(c);
      delay(2);
    }
    //Serial3.write(data);
    Serial.print(data);
    serWifi.print(data);
    digitalWrite(ledPin, LOW);
    //serWifi.println(data);
    delay(100);
    digitalWrite(ledPin, LOW);
  }
}