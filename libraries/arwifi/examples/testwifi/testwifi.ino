#include <AltSoftSerial.h>
#include <arwifi.h>
arwifi wifiClient;
String data1;
char c1;
#define ledPin 13
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  wifiClient.begin(9600);
  Serial.println("Test Begin");
  wifiClient.quitAP();
  Serial.println("Test 1");
  wifiClient.joinAP("CoolDog","####");
}

void loop(){
  wifiClient.getWifiInfo();
}

