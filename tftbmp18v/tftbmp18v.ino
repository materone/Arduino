#include <SD.h>
#include <SPI.h>
#include <TFTv2.h>

#define chipSelect 4
#define BUFFPIXEL 3*200
File bmpFile;
//unsigned char saved_spimode;
int bmpWidth, bmpHeight;
uint8_t bmpDepth, bmpImageoffset;
//set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
uint16_t files = 1000;
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
  uint8_t c = Tft.Read_Register(0x0C, 1);
  Serial.println(c, HEX);
  c = Tft.Read_Register(0x0B, 1);
  Serial.println(c, HEX);

  uint8_t buf[4];
  Tft.rcvData(0x0B, buf, 1);
  for (uint8_t idx = 0; idx < 1; idx++) {
    log(idx);
    Serial.println(buf[idx], HEX);
  }
  Tft.rcvData(0x0C, buf, 1);
  for (uint8_t idx = 0; idx < 1; idx++) {
    log(idx);
    Serial.println(buf[idx], HEX);
  }
  DDRB |= 0x04;
  //SPI_QUARTER_SPEED   SPI_HALF_SPEED, SPI_FULL_SPEED,
  card.init(SPI_FULL_SPEED, chipSelect);
  if (!SD.begin(chipSelect)) //SPI_QUARTER_SPEED,
  { //53 is used as chip select pin
    Serial.println("failed!");
    while (1);
  }
  Serial.println("SD OK!");
  TFT_BL_ON;
}

void loop()
{
  for (uint16_t i = 137; i < 141; i++)
  {
    //TFT_BL_OFF;
    //i = 138;
    sprintf(bmpfchar, "%i.bmp", i);
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
    uint16_t x = 0, y = 0;
    if (bmpWidth < 240);
    x = (240 - bmpWidth) / 2;
    if (bmpHeight < 320)
      y = (320 - bmpHeight) / 2;
    if (bmpWidth % 4 == 0) {
      feed = false;
    } else {
      feed = true;
    }
    bmpdraw(bmpFile, x, y);
    bmpFile.close();
    //TFT_BL_ON;
    delay(1000);
  }
}

/*********************************************/
// This procedure reads a bitmap and draws it to the screen
// its sped up by reading many pixels worth of data at a time
// instead of just one pixel at a time. increading the buffer takes
// more RAM but makes the drawing a little faster. 20 pixels' worth
// is probably a good place

void bmpdraw(File f, uint8_t x, uint8_t y)
{
  Tft.fillScreen();
  bmpFile.seek(bmpImageoffset);

  uint32_t time = millis();
  uint8_t  p, g, b;
  uint16_t i, j;

  uint8_t sdbuffer[BUFFPIXEL];  // 3 * pixels to buffer
<<<<<<< HEAD
  uint16_t buffidx = BUFFPIXEL;

  Tft.setCol(x, bmpWidth + x - 1);
  Tft.setPage(y, bmpHeight + y - 1);

//  uint8_t buf[4];
//  Tft.rcvData(0x09, buf, 4);
//  for (uint8_t idx = 0; idx < 4; idx++) {
//    log(idx);
//    Serial.println(buf[idx], HEX);
//  }
=======
  uint8_t buffidx = BUFFPIXEL;  

  uint8_t buf[4];
  Tft.rcvData(0x09, buf, 4);
  for (uint8_t idx = 0; idx < 4; idx++) {
    log(idx);
    Serial.println(buf[idx], HEX);
  }
  Tft.setCol(x, bmpWidth + x - 1);
  Tft.setPage(y, bmpHeight + y - 1);
>>>>>>> origin/master
  Tft.sendCMD(0x2c);
  TFT_DC_HIGH;
  TFT_CS_LOW;
  for (i = 0; i < bmpHeight; i++)
  {
    for (j = 0; j < bmpWidth; j++)
    {
      // read more pixels
      if (buffidx >= BUFFPIXEL)
      {
        TFT_CS_HIGH;
        bmpFile.read(sdbuffer, BUFFPIXEL);
        buffidx = 0;
        TFT_CS_LOW;
      }
<<<<<<< HEAD
      b = sdbuffer[buffidx++];     // blue
      g = sdbuffer[buffidx++];     // green
      p = sdbuffer[buffidx++];     // red
      SPI.transfer(p );
      SPI.transfer(g ); //&0xFC
      SPI.transfer(b );
    }
=======
      // convert pixel from 888 to 565
      // if(i%9<3){
      //   p=0x0;g=0x0;b=0xFF;
      // }else if(i%9<6){
      //  p=0x0;g=0xFF;b=0x0;
      // }else{
      //   p=0xFF;g=0x0;b=0x0;
      // }
      //      }else{
//      if (i >= bmpHeight / 2 && i < bmpHeight * 2 / 3) {
//        b = 0xFF & sdbuffer[buffidx++];     // blue
//        g = 0xFF & sdbuffer[buffidx++];     // green
//        p = 0xFF & sdbuffer[buffidx++];     // red
//      }else{
       if(i>100) buffidx+=3;
//      }
//      if (i == 0 || i == bmpHeight >> 1) {
//        Serial.print(p, HEX);
//        Serial.print(g, HEX);
//        Serial.print(b, HEX);
//      }      
//      Tft.setXY(j,i);
//      TFT_DC_HIGH;
//  TFT_CS_LOW;
      SPI.transfer(p & 0xFC);
      SPI.transfer(g & 0xFC); //&0xFC
      SPI.transfer(b & 0xFC);
//      TFT_CS_HIGH;
    }
    Serial.println(i);
    //    delay(100);
    //    if (i % 100 == 0)delay(1000);
>>>>>>> origin/master
    //pad last bit,for bmp must 4 * byte per line
    if (feed) {
      uint8_t pad = bmpWidth % 4;
      if (buffidx >=  BUFFPIXEL) {
        TFT_CS_HIGH;
        bmpFile.seek(bmpFile.position() + pad);
        TFT_CS_LOW;
      } else if (pad == 3) {
        buffidx += 3;
      } else {
        memmove(sdbuffer + buffidx, sdbuffer + buffidx + pad, BUFFPIXEL - pad - buffidx);
        TFT_CS_HIGH;
        bmpFile.read(sdbuffer + BUFFPIXEL - pad, pad);
<<<<<<< HEAD
        TFT_CS_LOW;
=======
        //bmpFile.seek(bmpFile.position() + pad);
        if (p + b + g > 0) {
          Serial.print("Some mistake\t");
          Serial.println(i);
        }
        //        Serial.println(bmpFile.position());
>>>>>>> origin/master
      }
    }
  }
  TFT_CS_HIGH;
  char s1[14 + sizeof(bmpfchar)];
  sprintf(s1, "%s %i * %i", bmpfchar, bmpWidth, bmpHeight);
  Tft.drawString(s1, 0, 0, 2, 0xFF00);
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
// These read data from the SD card file and convert them
// to big endian(the data is stored in little endian format!)

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
