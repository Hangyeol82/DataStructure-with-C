#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#define MAX_STACK_SIZE 100
#define STACKOVERFLOW -1
#define STACKUNDERFLOW -2

#define MAX_QUEUE_SIZE 100
#define QUEUE_OVERFLOW -1
#define QUEUE_UNDERFLOW -2

#define OK 1

int push(int loc);
int pop(void);
int is_stack_empty();
int enqueue(int loc);
int dequeue(void);
int is_queue_empty();
void init_stack(void);
void init_queue(void);

#endif