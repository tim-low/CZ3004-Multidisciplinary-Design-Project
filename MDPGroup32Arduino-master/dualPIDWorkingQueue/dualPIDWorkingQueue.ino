#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>


#define FULL 100

struct node
{
    char data;
    struct node *next;
};
typedef struct node node;

struct queue
{
    int count;
    node *front;
    node *rear;
};
typedef struct queue queue;

void initialize(queue *q)
{
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}

int isempty(queue *q)
{
    return (q->front == NULL);
}

void enqueue(queue *q, char value)
{
    if (true)
    {
        node *tmp;
        tmp = malloc(sizeof(node));
        tmp->data = value;
        tmp->next = NULL;
        if(!isempty(q))
        {
            q->rear->next = tmp;
            q->rear = tmp;
        }
        else
        {
            q->front = q->rear = tmp;
        }
        q->count++;
    }
    else
    {
        Serial.print("List is full\n");
    }
}

char dequeue(queue *q)
{
    node *tmp;
    char n = q->front->data;
    tmp = q->front;
    q->front = q->front->next;
    q->count--;
    free(tmp);
    return(n);
}
void displayer(node *head)
{
    if(head == NULL)
    {
        Serial.print("NULL\n");
    }
    else
    {
        Serial.print(head -> data);
        displayer(head->next);
    }
}
























unsigned long duration;
unsigned long rpm;
int ONE_SQUARE = 281;
int m1DirA, m1DirB, m1Diag, m2DirA, m2DirB, m1PWM, m2PWM, m2Diag, m1CS, m2CS;
unsigned long ticks1 = 0, ticks2 = 0;
int m2Enc, m1Enc;


queue *q;
  
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
  m1Enc = 11;
  pinMode(m1DirA,OUTPUT);
  pinMode(m1DirB,OUTPUT);
  pinMode(m2DirA,OUTPUT);
  pinMode(m2DirB,OUTPUT);
  pinMode(m1PWM,OUTPUT);
  pinMode(m2PWM,OUTPUT);
  pinMode(m2Enc,INPUT);
  pinMode(m1Enc,INPUT);

  pinMode(A0, INPUT);
  
  digitalWrite(m1DirA, HIGH);
  digitalWrite(m1DirB, LOW);
  digitalWrite(m2DirA, HIGH);
  digitalWrite(m2DirB, LOW);
  analogWrite(m1PWM, 128);
  analogWrite(m2PWM, 128);
  //attachInterrupt(digitalPinToInterrupt(3), pin_ISR, RISING);
  attachPCINT(digitalPinToPCINT(m2Enc), pin_ISR2, RISING);
  attachPCINT(digitalPinToPCINT(m1Enc), pin_ISR1, RISING);
  Serial.println("rpm,e,u");

  
  q = malloc(sizeof(queue));
  initialize(q);
  enqueue(q,'F');
  enqueue(q,'F');
  enqueue(q,'F');
  enqueue(q,'L');
  enqueue(q,'F');
  enqueue(q,'F');
  enqueue(q,'F');
  Serial.print("Queue before dequeue\n");
  displayer(q->front); 
  Serial.print("Queue after dequeue\n");
  displayer(q->front);
}
float ticksPerSecond = 0, revsPerSecond = 0, revsPerMinute1 = 0, revsPerMinute2 = 0;
float u1=100, pu1=0,pu2=0,u2=0,e1=0, pe1=0, pe2=0,e2=0,y=0;
int fu=0;
unsigned long curTicks1=0, pTicks1 = 0,curTicks2=0, pTicks2 = 0,t=0,pt=0, setPoint = 0;
bool forward = true;
double kp=6;
double ki=0.5;
double kd=0;
int period = 2;
double lpf = 0.04;
boolean lpfOn = false;
unsigned long se1 = 0, se2 = 0;
int endTicksMain = 0;
bool lock = false;
char c;
int endPos = 0;
int beginPos = 0;
char currentDir = 'F';
void loop() {
  c = Serial.read();
  if(c=='F' || c=='B' || c=='L' || c=='R')
  {
    Serial.print("receive: ");
    Serial.println(c);
    enqueue(q,c);
    displayer(q->front);
  }
  if(lock==false && !isempty(q))
  {
    c = dequeue(q);
    lock = true;
    Serial.print("dequeue: ");
    Serial.print(c);
    Serial.print("  end: ");
    Serial.print(endPos);
    currentDir = c;
    beginPos = endPos;
    endPos = endPos + ONE_SQUARE;
  }
  go(currentDir,beginPos,1,25);
  Serial.print(lock);
  Serial.print("current: ");
  Serial.print(currentDir);
  pt = t;
  t = millis();
  pTicks1 = curTicks1;
  curTicks1 = ticks1;
  pTicks2 = curTicks2;
  curTicks2 = ticks2;
  pe1 = e1;
  pu1 = u1;
  pe2 = e2;
  pu2 = u2;

//  if(((t/1000)/period)%2==0)
//  {
//    setPoint = 25;
//  }
//  else
//  {
//    setPoint = 25;
//  }
//   

  
  revsPerMinute1 = ((curTicks1-pTicks1)/((t-pt)/1000.0))*60/562.25;
  revsPerMinute2 = ((curTicks2-pTicks2)/((t-pt)/1000.0))*60/562.25;
  e1 = setPoint - revsPerMinute1;
  e2 = setPoint - revsPerMinute2;
  se1= se1 + e1;
  se2= se2 + e2;
  
  u1 = kp* e1 + ki * se1 + kd * (pe1-e1);
  u2 = kp* e2 + ki * se2 + kd * (pe2-e2);
  if(u1>255000) u1=0;
  if(u1>255) u1=255;
  if(u1<0) u1=0;
  if(u2>255000) u2=0;
  if(u2>255) u2=255;
  if(u2<0) u2=0;

  //drive(m1DirA, m1DirB, true);
  //drive(m2DirA, m2DirB, true);
  analogWrite(m2PWM,u2);
  analogWrite(m1PWM,u1);
//  
//  
//
//  Serial.print(u1);
//  Serial.print(",");
//  Serial.print(u2);
//  Serial.print(",");
//  Serial.print(revsPerMinute1);
//  Serial.print(",");
//  Serial.print(setPoint);
//  Serial.print(",");
//  Serial.print(ONE_SQUARE);
//  Serial.print(",");
//  Serial.println(ticks1);
//
//
//  
//
//  
//  Serial.println(" ");




}

void pin_ISR1()
{
  ticks1 = ticks1 + 1;
}
void pin_ISR2()
{
  ticks2 = ticks2 + 1;
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
  if(ticks1>=initialTicks && ticks1<initialTicks+ONE_SQUARE*numberOfSquares)
  {
    setPoint = sp;
    lock = true;
  }
  else
  {
    setPoint = 0;
    lock = false;
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
