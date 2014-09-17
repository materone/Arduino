#include <SeeedOLED.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>

#define SERIAL_BUFFER_SIZE 256

char c;
bool flag = true;
String data;
SoftwareSerial sPort(5, 6);
unsigned int cnt = 0;
unsigned int tCnt = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  sPort.begin(9600);
  delay(1000);
  sPort.println("Begin Test");
  //Serial.println("AT+RST");
  //init LED Display
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  DDRB |= 0x21;
  PORTB |= 0x21;
  SeeedOled.clearDisplay();           //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();       //Set display to Normal mode
  SeeedOled.setHorizontalMode();      //Set addressing mode to Horizontal Mode
  SeeedOled.setTextXY(0, 0);
  SeeedOled.putString("WiFi Test\n");
  Serial.println("AT");
  delay(100);
  Serial.println("AT+CWJAP=\"CoolDog\",\"86053436\"");
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
  if (Serial.available()) {
    data = "";
    cnt = 0;
    tCnt = 0;
    sPort.print("Get Wifi response:");
    SeeedOled.clearDisplay();
    while (Serial.available()) {
      c = Serial.read();
      cnt++;
      tCnt++;
      SeeedOled.putChar(c);
      data += c;
      if (cnt >= 32) {
        //delay(100);
        //sPort.println(data);
        cnt = 0;
        data = "";
      }
    }
    sPort.println(data);
    sPort.println(tCnt);
  }
}

void getCmd(){
  if (sPort.available()) {
    data = "";
    flag = true;
    while (sPort.available()) {
      c = (char)sPort.read();
      //sPort.println(c);
      /*
      if(c == '\n' && data.equals("datasent")){
        update1();
        flag=false;
        data = "";
        break;
      }
      */
      data += c;
    }
    sPort.print("Send AT:");
    if (flag)Serial.println(data);
    sPort.println(data);
  }
}
void update1() {
  /*
  Serial.println("AT+CIPCLOSE");
  delay(500);
  */
  sPort.print("In AT Cmd:");
  Serial.println("AT+CIPMUX=1");
  delay(2000);
  getWifiInfo();
  Serial.println("AT+CIPSTART=4,\"TCP\",\"materonep001.sinaapp.com\",80");
  delay(2000);
  getWifiInfo();
  Serial.println("AT+CIPSEND=4,77");
  delay(2000);
  getWifiInfo();
  Serial.print("GET /homestatus.php?h=9888&t=8889 HTTP/1.1\r\n");
  delay(1000);
  getWifiInfo();
  Serial.print("host:materonep001.sinaapp.com\r\n");
  delay(1000);
  getWifiInfo();
  Serial.print("\r\n");
  getWifiInfo();
}
