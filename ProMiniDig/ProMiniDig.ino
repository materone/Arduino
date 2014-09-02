//设置阴极接口
int a = 11;
int b = 7;
int c = 4;
int d = 2;
int e = 3;
int f = 10;
int g = 5;
int p = 1;
//设置阳极接口
int d4 = 6;
int d3 = 8;
int d2 = 9;
int d1 = 12;
//设置变量
long n = 123;
int x = 100;
int del = 55;  //此处数值对时钟进行微调 55
 
byte segs[7] = { a, b, c, d, e, f, g };
 
byte seven_seg_digits[10][7] = { { 0,0,0,0,0,0,1 },  // = 0
                                 { 1,0,0,1,1,1,1 },  // = 1
                                 { 0,0,1,0,0,1,0 },  // = 2
                                 { 0,0,0,0,1,1,0 },  // = 3
                                 { 1,0,0,1,1,0,0 },  // = 4
                                 { 0,1,0,0,1,0,0 },  // = 5
                                 { 0,1,0,0,0,0,0 },  // = 6
                                 { 0,0,0,1,1,1,1 },  // = 7
                                 { 0,0,0,0,0,0,0 },  // = 8
                                 { 0,0,0,0,1,0,0 }   // = 9
                             }; 
 
void setup()
{
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(p, OUTPUT);
}

void loop()
{
  clearLEDs();
  pickDigit(1);
  lightSegments((n/x/1000)%10);
  delayMicroseconds(del);
 
  clearLEDs();
  pickDigit(2);
  lightSegments((n/x/100)%10);
  delayMicroseconds(del);
 
  clearLEDs();
  pickDigit(3);
  dispDec(3);
  lightSegments((n/x/10)%10);
  delayMicroseconds(del);
 
  clearLEDs();
  pickDigit(4);
  lightSegments(n/x%10);
  delayMicroseconds(del);
 
  n++;
 
//  if (digitalRead(13) == HIGH)
//  {
//    n = 0;
//  }
}
 
void pickDigit(int x)  //定义pickDigit(x),其作用是开启dx端口
{
  digitalWrite(d1, LOW);
  digitalWrite(d2, LOW);
  digitalWrite(d3, LOW);
  digitalWrite(d4, LOW);
 
  switch(x)
  {
  case 1: 
    digitalWrite(d1, HIGH); 
    break;
  case 2: 
    digitalWrite(d2, HIGH); 
    break;
  case 3: 
    digitalWrite(d3, HIGH); 
    break;
  default: 
    digitalWrite(d4, HIGH); 
    break;
  }
}
 
void dispDec(int x)  //设定开启小数点
{
  digitalWrite(p, LOW);
}
 
void clearLEDs()  //清屏
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(p, HIGH);
}
 
 
// 点亮对应数字的数码管
void lightSegments(int x) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segs[i], seven_seg_digits[x][i]);
  }
}

