#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

LiquidCrystal_I2C lcd(0x27, 16, 2);
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void info(char * str) {
  lcd.println(str);
}
void info(String str) {
  lcd.println(str);
}

void setup() {
  // put your setup code here, to run once:
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  Serial.begin(115200);
  inputString.reserve(200);
  info("Begin test");
  delay(2000);
  Serial.println("AT+GMR");
  delay(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  printComm();
  delay(5000);
  Serial.println("AT+CIFSR");
}

void printComm(){
  if (stringComplete) {
    info(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
