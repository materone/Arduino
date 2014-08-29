#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;

byte year, month, date, DoW, hour, minute, second;
// constants won't change. They're used here to 
// set pin numbers:
//const int buttonPin = 7;     // the number of the pushbutton pin
const int a = 32;
const int b = 30;
const int c = 40;
const int d = 42;
const int e = 44;
const int f = 34;
const int g = 36;
const int p = 38;
const int pins[] = {a,b,c,d,e,f,g,p};
//1  2  4  5  10 9  7  6
//44 42 40 38 36 34 32 30
//const int pins[] = {0,1,3,5,6,11,12,13};      // the number port of the board pin
//const int pinsLED[] = {6,7,9,10,5,4,2,1};//the pins in LED(bafgpcde)764219a5
//display code lib
const int nd[][7]={
  //0
  {a,b,c,d,e,f},
  //1
  {b,c},
  //2
  {a,b,d,e,g},
  //3
  {a,b,c,d,g},
  //4
  {b,c,f,g},
  //5
  {a,f,g,c,d},
  //6
  {a,f,g,c,d,e},
  //7
  {a,b,c},
  //8
  {a,b,c,d,e,f,g},
  //9
  {a,b,c,d,f,g},
  //p dot
  {p}
};
const int ndsize[]={6,2,5,5,4,5,6,3,7,6,1};

// variables will change:
//int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  Wire.begin();
//        Clock.setSecond(50);//Set the second 
//        Clock.setMinute(13);//Set the minute 
//        Clock.setHour(1);  //Set the hour 
//        Clock.setDoW(5);    //Set the day of the week
//        Clock.setDate(29);  //Set the date of the month
//        Clock.setMonth(8);  //Set the month of the year
//        Clock.setYear(14);  //Set the year (Last two digits of the year)
  Serial.begin(115200);
  //Serial.println(sizeof(nd[2]));
  for(int i = 0 ;i < 8;i++){
    pinMode(pins[i], OUTPUT); 
  } 
  black();
}

void loop(){
  for(int n = 0;n <=10 ;n++){
    showdig(n);  
    delay(2000);
    black();
    delay(500);
  }
  ReadDS3231();
}
void ReadDS3231()
{
  int second,minute,hour,date,month,year,temperature; 
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  
  temperature=Clock.getTemperature();
  
  Serial.print("20");
  Serial.print(year,DEC);
  Serial.print('-');
  Serial.print(month,DEC);
  Serial.print('-');
  Serial.print(date,DEC);
  Serial.print(' ');
  Serial.print(hour,DEC);
  Serial.print(':');
  Serial.print(minute,DEC);
  Serial.print(':');
  Serial.print(second,DEC);
  Serial.print('\n');
  Serial.print("Temperature=");
  Serial.print(temperature); 
  Serial.print('\n');
}

void showdig(int num){
  if(num <0 || num >10) return;
  black();  
  for(int i = 0;i < ndsize[num];i++){
    digitalWrite(nd[num][i],HIGH);
  }
}

void black(){
  for(int i=0;i<8;i++){
    digitalWrite(pins[i], LOW); 
  }
}
