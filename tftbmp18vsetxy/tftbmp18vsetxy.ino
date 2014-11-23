#include <SD.h>
#include <SPI.h>
#include "TFTv2.h"
File bmpFile;

//unsigned char saved_spimode;
int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;

#define chipSelect 4

//set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
int files = 1000;
char bmpfchar[18];
boolean color18 = true;
boolean feed = true;
void setup()
{
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  TFT_CS_HIGH;
  pinMode(chipSelect, OUTPUT);
  digitalWrite(chipSelect, HIGH);

  Serial.begin(115200);
  SPI.begin();
  Tft.TFTinit();
  if (color18) {
    Tft.sendCMD(0X3A);
    Tft.WRITE_DATA(0x66);
  }
  unsigned char c = Tft.Read_Register(0x0C, 1);
  Serial.println(c, HEX);
  /*
  Tft.sendCMD(0X53);
  Tft.WRITE_DATA(0x6c);
  c = Tft.Read_Register(0x54, 1);
  Serial.println(c, HEX);
  Tft.sendCMD(0X51);
  Tft.WRITE_DATA(0x80);
  c = Tft.Read_Register(0x52, 1);
  Serial.println(c, HEX);
  c = Tft.Read_Register(0xda, 1);
  Serial.println(c, HEX);
  c = Tft.Read_Register(0xd3, 1);
  Serial.println(c, HEX);
  c = Tft.Read_Register(0xd3, 2);
  Serial.println(c, HEX);
  c = Tft.Read_Register(0xd3, 3);
  Serial.println(c, HEX);
  */
  //SPI.setClockDivider(SPI_CLOCK_DIV4);
  //SDcard_info();
  /**/
  DDRB |= 0x04;
  card.init(SPI_FULL_SPEED, chipSelect); //SPI_QUARTER_SPEED   SPI_HALF_SPEED, SPI_FULL_SPEED,
  if (!SD.begin(chipSelect)) //SPI_QUARTER_SPEED,
  { //53 is used as chip select pin
    Serial.println("failed!");
    while (1);
  }
  Serial.println("SD OK!");

  Tft.setCol(0, 239);
  Tft.setPage(0, 319);
  Tft.sendCMD(0x2c);//start to write to display ram
  TFT_BL_ON;
}

void loop()
{
  /*
   */

  for (unsigned char i = 1; i < 140; i++)
  //for (unsigned char i = 140; i > 0; i--)
  {
    //TFT_BL_OFF;
    sprintf(bmpfchar, "%i.bmp", i);
    //bmpf = i + ".bmp";
    //bmpf.toCharArray(bmpfchar,bmpf.length()+1);
    bmpFile = SD.open(bmpfchar);
    if (! bmpFile)
    {
      Serial.println("didnt find image");
      Serial.println(bmpfchar);
      //delay(1000);
      continue;
      //while (1);
    }

    if (! bmpReadHeader(bmpFile)) {
      Serial.println("bad bmp");
      return;
    }

    Serial.print("image size ");
    Serial.print(bmpWidth, DEC);
    Serial.print(", ");
    Serial.println(bmpHeight, DEC);
    int x=0,y = 0;
    y = (320 - bmpHeight)/2;
    x = (240 - bmpWidth)/2;
    bmpdraw(bmpFile, x, y);
    if(bmpWidth%4 == 0) feed = false;
    bmpFile.close();
    //TFT_BL_ON;
    delay(3000);
  }
}

/*********************************************/
// This procedure reads a bitmap and draws it to the screen
// its sped up by reading many pixels worth of data at a time
// instead of just one pixel at a time. increading the buffer takes
// more RAM but makes the drawing a little faster. 20 pixels' worth
// is probably a good place


#define BUFFPIXEL 72

void bmpdraw(File f, int x, int y)
{
  Tft.fillScreen();
  bmpFile.seek(bmpImageoffset);

  uint32_t time = millis();
  uint16_t p;
  uint8_t g, b;
  int i, j;

  uint8_t sdbuffer[3 * BUFFPIXEL];  // 3 * pixels to buffer
  uint8_t buffidx = 3 * BUFFPIXEL;

  for (i = 0; i < bmpHeight; i++)
  {
    for (j = bmpWidth - 1; j >= 0; j--)
    {
      // read more pixels
      if (buffidx >= 3 * BUFFPIXEL)
      {
        bmpFile.read(sdbuffer, 3 * BUFFPIXEL);
        buffidx = 0;
      }

      // convert pixel from 888 to 565
      b = sdbuffer[buffidx++];     // blue
      g = sdbuffer[buffidx++];     // green
      p = sdbuffer[buffidx++];     // red
      if (!color18) {
        p >>= 3;
        p <<= 6;

        g >>= 2;
        p |= g;
        p <<= 5;

        b >>= 3;
        p |= b;
        // write out the 16 bits of color
        Tft.setPixel(239 - (i + y), j + x, p);
      } else {
        Tft.setXY(j + x,319 - (i + y) );
        /*
        Tft.WRITE_DATA(p&0xFC);
        Tft.WRITE_DATA(g&0xFC);
        Tft.WRITE_DATA(b&0xFC);
        */
        TFT_DC_HIGH;
        TFT_CS_LOW;
        SPI.transfer(p&0xFC);
        SPI.transfer(g&0xFC);
        SPI.transfer(b&0xFC);
        TFT_CS_HIGH;
      }
    }
    if(feed){
      char buf[3];
      bmpFile.read(buf,bmpWidth%4);
    }
  }
  //TFT_CS_HIGH;
  Tft.drawString(bmpfchar, 0, 0, 2, 0x00FF);
  delay(100);
  scrollV();
  delay(100);
  Serial.print(millis() - time, DEC);
  Serial.println(" ms");
}

boolean bmpReadHeader(File f) {
  // read header
  uint32_t tmp;

  if (read16(f) != 0x4D42) {
    // magic bytes missing
    return false;
  }

  // read file size
  tmp = read32(f);
  Serial.print("size 0x");
  Serial.println(tmp, HEX);

  // read and ignore creator bytes
  read32(f);

  bmpImageoffset = read32(f);
  Serial.print("offset ");
  Serial.println(bmpImageoffset, DEC);

  // read DIB header
  tmp = read32(f);
  Serial.print("header size ");
  Serial.println(tmp, DEC);
  bmpWidth = read32(f);
  bmpHeight = read32(f);


  if (read16(f) != 1)
    return false;

  bmpDepth = read16(f);
  Serial.print("bitdepth ");
  Serial.println(bmpDepth, DEC);

  if (read32(f) != 0) {
    // compression not supported!
    return false;
  }

  Serial.print("compression ");
  Serial.println(tmp, DEC);

  return true;
}

/*********************************************/
// These read data from the SD card file and convert them to big endian
// (the data is stored in little endian format!)

// LITTLE ENDIAN!
uint16_t read16(File f)
{
  uint16_t d;
  uint8_t b;
  b = f.read();
  d = f.read();
  d <<= 8;
  d |= b;
  return d;
}

// LITTLE ENDIAN!
uint32_t read32(File f)
{
  uint32_t d;
  uint16_t b;

  b = read16(f);
  d = read16(f);
  d <<= 16;
  d |= b;
  return d;
}

void scrollV() {
  Tft.sendCMD(0x33);
  Tft.WRITE_DATA(0);
  Tft.WRITE_DATA(0);
  Tft.WRITE_DATA(0x1);
  Tft.WRITE_DATA(0x40);
  Tft.WRITE_DATA(0);
  Tft.WRITE_DATA(0);
  for (int i = 0; i < 320; i++) {
    Tft.sendCMD(0x37);
    Tft.sendData(i);
    delay(40);
  }
}

