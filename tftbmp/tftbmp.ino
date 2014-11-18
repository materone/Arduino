#include <SD.h>
#include <SPI.h>
#include "TFTv2.h"
File bmpFile;

unsigned char saved_spimode;
int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;

#define chipSelect 4

//set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

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
  unsigned char c = Tft.Read_Register(0x0C, 1);
  Serial.println(c, HEX);
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
  char bmpfiles[][18] =
  {
    "1.bmp", "2.bmp", "3.bmp",
    "4.bmp", "5.bmp", "6.bmp",
    "7.bmp", "8.bmp", "9.bmp",
    "10.bmp", "11.bmp", "12.bmp",
    "13.bmp", "14.bmp"
  };
  for (unsigned char i = 0; i < 14; i++)
  {
    //TFT_BL_OFF;
    bmpFile = SD.open(bmpfiles[i]);
    if (! bmpFile)
    {
      Serial.println("didnt find image");
      while (1);
    }

    if (! bmpReadHeader(bmpFile)) {
      Serial.println("bad bmp");
      return;
    }

    Serial.print("image size ");
    Serial.print(bmpWidth, DEC);
    Serial.print(", ");
    Serial.println(bmpHeight, DEC);
    int y = 0;
    if (bmpHeight <= 240) {
      y = (240 - bmpHeight) / 2;
    }
    bmpdraw(bmpFile, 0, y);
    bmpFile.close();
    //TFT_BL_ON;
    delay(5000);
  }
}

/*********************************************/
// This procedure reads a bitmap and draws it to the screen
// its sped up by reading many pixels worth of data at a time
// instead of just one pixel at a time. increading the buffer takes
// more RAM but makes the drawing a little faster. 20 pixels' worth
// is probably a good place


#define BUFFPIXEL 60

void bmpdraw(File f, int x, int y)
{
  bmpFile.seek(bmpImageoffset);

  uint32_t time = millis();
  uint16_t p;
  uint8_t g, b;
  int i, j;

  uint8_t sdbuffer[3 * BUFFPIXEL];  // 3 * pixels to buffer
  uint8_t buffidx = 3 * BUFFPIXEL;


  for (i = 0; i < bmpHeight; i++)
  {

    for (j = bmpWidth; j > 0; j--)
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

      p >>= 3;
      p <<= 6;

      g >>= 2;
      p |= g;
      p <<= 5;

      b >>= 3;
      p |= b;

      // write out the 16 bits of color
      Tft.setPixel(239 - (i + y), j + x, p);
    }
  }
  delay(2000);
  scrollV();
  delay(2000);
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

