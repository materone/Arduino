int timeWidth=0;
int dataOut=0;
int lostData=0;
int flag=0;
int P0=0;
int P1=0;
int P2=0;
int P3=0;
int P4=0;
int BPChour=0;
int BPCminuate=0;
int BPCsecond;
int BPCweek=0;
int BPCday=0;
int BPCmonth=0;
int BPCyear=0;
int BPCArray[19]={
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 
void setup() {
  Serial.begin(9600);
}
void loop() {
  int sensorValue = analogRead(A0);
  if(sensorValue<100){                //低位触发计数开始
    timeWidth++; 
    lostData=0;
  }
  else if(timeWidth!=0){              //高位输出，如果不为空，则输出脉宽信息
    dataOut=timeWidth-1; 
    //        Serial.print(dataOut);      //输出原始数据
    BPCArray[flag]=dataOut;
    flag++;
    timeWidth=0;
  }
  else{                               //低电平计时数开始
    BPCArray[18]=dataOut;
    lostData++; 
    if(lostData>12){                  //判断是否为1S缺秒脉冲标志，是则换行
      Serial.println("");
      //////////////////////////////////////////////////////////////////////////////
      int i;
      for (i = 0; i < 19; i = i + 1) {
        Serial.print(BPCArray[i]);  ////统一输出存储数据
      }
      Serial.println("");
      P1=BPCArray[0];
      P2=BPCArray[1];
      P3=BPCArray[9];
      P4=BPCArray[18];
 
      if(BPCArray[0]*20+20==60){
        BPCsecond=0;
        BPCminuate=BPCArray[4]*16+BPCArray[5]*4+BPCArray[6]+1;
      }
      else{
        BPCsecond=BPCArray[0]*20+20;
        BPCminuate=BPCArray[4]*16+BPCArray[5]*4+BPCArray[6];
      }      
      if(BPCminuate==60){
        BPChour=BPCArray[2]*4+BPCArray[3]+1;
        BPCminuate=0;
      }
      else{
        BPChour=BPCArray[2]*4+BPCArray[3];
      }
      if(BPChour==0){         //noon 12
        BPChour=12;
      }
      BPCweek=BPCArray[7]*4+BPCArray[8];
      BPCday=BPCArray[10]*16+BPCArray[11]*4+BPCArray[12];
      BPCmonth=BPCArray[13]*4+BPCArray[14];
      BPCyear=2000+BPCArray[15]*16+BPCArray[16]*4+BPCArray[17];
      Serial.print(BPCyear);
      Serial.print("-");
      Serial.print(BPCmonth);
      Serial.print("-");
      Serial.print(BPCday);
      Serial.print(" Week:");
      Serial.print(BPCweek);
      Serial.print(" Time: ");
      Serial.print(BPChour);
      Serial.print(":");
      Serial.print(BPCminuate);
      Serial.print(":");
      Serial.print(BPCsecond);
 
      if(P3>1){
        Serial.print("PM");
      }
      else{
        Serial.print("AM");
      }
      Serial.println("");
      Serial.println("------------------------------------");
 
      ///////////////////////////////////////////////////////////////////////////
      flag=0;
      lostData=0;    
    }
  }
  delay(100);        //采样时间。 取这个值抗干扰能力最好，如果取小反而不易处理
}

