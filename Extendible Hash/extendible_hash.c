/*---------------------------------------------------------------------
File Name: extendible_hash.c
Programmed by: Hangyeol Lee
Affiliation: Chungbuk University
Functions: insert(), delete(), retrieve(), update() in extendible_hash
Copyright (c) 2025 Hangyeol Lee. All rights reserved.
---------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SLOT 4
#define OK 1
#define FAIL 0

#define INSERT 1
#define DELETE 2
#define UPDATE 3
#define RETRIEVE 4
#define QUIT 5

typedef struct Student
{
    int id;        // 학번
    char name[10]; // 이름
} Student;

typedef struct Bucket
{
    int local_depth;        // 현재 버킷의 depth
    int cnt;                // 현재 버킷의 학생 수
    Student slot[MAX_SLOT]; // 학생을 담는 배열
} Bucket;

typedef struct Directory
{
    int global_depth; // 해시 전체의 Depth
    int size;         // 해시 전체의 크기 (2^n 개)
    Bucket **address; // 버킷의 주소를 담는 배열
} Directory;

Directory ext_hash; // 전역변수로 선언한 해시

/*--------------------------------------------------------------------------
 Function: Given a key, return the corresponding index in the hash table
 Interface: int hash_fun(int n, int k)
 Parameters: int n: number of a student
             int k: local_depth or global_depth
 Return: index in the bucket address of ext_hash
--------------------------------------------------------------------------*/
int hash_fun(int n, int k)
{
    return (n & ((1 << k) - 1)); // 하위 K개의 비트로 해시 값 생성
}

/*--------------------------------------------------------------------------
 Function: if a bucket is full,
           split a bucket or increase the number of buckets
 Interface: void bucket_split(Bucket *cur, int index)
 Parameters: Bucket *cur: address of bucket which is full
             int index: index of cur in the bucket address array
 Return: if error occurs, exit(1)
--------------------------------------------------------------------------*/
void bucket_split(Bucket *cur, int index)
{
    if (cur->local_depth == ext_hash.global_depth) // local_depth가 global_depth와 같으면 버킷 개수 늘리기
    {
        int new_size = ext_hash.size * 2; // 기존 버킷의 2배 늘려야 한다

        Bucket **tmp = (Bucket **)realloc(ext_hash.address, sizeof(Bucket *) * new_size);
        if (tmp == NULL)
        {
            printf("Error in bucket_split()\n");
            exit(1);
        }
        ext_hash.address = tmp;

        for (int i = new_size / 2; i < new_size; i++) // 우선 버킷의 주소 값을 똑같이 만들기
        {
            ext_hash.address[i] = ext_hash.address[i - new_size / 2];
        }

        ext_hash.size = new_size; // 사이즈 변경
        ext_hash.global_depth++;  // global_depth + 1
    }

    int cur_idx = index;                           // 기존 버킷
    int new_idx = (1 << cur->local_depth) | index; // 새로운 버킷 (기존 버킷비트 앞에 1 붙이기)
    int old_d = cur->local_depth;                  // depth 늘리기 전

    Bucket *new = (Bucket *)malloc(sizeof(Bucket));
    new->cnt = 0;
    new->local_depth = cur->local_depth; // 새로운 버킷 초기화

    for (int i = 0; i < ext_hash.size; ++i) // 모든 주소를 돌며 특정 비트가 같은 인덱스만 주소를 동일하게 저장
    {
        if (ext_hash.address[i] == cur && ((i >> old_d) & 1))
        {
            ext_hash.address[i] = new;
        }
    }

    int old_cnt = cur->cnt; // 기존의 개수
    cur->cnt = 0;
    cur->local_depth++;
    new->local_depth++; // Depth 늘려주기

    for (int i = 0; i < old_cnt; i++) // cur의 값을 New 버킷에 나누어 주기
    {
        int idx = hash_fun(cur->slot[i].id, cur->local_depth);
        if (idx == cur_idx)
        {
            cur->slot[cur->cnt++] = cur->slot[i];
        }
        else
        {
            new->slot[new->cnt++] = cur->slot[i];
        }
    }
}

/*--------------------------------------------------------------------------
 Function: insert a student info in an extendiblie hash
 Interface: int insert(int id, char name[])
 Parameters: int id: ID of a student
             char name: name of a student
 Return: if error occurs in bucket_split(), exit(1)
         otherwise, return OK
--------------------------------------------------------------------------*/
int insert(int id, char name[])
{
    while (1)
    {
        int idx = hash_fun(id, ext_hash.global_depth); // 해시 함수로 Index 찾기
        Bucket *cur = ext_hash.address[idx];
        if (cur->cnt >= MAX_SLOT) // 꽉찬 버킷이면 분할 해주고 재시작
        {
            bucket_split(cur, idx);
            continue;
        }

        cur->slot[cur->cnt].id = id;
        strcpy(cur->slot[cur->cnt++].name, name); // 알맞은 버킷 찾았으면 값 삽입
        break;
    }

    return OK;
}

/*--------------------------------------------------------------------------
 Function: Delete student information in the bucket, and if necessary,
           merge buckets or reduce the bucket count (global depth).
 Interface: int delete(int id)
 Parameters: int id: Id of student to be deleted
 Return: If there is no student with given ID, return FAIL
         otherwise, return OK
--------------------------------------------------------------------------*/
int delete(int id)
{
    int idx = hash_fun(id, ext_hash.global_depth); // 해시함수로 버킷 찾기
    Bucket *cur = ext_hash.address[idx];

    int check = 0; // 값이 존재하는지 확인하는 변수
    int i;

    for (i = 0; i < cur->cnt; i++)
    {
        if (cur->slot[i].id == id) // 존재하면 check = 1 바꾸고 break
        {
            check = 1;
            break;
        }
    }

    if (check) // 값이 존재하면
    {
        printf("Id: %d Name: %s is deleted\n", id, cur->slot[i].name);
        for (; i < cur->cnt - 1; i++) // 배열이므로 빈공간 압축
        {
            cur->slot[i] = cur->slot[i + 1];
        }
        cur->cnt -= 1;
        if (cur->cnt != 0) // 개수가 0이 되면 버킷 Merge 할 수도 있음
            return OK;
    }
    else // 값이 존재하지 않으면
    {
        printf("There is no student with such Id\n");
        return FAIL;
    }

    /* 여기서 부터 병합 시작 */
    int live_idx = idx ^ (1 << (cur->local_depth - 1));
    Bucket *live = ext_hash.address[live_idx];

    if (live->local_depth == cur->local_depth) // 짝꿍 버킷과 depth가 동일해야 병합 가능
    {
        check = (idx >> (cur->local_depth - 1)) & 1; // cnt가 0이 된 버킷의 맨 앞의 비트 확인

        if (!check) // 맨 앞의 비트가 0인 버킷을 남겨야지 global depth를 줄일 가능성이 생김
        {
            cur = live; // 맨 앞의 비트가 0이면 짝꿍의 주소 옮겨주기
        }

        for (i = idx; i >= 0; i -= (1 << (cur->local_depth)))
        {
            ext_hash.address[i] = live; // 주소 변경
        }
        live->local_depth--; // depth 줄이기

        check = 1;                                         // global depth를 줄여도 되는지 확인하는 변수
        if (cur->local_depth == ext_hash.global_depth - 1) // global depth 줄일수 있는지 시도
        {
            for (int i = 0; i < ext_hash.size / 2; i++)
            {
                if (ext_hash.address[i] != ext_hash.address[i + ext_hash.size / 2]) // 절반 접어서 동일한지 확인
                {
                    check = 0; // 동일하지 않으면 줄일수 없음
                    break;
                }
            }
        }

        if (check) // global depth 줄이기
        {
            Bucket **tmp = (Bucket **)realloc(ext_hash.address, sizeof(Bucket *) * ext_hash.size);
            ext_hash.address = tmp;
            ext_hash.global_depth -= 1; // global depth 줄이기
            ext_hash.size /= 2;         // 2로 나누기
        }
    }
    return OK;
}

/*--------------------------------------------------------------------------
 Function: Retrieve a student's name by the given ID
 Interface: int retrieve(int id)
 Parameters: int id: ID of a student
 Return: if there is no student with the given ID, return FAIL
         otherwise, return OK
--------------------------------------------------------------------------*/
int retrieve(int id)
{
    int idx = hash_fun(id, ext_hash.global_depth); // 해시 함수로 index 찾기
    Bucket *cur = ext_hash.address[idx];

    int check = 0; // 값이 존재하는지 확인하는 변수
    int i;

    for (i = 0; i < cur->cnt; i++) // 배열을 돌면서 값 존재 유무 확인
    {
        if (cur->slot[i].id == id)
        {
            check = 1; // 존재하면 Check = 1 변경하고 break
            break;
        }
    }

    if (check) // 학생이 존재함
    {
        printf("Name: %s\n", cur->slot[i].name);
        return OK;
    }
    else // 학생이 없음
    {
        printf("There is no student with such Id\n");
        return FAIL;
    }
}

/*--------------------------------------------------------------------------
 Function: update a student's name by the given ID
 Interface: int update(int id, char name[])
 Parameters: int id: ID of a student
             char name[]: New name to be assigned to the student
 Return: if there is no student with the given ID, return FAIL
         otherwise, return OK
--------------------------------------------------------------------------*/
int update(int id, char name[])
{
    int idx = hash_fun(id, ext_hash.global_depth); // 해시 함수로 index 찾기
    Bucket *cur = ext_hash.address[idx];

    int check = 0; // 값이 존재하는지 확인하는 변수
    int i;

    for (i = 0; i < cur->cnt; i++) // 배열을 돌면서 값 존재 유무 확인
    {
        if (cur->slot[i].id == id)
        {
            check = 1; // 존재하면 Check = 1 변경하고 break
            break;
        }
    }

    if (check) // 학생이 존재함
    {
        printf("%s is changed into %s\n", cur->slot[i].name, name);
        strcpy(cur->slot[i].name, name);
        return OK;
    }
    else // 학생이 없음
    {
        printf("There is no student with such Id\n");
        return FAIL;
    }
}

/*--------------------------------------------------------------------------
 Function: debuging function that prints all values in extendible_hash
 Interface: void dump_dir(void)
 Parameters: void
 Return: void
--------------------------------------------------------------------------*/
void dump_dir(void)
{
    printf("\n[DIR] g=%d size=%d\n", ext_hash.global_depth, ext_hash.size); // global depth와 size 출력
    for (int i = 0; i < ext_hash.size; ++i)
    {
        Bucket *b = ext_hash.address[i];
        printf("[%2d] -> %p", i, (void *)b); // index와 버킷의 주소 출력

        if (b) // 버킷이 존재하면 버킷과 학생 정보 출력
        {
            printf(" (ld=%d,cnt=%d):", b->local_depth, b->cnt);
            for (int k = 0; k < b->cnt; ++k)
                printf(" %d", b->slot[k].id);
        }
        putchar('\n');
    }
    putchar('\n');
}

int main()
{
    ext_hash.global_depth = 1;
    ext_hash.size = 2;
    ext_hash.address = calloc(2, sizeof(Bucket *)); // ext_hash 초기화
    Bucket *b0 = calloc(1, sizeof(Bucket));
    b0->local_depth = 0;
    b0->cnt = 0;
    ext_hash.address[0] = ext_hash.address[1] = b0; // 버킷 초기화

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

            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != QUIT);
}