
unsigned long duration;
unsigned long rpm;
int m1DirA, m1DirB, m1Diag, m2DirA, m2DirB, m1PWM, m2PWM, m2Diag, m1CS, m2CS, m1Enc, m2Enc;
unsigned long ticks1=0, ticks2=0, ticksTemp1=0, ticksTemp2=0;

volatile byte state = LOW;
void setup() {
  Serial.begin(9600);
  m1DirA = 2;
  m1DirB = 4;
  m1Diag = 6;
  m2DirA = 8;
  m2DirB = 7;
  m1PWM = 9;
  m2PWM = 10;
  m2Diag = 12;
  m1Enc = 3;
  m2Enc = 12;
  pinMode(m1DirA,OUTPUT);
  pinMode(m1DirB,OUTPUT);
  pinMode(m2DirA,OUTPUT);
  pinMode(m2DirB,OUTPUT);
  pinMode(m1PWM,OUTPUT);
  pinMode(m2PWM,OUTPUT);
  pinMode(m1Enc,INPUT);
  pinMode(m2Enc,INPUT);

  digitalWrite(m1DirA, HIGH);
  digitalWrite(m1DirB, LOW);
  digitalWrite(m2DirA, HIGH);
  digitalWrite(m2DirB, LOW);
  analogWrite(m1PWM, 128);
  analogWrite(m2PWM, 128);
  attachInterrupt(digitalPinToInterrupt(m1Enc), pin_ISR1, RISING);
  PCMSK1 = B00001000; //enable PCINT8
  PCIFR = B00000000; //clear all interrupt flags
  PCICR = B00000001;

  ticks1 = 0; ticks2 = 0;ticksTemp1=0;ticksTemp2=0;
}

float ticksPerSecond1 = 0, revsPerSecond1 = 0, revsPerMinute1 = 0, ticksPerSecond2 = 0, revsPerSecond2 = 0, revsPerMinute2 = 0;
float uone1=100, u1=0, uone2=100, u2=0, e1=0, eone1=0, eone2=0, e2=0;
unsigned long curTicks1=0, pTicks1 = 0,curTicks2=0, pTicks2 = 0,t=0,pt=0, setPoint = 100;
bool forward = true;
double kp=6;
double ki=0.5;
double kd=0;
unsigned long se1 = 0, se2 = 0;
void loop() {
  
  pt = t;
  t = millis();
  pTicks1 = curTicks1;
  curTicks1 = ticks1;
  pTicks2 = curTicks2;
  curTicks2 = ticks2;
  eone1 = e1;
  uone1 = u1;
  eone2 = e2;
  uone2 = u2;

//  if(((t/1000)/period)%2==0)
//  {
//    setPoint = 125;
//  }
//  else
//  {
//    setPoint = 0;
//  }
//   
//  
  ticksPerSecond1 = (curTicks1-pTicks1)/((t-pt)/1000.0);
  ticksPerSecond2 = (curTicks2-pTicks2)/((t-pt)/1000.0);
  revsPerSecond1 = ticksPerSecond1/562.25;
  revsPerSecond2 = ticksPerSecond2/562.25;
  revsPerMinute1 = revsPerSecond1*60;
  revsPerMinute2 = revsPerSecond2*60;
  e1 = setPoint - revsPerMinute1;
  e2 = setPoint - revsPerMinute2;
  se1 = se1 + e1;
  se2 = se2 + e2;
  
  u1 = kp* e1 + ki * se1 + kd * (eone1-e1);
  u2 = kp* e2 + ki * se2 + kd * (eone2-e2);
  u1 = checkOverflow(u1);
  u2 = checkOverflow(u2);
  drive(m1DirA, m1DirB, forward);
  drive(m2DirA, m2DirB, forward);
  analogWrite(m2PWM,u2);
  analogWrite(m1PWM,u1);
   
  Serial.print(ticksPerSecond1);
  Serial.print(",");
  Serial.println(ticksPerSecond2);
} 
void pin_ISR1()
{
  ticksTemp1 = ticksTemp1 + 1;
  ticks1 = ticksTemp1;
}
void pin_ISR2()
{
  ticksTemp2 = ticksTemp2 + 1;
  ticks2 = ticksTemp2;
}
ISR(PCINT0_vect) {
  ticksTemp2 = ticksTemp2 + 1;
  ticks2 = ticksTemp2;
}
void drive(int a, int b, bool dir)
{
  if(dir)
  {
    digitalWrite(a,HIGH);
    digitalWrite(b,LOW);
  }
  else
  {
    digitalWrite(a,LOW);
    digitalWrite(b,HIGH);
  }
}
int checkOverflow(int u)
{
  if(u>255000) u=0;
  if(u>255) u=255;
  if(u<0) u=0;
  return u;
}
