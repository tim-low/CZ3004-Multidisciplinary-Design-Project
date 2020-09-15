
int enable[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,1,0,0,0};
int longRange[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,1,1,0,0};
float runningDistance[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0};
float averageDistance[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0};
float currentDistance[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0};
int arrLength = 20;
int times = 25, count = 0;
float distance = 0, sensorValue = 0;
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
int i=0;
void loop() {
  // put your main code here, to run repeatedly:
  
  for(i=0;i<arrLength;i++)
  {
    if(enable[i]==1)
    {
      sensorValue = analogRead(i);
      if(longRange[i]==1)
      {
        distance = 30431 * pow(sensorValue, -1.169);
      }
      else
      {
        distance = 12343.85 * pow(sensorValue, -1.15);
      }
      
      //Serial.print(distance);
      runningDistance[i] = runningDistance[i] + distance;
    }
    
  }
  if(count % times == 0)
  {
    for(i=0;i<arrLength;i++)
    {
      if(enable[i]==1)
      {
        averageDistance[i] = runningDistance[i]/times;
        currentDistance[i] = averageDistance[i];
        //Serial.print(i);
        //Serial.print(",");
        //Serial.print(averageDistance[i]);
        //Serial.println("     ");
        runningDistance[i] = 0;
        averageDistance[i] = 0;
        
      }
    }
  }
  for(i=0;i<arrLength;i++)
  {
    if(enable[i]==1)
    {
      //Serial.print(count%times);
      Serial.print(",");
      Serial.println(currentDistance[i]);
      
    }
  }

      
  count++;
}
