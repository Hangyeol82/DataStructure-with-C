/*---------------------------------------------------------------------
File Name: B+tree.c
Programmed by: Hangyeol Lee
Affiliation: Chungbuk University
Functions: insert(), delete(), retrieve(), update(),
           dfs(), bfs() in B+tree
Copyright (c) 2025 Hangyeol Lee. All rights reserved.
---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define FAIL 0
#define TREEFILENAME "B+tree.bin"
#define DATAFILENAME "data.bin"
#define ORDER 3 // 차수 3

#define MAX_STACK_SIZE 100
#define STACKOVERFLOW -1
#define STACKUNDERFLOW -2

#define MAX_QUEUE_SIZE 100
#define QUEUE_OVERFLOW -1
#define QUEUE_UNDERFLOW -2

#define INSERT 1
#define DELETE 2
#define RETRIEVE 3
#define UPDATE 4
#define DFS 5
#define BFS 6
#define QUIT 7

typedef struct Node
{
    int is_leaf;         // 리프 노드 유무
    int cnt;             // 자식 개수
    int parent;          // 부모 노드 위치
    int keys[ORDER - 1]; // 자식의 Key, 위치
    int children[ORDER]; // 자식 노드 위치
    int next;            // 형제 위치
} Node;

typedef struct FreeNode
{
    int next;                              // 다음 Free node 위치
    char padding[Node_size - sizeof(int)]; // Node크기 맞추기 위한 Padding
} FreeNode;

typedef struct HeadNode
{
    int root_loc;                              // root node의 위치
    int insert_loc;                            // free 연결리스트의 첫번째 노드 위치
    char padding[Node_size - sizeof(int) * 2]; // Node크기 맞추기 위한 Padding
} HeadNode;

typedef struct Data
{
    char name[10]; // data.bin에 저장할 데이터
} Data;

typedef struct FreeData
{
    int next;                              // 다음 FreeData 위치
    char padding[Data_size - sizeof(int)]; // Data크기 맞추기 위한 padding
} FreeData;

typedef struct HeadData
{
    int insert_loc;                        // free 연결리스트의 첫번째 노드 위치
    char padding[Data_size - sizeof(int)]; // Data크기 맞추기 위한 padding
} HeadData;

const int Node_size = sizeof(Node); // 구조체 Node의 크기
const int Data_size = sizeof(Data); // 구조체 Data의 크기
/*
   바이너리 파일:
    .bin 확장자로 끝나는 파일, 이진 데이터를 저장하여 그냥 열면 읽을 수 없다
    바이너리 파일엔 int, char, 구조체 노드 등 다양한 정보를 저장할 수 있다.
*/

/*
   사용하는 함수 정리:
    size_t fread(void *ptr, size_t size, size_t count, FILE *stream)
           - 파일 포인터가 가르키는 위치에서 size 만큼 count 개수의 정보를 ptr에 읽어 온다
    int fseek(FILE *stream, long offset, int origin)
           - origin에서부터 원하는 offset만큼 떨어진 위치로 파일 포인터를 옮겨준다
    size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream)
           - 파일 포인터가 가르키는 위치에 size크기만큼 count개수의 정보를 덮어쓴다.

    size_t: 양의 정수형 typedef로 정의되어 있는데
            운영체제나 컴파일러에 따라 int, long ,long long에 매핑
*/

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
 Function: initialize bin file and location variable
 Interface: void initialize_tree()
 Parameter: None
 return: void
------------------------------------------------------------------------------*/
void initialize_tree()
{
    FILE *tree = fopen(TREEFILENAME, "wb");
    FILE *data = fopen(DATAFILENAME, "wb");

    HeadNode head;
    head.insert_loc = -1;
    head.root_loc = 1; // 헤드노드 초기화

    fwrite(&head, Node_size, 1, tree);

    fclose(tree);
}

int insert_data(char name[])
{
    FILE *data = fopen(DATAFILENAME, "r+b");
    if (data == NULL)
    {
        printf("Error in insert()\n");
        exit(1);
    }

    Data insert;               // 삽입할 데이터
    strcpy(insert.name, name); // insert 초기화 (학생 이름)

    HeadData head; // 헤드 데이터

    if (head.insert_loc == -1) // FreeData 연결리스트가 존재하지 않는 경우
    {
        fseek(data, 0, SEEK_END);
        fwrite(&insert, Data_size, 1, data); // 맨 뒤에 삽입
    }
    else // 빈공간(연결리스트)에 삽입
    {
        FreeData tmp;                           // FreeNode의 정보 저장용 변수
        fseek(data, head.insert_loc, SEEK_SET); // 연결리스트 첫번째 노드 접근
        fwrite(&tmp, Data_size, 1, data);       // 첫번째 노드 tmp에 저장

        fseek(data, head.insert_loc, SEEK_END); // 다시 첫번째 노드 접근
        fwrite(&insert, Data_size, 1, data);    // 빈 공간(연결리스트 첫번째 노드 위치)에 삽입

        head.insert_loc = tmp.next; // 첫번째 노드의 다음 노드의 위치 Insert_loc에 저장

        fseek(data, 0, SEEK_SET);          // 헤드 데이터로 이동
        fwrite(&head, Data_size, 1, data); // 최신화된 헤드 데이터 파일에 저장
    }

    fclose(data);
    return head.insert_loc;
}

int insert(int num, char name[])
{
    FILE *tree = fopen(TREEFILENAME, "r+b"); // 읽기+쓰기 모드 기존의 데이터 유지
    if (tree == NULL)
    {
        printf("Error in insert()\n");
        exit(1);
    }

    HeadNode head;
    fread(&head, Node_size, 1, tree);

    fseek(tree, 0, SEEK_END);               // 파일 끝으로 이동
    int size = ftell(tree) / Node_size - 1; // 파일 크기, 0이면 노드가 비어있다 (헤드 노드 제외)

    Node new_node;

    if (size == 0) // 트리가 존재하지 않을 때
    {
        new_node.is_leaf = 1;
        new_node.parent = -1;
        new_node.cnt = 1;
        new_node.keys[0] = num;
        new_node.children[0] = size;
        new_node.next = -1;

        fwrite(&new_node, Node_size, 1, tree); // 노드 저장
        insert_data(name);                     // 데이터 저장
    }
    else
    {
        int loc = head.root_loc; // 탐색하는 위치

        Node cur;                               // 탐색용 노드
        fseek(tree, Node_size * loc, SEEK_SET); // 루트 노드의 위치로 이동
        fread(&cur, Node_size, 1, tree);        // 루트 노드 읽기

        while (cur.is_leaf != 1) // 리프 노드를 찾는 과정
        {
            for (int i = 0; i < cur.cnt; i++) // keys(분기)
            {
                if (num < cur.keys[i]) // keys[i]의 값보다 작으면 children[i]로 이동
                {
                    loc = cur.children[i];
                }
            }
        }
        /* 노드의 Keys & children 밀어내기 및 값 삽입 */
        int index; // keys 중에 Num보다 큰 key의 index 저장

        for (int i = 0; i < cur.cnt; i++) // 올바른 index 찾기
        {
            if (num < cur.keys[i])
            {
                index = i;
            }
        }

        for (int i = cur.cnt; i >= index; i--) // keys & children 배열 밀어내기
        {
            cur.keys[i + 1] = cur.keys[i];
            cur.children[i + 1] = cur.children[i];
        }

        int data_loc = insert_data(name); // index에 data.bin에 저장된 데이터 위치 저장

        cur.keys[index] = num;          // keys에 분기점 저장
        cur.children[index] = data_loc; // children에 data.bin에 저장된 위치 저장
        cur.cnt++;                      // cnt값 1증가

        if (cur.cnt >= ORDER) // 오버플로우 -> split
        {
            /* 리프노드 분기 */
            Node tmp_node = cur; // 포인터 없어서 깊은 복사
            cur.cnt = (ORDER - 1) / 2;

            Node second; // 노드를 두개로 분기
            second.is_leaf = 1;
            second.cnt = ORDER - cur.cnt;

            int fi, si = 0; // cur, second의 key & children index 변수
            for (int i = 0; i < ORDER; i++)
            {
                if (i < cur.cnt)
                {
                    cur.keys[fi] = tmp_node.keys[i];
                    cur.children[fi++] = tmp_node.children[i];
                }
                else
                {
                    second.keys[si] = tmp_node.keys[i];
                    second.children[si] = tmp_node.children[i];
                }
            }

            if(tmp_node.parent != -1) // 원래 리프노드의 부모가 있었으면
            {
                second.parent = tmp_node.parent;
            }
            else // 부모가 없으면 새로운 Non-leaf노드 생성
            {

            }
        }
    }
}

/*------------------------------------------------------------------------------
 Function: A debugging function that prints the nodes
           in the order they are stored in the file
 Interface: void print_tree()
 Parameter: None
 return: void
------------------------------------------------------------------------------*/
void print_tree()
{
    FILE *tree = fopen(TREEFILENAME, "r+b");

    Node node;

    int line = 0;

    while (fread(&node, Node_size, 1, tree) == 1)
    {
        printf("%d\t%d\t%d\n", node.num, node.left, node.right);
        line++;
    }
    printf("insert location: %d\nfree location: %d\nroot location: %d\n", insert_loc, free_loc, root_loc);
    fclose(tree);
}

int main()
{
    initialize_tree(); // 트리 초기화

    int command;
    do
    {
        printf("----------------------------------------------------------------\n");
        printf("                      binary serach tree                        \n");
        printf("----------------------------------------------------------------\n");
        printf(" Add Node       = 1           Delete Node    = 2 \n");
        printf(" Retrieve Node  = 3           Update Node    = 4 \n");
        printf(" Dfs            = 5           Bfs            = 6 \n");
        printf(" Quit           = 7 \n");
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
            int num;       // 학번 (노드 번호)
            char name[10]; // 이름

            printf("Input number and name :");
            while (scanf("%d %s", &num, name) != 2) // 입력값이 정상이 아닐 경우
            {
                printf("Wrong Input!\nInput command again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (insert(num, name) == FAIL)
            {
                printf("insert() is failed\n");
            }
            else
            {
                printf("insert() is complete\n");
            }

            break;
        }
        case DELETE: // 2
        {
            int num; // 삭제할 노드 번호

            printf("Input number :");
            while (scanf("%d", &num) != 1) // 입력값이 정상이 아닐 경우
            {
                printf("Wrong Input!\nInput command again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (delete(num) == FAIL)
            {
                printf("delete() is failed\n");
            }
            else
            {
                printf("delete() is complete\n");
            }

            break;
        }
        case RETRIEVE:
        {
            int num; // 삭제할 노드 번호

            printf("Input number :");
            while (scanf("%d", &num) != 1) // 입력값이 정상이 아닐 경우
            {
                printf("Wrong Input!\nInput command again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (delete(num) == FAIL)
            {
                printf("retreive() is failed\n");
            }
            else
            {
                printf("retrieve() is complete\n");
            }

            break;
        }
        case UPDATE:
        {
            int num;       // 학번 (노드 번호)
            char name[10]; // 이름

            printf("Input number and name :");
            while (scanf("%d %s", &num, name) != 2) // 입력값이 정상이 아닐 경우
            {
                printf("Wrong Input!\nInput command again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (update(num, name) == FAIL)
            {
                printf("update() is failed\n");
            }
            else
            {
                printf("update() is complete\n");
            }

            break;
        }
        case DFS: // 5
        {
            if (dfs() == FAIL)
            {
                printf("dfs() is failed\n");
            }
            else
            {
                printf("dfs() is complete\n");
            }
            break;
        }
        case BFS: // 6
        {
            if (bfs() == FAIL)
            {
                printf("bfs() is failed\n");
            }
            else
            {
                printf("bfs() is complete\n");
            }
            break;
        }
        case QUIT: // 7
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != QUIT);
}