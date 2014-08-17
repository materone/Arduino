// constants won't change. They're used here to 
// set pin numbers:
//const int buttonPin = 7;     // the number of the pushbutton pin
const int a = 1;
const int b = 0;
const int c = 11;
const int d = 12;
const int e = 13;
const int f = 3;
const int g = 5;
const int p = 6;
const int pins[] = {a,b,c,d,e,f,g,p};
//const int pins[] = {0,1,3,5,6,11,12,13};      // the number port of the board pin
//const int pinsLED[] = {6,7,9,10,5,4,2,1};//the pins in LED(bafgpcde)764219a5
//display code lib
const int nd[][7]={
  //0
  {a,b,c,d,e,f},
  //1
  {b,c},
  //2
  {a,b,d,e,g},
  //3
  {a,b,c,d,g},
  //4
  {b,c,f,g},
  //5
  {a,f,g,c,d},
  //6
  {a,f,g,c,d,e},
  //7
  {a,b,c},
  //8
  {a,b,c,d,e,f,g},
  //9
  {a,b,c,d,f,g},
  //p dot
  {p}
};
const int ndsize[]={6,2,5,5,4,5,6,3,7,6,1};

// variables will change:
//int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  //Serial.begin(9600);
  //Serial.println(sizeof(nd[2]));
  for(int i = 0 ;i < 8;i++){
    pinMode(pins[i], OUTPUT); 
  } 
  black();
}

void loop(){
  for(int n = 0;n <=10 ;n++){
    showdig(n);  
    delay(2000);
    black();
    delay(500);
  }
}

void showdig(int num){
  if(num <0 || num >10) return;
  black();  
  for(int i = 0;i < ndsize[num];i++){
    digitalWrite(nd[num][i],HIGH);
  }
}

void black(){
  for(int i=0;i<8;i++){
    digitalWrite(pins[i], LOW); 
  }
}
