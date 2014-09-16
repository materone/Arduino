char c;
String data;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(1000);
  Serial.println("Begin Test");
  Serial1.println("AT+RST");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    data = "";
    while (Serial.available()) {
      c = Serial.read();
      Serial.println(c);
      data += c;
    }
    Serial.println(data);
    Serial1.println(data);
  }
  delay(500);
  if (Serial1.available()) {
    data = "";
    Serial.print("Get Wifi response:");
    while (Serial1.available()) {
      c = Serial1.read();
      Serial.println(c);
    }
    Serial.println();
  }
  delay(1000);
}
