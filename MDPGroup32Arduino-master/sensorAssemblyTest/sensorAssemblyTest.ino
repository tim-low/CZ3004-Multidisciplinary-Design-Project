void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
}
double buf1 = 0,bufs1=0,bufs2=0,bufs3=0;
int count = 0;

double s1,s2,s3,s4,s5,l1, avg1;
void loop() {
  // put your main code here, to run repeatedly:
  s1 = s1Equation(analogRead(A0));
  s2 = s2Equation(analogRead(A1));
  s3 = s3Equation(analogRead(A2));
  s4 = s4Equation(analogRead(A3));
  s5 = s5Equation(analogRead(A4));
  l1 = l1Equation(analogRead(A5));
  
  avg1 = 1.111111*(s1+s2+s3)/3;
  
  
  if(count%50==0)
  {
    Serial.print(buf1/50); 
    Serial.print(",");
    Serial.print(bufs1/50);
    Serial.print(",");
    Serial.print(bufs2/50);
    Serial.print(",");
    Serial.print(bufs3/50);
    count = 0;
    buf1 = 0;
    bufs1 = 0;
    bufs2 = 0;
    bufs3 = 0;
  }
  else
  {
    buf1 = buf1 + avg1;
    bufs1 = bufs1 + s1;
    bufs2 = bufs2 + s2;
    bufs3 = bufs3 + s3;
  }
//  Serial.print(s1);
//  Serial.print(",");
//  Serial.print(s2);
//  Serial.print(",");
//  Serial.print(s3);
//  Serial.print(",");
//  Serial.print(",");
//  Serial.print(s4);
//  Serial.print(",");
//  Serial.print(s5);
//  Serial.print(",");
//  Serial.print(l1);
  Serial.println("");
  count++;
}

double s1Equation(double x)
{
  double y = 1.945713 + (96.19357 - 1.945713)/(1 + pow((x/55.95114),1.231899));
  return y;
}
double s2Equation(double x)
{
  double y = 6.580329 + (121.1835 - 6.580329)/(1 + pow((x/67.21264),1.730286));
  return y;
}
double s3Equation(double x)
{
  double y = 12.52479 + (114447400 - 12.52479)/(1 + pow((x/3.363776),3.819309));
  return y;
}
double s4Equation(double x)
{
  double y = 2.611496 + (162.9981 - 2.611496)/(1 + pow((x/61.09693),1.508174));
  return y;
}
double s5Equation(double x)
{
  double y = 4.494588 + (103253000 - 4.494588)/(1 + pow((x/0.002043156),1.352362));
  return y;
}
double l1Equation(double x)
{
  double y = 3.206659 + (147845900 - 3.206659)/(1 + pow((x/0.01134231),1.49321));
  return y;
}
