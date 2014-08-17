// link between the computer and the SoftSerial Shield
//at 9600 bps 8-N-1
//Computer is connected to Hardware UART
//SoftSerial Shield is connected to the Software UART:D2&D3 

#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial SoftSerial(0, 1);
unsigned char buffer[64]; // buffer array for data recieve over serial port
int count=0;     // counter for buffer array 
void setup()
{
  Wire.begin();
  SoftSerial.begin(9600);               // the SoftSerial baud rate   
  Serial.begin(9600);             // the Serial port of Arduino baud rate.

}

void loop()
{
  delay(5000);
  Serial.print("Begin RFID Module:");
  Serial.println(millis());
  if (SoftSerial.available())              // if date is comming from softwareserial port ==> data is comming from SoftSerial shield
  {    
  Serial.println("Begin RFID Read Module");
    while(SoftSerial.available())          // reading data into char array 
    {
      buffer[count++]=SoftSerial.read();     // writing data into array
      if(count == 64)break;
    }
    Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
    clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
    count = 0;                       // set counter of while loop to zero


  }
  if (Serial.available())            // if data is available on hardwareserial port ==> data is comming from PC or notebook
    SoftSerial.write(Serial.read());       // write it to the SoftSerial shield
}
void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<count;i++)
  { 
    buffer[i]=NULL;
  }                  // clear all index of array with command NULL
}

