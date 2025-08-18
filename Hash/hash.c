/*---------------------------------------------------------------------
File Name: hash.c
Programmed by: Hangyeol Lee
Affiliation: Chungbuk University
Functions: insert(), delete(), retrieve(), update() in hash
Copyright (c) 2025 Hangyeol Lee. All rights reserved.
---------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TABLE_SIZE 101
#define OK 1
#define FAIL 0

#define INSERT 1
#define DELETE 2
#define UPDATE 3
#define RETRIEVE 4
#define QUIT 5

typedef struct Entry
{
    int id;             // 학번
    char name[10];      // 이름
    struct Entry *next; // 체이닝으로 같은 해시값의 value 저장
} Entry;

Entry *hash_table[MAX_TABLE_SIZE]; // 학생 정보를 저장하는 해시 테이블

/*--------------------------------------------------------------------------
 Function: Given a key, return the corresponding index in the hash table
 Interface: unsinged int hash_fun(int key)
 Parameters: int key: id number of a student
 Return: index in the hash table
--------------------------------------------------------------------------*/
unsigned int hash_fun(int key)
{
    return key % MAX_TABLE_SIZE; // 모듈러 방식을 채택
}

/*--------------------------------------------------------------------------
 Function: insert the student information into the table using hash function
 Interface: int insert(int id, char name[])
 Parameters: int id: number of a student
             char name[]: name of a student
 Return: If memory allocation failed, exit(1)
         otherwise, return OK
--------------------------------------------------------------------------*/
int insert(int id, char name[])
{
    int idx = hash_fun(id); // 해시 함수로 Index 알아내기

    Entry *new_student = (Entry *)malloc(sizeof(Entry)); // 할당해서 삽입
    if (!new_student)
    {
        printf("Memory allocation failed in insert()\n");
        exit(1);
    }

    new_student->id = id;
    strcpy(new_student->name, name);
    new_student->next = NULL; // 학생 정보 초기화

    if (hash_table[idx] == NULL) // idx에 아무 값이 없으면
    {
        hash_table[idx] = new_student;
    }
    else // 충돌. 연결리스트로 추가
    {
        Entry *cur;
        for (cur = hash_table[idx]; cur->next != NULL; cur = cur->next)
            ;

        cur->next = new_student;
    }

    return OK;
}

/*--------------------------------------------------------------------------
 Function: delete the student information in the table using hash function
 Interface: int delete(int id)
 Parameters: int id: number of a student
 Return: if there is no student, return FAIL
         otherwise, return OK
--------------------------------------------------------------------------*/
int delete(int id)
{
    int idx = hash_fun(id); // 해시 함수로 Index 알아내기

    Entry *cur;
    Entry *pre = NULL;
    for (cur = hash_table[idx]; cur != NULL; pre = cur, cur = cur->next) // 연결리스트에서 올바른 id의 값 찾아내기
    {
        if (cur->id == id)
        {
            break;
        }
    }

    if (cur == NULL) // 입력 받은 Id에 해당하는 학생 정보 존재하지 않음
    {
        printf("There is no student with that Id\n");
        return FAIL;
    }

    if (pre == NULL) // 연결리스트의 첫번째를 삭제
    {
        hash_table[idx] = cur->next;
        free(cur);
    }
    else // 첫번째가 아닌 값 삭제
    {
        pre->next = cur->next;
        free(cur);
    }

    return OK;
}

/*--------------------------------------------------------------------------
 Function: retrieve a student's name with certain id
 Interface: int retrieve(int id)
 Parameters: int id: number of a student
 Return: if there is no student, return FAIL
         otherwise, return OK
--------------------------------------------------------------------------*/
int retrieve(int id)
{
    int idx = hash_fun(id); // 해시 함수로 Index 알아내기

    Entry *cur;
    for (cur = hash_table[idx]; cur != NULL; cur = cur->next) // 연결리스트에서 올바른 Id의 학생 정보 찾아내기
    {
        if (cur->id == id)
        {
            break;
        }
    }

    if (cur == NULL) // id를 가진 학생이 존재하지 않음
    {
        printf("There is no student with that Id\n");
        return FAIL;
    }
    else // 학생 정보 출력
    {
        printf("Id: %d\n", cur->id);
        printf("Name: %s\n", cur->name);
    }

    return OK;
}

/*--------------------------------------------------------------------------
 Function: updata a student's name with certain id
 Interface: int update(int id, char name[])
 Parameters: int id: number of a student
             char name[]: name of a student
 Return: if there is no student, return FAIL
         otherwise, return OK
--------------------------------------------------------------------------*/
int update(int id, char name[])
{
    int idx = hash_fun(id); // 해시 함수로 Index 알아내기

    Entry *cur;
    for (cur = hash_table[idx]; cur != NULL; cur = cur->next) // 연결리스트에서 올바른 Id의 학생 정보 찾아내기
    {
        if (cur->id == id)
        {
            break;
        }
    }

    if (cur == NULL) // id를 가진 학생이 존재하지 않음
    {
        printf("There is no student with that Id\n");
        return FAIL;
    }
    else // update 해주기
    {
        printf("Id: %d\n", cur->id);
        printf("%s is changed into %s\n", cur->name, name);
        strcpy(cur->name, name);
    }

    return OK;
}

/*--------------------------------------------------------------------------
 Function: free all entries in the hash table
 Interface: void free_hash()
 Parameters: none
 Return: none
--------------------------------------------------------------------------*/
void free_hash()
{
    for (int i = 0; i < MAX_TABLE_SIZE; i++)
    {
        Entry *cur = hash_table[i];
        while (cur != NULL)
        {
            Entry *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        hash_table[i] = NULL; // 안전하게 초기화
    }
}

int main()
{
    int command;
    int ret; // scanf에 잘못 입력됨을 확인하는 변수

    do
    {
        printf("----------------------------------------------------------------\n");
        printf("                     singly linked list                         \n");
        printf("----------------------------------------------------------------\n");
        printf(" Insert         = 1           Delete         = 2 \n");
        printf(" Update         = 3           Retrieve       = 4 \n");
        printf(" Quit           = 5 \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        while (scanf("%d", &command) != 1) // 입력값이 정수가 아닐 경우
        {
            printf("Wrong Input!\nInput command again = ");
            while (getchar() != '\n')
                ; // 입력 버퍼 비우기
        }

        switch (command)
        {
        case INSERT: // 1
        {
            int id;
            char name[10];

            printf("Input id: ");
            while (scanf("%d", &id) != 1) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput id again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            printf("Input Name: ");
            while (scanf("%s", name) != 1) // 입력값이 문자열이 아닐 경우
            {
                printf("Wrong Input!\nInput name again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (insert(id, name) == OK)
            {
                printf("Inserting is done!\n");
            }
            else
            {
                printf("Inserting is failed\n");
            }
            break;
        }
        case DELETE: // 2
        {
            int id;

            printf("Input Id: ");
            while (scanf("%d", &id) != 1) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput id again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }
            if (delete(id) == OK)
                printf("delete() was complete\n");
            else
                printf("delete() was failed\n");

            break;
        }
        case UPDATE: // 3
        {
            int id;
            char name[10];

            printf("Input id: ");
            while (scanf("%d", &id) != 1) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput id again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            printf("Input Name: ");
            while (scanf("%s", name) != 1) // 입력값이 문자열이 아닐 경우
            {
                printf("Wrong Input!\nInput name again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            update(id, name);

            break;
        }
        case RETRIEVE: // 4
        {
            int id;

            printf("Input Id: ");
            while (scanf("%d", &id) != 1) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput id again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (retrieve(id) == FAIL)
            {
                printf("Retreive() was failed\n");
            }
            else
            {
                printf("Retreive() was complete\n");
            }

            break;
        }
        case QUIT: // 6
            free_hash();
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != QUIT);
}