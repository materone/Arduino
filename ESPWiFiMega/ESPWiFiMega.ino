#include <SeeedOLED.h>
#include <Wire.h>

char c;
String data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(1000);
  Serial.println("Begin Test");
  Serial1.println("AT+RST");
  delay(2000);
  Serial1.println("AT");
  Serial1.println("AT+CWJAP=\"CoolDog\",\"86053436\"");
  getWifiInfo();
  delay(5000);
  update1();
}

void loop() {
  // put your main code here, to run repeatedly:
  getWifiInfo();
  getCmd();
}

void getWifiInfo(){
  if (Serial1.available()) {
    data = "";
    Serial.print("Get Wifi response:");
    SeeedOled.clearDisplay();
    while (Serial1.available()) {
      c = Serial1.read();
      SeeedOled.putChar(c);
      data += c;
    }
    Serial.println(data);
  }
}

void getCmd(){
  if (Serial.available()) {
    data = "";
    while (Serial.available()) {
      c = (char)Serial.read();
      data += c;
    }
    Serial.print("Send AT:");
    Serial1.println(data);
    Serial.println(data);
  }
}
void update1() {
  /*
  altPort.println("AT+CIPCLOSE");
  delay(500);
  */
  Serial.print("In AT Cmd:");
  Serial1.println("AT+CIPMUX=1");
  delay(2000);
  getWifiInfo();
  Serial1.println("AT+CIPSTART=4,\"TCP\",\"materonep001.sinaapp.com\",80");
  delay(2000);
  getWifiInfo();
  Serial1.println("AT+CIPSEND=4,77");
  delay(2000);
  getWifiInfo();
  Serial1.print("GET /homestatus.php?h=9888&t=8889 HTTP/1.1\r\n");
  delay(1000);
  getWifiInfo();
  Serial1.print("host:materonep001.sinaapp.com\r\n");
  delay(1000);
  getWifiInfo();
  Serial1.print("\r\n");
  getWifiInfo();
}
