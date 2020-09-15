
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
    return (q->rear == NULL);
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








queue* q;

void setup() {
  // put your setup code here, to run once:
  q = malloc(sizeof(queue));
  initialize(q);
  enqueue(q,'F');
  Serial.print("Queue before dequeue\n");
  displayer(q->front); 
  dequeue(q);
  Serial.print("Queue after dequeue\n");
  displayer(q->front);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Queue after dequeue\n");

}
