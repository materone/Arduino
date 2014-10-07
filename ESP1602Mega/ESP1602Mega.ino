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
char inChar,preChar;
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
  Serial3.begin(115200);
  Serial.begin(115200);
  inputString.reserve(200);
  info("Begin test");
  delay(2000);
  Serial3.println("AT+GMR");
  delay(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  printComm();
  delay(5000);
  Serial3.println("AT+CIFSR");
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
void serialEvent3() {
  while (Serial3.available()) {
    // get the new byte:
    inChar = (char)Serial3.read();
    Serial.print(inChar);
    // add it to the inputString:
    inputString += inChar;    
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == 0x6C && (preChar==0x72 || preChar==0x73)) {
      stringComplete = true;
      preChar=0;
      Serial.println();
    }
    preChar=inChar;
  }
}
