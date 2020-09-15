unsigned long duration;
unsigned long rpm;
int m1DirA, m1DirB, m1Diag, m2DirA, m2DirB, m1PWM, m2PWM, m2Diag, m1CS, m2CS;

unsigned long ticks;
int m2Enc;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

  m1DirA = 2;
  m1DirB = 4;
  m1Diag = 6;
  m2DirA = 7;
  m2DirB = 8;
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
  digitalWrite(m1DirA, HIGH);
  digitalWrite(m1DirB, LOW);
  digitalWrite(m2DirA, HIGH);
  digitalWrite(m2DirB, LOW);
  analogWrite(m1PWM, 128);
  analogWrite(m2PWM, 128);
  attachInterrupt(digitalPinToInterrupt(3), pin_ISR, RISING);
  ticks = 0;
  Serial.println("run,time,ticks,power,direction");
}
unsigned long t;
unsigned long power;
double f = 0.5;
unsigned long period = (unsigned long)(1000/f), commence = 55, terminate = 255, numberOfTimes = 5;
unsigned long runDuration = (terminate-commence)*period;
unsigned long switchDirections = numberOfTimes*runDuration;
boolean forward = true;
void loop() {
  if (t>2*switchDirections)
  {
    analogWrite(m2PWM,0);
    analogWrite(m1PWM,0);
    while(1);
  }
  else if (t>switchDirections)
  {
    forward = false;
  }
  else
  {
    forward = true;
  }
  t = millis();
  
  power = commence + ((t/period) % (terminate-commence));
  drive(m1DirA, m1DirB, forward);
  drive(m2DirA, m2DirB, forward);
  analogWrite(m2PWM,power);
  analogWrite(m1PWM,power);

  
  Serial.print(t/runDuration);
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.print(ticks);
  Serial.print(",");
  Serial.print(power);
  Serial.print(",");
  Serial.println(forward);

  //delay(100);
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
