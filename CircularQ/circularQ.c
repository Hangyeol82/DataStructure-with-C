/*------------------------------------------------------------------------------
File Name: circularQ.c
Programmed by: Hangyeol Lee
Affiliation: Chungbuk University
Functions: enqueue(), dequeue(), print() in circluarQ
Copyright (c) 2025 Hangyeol Lee. All rights reserved.
------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 10
#define QUEUE_UNDERFLOW -1

#define ENQUEUE 1
#define DEQUEUE 2
#define PRINT 3
#define QUIT 4
#define INITIALIZAITON 5
/*-----------------------------------------------------------------------------
 MAX_QUEUE_SIZE의 나머지로 rear와 front값을 지정하는 이유:
 '원형 큐'이므로 rear와 front 값이 MAX_QUEUE_SIZE를 넘어서면 안되기 때문
 엄밀히 말하면 MAX_QUEUE_SIZE-1를 넘어서면 안됨. 그 이유는 아래.
-------------------------------------------------------------------------------
  full 조건 이유: front == rear가 포화인지 공백인지
  구분하기 위해서 '최대 원소수'를 MAX_QUEUE_SIZE-1로 한다.
  즉, 원형 큐의 마지막 칸은 비워둔다.
------------------------------------------------------------------------------*/

typedef struct CircularQueue // 원형 큐의 구조체
{
    int arr[MAX_QUEUE_SIZE]; // 값을 저장할 배열
    int front;               // 맨 앞의 원소를 가르킬 변수
    int rear;                // 맨 마지막 + 1을 가르킬 변수
} Queue;

Queue *queue = NULL;

/*------------------------------------------------------------------------------
 Function: 큐에 값을 추가하는 함수
 Interface: enqueue()
 Parameter: Queue *queue = pointer for the queue to enqueue
            int value = value to enqueue
 return: void
------------------------------------------------------------------------------*/
void enqueue(Queue *queue, int value)
{
    if ((queue->rear + 1) % MAX_QUEUE_SIZE == queue->front) // 큐가 꽉 차있을때
        printf("OverFlow!\n");
    else
    {
        queue->arr[queue->rear] = value;                  // 원소 추가
        queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE; // rear + 1
    }
}

/*------------------------------------------------------------------------------
 Function: 맨 앞의 값을 출력하는 함수
 Interface: dequeue()
 Parameter: Queue *queue = pointer for the queue to dequeue
 return: int result = value of the first node in queue
         QUEUE_UNDERFLOW = if the queue is empty
------------------------------------------------------------------------------*/
int dequeue(Queue *queue)
{
    if (queue->front == queue->rear) // 큐가 비어있을때
        return QUEUE_UNDERFLOW;
    else
    {
        int result = queue->arr[queue->front];              // 결과값 미리 저장
        queue->front = (queue->front + 1) % MAX_QUEUE_SIZE; // front + 1
        return result;
    }
}

/*------------------------------------------------------------------------------
 Function: 큐 안에 있는 값들을 출력하는 함수
 Interface: print_queue()
 Parameter: Queue* queue = pointer for the queue to print
 return: void
------------------------------------------------------------------------------*/
void print_queue(Queue *queue)
{
    int size = 0; // 큐 안에 들어있는 원소의 개수를 저장할 값
    if (queue->front <= queue->rear)
        size = queue->rear - queue->front; // rear가 front보다 크면, rear에서 front을 빼면 원소 개수
    else
        size = MAX_QUEUE_SIZE - queue->front + queue->rear; // front가 rear보다 크면, 총 용량에서 rear을 더하고 front를 빼면 원소 개수

    for (int i = 0; i < size; i++)
    {
        printf("%d ", queue->arr[(queue->front + i) % MAX_QUEUE_SIZE]); // 원소 출력
    }
    printf("\nSize: %d\n", size); // 원소 개수 출력
}

/*------------------------------------------------------------------------------
 Function: 큐 안에 있는 값들을 출력하는 함수
 Interface: print_queue()
 Parameter: Queue *queue = pointer for the queue to initialize
 return: void
------------------------------------------------------------------------------*/
void initialization_queue()
{
    queue = (Queue *)malloc(sizeof(Queue));
    queue->front = 0;
    queue->rear = 0;
    printf("Initialization is complete\n");
}

/* 큐가 잘 작동하는지 확인하는 인터페이스 */
int main()
{
    int command;
    int key;

    do
    {
        printf("----------------------------------------------------------------\n");
        printf(" Enqueue        = 1           Dequeue        = 2 \n");
        printf(" Print          = 3           Quit           = 4 \n");
        printf(" Initialize     = 5 \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %d", &command);

        switch (command)
        {
        case ENQUEUE: // 1
        {
            int ret = 0;
            printf("Input number = ");
            ret = scanf("%d", &key);
            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }
            enqueue(queue, key);
            break;
        }
        case DEQUEUE: // 2
        {
            int result = dequeue(queue);
            if (result == QUEUE_UNDERFLOW)
                printf("UnderFlow!\n");
            else
            {
                printf("%d\n", result);
            }
            break;
        }
        case PRINT: // 3
            print_queue(queue);
            break;
        case QUIT: // 4
            free(queue);
            break;
        case INITIALIZAITON: // 5
            initialization_queue();
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != 4);
}