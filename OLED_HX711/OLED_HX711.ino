#include <Hx711.h>

#include <Wire.h>
#include <SeeedOLED.h>

Hx711 scale(A1,A0);

void setup()
{
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  DDRB|=0x21;        
  PORTB |= 0x21;

  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Hello World!"); //Print the String  
}

void loop()
{
  SeeedOled.setTextXY(2,0);
  SeeedOled.putString("W:");
  SeeedOled.putFloat(scale.getGram(),1);
  delay(2000);
}

