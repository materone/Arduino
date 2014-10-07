#include <esp8266.h>
#include <SoftSerial.h>
esp8266 wifi;
void setup() {
  // put your setup code here, to run once:
  wifi.init(9600,5,6);
  Serial.begin(9600);
  //wifi.bridge();
  wifi.runAT_End("AT+CWJAP=\"CoolDog\",\"86053436\"");
  delay(2000);
  wifi.GetInfo();
  wifi.runAT_End("CWLAP");
  delay(100);
  wifi.bridge();
}

void loop() {
  // put your main code here, to run repeatedly:
//  delay(3000);
//  wifi.GetInfo();
  delay(20);
  getWifiInfo();
  update();
  getWifiInfo();
  delay(180000);
}
void getWifiInfo(){
  wifi.bridge();
}
void update() {
  Serial.println("In AT Cmd:");
  getWifiInfo();
  wifi.runAT_End("AT+CIPMUX=1");
  delay(2000);
  getWifiInfo();
  wifi.runAT_End("AT+CIPSTART=4,\"TCP\",\"materonep001.sinaapp.com\",80");
  delay(2000);
  getWifiInfo();
  wifi.runAT_End("AT+CIPSEND=4,77");
  delay(2000);
  getWifiInfo();
  wifi.runAT_End("GET /homestatus.php?h=9988&t=6868 HTTP/1.1");
  delay(1000);
  getWifiInfo();
  wifi.runAT_End("host:materonep001.sinaapp.com");
  delay(1000);
  getWifiInfo();
  wifi.runAT_End("");
  getWifiInfo();
  delay(5000);
  wifi.runAT_End("AT+CIPCLOSE=4");
  getWifiInfo();
}

