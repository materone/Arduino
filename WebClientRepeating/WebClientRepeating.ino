/*
  Repeating Web client
 
 This sketch connects to a a web server and makes a request
 using a Wiznet Ethernet shield. You can use the Arduino Ethernet shield, or
 the Adafruit Ethernet shield, either one will work, as long as it's got
 a Wiznet Ethernet module on board.
 
 This example uses DNS, by assigning the Ethernet client with a MAC address,
 IP address, and DNS address.
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 19 Apr 2012
 by Tom Igoe
 modified 12 Aug 2013
 by Soohwan Kim
 
 http://arduino.cc/en/Tutorial/WebClientRepeating
 This code is in the public domain.
 
 */

#include <SPI.h>
#include <Ethernet.h>

// assign a MAC address for the ethernet controller.
// fill in your address here:
#if defined(WIZ550io_WITH_MACADDRESS) // Use assigned MAC address of WIZ550io
;
#else
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
#endif  
// fill in an available IP address on your network here,
// for manual configuration:
// If you don't specify the IP address, DHCP is used(only in Arduino 1.0 or later).
// fill in an available IP address on your network here,
IPAddress ip(192,168,199,177);
IPAddress gwip(192,168,199,1);
IPAddress snip(255,255,255,0);
IPAddress myDns(192,168,199,1);


// initialize the library instance:
EthernetClient client;

char server[] = "materonep001.sinaapp.com";

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 10*1000;  // delay between updates, in milliseconds
bool flag = false;


void setup() {
  // start serial port:
  Serial.begin(9600);
  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection using a fixed IP address and DNS server:
#if defined(WIZ550io_WITH_MACADDRESS)
  Ethernet.begin(ip, myDns, gwip, snip);
#else
  Ethernet.begin(mac, ip, myDns, gwip, snip);
#endif  
  
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  Serial.println(Ethernet.subnetMask());  
  Serial.println(Ethernet.gatewayIP());
  Serial.println(Ethernet.dnsServerIP());
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval) && !flag) {
    Serial.println("connnn....");
    httpRequest();
    flag = true;
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    //client.println("GET / HTTP/1.1");
    client.print("GET /homestatus.php?t=");
    client.print(18);
    client.print("&h=");
    client.print(88);    
    client.println(" HTTP/1.1");
    client.println("Host: materonep001.sinaapp.com");
    client.println("Connection: close");
    client.println();
    // note the time that the connection was made:
    lastConnectionTime = millis();
    Serial.println("done...");
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}




