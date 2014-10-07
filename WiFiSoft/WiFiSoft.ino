#include <SeeedOLED.h>
#include <Wire.h>

String data;
unsigned char c;

void setup() {
  //init ooled
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  DDRB |= 0x21;
  PORTB |= 0x21;

  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setHorizontalMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putString("Go:"); //Print the String
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("AT+RST");
  delay(100);
  if (Serial.available()) {
    showWifiInfo();
  }
  Serial.println("AT+GMR");
  delay(100);
  Serial.println("AT+CWMODE=3");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    showWifiInfo();
  }

  delay(100);

//  if (Serial.available()) {
//    data = "";
//    Serial.print("PC:") ;
//    while (Serial.available()) {
//      c = Serial.read();
//      data += c;
//    }
//    Serial.println(data);
//  }
//  delay(1000);
}

void showWifiInfo() {
  if (Serial.available()) {
    data = "";
    SeeedOled.putString("WiFi :") ;
    while (Serial.available()) {
      c = Serial.read();
      data += c;
      SeeedOled.putChar(c);
    }
  }
}
