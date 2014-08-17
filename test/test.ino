// constants won't change. They're used here to 
// set pin numbers:
//const int buttonPin = 7;     // the number of the pushbutton pin

const int pins[] = {0,1,3,5,6,11,12,13};      // the number port of the board pin

// variables will change:
//int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  for(int i = 0 ;i < 8;i++){
    pinMode(pins[i], OUTPUT); 
  } 
}

void loop(){
  for(int n = 0;n <10 ;n++){
    for(int i = 0 ;i < 8;i++){
      digitalWrite(pins[i], HIGH); 
    }   
    delay(5000);
    for(int i = 0 ;i < 8;i++){
      digitalWrite(pins[i], LOW); 
    } 
    delay(2000);
  }
}
