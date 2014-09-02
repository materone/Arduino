// -----------------------------------------------------
// Arduino ShowInfo
// Show what the Arduino has to tell you.
//
// 10 April 2012: Extra speed tests,
//                and a few other additions.
// 17 April 2012: The name 'ADCW' is preferred over 'ADC'.
// 23 April 2012: Changed PB5 into PORTB5.
// 1 July 2012  : Added i2c_scanner.
//                More speed tests.
// 21 July 2012 : Added divide speed tests.
// 25 July 2012 : Added sketchSize.
// 26 July 2012 : Changed sketchSize for more than 64kB.
//                Also show sketchSize in percentage.
//                Added processor defines.
//                Arduino Mega compatible (not tested).
// -----------------------------------------------------

#include <avr/boot.h>
#include <Wire.h>

void setup() 
{
  // The system led is present on most Arduino boards.
  pinMode(13,OUTPUT);

  Serial.begin(9600);

  Serial.println(F("Arduino ShowInfo"));
  Serial.println(F("Show what the Arduino has to tell you."));

  ShowMenu();
}


void loop() 
{
  int incomingByte, i, n;

  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();

    switch (incomingByte)
    {
      case '0':
        Serial.println(F("System LED off"));
        digitalWrite(13,LOW);
        Serial.println(F("-----------"));
        break;
      case '1':
        Serial.println(F("System LED on"));
        digitalWrite(13,HIGH);
        Serial.println(F("-----------"));
        break;
      case '2':
        i2c_scanner();
        break;
      case 'i':
        Information();
        break;
      case 'r':
        Serial.println(F("Test serial communcation"));
        Serial.println(F("Will your terminal program send characters immediately?"));
        Serial.println(F("Enter characters, followed by Enter"));
        for (i=0; i<25; i++)
        {
          Serial.print(F("Serial.available() = "));
          n = Serial.available();
          Serial.print(n,DEC);
          Serial.print(F("    Serial.peek() = "));
          n = Serial.peek();
          Serial.println(n,DEC);
          delay(500);
        }
        // empty the buffer of incoming data
        while (Serial.available() > 0)
          Serial.read();
        Serial.println(F("-----------"));
        break;
      case 's':
        SpeedTest();
        break;
      case 't':
        TimerRegisterDump();
        break;
      case 'h':
      case '?':
        ShowMenu();
        break;
      // Ignore some characters, like the carriage return and line feed.
      case '\'':
      case '\r':
      case '\n':
        break;
      default:
        Serial.print(F("Unknown command: 0x"));
        Serial.println(incomingByte,HEX);
        Serial.println(F("Type '?' followed by Enter"));
        // Something could be wrong, delete incoming data in buffer
        while (Serial.available() > 0)
          Serial.read();
        Serial.println(F("-----------"));
        break;
    }
  }
}



void ShowMenu(void)
{
  Serial.println(F(""));
  Serial.println(F("Menu"));
  Serial.println(F("-----------"));
  Serial.println(F("0 = System LED off"));
  Serial.println(F("1 = System LED on"));
  Serial.println(F("2 = i2c_scanner"));
  Serial.println(F("i = Show information"));
  Serial.println(F("h = Show menu"));
  Serial.println(F("r = Test serial communication"));
  Serial.println(F("s = Speed tests"));
  Serial.println(F("t = Timer Register Dump"));
  Serial.println(F("? = Show menu"));
  Serial.println(F(""));
  Serial.println(F("Type command, followed by Enter"));
}



double GetTemp(void)
{
  unsigned int wADC;
  double t;

  // The internal temperature has to be used 
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with 
  // the analogRead function yet.

  // This code is not valid for the Arduino Mega,
  // and the Arduino Mega 2560.

#ifdef THIS_MIGHT_BE_VALID_IN_THE_FUTURE
  analogReference (INTERNAL);
  delay(20);            // wait for voltages to become stable.
  wADC = analogRead(8); // Channel 8 is temperature sensor.
#else
  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
#if defined (__AVR_ATmega32U4__)
  wADC = ADC;      // For Arduino Leonardo
#else
  wADC = ADCW;     // 'ADCW' is preferred over 'ADC'
#endif
#endif

  // The offset of 337.0 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  return (t);
}



// Helper function for free ram.
//   With use of http://playground.arduino.cc/Code/AvailableMemory
//
int freeRam(void)
{
  extern unsigned int __heap_start;
  extern void *__brkval;

  int free_memory;
  int stack_here;

  if (__brkval == 0)
    free_memory = (int) &stack_here - (int) &__heap_start;
  else
    free_memory = (int) &stack_here - (int) __brkval; 

  return (free_memory);
}



// Helper function for sketch size.
// The sketch size is runtime calculated.
// From user "Coding Badly" in his post:
//   http://arduino.cc/forum/index.php/topic,115870.msg872309.html#msg872309
// Changed into unsigned long for code size larger than 64kB.
//
// This function returns the sketch size 
// for a size between 0 and 32k. If the code
// size is larger (for example with an Arduino Mega),
// the return value is not valid.
//
unsigned long sketchSize(void)
{
  extern int _etext;
  extern int _edata;

  return ((unsigned long)(&_etext) + ((unsigned long)(&_edata) - 256L));
}



void Information(void)
{
  int i,j;
  int data1,data2,data3,data4;
  unsigned long ul;
  double percentage;

  Serial.println(F(""));
  Serial.println(F("Information"));
  Serial.println(F("-----------"));

  Serial.print(F("sketch Size = "));
  ul = sketchSize();
  Serial.print(ul,DEC);
  Serial.print(F(" ("));
  percentage = (double) ul / ((double) FLASHEND + 1.0) * 100.0;
  Serial.print(percentage,0);
  Serial.println(F("%)"));

  Serial.print(F("free RAM    = "));
  i = freeRam();
  Serial.println(i,DEC);
  Serial.print(F("RAM used    = "));
  j = (RAMEND + 1) - i;
  Serial.print(j, DEC);
  Serial.print(F(" ("));
  percentage = (double) j / ((double) RAMEND + 1.0) * 100.0;
  Serial.print(percentage,0);
  Serial.println(F("%)"));

  Serial.print(F("__VERSION__ = "));
  Serial.println(F(__VERSION__));

  Serial.print(F("__DATE__    = "));
  Serial.println(F(__DATE__));

  Serial.print(F("__TIME__    = "));
  Serial.println(F(__TIME__));

  Serial.print(F("__AVR_LIBC_VERSION_STRING__ = "));
  Serial.println(F(__AVR_LIBC_VERSION_STRING__));

  Serial.print(F("__FILE__    = "));
  Serial.println(F(__FILE__));

  Serial.print(F("__STDC__    = "));
  Serial.println(__STDC__,DEC);

  Serial.print(F("OSCCAL = "));
  Serial.println(OSCCAL,DEC);

  Serial.print(F("GPIOR0 = 0x"));
  Serial.println(GPIOR0,HEX);

  Serial.print(F("GPIOR1 = 0x"));
  Serial.println(GPIOR1,HEX);

  Serial.print(F("GPIOR1 = 0x"));
  Serial.println(GPIOR1,HEX);

  Serial.print(F("RAMEND   = 0x"));
  Serial.println(RAMEND,HEX);

  Serial.print(F("XRAMEND  = 0x"));
  Serial.println(XRAMEND,HEX);

  Serial.print(F("E2END    = 0x"));
  Serial.println(E2END,HEX);

  Serial.print(F("FLASHEND = 0x"));
  Serial.println(FLASHEND,HEX);

  cli();
  data1 = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
  data2 = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
  data3 = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
  data4 = boot_lock_fuse_bits_get(GET_LOCK_BITS);
  sei();

  Serial.print(F("LOW FUSE      = 0x"));
  Serial.println(data1,HEX);

  Serial.print(F("HIGH FUSE     = 0x"));
  Serial.println(data2,HEX);

  Serial.print(F("EXTENDED FUSE = 0x"));
  Serial.println(data3,HEX);

  Serial.print(F("LOCK BITS     = 0x"));
  Serial.println(data4,HEX);

  Serial.print(F("Processor according to compiler  = "));
#if defined (__AVR_ATtiny45__)
  Serial.println(F("__AVR_ATtiny45__"));
#elif defined (__AVR_ATtiny85__)
  Serial.println(F("__AVR_ATtiny85__"));
#elif defined (__AVR_ATtiny2313__)
  Serial.println(F("__AVR_ATtiny2313__"));
#elif defined (__AVR_ATtiny2313A__)
  Serial.println(F("__AVR_ATtiny2313A__"));
#elif defined (__AVR_ATmega48__)
  Serial.println(F("__AVR_ATmega48__"));
#elif defined (__AVR_ATmega48A__)
  Serial.println(F("__AVR_ATmega48A__"));
#elif defined (__AVR_ATmega48P__)
  Serial.println(F("__AVR_ATmega48P__"));
#elif defined (__AVR_ATmega8__)
  Serial.println(F("__AVR_ATmega8__"));
#elif defined (__AVR_ATmega8U2__)
  Serial.println(F("__AVR_ATmega8U2__"));
#elif defined (__AVR_ATmega88__)
  Serial.println(F("__AVR_ATmega88__"));
#elif defined (__AVR_ATmega88A__)
  Serial.println(F("__AVR_ATmega88A__"));
#elif defined (__AVR_ATmega88P__)
  Serial.println(F("__AVR_ATmega88P__"));
#elif defined (__AVR_ATmega88PA__)
  Serial.println(F("__AVR_ATmega88PA__"));
#elif defined (__AVR_ATmega16__)
  Serial.println(F("__AVR_ATmega16__"));
#elif defined (__AVR_ATmega168__)
  Serial.println(F("__AVR_ATmega168__"));
#elif defined (__AVR_ATmega168A__)
  Serial.println(F("__AVR_ATmega168A__"));
#elif defined (__AVR_ATmega168P__)
  Serial.println(F("__AVR_ATmega168P__"));
#elif defined (__AVR_ATmega32__)
  Serial.println(F("__AVR_ATmega32__"));
#elif defined (__AVR_ATmega328__)
  Serial.println(F("__AVR_ATmega328__"));
#elif defined (__AVR_ATmega328P__) 
  Serial.println(F("__AVR_ATmega328P__"));
#elif defined (__AVR_ATmega32U2__)
  Serial.println(F("__AVR_ATmega32U2__"));
#elif defined (__AVR_ATmega32U4__)
  Serial.println(F("__AVR_ATmega32U4__"));
#elif defined (__AVR_ATmega32U6__)
  Serial.println(F("__AVR_ATmega32U6__"));
#elif defined (__AVR_ATmega128__)
  Serial.println(F("__AVR_ATmega128__"));
#elif defined (__AVR_ATmega1280__)
  Serial.println(F("__AVR_ATmega1280__"));
#elif defined (__AVR_ATmega2560__)
  Serial.println(F("__AVR_ATmega2560__"));
#endif

#ifdef SIGRD
  Serial.print(F("SIGRD = "));
  Serial.println(SIGRD,DEC);
#else
  Serial.println(F("SIGRD : not defined (let's make it 5 and see what happens)."));
#define SIGRD 5
#endif

  Serial.print(F("Signature = 0x"));

  data1 = boot_signature_byte_get(0x00);
  data2 = boot_signature_byte_get(0x02);
  data3 = boot_signature_byte_get(0x04);
  data4 = boot_signature_byte_get(0x01);

  Serial.print(data1,HEX);
  Serial.print(F(", 0x"));
  Serial.print(data2,HEX);
  Serial.print(F(", 0x"));
  Serial.println(data3,HEX);

  Serial.print(F("calibration = "));
  Serial.println(data3,DEC);

  Serial.print(F("Number of seconds since start = "));
  Serial.println(millis()/1000L,DEC);

  Serial.print(F("Internal Temperature = "));
  Serial.print(GetTemp(),1);
  Serial.println(F(" Celsius   (the offset could be wrong)."));

  Serial.println(F("-----------"));
}



void TimerRegisterDump(void)
{
  Serial.println(F("Timer Register dump:"));
  Serial.print(F("TIMER0 TCCR0A = 0x")); Serial.println(TCCR0A,HEX);
  Serial.print(F("TIMER0 TCCR0B = 0x")); Serial.println(TCCR0B,HEX);
  Serial.print(F("TIMER0 OCR0A  = 0x")); Serial.println(OCR0A,HEX);
  Serial.print(F("TIMER0 OCR0B  = 0x")); Serial.println(OCR0B,HEX);
  Serial.print(F("TIMER0 TIMSK0 = 0x")); Serial.println(TIMSK0,HEX);
  Serial.print(F("TIMER0 TCNT0  = 0x")); Serial.println(TCNT0,HEX);
  Serial.print(F("TIMER0 TIFR0  = 0x")); Serial.println(TIFR0,HEX);

  Serial.print(F("TIMER1 TCCR1A = 0x")); Serial.println(TCCR1A,HEX);
  Serial.print(F("TIMER1 TCCR1B = 0x")); Serial.println(TCCR1B,HEX);
  Serial.print(F("TIMER1 TCCR1C = 0x")); Serial.println(TCCR1C,HEX);
  Serial.print(F("TIMER1 OCR1A  = 0x")); Serial.println(OCR1A,HEX);
  Serial.print(F("TIMER1 OCR1B  = 0x")); Serial.println(OCR1B,HEX);
  Serial.print(F("TIMER1 TIMSK1 = 0x")); Serial.println(TIMSK1,HEX);
  Serial.print(F("TIMER1 TCNT1  = 0x")); Serial.println(TCNT1,HEX);
  Serial.print(F("TIMER1 ICR1   = 0x")); Serial.println(ICR1,HEX);
  Serial.print(F("TIMER1 TIFR1  = 0x")); Serial.println(TIFR1,HEX);

  Serial.print(F("TIMER2 TCCR2A = 0x")); Serial.println(TCCR2A,HEX);
  Serial.print(F("TIMER2 TCCR2B = 0x")); Serial.println(TCCR2B,HEX);
  Serial.print(F("TIMER2 OCR2A  = 0x")); Serial.println(OCR2A,HEX);
  Serial.print(F("TIMER2 OCR2B  = 0x")); Serial.println(OCR2B,HEX);
  Serial.print(F("TIMER2 TIMSK2 = 0x")); Serial.println(TIMSK2,HEX);
  Serial.print(F("TIMER2 TCNT2  = 0x")); Serial.println(TCNT2,HEX);
  Serial.print(F("TIMER2 TIFR2  = 0x")); Serial.println(TIFR2,HEX);
#if defined (__AVR_ATmega32U4__)
  // no ASSR register for Arduino Leonardo ?
#else
  Serial.print(F("TIMER2 ASSR   = 0x")); Serial.println(ASSR,HEX);
#endif

  Serial.print(F("TIMERn GTCCR  = 0x")); Serial.println(GTCCR,HEX);

  Serial.println(F("-----------"));
}



void i2c_scanner(void)
{
  byte error, address;
  int nDevices;

  Serial.println(F(""));
  Serial.println(F("i2c_scanner"));
  Serial.println(F("Scanning..."));

  Wire.begin();

  nDevices = 0;
  for(address = 0; address <= 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print(F("I2C device found at address 0x"));
      if (address<16) 
        Serial.print(F("0"));
      Serial.print(address,HEX);
      Serial.println(F(" !"));

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print(F("Unknow error at address 0x"));
      if (address<16) 
        Serial.print(F("0"));
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println(F("No I2C devices found."));
  else
    Serial.println(F("Done."));

  Serial.println(F("-----------"));
}



void SpeedTest(void)
{
  register int i,j;
  int p,q,r;
  volatile int v;
  unsigned long m,n;
  double d, overhead;
  char buffer[30];

  Serial.println(F(""));
  Serial.println(F("Speed test"));
  Serial.println(F("----------"));

  Serial.print(F("F_CPU = "));
  Serial.print(F_CPU,DEC);
  Serial.println(F(" Hz"));
  Serial.print(F("1/F_CPU = "));
  Serial.print((1000000.0/(double)F_CPU),4);
  Serial.println(F(" us"));

  Serial.println(F("The next tests are runtime compensated for overhead"));

  Serial.print(F("  nop                       : "));
  m=millis();
  for (i=0; i<100; i++)
  {
    for (j=0; j<10000; j++)
    {
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
      asm volatile ("nop");
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;           // in micro seconds
  // Calculate overhead with 'nop' instruction per loop in microseconds
  overhead = d - (20.0 * (1000000.0/(double)F_CPU));
  d -= overhead;
  d /= 20.0;             // per instruction
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  avr gcc I/O               : "));
  m=millis();
  for (i=0; i<50; i++)
  {
    for (j=0; j<10000; j++)
    {
      // Use system led
      PORTB |= _BV(PORTB5);
      PORTB &= ~_BV(PORTB5);

      PORTB |= _BV(PORTB5);
      PORTB &= ~_BV(PORTB5);

      PORTB |= _BV(PORTB5);
      PORTB &= ~_BV(PORTB5);

      PORTB |= _BV(PORTB5);
      PORTB &= ~_BV(PORTB5);

      PORTB |= _BV(PORTB5);
      PORTB &= ~_BV(PORTB5);

      PORTB |= _BV(PORTB5);
      PORTB &= ~_BV(PORTB5);

      PORTB |= _BV(PORTB5);
      PORTB &= ~_BV(PORTB5);

      PORTB |= _BV(PORTB5);
      PORTB &= ~_BV(PORTB5);

      PORTB |= _BV(PORTB5);
      PORTB &= ~_BV(PORTB5);

      PORTB |= _BV(PORTB5);
      PORTB &= ~_BV(PORTB5);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  Arduino digitalRead       : "));
  m=millis();
  for (i=0; i<2; i++)
  {
    for (j=0; j<10000; j++)
    {
      digitalRead(13);       
      digitalRead(13);       

      digitalRead(13);       
      digitalRead(13);       

      digitalRead(13);       
      digitalRead(13);       

      digitalRead(13);       
      digitalRead(13);       

      digitalRead(13);       
      digitalRead(13);       

      digitalRead(13);       
      digitalRead(13);       

      digitalRead(13);       
      digitalRead(13);       

      digitalRead(13);       
      digitalRead(13);       

      digitalRead(13);       
      digitalRead(13);       

      digitalRead(13);       
      digitalRead(13);       
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  Arduino digitalWrite      : "));
  m=millis();
  for (i=0; i<2; i++)
  {
    for (j=0; j<10000; j++)
    {
      digitalWrite(13, HIGH);       
      digitalWrite(13, LOW);       

      digitalWrite(13, HIGH);       
      digitalWrite(13, LOW);       

      digitalWrite(13, HIGH);       
      digitalWrite(13, LOW);       

      digitalWrite(13, HIGH);       
      digitalWrite(13, LOW);       

      digitalWrite(13, HIGH);       
      digitalWrite(13, LOW);       

      digitalWrite(13, HIGH);       
      digitalWrite(13, LOW);       

      digitalWrite(13, HIGH);       
      digitalWrite(13, LOW);       

      digitalWrite(13, HIGH);       
      digitalWrite(13, LOW);       

      digitalWrite(13, HIGH);       
      digitalWrite(13, LOW);       

      digitalWrite(13, HIGH);       
      digitalWrite(13, LOW);       
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  pinMode                   : "));
  m=millis();
  for (i=0; i<2; i++)
  {
    for (j=0; j<10000; j++)
    {
      pinMode(13, INPUT);       
      pinMode(13, OUTPUT);       

      pinMode(13, INPUT);       
      pinMode(13, OUTPUT);       

      pinMode(13, INPUT);       
      pinMode(13, OUTPUT);       

      pinMode(13, INPUT);       
      pinMode(13, OUTPUT);       

      pinMode(13, INPUT);       
      pinMode(13, OUTPUT);       

      pinMode(13, INPUT);       
      pinMode(13, OUTPUT);       

      pinMode(13, INPUT);       
      pinMode(13, OUTPUT);       

      pinMode(13, INPUT);       
      pinMode(13, OUTPUT);       

      pinMode(13, INPUT);       
      pinMode(13, OUTPUT);       

      pinMode(13, INPUT);       
      pinMode(13, OUTPUT);       
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  multiply volatile byte    : "));
  volatile unsigned char c1,c2;
  c1 = 2;
  c2 = 3;
  m=millis();
  for (i=0; i<10; i++)
  {
    for (j=0; j<10000; j++)
    {
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
      c1 *= c2;
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  divide volatile byte      : "));
  // volatile unsigned char c1,c2;
  c1 = 253;
  c2 = 3;
  m=millis();
  for (i=0; i<1; i++)
  {
    for (j=0; j<10000; j++)
    {
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
      c1 /= c2;
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  multiply volatile integer : "));
  volatile int x,y;
  x = 2;
  y = 3;
  m=millis();
  for (i=0; i<10; i++)
  {
    for (j=0; j<10000; j++)
    {
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
      x *= y;
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  divide volatile integer   : "));
  // volatile int x,y;
  x = 31415;
  y = 3;
  m=millis();
  for (i=0; i<1; i++)
  {
    for (j=0; j<10000; j++)
    {
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
      x /= y;
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  multiply volatile long    : "));
  volatile long l1,l2;
  l1 = 2;
  l2 = 3;
  m=millis();
  for (i=0; i<2; i++)
  {
    for (j=0; j<10000; j++)
    {
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
      l1 *= l2;
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  multiply single float     : "));
  volatile float f1,f2;
  f1 = 3.24;
  f2 = 1.25;
  m=millis();
  for (i=0; i<2; i++)
  {
    for (j=0; j<10000; j++)
    {
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
      f1 *= f2;
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  multiply double float     : "));
  volatile double d1,d2;
  d1 = 3.24;
  d2 = 1.25;
  m=millis();
  for (i=0; i<2; i++)
  {
    for (j=0; j<10000; j++)
    {
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
      d1 *= d2;
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  divide double float       : "));
  // volatile double d1,d2;
  d1 = 312645.24;
  d2 = 1.21;
  m=millis();
  for (i=0; i<1; i++)
  {
    for (j=0; j<2000; j++)
    {
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
      d1 /= d2;
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  itoa()                    : "));
  m=millis();
  for (i=0; i<1; i++)
  {
    for (j=0; j<10000; j++)
    {
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
      itoa(i,buffer,10);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  long int l = 314159L;
  Serial.print(F("  ltoa()                    : "));
  m=millis();
  for (i=0; i<1; i++)
  {
    for (j=0; j<500; j++)
    {
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
      ltoa(l,buffer,10);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  dtostrf()                 : "));
  double d3;
  d3 = 3.14159265;
  m=millis();
  for (i=0; i<2; i++)
  {
    for (j=0; j<1000; j++)
    {
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
      dtostrf (d3, 6, 2, buffer);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  random()                  : "));
  m=millis();
  for (i=0; i<2; i++)
  {
    for (j=0; j<1000; j++)
    {
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
      r=random();
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  y |= (1<<x) with volatile : "));
  m=millis();
  for (i=0; i<20; i++)
  {
    for (j=0; j<10000; j++)
    {
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
      v |= _BV(12);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  bitSet() with volatile    : "));
  m=millis();
  for (i=0; i<20; i++)
  {
    for (j=0; j<10000; j++)
    {
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
      bitSet (v, 12);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  analogReference()         : "));
#if defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
  #define _NOT_DEFAULT_ INTERNAL1V1
#else
  #define _NOT_DEFAULT_ INTERNAL
#endif
  m=millis();
  for (i=0; i<20; i++)
  {
    for (j=0; j<10000; j++)
    {
      analogReference (_NOT_DEFAULT_);
      analogReference (DEFAULT);
      analogReference (_NOT_DEFAULT_);
      analogReference (DEFAULT);
      analogReference (_NOT_DEFAULT_);
      analogReference (DEFAULT);
      analogReference (_NOT_DEFAULT_);
      analogReference (DEFAULT);
      analogReference (_NOT_DEFAULT_);
      analogReference (DEFAULT);
      analogReference (_NOT_DEFAULT_);
      analogReference (DEFAULT);
      analogReference (_NOT_DEFAULT_);
      analogReference (DEFAULT);
      analogReference (_NOT_DEFAULT_);
      analogReference (DEFAULT);
      analogReference (_NOT_DEFAULT_);
      analogReference (DEFAULT);
      analogReference (_NOT_DEFAULT_);
      analogReference (DEFAULT);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  analogRead()              : "));
  m=millis();
  for (i=0; i<1; i++)
  {
    for (j=0; j<1000; j++)
    {
      analogRead (0);
      analogRead (1);
      analogRead (0);
      analogRead (1);
      analogRead (0);
      analogRead (1);
      analogRead (0);
      analogRead (1);
      analogRead (0);
      analogRead (1);
      analogRead (0);
      analogRead (1);
      analogRead (0);
      analogRead (1);
      analogRead (0);
      analogRead (1);
      analogRead (0);
      analogRead (1);
      analogRead (0);
      analogRead (1);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  analogWrite() PWM         : "));
  // Using pin 13 (system led) for output.
  m=millis();
  for (i=0; i<1; i++)
  {
    for (j=0; j<10000; j++)
    {
      analogWrite (13, 100);
      analogWrite (13, 200);
      analogWrite (13, 100);
      analogWrite (13, 200);
      analogWrite (13, 100);
      analogWrite (13, 200);
      analogWrite (13, 100);
      analogWrite (13, 200);
      analogWrite (13, 100);
      analogWrite (13, 200);
      analogWrite (13, 100);
      analogWrite (13, 200);
      analogWrite (13, 100);
      analogWrite (13, 200);
      analogWrite (13, 100);
      analogWrite (13, 200);
      analogWrite (13, 100);
      analogWrite (13, 200);
      analogWrite (13, 100);
      analogWrite (13, 200);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  delay(1)                  : "));
  m=millis();
  for (i=0; i<1; i++)
  {
    for (j=0; j<100; j++)
    {
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
      delay(1);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  delay(100)                : "));
  m=millis();
  for (i=0; i<1; i++)
  {
    for (j=0; j<2; j++)
    {
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
      delay(100);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  delayMicroseconds(1)      : "));
  m=millis();
  for (i=0; i<20; i++)
  {
    for (j=0; j<10000; j++)
    {
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
      delayMicroseconds(1);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  delayMicroseconds(5)      : "));
  m=millis();
  for (i=0; i<5; i++)
  {
    for (j=0; j<10000; j++)
    {
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
      delayMicroseconds(5);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.print(F("  delayMicroseconds(100)    : "));
  m=millis();
  for (i=0; i<1; i++)
  {
    for (j=0; j<1000; j++)
    {
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
      delayMicroseconds(100);
    }
  }
  n=millis();
  d = ((double)n - (double)m) / ((double)i * (double)j);
  d *= 1000.0;
  d -= overhead;
  d /= 20.0;
  Serial.print (d,3);
  Serial.println (F(" us"));



  Serial.println(F("-----------"));
}

