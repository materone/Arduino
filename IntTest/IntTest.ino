
volatile int state = LOW;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  attachInterrupt(1, blink, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(true){
    Serial.print("IN Main Loop  ");
    Serial.println(state);
    delay(3000);
  }
}

void blink(){
  state = !state;
  Serial.println("The Switch is pressed!!!");
}
