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
unsigned long tStart, timeInterval = 10000UL, timeFree = 0UL, timeLast = 0UL;
boolean ledStatus = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  serWifi.begin(9600);
  //serWifi.setTimeout(2000);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  getDHT();
  tStart = millis();
  //delay(1000);
  serWifi.println("AT+GMR");
  Serial.println(waitData("OK", "ERROR", "", ""));
  delay(50);
  connectToAP();
  //joinAP();
  update();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("get info");
  getWifiInfo();
  if ((millis() - tStart) >= 3600000UL) {
    tStart = millis();
    getDHT();
    connectToAP();
    update();
  }
}

void getWifiInfo() {
  if (serWifi.available()) {
    digitalWrite(ledPin, HIGH);
    //char ss[255] = {};
    data = "";
    //Serial.print("WiFi:") ;
    while (serWifi.available()) {
      c = char(serWifi.read());
      data += c;
      //Serial.write(c);
      delay(1);
    }
    //while (
    //serWifi.readBytes(ss, 255) != 0) {
    //    Serial.print(data);
    //Serial.println(ss);
    //}
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
  Serial.print("In AT Cmd:");
  Serial.print(h, 1);
  Serial.print(",\t");
  Serial.println(t, 1);
  //getWifiInfo();
  serWifi.println("AT+CIPMUX=1");
  delay(200);
  timelog("Wait CIPMUX OK");
  Serial.println(waitData("OK", "", "", ""));
  //  Serial.println("Begin");
  serWifi.println("AT+CIPSTART=1,\"TCP\",\"materonep001.sinaapp.com\",80");

  timelog("Wait CIPStart OK");
  Serial.println(waitData("OK", "", "", ""));
  String s = "GET /homestatus.php?h=";
  s = s + h;
  s = s + "&t=";
  s = s + t;
  s = s + " HTTP/1.1\r\n";
  s = s + "host:materonep001.sinaapp.com\r\n\r\n";
  //  Serial.println(s);
  //  Serial.println("End send info");
  String cmd = "AT+CIPSEND=1,";
  cmd += s.length();
  //  Serial.println(cmd);
  //  Serial.println(s);
  if (s.length() > 0) {
    serWifi.println(cmd);
    //    delay(200);
    //    getWifiInfo();
    timelog("Wait >");
    Serial.println(waitData(">", "", "", ""));
    timelog("Wait > END");
    serWifi.print(s);
    timelog("Wait Return from SVR");
    //delay(300);
    //getWifiInfo();
    Serial.println(waitData("SEND OK", "", "", ""));
    timelog("Wait SVR OK");
    //Serial.println(waitData("OK", "", "", ""));
    //timelog("Wait SVR RET OK");
    delay(1000);
  }
  timelog("Begin Close conn");
  serWifi.println("AT+CIPCLOSE=1");
  getWifiInfo();
}

String joinAP() {
  Serial.println("In JoinAP");
  serWifi.println("AT+CWJAP=\"CoolMi\",\"26856204\"");
  //delay(8000);
  //rets = wifi.waitData(T_OK, T_READY);
  //Serial.println(rets);
  return waitData("OK", "ERROR", "", "");
}

void blinkLed() {
  ledStatus ? digitalWrite(ledPin, HIGH) : digitalWrite(ledPin, LOW);
  ledStatus != ledStatus;
}

String waitData(String Tag1, String Tag2, String Tag3, String Tag4)
{
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

void connectToAP() {
  String ret = "";
  for (int i = 0; i < 3 ; i++) {
    Serial.println("In Connect to ap");
    serWifi.println("AT+CIFSR");
    ret = waitData("OK", "", "", "");
    if (ret.indexOf("0.0.0.0") != -1) {
      Serial.println("Connetct to AP begin");
      ret = joinAP();
      Serial.print("Connetct to AP ret:");
      Serial.println(ret);
    } else {
      Serial.println(ret);
      break;
    }
    delay(1000);
  }
}
void timelog() {
  timelog("");
}
void timelog(String s) {
  unsigned long t = millis();
  Serial.print(s);
  Serial.print(" :[ ");
  Serial.print(t - tStart);
  Serial.println(" ]ms");
}

