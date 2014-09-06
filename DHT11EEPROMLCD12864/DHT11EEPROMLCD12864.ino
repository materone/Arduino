#include <I2CEEPROM.h>
#include <dht.h>
#include <Wire.h>
#include <SeeedOLED.h>

//超声波距离探测仪温湿度
unsigned char fontBuff[32];
dht DHT;
#define DHT11_PIN 4 //put the sensor in the digital pin 4

void setup()
{
  Wire.begin();	
  SeeedOled.init();  //initialze SEEED OLED display  
  DDRB|=0x21;        
  PORTB |= 0x21;
  Serial.begin(9600);
  //init display
  SeeedOled.init();                       //initialze SEEED OLED display
  SeeedOled.clearDisplay();               // clear the screen and set start position to top left corner
  SeeedOled.deactivateScroll();           // deactivete Scroll (might be activated by previous test case)
  SeeedOled.setNormalDisplay();           // Non-inverted Display 
  //SeeedOled.setHorizontalMode();                // Page mode to start with

  //print title
  SeeedOled.setBitmapRect(0,1,0,127);
  //p0
  for(int i = 0 ;i< 8;i++){
    SeeedOled.drawBitmap(getFontData(i*32,16),16,false);
  }
  //p1
  for(int i = 0 ;i< 8;i++){
    SeeedOled.drawBitmap(getFontData(i*32 + 16,16),16,false);
  }
  //wendu
  SeeedOled.setBitmapRect(3,4,16,47);
  SeeedOled.drawBitmap(getFontData(8*32,16),16,false);
  SeeedOled.drawBitmap(getFontData(10*32,16),16,false);
  SeeedOled.drawBitmap(getFontData(8*32+16,16),16,false);
  SeeedOled.drawBitmap(getFontData(10*32+16,16),16,false);
  
  //shidu  
  SeeedOled.setBitmapRect(6,7,16,47);
  SeeedOled.drawBitmap(getFontData(9*32,16),16,false);
  SeeedOled.drawBitmap(getFontData(10*32,16),16,false);
  SeeedOled.drawBitmap(getFontData(9*32+16,16),16,false);
  SeeedOled.drawBitmap(getFontData(10*32+16,16),16,false);
}
void loop()
{
  testTemperature();
  delay(2000);
  testHumidity();
  delay(2000);
}
/**
*  Get Font Data from eeprom 256Kb
*/
unsigned char* getFontData(int idx,int len){
  i2ceeprom.readBuffer(idx,fontBuff,len);
  return fontBuff;
}

void testTemperature(){
  //begin measure d and t
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case 0:  Serial.println("OK,\t"); break;
    case -1: Serial.println("Checksum error,\t"); break;
    case -2: Serial.println("Time out error,\t"); break;
    default: Serial.println("Unknown error,\t"); break;
  }
 
 // DISPLAT DATA
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);
 
  //display in lcd page 5,6 col 16
  SeeedOled.setBitmapRect(3,4,64,111);
  unsigned char char_buffer[3]="";
  unsigned char i = 0;
  unsigned char f = 0;
  long long_num = DHT.temperature;

  //SeeedOled.sendCommand(SeeedOLED_Display_Off_Cmd); 	//display off
  //clear digi zone 3
  for(i=0;i<96;i++){
    SeeedOled.setHorizontalMode();
    SeeedOled.sendData(0x00);
  }
  
  i=0;

  if (long_num == 0) {
    f=1;    
    SeeedOled.setBitmapRect(3,4,64,79);
    SeeedOled.drawBitmap(getFontData(352,32),32,false);
  } 
  else{
    while (long_num > 0) 
    {
      char_buffer[i++] = long_num % 10;
      long_num /= 10;
    }
    f=f+i;
    for(; i > 0; i--)
    {
      Serial.print(char_buffer[i-1]);       
      SeeedOled.setBitmapRect(3,4,112-i*16,128-i*16);
      SeeedOled.drawBitmap(getFontData((352 + char_buffer[i-1]*32),32),32,false);      
      //putChar('0'+ char_buffer[i - 1]);
    }
  }
  //SeeedOled.sendCommand(SeeedOLED_Display_On_Cmd); 	//display off
}

void testHumidity(){
  //begin measure d and t
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case 0:  Serial.println("OK,\t"); break;
    case -1: Serial.println("Checksum error,\t"); break;
    case -2: Serial.println("Time out error,\t"); break;
    default: Serial.println("Unknown error,\t"); break;
  }
 
 // DISPLAT DATA
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);
 
  //display in lcd page 5,6 col 16
  SeeedOled.setBitmapRect(6,7,64,111);
  unsigned char char_buffer[3]="";
  unsigned char i = 0;
  unsigned char f = 0;
  long long_num = DHT.humidity;

  //SeeedOled.sendCommand(SeeedOLED_Display_Off_Cmd); 	//display off
  //clear digi zone 3
  for(i=0;i<96;i++){
    SeeedOled.setHorizontalMode();
    SeeedOled.sendData(0x00);
  }
  
  i=0;

  if (long_num == 0) {
    f=1;    
    SeeedOled.setBitmapRect(6,7,64,79);
    SeeedOled.drawBitmap(getFontData(352,32),32,false);
  } 
  else{
    while (long_num > 0) 
    {
      char_buffer[i++] = long_num % 10;
      long_num /= 10;
    }
    f=f+i;
    for(; i > 0; i--)
    {
      Serial.print(char_buffer[i-1]);       
      SeeedOled.setBitmapRect(6,7,112-i*16,128-i*16);
      SeeedOled.drawBitmap(getFontData((352 + char_buffer[i-1]*32),32),32,false);       
    }
  }
  //SeeedOled.sendCommand(SeeedOLED_Display_On_Cmd); 	//display off
}

