#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <Wire.h>

dht DHT;
#define DHT11_PIN 3 //put the sensor in the digital pin 3
long t,h;
LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  Wire.begin();	
  
  Serial.begin(9600);
  //init display
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(0,1);
  lcd.print("TEMP:");
  lcd.setCursor(9,1);
  lcd.print("HUMI:");
}
void loop()
{
  testHandT();
  delay(2000);
}

void testHandT(){
  //begin measure d and t
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case 0:  Serial.print("\nOK,\t"); break;
    case -1: Serial.print("Checksum error,\t"); break;
    case -2: Serial.print("Time out error,\t"); break;
    default: Serial.print("Unknown error,\t"); break;
  }
 
 // DISPLAT DATA
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.print(DHT.temperature,1);
 
  //display in lcd page 5,6 col 16
  h = DHT.humidity;
  t = DHT.temperature;

  showInLcd(5,1,DHT.temperature);
  showInLcd(14,1,DHT.humidity);
  //SeeedOled.sendCommand(SeeedOLED_Display_On_Cmd); 	//display off
}

void showInLcd(int col,int row,int long_num){
  unsigned char char_buffer[3]="";
  int i=0;
  lcd.setCursor(col,row);

  if (long_num == 0) {
    lcd.print("0");
  } 
  else{
    while (long_num > 0) 
    {
      char_buffer[i++] = long_num % 10;
      long_num /= 10;
    }
    for(; i > 0; i--)
    {
      Serial.print(char_buffer[i-1]);
      lcd.print(char_buffer[i-1]);      
      //putChar('0'+ char_buffer[i - 1]);
    }
  }
}

