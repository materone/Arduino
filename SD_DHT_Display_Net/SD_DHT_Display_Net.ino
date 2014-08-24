#include <dht.h>
#include <Wire.h>
#include <SeeedOLED.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include <Ethernet.h>

#include <SD.h>

// set up variables using the SD utility library functions:
//Sd2Card card;
//SdVolume volume;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 4;

static unsigned char fontDatas[11][32] PROGMEM={
  //超
  {
    0x40,0x48,0x48,0xFF,0x48,0x48,0x82,0x42,
    0x3E,0x02,0x22,0x42,0x3F,0x02,0x00,0x00,
    0x40,0x3F,0x10,0x1F,0x22,0x42,0x40,0x5F,
    0x51,0x51,0x51,0x51,0x5F,0x60,0x20,0x00
  }
  ,
  //声
  {
    0x04,0x14,0xD4,0x54,0x54,0x54,0x54,0xDF,  
    0x54,0x54,0x54,0x54,0xF4,0x56,0x04,0x00,  
    0x80,0x60,0x1F,0x02,0x02,0x02,0x02,0x03,  
    0x02,0x02,0x02,0x02,0x07,0x00,0x00,0x00     
  }
  ,
  //波
  {
    0x20,0x42,0x04,0x0C,0x80,0xF8,0x88,0x88,
    0x88,0xFF,0x88,0x88,0xA8,0x18,0x00,0x00,
    0x04,0x04,0xFE,0x41,0x30,0x8F,0x40,0x22,
    0x14,0x08,0x14,0x22,0x41,0xC0,0x40,0x00
  }
  ,
  //距
  {
    0x00,0x3E,0x22,0xE2,0x22,0x3E,0x00,0xFE,
    0x22,0x22,0x22,0x22,0xF3,0x22,0x00,0x00,
    0x20,0x7F,0x20,0x3F,0x12,0x12,0x00,0x7F,
    0x42,0x42,0x42,0x42,0x47,0x60,0x40,0x00
  }
  ,
  //离
  {
    0x04,0x04,0x04,0xF4,0x04,0x8C,0x55,0x26,
    0x54,0x8C,0x04,0xF4,0x04,0x06,0x04,0x00,
    0x00,0xFC,0x04,0x05,0x25,0x35,0x2D,0x27,
    0x25,0x35,0x25,0x45,0x84,0x7E,0x04,0x00
  }
  ,
  //探
  {
    0x10,0x10,0x10,0xFF,0x90,0x58,0x06,0x22,
    0x12,0x0A,0xC2,0x0A,0x12,0xAA,0x06,0x00,
    0x02,0x42,0x81,0x7F,0x00,0x00,0x21,0x11,
    0x09,0x05,0xFF,0x05,0x09,0x31,0x11,0x00
  }
  ,
  //测
  {
    0x10,0x22,0x6C,0x00,0x80,0xFC,0x04,0xF4,
    0x04,0xFE,0x04,0xF8,0x00,0xFE,0x00,0x00,
    0x04,0x04,0xFE,0x01,0x40,0x27,0x10,0x0F,
    0x10,0x67,0x00,0x47,0x80,0x7F,0x00,0x00
  }
  ,
  //仪
  {
    0x80,0x40,0x20,0xF8,0x07,0x00,0x38,0xC0,
    0x02,0x04,0x0C,0x80,0x60,0x1C,0x00,0x00,
    0x00,0x00,0x00,0xFF,0x80,0x40,0x20,0x10,
    0x0B,0x04,0x0A,0x11,0x60,0xC0,0x40,0x00
  },
  //wen
  {
    0x10,0x22,0x64,0x0C,0x80,0x00,0xFE,0x92,
    0x92,0x92,0x92,0x92,0xFF,0x02,0x00,0x00,
    0x04,0x04,0xFE,0x01,0x40,0x7E,0x42,0x42,
    0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00
  },
  //shi
  {
    0x10,0x22,0x64,0x0C,0x80,0xFE,0x92,0x92,
    0x92,0x92,0x92,0x92,0xFF,0x02,0x00,0x00,
    0x04,0x04,0xFE,0x41,0x44,0x48,0x50,0x7F,
    0x40,0x40,0x7F,0x50,0x48,0x64,0x40,0x00
  },
  //du
  {
    0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0xA5,
    0xA6,0xA4,0xFC,0x24,0x34,0x26,0x04,0x00,
    0x40,0x20,0x9F,0x80,0x42,0x42,0x26,0x2A,
    0x12,0x2A,0x26,0x42,0x40,0xC0,0x40,0x00
  }
};

//0-9 :
static unsigned char fontDigital[11][32] PROGMEM = {
  //0
  {
    0x00,0x00,0x00,0x00,0xF0,0xF8,0x0C,0x04,
    0x04,0x04,0x0C,0xF8,0xF0,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x0F,0x1F,0x30,0x20,
    0x20,0x20,0x30,0x1F,0x0F,0x00,0x00,0x00
  }
  ,
  //1
  {
    0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xF8,
    0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,
    0x3F,0x20,0x00,0x00,0x00,0x00,0x00,0x00
  }
  ,
  //2
  {
    0x00,0x00,0x00,0x00,0x30,0x38,0x0C,0x04,
    0x04,0x0C,0xF8,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x20,0x30,0x38,0x2C,
    0x26,0x23,0x21,0x38,0x00,0x00,0x00,0x00
  }
  ,
  //3
  {
    0x00,0x00,0x00,0x00,0x10,0x18,0x0C,0x84,
    0x84,0xCC,0x78,0x30,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x08,0x18,0x30,0x20,
    0x20,0x31,0x1F,0x0E,0x00,0x00,0x00,0x00 
  }
  ,
  //4
  {
    0x00,0x00,0x00,0x00,0x80,0xC0,0x70,0x18,
    0xFC,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x06,0x07,0x05,0x04,0x24,
    0x3F,0x3F,0x24,0x04,0x00,0x00,0x00,0x00
  }
  ,
  //5
  {
    0x00,0x00,0x00,0x00,0xFC,0xFC,0xC4,0x44,
    0x44,0xC4,0x84,0x04,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x08,0x18,0x30,0x20,
    0x20,0x30,0x1F,0x0F,0x00,0x00,0x00,0x00
  }
  ,
  //6
  {
    0x00,0x00,0x00,0x00,0xF0,0xF8,0x8C,0x44,
    0x44,0xCC,0x98,0x10,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x0F,0x1F,0x30,0x20,
    0x20,0x30,0x1F,0x0F,0x00,0x00,0x00,0x00
  }
  ,
  //7
  {
    0x00,0x00,0x00,0x00,0x1C,0x0C,0x0C,0x0C,
    0xCC,0xEC,0x3C,0x1C,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x3F,
    0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00
  }
  ,
  //8
  {
    0x00,0x00,0x00,0x00,0x30,0x78,0xCC,0x84,
    0x84,0xCC,0x78,0x30,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x0E,0x1F,0x31,0x20,
    0x20,0x31,0x1F,0x0E,0x00,0x00,0x00,0x00
  }
  ,
  //9
  {
    0x00,0x00,0x00,0x00,0xF0,0xF8,0x0C,0x04,
    0x04,0x0C,0xF8,0xF0,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x08,0x19,0x33,0x22,
    0x22,0x33,0x1F,0x0F,0x00,0x00,0x00,0x00
  }
  ,
  //:
  {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x33,0x33,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
  }
};

dht DHT;
#define DHT11_PIN 3 //put the sensor in the digital pin 3
long t,h;
#if defined(WIZ550io_WITH_MACADDRESS) // Use assigned MAC address of WIZ550io
;
#else
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
#endif  
IPAddress ip(192,168,199,177);
IPAddress gwip(192,168,199,1);
IPAddress snip(255,255,255,0);
IPAddress myDns(192,168,199,1);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);
EthernetClient webClient;

char reqserver[] = "materonep001.sinaapp.com";
unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 60*60*1000;  // delay between updates, in milliseconds
boolean first = true;
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
    SeeedOled.drawBitmap(fontDatas[i],16);
  }
  //p1
  for(int i = 0 ;i< 8;i++){
    SeeedOled.drawBitmap(fontDatas[i] + 16,16);
  }
  //wendu
  SeeedOled.setBitmapRect(3,4,16,47);
  SeeedOled.drawBitmap(fontDatas[8],16);
  SeeedOled.drawBitmap(fontDatas[10],16);
  SeeedOled.drawBitmap(fontDatas[8] + 16,16);
  SeeedOled.drawBitmap(fontDatas[10] + 16,16);
  
  //shidu  
  SeeedOled.setBitmapRect(6,7,16,47);
  SeeedOled.drawBitmap(fontDatas[9],16);
  SeeedOled.drawBitmap(fontDatas[10],16);
  SeeedOled.drawBitmap(fontDatas[9] + 16,16);
  SeeedOled.drawBitmap(fontDatas[10] + 16,16);
  
  //init network
  #if defined(WIZ550io_WITH_MACADDRESS)
  Ethernet.begin(ip, myDns, gwip, snip);
  #else
    Ethernet.begin(mac, ip, myDns, gwip, snip);
  #endif  
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.println(Ethernet.subnetMask());  
  Serial.println(Ethernet.gatewayIP());
  Serial.println(Ethernet.dnsServerIP());
  
}
void sdcard(){
  /*
  //init SDCard
  pinMode(10, OUTPUT);     // change this to 53 on a mega


  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  Serial.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  */
}

void loop()
{
  //testTemperature();
  //delay(100);
  //testHumidity();
  //delay(100);
  
  startWebSvr();
  if((millis() - lastConnectionTime) > postingInterval|| first){
    webReq();
    first = false;
  }
}
void startWebSvr(){
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          testTemperature();
          testHumidity();
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("Temp:");
          client.print(t);
          client.println("<BR>");
          client.print("Humi:");
          client.print(h);
          client.println("<BR>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
    webReq();
  }
}

void webReq(){
  //upload data
  if (webClient.connect(reqserver, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    webClient.print("GET /homestatus.php?t=");
    webClient.print(t);
    webClient.print("&h=");
    webClient.print(h);    
    webClient.println(" HTTP/1.1");
    webClient.println("Host: materonep001.sinaapp.com");
    webClient.println("User-Agent: arduino-ethernet");
    webClient.println("Connection: close");
    webClient.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("web connection failed");
    Serial.println("web disconnecting.");
    webClient.stop();
  }  
  delay(2000);
  while (webClient.available()) {
    char c = webClient.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!webClient.connected() ) {
    Serial.println();
    Serial.println("web disconnecting.");
    webClient.stop();
  }
  //delay(60*60*1000);
}

void testTemperature(){
  //begin measure d and t
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case 0:  Serial.print("OK,\t"); break;
    case -1: Serial.print("Checksum error,\t"); break;
    case -2: Serial.print("Time out error,\t"); break;
    default: Serial.print("Unknown error,\t"); break;
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
  t = long_num;

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
    SeeedOled.drawBitmap(fontDigital[0],32);
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
      //Serial.print(char_buffer[i-1]);       
      SeeedOled.setBitmapRect(3,4,112-i*16,128-i*16);
      SeeedOled.drawBitmap(fontDigital[char_buffer[i-1]],32);      
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
    case 0:  Serial.print("OK,\t"); break;
    case -1: Serial.print("Checksum error,\t"); break;
    case -2: Serial.print("Time out error,\t"); break;
    default: Serial.print("Unknown error,\t"); break;
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
  h = long_num;

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
    SeeedOled.drawBitmap(fontDigital[0],32);
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
      //Serial.print(char_buffer[i-1]);       
      SeeedOled.setBitmapRect(6,7,112-i*16,128-i*16);
      SeeedOled.drawBitmap(fontDigital[char_buffer[i-1]],32);      
    }
  }
  //SeeedOled.sendCommand(SeeedOLED_Display_On_Cmd); 	//display off
}

