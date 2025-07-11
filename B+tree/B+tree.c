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
#define EMPTY 1
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
    int is_leaf;             // 리프 노드 유무
    int cnt;                 // 자식 개수
    int parent;              // 부모 노드 위치
    int keys[ORDER];         // 자식의 Key, 위치
    int children[ORDER + 1]; // 자식 노드 위치
    int next;                // 형제 위치
} Node;

const int Node_size = sizeof(Node); // 구조체 Node의 크기

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

const int Data_size = sizeof(Data); // 구조체 Data의 크기

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

    HeadNode head_node;
    head_node.insert_loc = -1;
    head_node.root_loc = 1; // 헤드노드 초기화

    HeadData head_data;
    head_data.insert_loc = -1;

    fwrite(&head_node, Node_size, 1, tree);
    fwrite(&head_data, Data_size, 1, data);

    fclose(tree);
    fclose(data);
}

/*------------------------------------------------------------------------------
 Function: insert a data in the data binary file
 Interface: int insert_data(char name[])
 Parameter: char name[]: data(student name)
 return: the location of inserted data in the data binary file
------------------------------------------------------------------------------*/
int insert_data(char name[])
{
    FILE *data = fopen(DATAFILENAME, "r+b");
    if (data == NULL)
    {
        printf("Error in insert_data() \n");
        exit(1);
    }
    Data insert;               // 삽입할 데이터
    strcpy(insert.name, name); // insert 초기화 (학생 이름)

    HeadData head; // 헤드 데이터
    fread(&head, Data_size, 1, data);

    int result; // 데이터가 삽입된 위치

    if (head.insert_loc == -1) // FreeData 연결리스트가 존재하지 않는 경우
    {
        fseek(data, 0, SEEK_END);
        result = ftell(data) / Data_size;    // 삽입하는 데이터 위치 저장
        fwrite(&insert, Data_size, 1, data); // 맨 뒤에 삽입
    }
    else // 빈공간(연결리스트)에 삽입
    {
        FreeData tmp;                           // FreeNode의 정보 저장용 변수
        fseek(data, head.insert_loc, SEEK_SET); // 연결리스트 첫번째 노드 접근
        fwrite(&tmp, Data_size, 1, data);       // 첫번째 노드 tmp에 저장

        fseek(data, head.insert_loc, SEEK_END); // 다시 첫번째 노드 접근
        result = head.insert_loc;               // 삽입하는 데이터 위치 저장
        fwrite(&insert, Data_size, 1, data);    // 빈 공간(연결리스트 첫번째 노드 위치)에 삽입

        head.insert_loc = tmp.next; // 첫번째 노드의 다음 노드의 위치 Insert_loc에 저장

        fseek(data, 0, SEEK_SET);          // 헤드 데이터로 이동
        fwrite(&head, Data_size, 1, data); // 최신화된 헤드 데이터 파일에 저장
    }
    fclose(data);

    return result;
}

/*------------------------------------------------------------------------------
 Function: insert a node in the tree binary file
 Interface: int insert_node(FILE *tree, Node node)
 Parameter: FILE *tree: file pointer of tree binary file
            Node node: a node to be inserted
 return: the location of inserted node in the tree binary file
------------------------------------------------------------------------------*/
int insert_node(FILE *tree, Node node)
{
    HeadNode head;
    fseek(tree, 0, SEEK_SET);
    fread(&head, Node_size, 1, tree);

    int result; // 삽입한 노드 위치

    if (head.insert_loc == -1)
    {
        fseek(tree, 0, SEEK_END);
        result = ftell(tree) / Node_size;
        fwrite(&node, Node_size, 1, tree);
    }
    else
    {
        FreeNode tmp;                           // FreeNode의 정보 저장용 변수
        fseek(tree, head.insert_loc, SEEK_SET); // 연결리스트 첫번째 노드 접근
        fread(&tmp, Node_size, 1, tree);        // 첫번째 노드 tmp에 저장

        fseek(tree, head.insert_loc, SEEK_END); // 다시 첫번째 노드 접근
        result = head.insert_loc;
        fwrite(&node, Node_size, 1, tree); // 빈 공간(연결리스트 첫번째 노드 위치)에 삽입

        head.insert_loc = tmp.next; // 첫번째 노드의 다음 노드의 위치 Insert_loc에 저장

        fseek(tree, 0, SEEK_SET);          // 헤드 데이터로 이동
        fwrite(&head, Node_size, 1, tree); // 최신화된 헤드 데이터 파일에 저장
    }

    return result;
}

/*------------------------------------------------------------------------------
 Function: insert node into parent and split a node
 Interface: insert_into_parent(FILE *tree, int parent_loc,
            int left_child_loc, int right_child_loc, int key)
 Parameter: FILE *tree: file pointer of tree binary file
            int parent_loc: location of parent to be inserted
            int left_child_loc: the left child node after split
            int right_child_loc: the right child node after split
            int key: the value to be inserted in a parent node
 return: if a root node is splited, return a location of a new root node
         otherwise, return OK
------------------------------------------------------------------------------*/
int insert_into_parent(FILE *tree, int parent_loc, int left_child_loc, int right_child_loc, int key)
{
    Node parent;
    if (parent_loc == -1) // 루트 노드에서 Key값을 올리는 경우
    {
        /* 새로운 루트노드 생성후 삽입 */
        parent.is_leaf = 0;
        parent.parent = -1;
        parent.cnt = 1;
        parent.children[0] = left_child_loc;
        parent.children[1] = right_child_loc;
        parent.keys[0] = key;

        int result = insert_node(tree, parent); // 새로만든 루트 노드의 위치를 저장

        HeadNode head;
        fseek(tree, 0, SEEK_SET);
        fread(&head, Node_size, 1, tree);

        head.root_loc = result; // 헤더노드의 Root_loc의 위치 변경

        fseek(tree, 0, SEEK_SET);
        fwrite(&head, Node_size, 1, tree); // 헤더노드 최신화

        return result; // 새로만든 루트노드의 위치 return
    }
    else
    {
        fseek(tree, Node_size * parent_loc, SEEK_SET);
        fread(&parent, Node_size, 1, tree);
        int index; // key가 들어가야하는 위치

        for (int i = 0; i <= parent.cnt; i++) // index 찾기
        {
            index = i;
            if (parent.keys[i] > key)
            {
                break;
            }
        }

        for (int i = parent.cnt; i > index; i--) // 밀어내기
        {
            parent.keys[i] = parent.keys[i - 1];
            parent.children[i + 1] = parent.children[i];
        }
        parent.keys[index] = key;                     // key 삽입
        parent.children[index + 1] = right_child_loc; // 우측 자식 위치 삽입
        parent.cnt++;                                 // Cnt + 1

        if (parent.cnt >= ORDER) // overflow!
        {
            parent.cnt = (ORDER - 1) / 2; // cnt 개수 변경

            Node second;
            second.is_leaf = 0;
            second.parent = parent.parent;
            second.cnt = ORDER / 2; // 새로운 노드 second 초기화

            int si = 0;
            for (int i = parent.cnt + 1; i <= ORDER; i++) // second 초기화
            {
                second.children[si] = parent.children[i];
                second.keys[si++] = parent.keys[i];
            }
            second.children[si] = parent.children[ORDER];

            int second_loc = insert_node(tree, second);                                                                    // 삽입하고 삽입한 위치 반환
            int new_parent_loc = insert_into_parent(tree, parent.parent, parent_loc, second_loc, parent.keys[parent.cnt]); // 재귀적으로 해결

            if (second.parent == -1)
            {
                parent.parent = second.parent = new_parent_loc; // 생성된 부모 위치 기입

                fseek(tree, Node_size * second_loc, SEEK_SET);
                fwrite(&second, Node_size, 1, tree); // 업데이트
            }
        }
        fseek(tree, Node_size * parent_loc, SEEK_SET);
        fwrite(&parent, Node_size, 1, tree); // 업데이트

        return OK;
    }
}

/*------------------------------------------------------------------------------
 Function: insert data and nodes through a tree and adjust the tree structure
 Interface: int insert(int num, char name[])
 Parameter: int num: a number of a student
            char name[]: a name of a student
 return: If the file pointer is valid, return OK
------------------------------------------------------------------------------*/
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

    fseek(tree, 0, SEEK_END);           // 파일 끝으로 이동
    int size = ftell(tree) / Node_size; // 파일 크기, 0이면 노드가 비어있다 (헤드 노드 제외)

    Node new_node;

    if (size == EMPTY) // 트리가 존재하지 않을 때
    {
        // 새로운 노드와 데이터 삽입
        new_node.is_leaf = 1;
        new_node.parent = -1;
        new_node.cnt = 1;
        new_node.keys[0] = num;
        new_node.children[0] = size;
        new_node.next = -1;

        insert_data(name);                     // 데이터 저장
        fwrite(&new_node, Node_size, 1, tree); // 노드 저장
    }
    else
    {
        int loc = head.root_loc; // 탐색하는 위치

        Node cur;                               // 탐색용 노드
        fseek(tree, Node_size * loc, SEEK_SET); // 루트 노드의 위치로 이동
        fread(&cur, Node_size, 1, tree);        // 루트 노드 읽기

        while (cur.is_leaf != 1) // 리프 노드를 찾는 과정
        {
            for (int i = 0; i <= cur.cnt; i++) // keys(분기)
            {
                loc = cur.children[i];
                if (num < cur.keys[i]) // keys[i]의 값보다 작으면 children[i]로 이동
                {
                    break;
                }
            }
            fseek(tree, Node_size * loc, SEEK_SET); // 노드의 위치로 이동
            fread(&cur, Node_size, 1, tree);        // 노드 읽기
        }
        /* 노드의 Keys & children 밀어내기 및 값 삽입 */
        int index = 0; // keys 중에 Num보다 큰 key의 index 저장

        for (int i = 0; i <= cur.cnt; i++) // 올바른 index 찾기
        {
            index = i;
            if (num < cur.keys[i])
            {
                break;
            }
        }

        for (int i = cur.cnt - 1; i >= index; i--) // keys & children 배열 밀어내기
        {
            cur.keys[i + 1] = cur.keys[i];
            cur.children[i + 1] = cur.children[i];
        }
        int data_loc = insert_data(name); // index에 data.bin에 저장된 데이터 위치 저장
        if (num == 18)
            printf("Finding Error!\n");

        cur.keys[index] = num;          // keys에 분기점 저장
        cur.children[index] = data_loc; // children에 data.bin에 저장된 위치 저장
        cur.cnt++;                      // cnt값 1증가

        if (cur.cnt >= ORDER) // 오버플로우 -> split
        {
            /* 리프노드 분기 */
            cur.cnt = ORDER / 2;

            Node second; // 노드를 두개로 분기
            second.is_leaf = 1;
            second.cnt = ORDER - cur.cnt;
            second.parent = cur.parent;

            int si = 0; // second의 key & children의 값을 할당
            for (int i = cur.cnt; i < ORDER; i++)
            {
                second.keys[si] = cur.keys[i];
                second.children[si++] = cur.children[i];
            }
            int second_loc = insert_node(tree, second);                                                    // 새로 만든 리프노드 파일에 삽입
            int new_parent_loc = insert_into_parent(tree, second.parent, loc, second_loc, second.keys[0]); // 부모가 없으면 새로운 부모위치 리턴, 부모가 있으면 OK 리턴

            cur.next = second_loc; // 리프노드끼리 연결리스트로 연결

            if (cur.parent == -1) // 부모가 없는 경우
            {
                cur.parent = second.parent = new_parent_loc;

                fseek(tree, Node_size * second_loc, SEEK_SET);
                fwrite(&second, Node_size, 1, tree); // second 노드 업데이트 (부모)
            }
        }
        fseek(tree, Node_size * loc, SEEK_SET);
        fwrite(&cur, Node_size, 1, tree); // Cur 노드 업데이트
    }
    fclose(tree);

    return OK;
}

int delete(int num)
{
    FILE *tree = fopen(TREEFILENAME, "r+b");
    if (tree == NULL)
    {
        printf("Error in delete()\n");
        exit(1);
    }

    HeadNode head;
    fread(&head, Node_size, 1, tree);

    Node cur;
    fseek(tree, head.root_loc * Node_size, SEEK_SET);
    fread(&cur, Node_size, 1, tree);

    int index; // 삭제하는 key의 위치

    while (cur.is_leaf != 1) // 리프노드를 찾는 과정
    {
        for (int i = 0; i <= cur.cnt; i++)
        {
            index = i;
            if (cur.keys[i] > num)
            {
                break;
            }
        }
        fseek(tree, cur.children[index] * Node_size, SEEK_SET);
        fread(&cur, Node_size, 1, tree);
    }

    for (index = 0; index < cur.cnt; index++) // 리프노드에 동일한 학번이 있는지 검사
    {
        if (cur.keys[index] >= num)
        {
            break;
        }
    }

    if (cur.keys[index] != num) // 학번이 없으면 return FAIL
    {
        printf("There is no such node\n");
        return FAIL;
    }
    /* data 파일에 있는 데이터 삭제하는 과정 */
    FILE *fdata = fopen(DATAFILENAME, "r+b");
    if (fdata == NULL)
    {
        printf("Error in delte()\n");
        exit(1);
    }

    HeadData data_head;
    fread(&data_head, Data_size, 1, fdata); // data파일의 헤더 읽기

    FreeData free;
    if (data_head.insert_loc != -1) // Free연결리스트가 존재하면
    {
        free.next = data_head.insert_loc; // 연결리스트 맨 앞에 삽입
    }
    else // 연결리스트가 없으면
    {
        free.next = -1;
    }
    data_head.insert_loc = cur.children[index];

    fseek(fdata, cur.children[index] * Data_size, SEEK_SET);
    fwrite(&free, Data_size, 1, fdata); // 삭제한 부분 Free 연결리스트에 추가

    fseek(fdata, 0, SEEK_SET);
    fwrite(&data_head, Data_size, 1, fdata); // 헤더데이터 수정

    fclose(fdata);

    for (int i = index; index < cur.cnt - 1; i++) // index에 해당하는 Key & child 삭제하는 과정
    {
        cur.keys[i] = cur.keys[i + 1];
        cur.children[i] = cur.keys[i + 1];
    }
    cur.cnt--; // cnt - 1

    if (cur.cnt < ORDER / 2 - 1) // underflow!
    {
        
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
    FILE *data = fopen(DATAFILENAME, "r+b");

    HeadNode head;
    fread(&head, Node_size, 1, tree);

    Node node;
    Data dataum;

    int line = 0;

    while (fread(&node, Node_size, 1, tree) == 1) // 노드가 안 읽힐 때 까지
    {
        printf("==========================\n");
        printf("count: %d\n", node.cnt);
        if (node.is_leaf) // 리프노드인 경우
        {
            printf("leaf node!\n");
            for (int i = 0; i < node.cnt; i++)
            {
                fseek(data, node.children[i] * Data_size, SEEK_SET);
                fread(&dataum, Data_size, 1, data); // 데이터 읽기
                // 학번(Key) 파일의 위치(data_loc) 실제 데이터(Data) 출력
                printf("key: %d data_loc: %d data: %s\n", node.keys[i], node.children[i], dataum.name);
            }
        }
        else // 내부노드인 경우
        {
            printf("Non-leaf node\n");
            int i;
            for (i = 0; i < node.cnt; i++)
            {
                // 자식노드 위치(child location) 분기점(key) 출력
                printf("child location: %d\tkey: %d\n", node.children[i], node.keys[i]);
            } // 마지막 자식노드 위치 출력
            printf("last child location: %d\n", node.children[i]);
        }
        line++;
    }
    printf("==========================\n");
    printf("insert location: %d\nroot location: %d\n", head.insert_loc, head.root_loc); // 헤더노드 출력
    fclose(tree);
}

int main()
{
    initialize_tree(); // 트리 초기화

    insert(10, "A");
    insert(20, "B");
    insert(17, "C");
    print_tree();
}