
/*===============================
ESP8266 UART WiFi Module Testing...
================================*/
#include "esp8266.h"
#include "DHT.h"

esp8266 wifi;
unsigned long timeBegin, timeLast, timeInterval = 2000;

int DHT22_PIN = 2;
int LED_PIN = 13;
int MOISTURE_PIN = A0;

double VoltageRatio = 0.0048828125; // 5/1024
String m_strOid = "50102"; //10001
DHT m_DHT22;
char m_pstrDouble2String[15];

void setup()
{
  pinMode( DHT22_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Initialize Serial @9600bps.");
  wifi.init(9600, 10, 11);
  Serial.println("Initialize SoftSerial @9600bps.");

  //delay(1000);
  //wifi.GetInfo();
  //wifi.startServer("HiWiFi_SMT","87654321",8888);
  wifi.configMode(1);	delay(200);
  Serial.println("IPServer will set CIPMUX=1.");
  wifi.configMux(1); delay(200);

  Serial.println("Will Close IPServer if already started.");
  wifi.stopServer(8888);
  wifi.startServer(8888);

  timeBegin = millis();
}

void loop()
{
  //wifi.bridge();

  setupTimer();
  wifi.xCommander();
  if (wifi.xReceiver() == false)	return;

  String strData = wifi.onReceive();
  if (strData == "") return;

  //doingSome();
  //check the received data of wifi.
  //...
}

void doingSome(String strCMD)
{
  Serial.print("\rDoing...");
  Serial.println(strCMD);
}

void setupTimer()
{
  timeLast = millis();
  if (timeLast < timeBegin)	//overflow.
    timeBegin = millis();
  if (timeLast - timeBegin > timeInterval)
  {
    onTimer();
    timeBegin = timeLast;
  }
}

//====================
void onTimer()
{
  Serial.println("OnTimer...");
  Serial.println(Read_DHT());
  //wifi.Send(0,"Arduino Timer...");
  wifi.Send(0, Read_DHT());
}

String Read_DHT()
{
  String strMsg = "";
  int nRetCode = m_DHT22.read11( DHT22_PIN );
  if ( nRetCode == DHTLIB_OK )
  {
    dtostrf( m_DHT22.humidity, 0, 2, m_pstrDouble2String );
    strMsg = "H:" + String(m_pstrDouble2String);

    dtostrf( m_DHT22.temperature, 0, 2, m_pstrDouble2String );
    strMsg = strMsg + ",T:" + String(m_pstrDouble2String);
  }
  String strData = "OID:" + m_strOid + "," + strMsg;	// + "," + Read_Moisture();
  return strData;
}

