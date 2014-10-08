/*show you home wifi AP*/
#include <uarlWIFI_328.h>
WIFI wifi;
void setup() {
  Serial.begin(9600);
  Serial.println("power go");
  delay(2000);
  wifi.confMode(STA);  // STA or  AP or  AP+STA
  Serial.println("STA go");
  wifi.Reset();
  Serial.println("reboot go");
  Serial.println(wifi.showAP()); //print wifi
}
void loop() {

}
