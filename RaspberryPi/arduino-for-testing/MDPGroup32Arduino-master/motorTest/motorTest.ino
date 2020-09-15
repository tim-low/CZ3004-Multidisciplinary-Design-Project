unsigned long duration;
unsigned long rpm;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(3,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(2,HIGH);
  digitalWrite(4,LOW);
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
  analogWrite(9,128);
  analogWrite(10,128);
  duration = pulseIn(3, HIGH);
  rpm = calculateRpm(duration);
  Serial.println(rpm);
}

unsigned long calculateRpm(unsigned long i)
{
  duration = i;
  rpm = 36000000/562.25/duration;
  return rpm;
}
