#include <DHT.h>
#include <esp8266.h>
#include <SoftSerial.h>
esp8266 wifi;
// Uncomment whatever type you're using!
#define DHT11_PIN 3 //put the sensor in the digital pin 3
DHT dht;
double h;
double t;
void setup() {
  // put your setup code here, to run once:
  wifi.init(19200,5,6);
  Serial.begin(19200);
  Serial.println(_SS_MAX_RX_BUFF, DEC);
  //wifi.bridge();
  joinAP();
  Serial.println("mark 1");
  //wifi.GetInfo();
  delay(20);
  //wifi.runAT_End("CWLAP");
  //delay(100);
  wifi.bridge(true);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  delay(3000);
  //  wifi.GetInfo();
  getDHT();
  delay(20);
  wifi.GetInfo();
  //getWifiInfo();
  //joinAP();
  //update();
  //getWifiInfo();
  Serial.println("Main Loop");
  delay(10000);
}
void getWifiInfo() {
  wifi.bridge(true);
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
  wifi.runAT_End("AT+CIPMUX=1");
  delay(2000);
  getWifiInfo();
  wifi.runAT_End("AT+CIPSTART=1,\"TCP\",\"materonep001.sinaapp.com\",80");
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
  wifi.runAT_End(cmd);
  delay(200);
  getWifiInfo();
  wifi.runAT(s);
  delay(200);
  getWifiInfo();
  delay(2000);
  wifi.runAT_End("AT+CIPCLOSE=1");
  getWifiInfo();
}
void joinAP() {
  String rets;
  int cnt = 0;
  for (int i = 0; i < 3; i++) {
    cnt++;
    Serial.print("Try :");
    Serial.println(cnt);
    wifi.configJAP("CoolDog", "86053436");
    delay(5000);
    //rets = wifi.waitData(T_OK, T_READY);
    //Serial.println(rets);
    getWifiInfo();
    Serial.println("Begin get ip");
    wifi.runAT_End("AT+CIFSR");
    delay(200);
    getWifiInfo();
   // rets = wifi.waitData(T_OK, T_READY,T_ERROR);
    //Serial.println(rets);
    Serial.println("End get IP");
    /*if (rets.indexOf("0.0.0.0") == -1) {
      Serial.println("Get IP");
      return;
    }
    */
    delay(200);
  }
}
