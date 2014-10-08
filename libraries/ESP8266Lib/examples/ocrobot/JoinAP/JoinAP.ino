/*Join wifi AP*/
#include <uarlWIFI_328.h>
WIFI wifi;
void setup() {
  Serial.begin(9600);
  Serial.println("Power Go");
  delay(2000);
  wifi.confMode(STA);  // STA or  AP or  AP+STA
  Serial.println("STA Go");
  wifi.Reset();     //reboot wifi Modules
  Serial.println("Reboot Go");
 wifi.confJAP("SSID","PASSWORD");   //Join AP  SSID is You home wifi SSID  PASSWORLD is you home passworld
 delay(5000);                       //delay DHCP IP
   Serial.println(wifi.showIP());  // print Modules IP
}
void loop() {

}