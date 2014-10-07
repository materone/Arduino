#include <AltSoftSerial.h>

AltSoftSerial serWifi;
String data;
char c;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  serWifi.begin(9600);
  serWifi.println("AT+CWLAP");
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("get info");
  if (serWifi.available()) {
    data = "";
    Serial.print("WiFi:") ;
    while (serWifi.available()) {
      c = char(serWifi.read());
      data += c;
      //Serial.write(c);
      delay(1);
    }
    Serial.println(data);
  }

  if (Serial.available()) {
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
    //serWifi.println(data);
    delay(100);
  }
}
