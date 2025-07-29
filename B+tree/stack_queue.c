#include "stack_queue.h"

int stack[MAX_STACK_SIZE];
int top = -1;

/*-------------------------------------------------------------------------------------
 Function: 스택에 노드의 위치를 추가한다
 Interface: int push(int loc)
 Parameter: int loc: location of a node in bin file
 return:  if the pushing is complete, return OK
          if the stack is full, return STACKOVERFLOW
-------------------------------------------------------------------------------------*/
int push(int loc)
{
    if (top + 1 >= MAX_STACK_SIZE)
        return STACKOVERFLOW;
    else
    {
        stack[++top] = loc;
        return OK;
    }
}

/*-------------------------------------------------------------------------------------
 Function: 스택에서 top이 가르키는 최상위 원소를 리턴한다
 Interface: int pop()
 Paramete: None
 return: if error, return STACKUNDERFLOW
         otherwise return location of a node in bin file
-------------------------------------------------------------------------------------*/
int pop()
{
    if (top <= -1)
        return STACKUNDERFLOW;
    else
    {
        return stack[top--];
    }
}

/*------------------------------------------------------------------------------
 Function: 스택을 초기화 한다
 Interface: void init_stack()
 Parameter: None
 return: void
------------------------------------------------------------------------------*/
void init_stack()
{
    top = -1;
}

/*------------------------------------------------------------------------------
 Function: 스택이 empty인지 확인한다
 Interface: int is_stack_empty()
 Parameter: None
 return: if stack is empty, return 1
         otherwise, return 0
------------------------------------------------------------------------------*/
int is_stack_empty()
{
    if (top < 0) return 1;
    else return 0;
}

int queue[MAX_QUEUE_SIZE];
int front = 0;
int rear = 0;

/*------------------------------------------------------------------------------
 Function: 큐에 노드의 위치를 추가하고 rear의 값에 +1 연산을 수행한다
 Interface: int enqueue(int loc)
 Parameter: int loc = location of a node in bin file
 return: if the queue is full, return QUEUE_OVERFLOW
         otherwise return OK.
------------------------------------------------------------------------------*/
int enqueue(int loc)
{
    if (rear >= MAX_QUEUE_SIZE)
        return QUEUE_OVERFLOW;
    else
    {
        queue[rear++] = loc;
        return OK;
    }
}

/*------------------------------------------------------------------------------
 Function: 큐의 front가 가르키는 원소를 출력한다
 Interface: int dequeue()
 Parameter: None
 return: if the queue is empty, return QUEUE_UNDERFLOW
         otherwise return location of a node in bin file
------------------------------------------------------------------------------*/
int dequeue()
{
    if (front == rear)
        return QUEUE_UNDERFLOW;
    else
    {
        return queue[front++];
    }
}

/*------------------------------------------------------------------------------
 Function: 큐를 초기화 한다
 Interface: void init_queue()
 Parameter: None
 return: void
------------------------------------------------------------------------------*/
void init_queue()
{
    front = 0;
    rear = 0;
}

/*------------------------------------------------------------------------------
 Function: 큐가 empty인지 확인한다
 Interface: int is_stack_empty()
 Parameter: None
 return: if queue is empty, return 1
         otherwise, return 0
------------------------------------------------------------------------------*/
int is_queue_empty()
{
    if (front == rear) return 1;
    else return 0;
}