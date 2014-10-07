#include <SoftwareSerial.h>

SoftwareSerial sSerial(5,6);
void trans(boolean readonly=false)
{
  while (sSerial.available())
  {
    Serial.write(sSerial.read());
  }
  if (!readonly) {
    while (Serial.available())
    {
      sSerial.write(Serial.read());
    }
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sSerial.begin(9600);
  Serial.println("Go");
  sSerial.print("AT+CWJAP=\"CoolDog\",\"86053436\"\r\n");
  delay(5000);
  //info();
  delay(200);
  Serial.println("Get IP:");
  sSerial.print("AT+CIFSR\r\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(20);
  info();
  trans(false);
}
void info(){
  trans(true);
}

