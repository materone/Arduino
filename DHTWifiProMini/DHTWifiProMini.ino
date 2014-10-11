#include <AltSoftSerial.h>
#include <DHT.h>

#define DHT11_PIN 3 //put the sensor in the digital pin 3
#define  ledPin  13

AltSoftSerial serWifi;
String data;
char c;
DHT dht;
double h;
double t;
unsigned long tStart;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  serWifi.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  getDHT();
  tStart = millis();
  serWifi.println("AT+CWLAP");
  delay(10);
  joinAP();
  update();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("get info");
  getWifiInfo();
  if ((millis() - tStart) >= 3600000) {
    tStart = millis();
    getDHT();
    update();
  }
}

void getWifiInfo() {
  if (serWifi.available()) {
    digitalWrite(ledPin, HIGH);
    data = "";
    //Serial.print("WiFi:") ;
    while (serWifi.available()) {
      c = char(serWifi.read());
      data += c;
      //Serial.write(c);
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
  }
}

void getDHT() {
  int chk = dht.read11(DHT11_PIN);
  switch (chk)
  {
    case 0:  Serial.print("OK,\t"); break;
    case -1: Serial.print("Checksum error,\t"); break;
    case -2: Serial.print("Time out error,\t"); break;
    default: Serial.print("Unknown error,\t"); break;
  }

  // DISPLAT DATA
  Serial.print(dht.humidity, 1);
  Serial.print(",\t");
  Serial.println(dht.temperature, 1);
  h = dht.humidity;
  t = dht.temperature;
}
void update() {
  String s;
  Serial.print("In AT Cmd:");
  Serial.print(h, 1);
  Serial.print(",\t");
  Serial.println(t, 1);
  getWifiInfo();
  serWifi.println("AT+CIPMUX=1");
  delay(2000);
  getWifiInfo();
  serWifi.println("AT+CIPSTART=1,\"TCP\",\"materonep001.sinaapp.com\",80");
  delay(2000);
  getWifiInfo();
  s = "GET /homestatus.php?h=";
  s = s + h;
  s = s + "&t=";
  s = s + t;
  s = s + " HTTP/1.1\r\n";
  s = s + "host:materonep001.sinaapp.com\r\n\r\n";
  String cmd = "AT+CIPSEND=1,";
  cmd += s.length();
  //  Serial.println(cmd);
  //  Serial.println(s);
  serWifi.println(cmd);
  delay(200);
  getWifiInfo();
  serWifi.print(s);
  delay(200);
  getWifiInfo();
  delay(2000);
  serWifi.println("AT+CIPCLOSE=1");
  getWifiInfo();
}
void joinAP() {
  serWifi.println("AT+CWJAP=\"CoolDog\", \"86053436\"");
  delay(8000);
  //rets = wifi.waitData(T_OK, T_READY);
  //Serial.println(rets);
  getWifiInfo();
}
