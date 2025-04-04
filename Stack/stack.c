/*---------------------------------------------------------------------------------
File Name: stack.c
Programmed by: Hangyeol Lee
Affiliation: Chungbuk University
Functions: push(), pop(), print() in stack
Copyright (c) 2025 Hangyeol Lee. All rights reserved.
Date: 2025.03.31
---------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACKOVERFLOW -1
#define STACKUNDERFLOW -2
#define OK 0
#define STACK_MAXSIZE 3

#define PUSH 1
#define POP 2
#define PRINT 3
#define QUIT 4
#define INITIALIZATION 5

typedef struct Node // the Node structure of Stack
{
    int number;    // 학번
    char name[10]; // 이름
} STACK_NODE;

STACK_NODE Stack[STACK_MAXSIZE]; // Stack
int top = -1;                    // the top pointer's definition and initilization in Stack

/*-------------------------------------------------------------------------------------
 Function: 스택에서 top이 가르키는 최상위 원소의 주소를 리턴한다
 Interface: STACK_NODE* pop()
 Paramete: None
 return: if error, return address of STACKUNDERFLOW
         otherwise return address of top node
-------------------------------------------------------------------------------------*/
STACK_NODE *pop()
{
    if (top < 0) // 스택의 사이즈가 0이면 언더플로우
    {
        return (STACK_NODE *)STACKUNDERFLOW;
    }
    else
        return &Stack[top--]; // top 리턴 즉, 맨 위의 스택 index 리턴
}

/*-------------------------------------------------------------------------------------
 Function: 스택에 한 원소를 추가한다
 Interface: int push()
 Parameter: int number = a student number to push
            char name[] = a name of student to push
 return:  if the pushing is complete, return OK
          if the stack is full, return STACKOVERFLOW
-------------------------------------------------------------------------------------*/
int push(int number, char name[])
{
    if (top + 1 >= STACK_MAXSIZE) // 스택이 꽉찼으면 오버플로우
        return STACKOVERFLOW;
    else
    {
        Stack[++top].number = number;
        strcpy(Stack[top].name, name); // push할 변수 초기화
        return OK;
    }
}

/*-------------------------------------------------------------------------------------
 Function: 스택의 값들을 출력한다
 Interface: void print_stack()
 Parameter: None
 return: void
-------------------------------------------------------------------------------------*/
void print_stack()
{
    if (top <= -1) // 스택이 비어있을때
        printf("Stack is Empty!\n");
    else
    {
        for (int i = 0; i <= top; i++)
        {
            printf("%d\t%s\n", Stack[i].number, Stack[i].name); // 학번 먼저 출력
        }
    }
}

/*------------------------------------------------------------------------------------
 Function: 스택 초기화
 Interface: void initialize_stack()
 Parameter: None
 return: void
-------------------------------------------------------------------------------------*/
void initialize_stack()
{
    top = -1;
}

/* -----------------------------------------------------------------------------------
 Stack이 잘 구현됐는지 확인하는 인터페이스
 command에 숫자를 입력받아서 원하는 명령 실행
-------------------------------------------------------------------------------------*/
int main()
{
    int command;
    do
    {
        printf("----------------------------------------------------------------\n");
        printf("                     stack                         \n");
        printf("----------------------------------------------------------------\n");
        printf(" Push            = 1           Pop           = 2 \n");
        printf(" Print           = 3           Quit          = 4 \n");
        printf(" Initialization  = 5 \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");

        /*
             scanf()의 리턴값:
               - 양의 정수 (1, 2, 3...) : 형식에 맞기 입력받은 항목의 개수
               - 0 : 형식에 맞지 않은 입력
               - EOF(-1) : 입력 스트림이 종료됨
        */

        if (scanf("%d", &command) != 1) // 입력값이 정수가 아닐 경우
        {
            printf("Wrong input!\n");
            while (getchar() != '\n')
                ;     // 입력 버퍼 비우기
            continue; // 다음 반복으로 넘어가기
        }

        switch (command)
        {
        case PUSH: // 1
        {
            int number;
            char name[10];
            int ret;

            printf("Input number = "); // id 입력 받기
            while (scanf("%d", &number) == 0)
            {
                printf("Wrong Input!\nInput number again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            printf("Input name = "); // name 입력 받기
            while (scanf("%s", name) == 0)
            {
                printf("Wrong Input!\nInput name again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (push(number, name) == STACKOVERFLOW)
            {
                printf("Stack OverFlow!\n");
            }
            else
            {
                printf("push() is complete!\n");
            }
            break;
        }
        case POP: // 2
        {
            STACK_NODE *result = pop(); // 리턴 받은 인덱스 result에 저장
            if (result == (STACK_NODE *)STACKUNDERFLOW)
            {
                printf("Stack UnderFlow!\n");
            }
            else
            {
                printf("%d\n", result->number);
                printf("%s\n", result->name); // 인덱스에 해당하는 값 출력
            }
            break;
        }
        case PRINT: // 3
            print_stack();
            break;
        case INITIALIZATION: // 5
            initialize_stack();
            break;
        case QUIT: // 4
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != QUIT);
}