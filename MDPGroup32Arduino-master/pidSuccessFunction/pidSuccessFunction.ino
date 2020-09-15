
unsigned long duration;
unsigned long rpm;
int ONE_SQUARE = 281;
int m1DirA, m1DirB, m1Diag, m2DirA, m2DirB, m1PWM, m2PWM, m2Diag, m1CS, m2CS;
unsigned long ticks;
int m2Enc;
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
  m1CS = A0;
  m2CS = A1;
  m2Enc = 3;
  pinMode(m1DirA,OUTPUT);
  pinMode(m1DirB,OUTPUT);
  pinMode(m2DirA,OUTPUT);
  pinMode(m2DirB,OUTPUT);
  pinMode(m1PWM,OUTPUT);
  pinMode(m2PWM,OUTPUT);
  pinMode(m2Enc,INPUT);

  pinMode(A0, INPUT);
  
  digitalWrite(m1DirA, HIGH);
  digitalWrite(m1DirB, LOW);
  digitalWrite(m2DirA, HIGH);
  digitalWrite(m2DirB, LOW);
  analogWrite(m1PWM, 128);
  analogWrite(m2PWM, 128L);
  attachInterrupt(digitalPinToInterrupt(3), pin_ISR, RISING);
  ticks = 0;
  Serial.println("rpm,e,u");
}
float ticksPerSecond = 0, revsPerSecond = 0, revsPerMinute = 0;
float u=100, u1=0,e=0, e1=0, e2=0,y=0;
int fu=0;
unsigned long curTicks=0, pTicks = 0,t=0,pt=0, setPoint = 25;
bool forward = true;
double kp=6;
double ki=0.5;
double kd=0;
int period = 2;
double lpf = 0.04;
boolean lpfOn = false;
unsigned long se = 0;
int endTicksMain = 0;
bool lock = false;
void loop() {
  pt = t;
  t = millis();
  pTicks = curTicks;
  curTicks = ticks;
  e1 = e;
  u1 = u;

//  if(((t/1000)/period)%2==0)
//  {
//    setPoint = 25;
//  }
//  else
//  {
//    setPoint = 25;
//  }
//   

  
  ticksPerSecond = (curTicks-pTicks)/((t-pt)/1000.0);
  revsPerSecond = ticksPerSecond/562.25;
  revsPerMinute = revsPerSecond*60;
  y = revsPerMinute;
  e = setPoint - y;
  se= se + e;
  
  u = kp* e + ki * se + kd * (e1-e);
  if(u>255000) u=0;
  if(u>255) u=255;
  if(u<0) u=0;

  drive(m1DirA, m1DirB, true);
  drive(m2DirA, m2DirB, true);
  fu = (1-lpf)*fu + lpf*u;
  if(lpfOn)
  {
    analogWrite(m2PWM,fu);
    analogWrite(m1PWM,fu);
  }
  else
  {
    analogWrite(m2PWM,u);
    analogWrite(m1PWM,u*0.9);
  
  }
//  
//  
//
  Serial.print(u);
  Serial.print(",");
  Serial.print(fu);
  Serial.print(",");
  Serial.print(revsPerMinute);
  Serial.print(",");
  Serial.print(setPoint);
  Serial.print(",");
  Serial.print(ONE_SQUARE);
  Serial.print(",");
  Serial.println(ticks);
//
//
//  
//
//  
//  Serial.println(" ");
  go('L',0,1,25);

}

void pin_ISR()
{
  ticks = ticks + 1;
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

void mover(bool m1, bool m2, int initialTicks, int numberOfSquares, int sp)
{
  drive(m1DirA, m1DirB, m1);
  drive(m2DirA, m2DirB, m2);
  if(ticks<initialTicks+ONE_SQUARE*numberOfSquares)
  {
    setPoint = sp;
  }
  else
  {
    setPoint = 0;
  }
  
}

void go(char dir,int initialTicks, int numberOfSquares, int sp)
{
  switch(dir)
  {
    case 'F': mover(true, true, initialTicks, numberOfSquares, sp); break;
    case 'B': mover(false, false, initialTicks, numberOfSquares, sp); break;
    case 'L': mover(true, false, initialTicks, numberOfSquares, sp); break;
    case 'R': mover(false, true, initialTicks, numberOfSquares, sp); break;
  }
  
}

//void mover(bool m1, bool m2, int endTicks, int sp)
//{
//  drive(m1DirA, m1DirB, m1);
//  drive(m2DirA, m2DirB, m2);
//  if(ticks<initialTicks+endTicks)
//  {
//    setPoint = sp;
//  }
//  else
//  {
//    setPoint = 0;
//  }
//  
//}
//void go(char dir,int endTicks, int sp)
//{
//  switch(dir)
//  {
//    case 'F': mover(true, true, endTicks, sp); break;
//    case 'B': mover(false, false, endTicks, sp); break;
//    case 'L': mover(true, false, endTicks, sp); break;
//    case 'R': mover(false, true, endTicks, sp); break;
//  }
//}
