unsigned long duration;
unsigned long rpm;
int m1DirA, m1DirB, m1Diag, m2DirA, m2DirB, m1PWM, m2PWM, m2Diag, m1CS, m2CS;

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
}
int i,j,k, power, commence = 150, terminate = 200, numberOfSamples = 2, numberOfCycles = 3, del = 0;//test motors
//int i,j,k, power, commence = 35, terminate = 255, numberOfSamples = 25, numberOfCycles = 3, del = 25;
bool forward;
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("run,power,rpm,direction");
  for(k=0;k<numberOfCycles*2;k++)
  {
    for(i=commence;i<terminate+(terminate-commence);i++)
    {
      
      if(i>terminate)
      {
        power = terminate-(i-terminate);
      }
      else
      {
        power = i;
      }
      for(j=0;j<numberOfSamples;j++)
      {
        if(k*2<numberOfCycles*2)
        {
          forward = true;
        }
        else
        {
          forward = false;
        }
        drive(m1DirA, m1DirB, forward);
        drive(m2DirA, m2DirB, forward);
        analogWrite(m2PWM,power);
        analogWrite(m1PWM,power);
        duration = pulseIn(m2Enc, HIGH);
        rpm = calculateRpm(duration);
        Serial.print(k);
        Serial.print(",");
        Serial.print(power);
        Serial.print(",");
        Serial.print(rpm);
        Serial.print(",");
        Serial.println(forward);
        delay(del);
      }
    }
  }
  
  analogWrite(m2PWM,0);
  analogWrite(m1PWM,0);
  delay(1000000);
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
unsigned long calculateRpm(unsigned long i)
{
  duration = i;
  rpm = 36000000/562.25/duration;
  return rpm;
}
