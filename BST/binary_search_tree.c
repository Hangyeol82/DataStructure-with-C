/*---------------------------------------------------------------------
File Name: binary_serach_tree.c
Programmed by: Hangyeol Lee
Affiliation: Chungbuk University
Functions: insert(), delete(), retrieve(), update(),
           dfs(), bfs() in BST
Copyright (c) 2025 Hangyeol Lee. All rights reserved.
---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "stack_queue.h"

#define OK 1
#define FAIL 0
#define FILENAME "bst.bin"
#define EMPTY 1

#define INSERT 1
#define DELETE 2
#define RETRIEVE 3
#define UPDATE 4
#define DFS 5
#define BFS 6
#define QUIT 7

// 헤드 노드를 둬서 파일에 저장하기

typedef struct node
{
    int num;       // 학번
    char name[10]; // 이름
    int right;     // 우측 노드
    int left;      // 좌측 노드
} Node;

const int Node_size = sizeof(Node); // 구조체 Node의 크기

typedef struct freeNode
{
    int next;                              // 다음 Free node 위치
    char padding[Node_size - sizeof(int)]; // Node크기 맞추기 위한 Padding
} FreeNode;

typedef struct headNode
{
    int root_loc;                              // 트리의 루트 노드 위치
    int free_loc;                              // free 연결리스트의 첫번째 노드 위치
    char padding[Node_size - sizeof(int) * 2]; // 패딩
} HeadNode;

/* 바이너리 파일:
    .bin 확장자로 끝나는 파일, 이진 데이터를 저장하여 그냥 열면 읽을 수 없다
    바이너리 파일엔 int, char, 구조체 노드 등 다양한 정보를 저장할 수 있다.  */

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

/*------------------------------------------------------------------------------
 Function: initialize bin file and location variable
 Interface: void initialize_tree()
 Parameter: None
 return: void
------------------------------------------------------------------------------*/
void initialize_tree()
{
    FILE *tree = fopen(FILENAME, "wb");
    HeadNode head;      // 헤더 노드 값 읽기
    head.free_loc = -1; // free 연결리스트의 첫번째 위치
    head.root_loc = 1;  // 트리의 루트 노드 위치
    fwrite(&head, Node_size, 1, tree);
    fclose(tree);
}

/*------------------------------------------------------------------------------
 Function: insert a node in a binary search tree
 Interface: int insert(int num, char name[])
 Parameter: int num: number of student
            char name[]: name of student
 return: if duplicate id is inserted, return FAIL
         otherwiser, return OK
------------------------------------------------------------------------------*/
int insert(int num, char name[])
{
    FILE *tree = fopen(FILENAME, "r+b"); // 읽기+쓰기 모드 기존의 데이터 유지
    if (tree == NULL)
    {
        printf("Error in insert()\n");
        exit(1);
    }

    HeadNode head; // 헤더 노드 값 읽기
    fread(&head, Node_size, 1, tree);

    fseek(tree, 0, SEEK_END);           // 파일 끝으로 이동
    int size = ftell(tree) / Node_size; // 파일 크기 ( EMPTY 이면 파일이 비어있다)

    Node new_node; // 삽입할 노드
    new_node.num = num;
    strcpy(new_node.name, name);
    new_node.right = -1;
    new_node.left = -1;

    Node now_node; // 탐색용 노드

    /*
     다음부터 삽입한 노드의 부모 노드의 left or right를 수정.
    */
    if (size != EMPTY) // 트리가 비어있지 않을때
    {
        int loc = head.root_loc; // 시작 위치
        while (1)                // 삽입할 노드의 부모를 찾아 수정하는 로직 루프
        {
            fseek(tree, loc * Node_size, SEEK_SET); // loc의 위치의 노드 찾기
            fread(&now_node, Node_size, 1, tree);   // 그 위치의 노드 구조체 now_node에 읽어오기

            /* fread를 하면 자동으로 파일포인터는 다음노드를 가르킨다 그러므로
               fwrite함수 전에 fseek함수로 파일포인터를 재조정 해야한다.     */
            if (now_node.num < num) // 우측 노드 방향
            {
                if (now_node.right == -1) // 우측에 노드가 없음. 삽입할 차레
                {
                    if (head.free_loc != -1)
                        now_node.right = head.free_loc;
                    else
                        now_node.right = size;
                    fseek(tree, loc * Node_size, SEEK_SET);
                    fwrite(&now_node, Node_size, 1, tree); // 덮어쓰기
                    break;
                }
                loc = now_node.right; // 탐색할 위치 오른쪽으로 바꾸기
            }
            else if (now_node.num > num) // 좌측 노드 방향
            {
                if (now_node.left == -1) // 좌측에 노드가 없음. 삽입할 차레
                {
                    if (head.free_loc != -1)
                        now_node.left = head.free_loc;
                    else
                        now_node.left = size;
                    fseek(tree, loc * Node_size, SEEK_SET);
                    fwrite(&now_node, Node_size, 1, tree); // 덮어쓰기
                    break;
                }
                loc = now_node.left; // 탐색할 위치 왼쪽으로 비꾸기
            }
            else // 노드 Id의 중복은 허용하지 않음.
            {
                printf("Duplicate Id is not allowed\n");
                fclose(tree);

                return FAIL;
            }
        }
    }

    if (head.free_loc == -1) // free_loc이 -1 이면 free 연결리스트가 없음 -> 맨 끝에 노드 삽입
    {
        fseek(tree, 0, SEEK_END);
        fwrite(&new_node, Node_size, 1, tree);
    }
    else
    {
        FreeNode tmp; // free 연결리스트의 첫번째 노드를 저장할 변수
        /*
         노드를 삽입하는 과정
         inset_loc위치에 노드를 삽입하고 head.free_loc의 값을 수정
        */
        fseek(tree, Node_size * head.free_loc, SEEK_SET); // head.free_loc에 해당하는 위치로 이동
        fread(&tmp, sizeof(FreeNode), 1, tree);           // tmp에 free연결리스트 첫번째 값 저장

        fseek(tree, Node_size * head.free_loc, SEEK_SET); // head.free_loc의 위치로 이동
        fwrite(&new_node, Node_size, 1, tree);            // 노드 삽입

        if (tmp.next == -1)
        {
            head.free_loc = -1; // 삭제된 부분이 다 채워졌으면 head.free_loc를 -1로 바꿈
        }
        else
        {
            head.free_loc = tmp.next; // head.free_loc를 연결리스트의 두번째로 변경
        }

        fseek(tree, 0, SEEK_SET);
        fwrite(&head, Node_size, 1, tree); // Head 수정한거 저장
    }

    fclose(tree);
    return OK;
}

/*------------------------------------------------------------------------------
 Function: Retrieve a node with certain id
 Interface: int retrieve(int num)
 Parameter: int num: a number of a student
 return: if a tree or a specific node doesn't exist, return FAIL
         otherwiser, return OK
------------------------------------------------------------------------------*/
int retrieve(int num)
{
    FILE *tree = fopen(FILENAME, "r+b"); // 읽기+쓰기 모드 기존의 데이터 유지
    if (tree == NULL)
    {
        printf("Error in insert()\n");
        exit(1);
    }

    HeadNode head; // 헤더 노드 값 읽기
    fread(&head, Node_size, 1, tree);

    fseek(tree, 0, SEEK_END);           // 파일 끝으로 이동
    int size = ftell(tree) / Node_size; // 파일 크기 ( EMPTY 이면 파일이 비어있다)

    Node cur; // edge를 추가할 노드

    if (size != EMPTY) // 트리가 비어있지 않을때
    {
        int loc = head.root_loc; // 시작 위치
        while (1)                // 삽입할 노드의 부모를 찾아 수정하는 로직 루프
        {
            if (loc == -1) // 노드가 존재하지 않음
            {
                printf("There is no such node!\n");
                fclose(tree);
                return FAIL;
            }

            fseek(tree, loc * Node_size, SEEK_SET); // loc의 위치의 노드 찾기
            fread(&cur, Node_size, 1, tree);        // 그 위치의 노드 구조체 cur에 읽어오기

            if (cur.num < num) // 우측 노드 방향
            {
                loc = cur.right; // 탐색할 위치 오른쪽으로 바꾸기
            }
            else if (cur.num > num) // 좌측 노드 방향
            {
                loc = cur.left; // 탐색할 위치 왼쪽으로 비꾸기
            }
            else // 노드 찾기 성공
            {
                printf("Id: %d\tName: %s\n", cur.num, cur.name);
                fclose(tree);

                return OK;
            }
        }
    }
    else // 트리가 비어있을 때
    {
        printf("There is no Tree!\n");
        fclose(tree);

        return FAIL;
    }
}

/*------------------------------------------------------------------------------
 Function: Update a node in a binary search tree
 Interface: int update(int num, char name[])
 Parameter: int num: a number of a student for updating
            char name[]: name for updating
 return: if a tree or a specific node doesn't exist, return FAIL
         otherwiser, return OK
------------------------------------------------------------------------------*/
int update(int num, char name[])
{
    FILE *tree = fopen(FILENAME, "r+b"); // 읽기+쓰기 모드 기존의 데이터 유지
    if (tree == NULL)
    {
        printf("Error in insert()\n");
        exit(1);
    }

    HeadNode head; // 헤더 노드 값 읽기
    fread(&head, Node_size, 1, tree);

    fseek(tree, 0, SEEK_END);           // 파일 끝으로 이동
    int size = ftell(tree) / Node_size; // 파일 크기 ( EMPTY 이면 파일이 비어있다)

    Node cur; // edge를 추가할 노드

    if (size != EMPTY) // 트리가 비어있지 않을때
    {
        int loc = head.root_loc; // 시작 위치
        while (1)                // 삽입할 노드의 부모를 찾아 수정하는 로직 루프
        {
            if (loc == -1) // 노드가 존재하지 않음
            {
                printf("There is no such node!\n");
                fclose(tree);
                return FAIL;
            }

            fseek(tree, loc * Node_size, SEEK_SET); // loc의 위치의 노드 찾기
            fread(&cur, Node_size, 1, tree);        // 그 위치의 노드 구조체 cur에 읽어오기

            if (cur.num < num) // 우측 노드 방향
            {
                loc = cur.right; // 탐색할 위치 오른쪽으로 바꾸기
            }
            else if (cur.num > num) // 좌측 노드 방향
            {
                loc = cur.left; // 탐색할 위치 왼쪽으로 비꾸기
            }
            else // 노드 찾기 성공
            {
                strcpy(cur.name, name);                 // 노드 Update
                fseek(tree, loc * Node_size, SEEK_SET); // 위치 재조정 (fread로 파일포인터 위치 변경됨)
                fwrite(&cur, Node_size, 1, tree);       // 덮어쓰기

                printf("Id: %d\tName: %s\n", cur.num, cur.name); // 바뀐 노드 출력
                fclose(tree);

                return OK;
            }
        }
    }
    else // 트리가 비어있을 때
    {
        printf("There is no Tree!\n");
        fclose(tree);

        return FAIL;
    }
}

/*------------------------------------------------------------------------------
 Function: Delete a node in a binary search tree
 Interface: int delete(int num)
 Parameter: int num: a number of a student
 return: if a tree or a specific node doesn't exist, return FAIL
         otherwiser, return OK
------------------------------------------------------------------------------*/
int delete(int num)
{
    FILE *tree = fopen(FILENAME, "r+b");
    if (tree == NULL)
    {
        printf("Error opening file in delete()\n");
        exit(1);
    }

    HeadNode head; // 헤더 노드 값 읽기
    fread(&head, Node_size, 1, tree);

    fseek(tree, 0, SEEK_END);           // 파일 끝으로 이동
    int size = ftell(tree) / Node_size; // 파일 크기 (EMPTY이면 파일이 비어있다)

    if (size == EMPTY) // 파일이 비어있을 경우
    {
        printf("There is no such tree!\n");
        fclose(tree);
        return FAIL;
    }

    Node now_node;           // 탐색할 노드의 정보를 저장할 노드
    int loc = head.root_loc; // 탐색할 노드의 위치
    int parent_loc = -1;     // 삭제할 노드의 부모 노드 위치

    FreeNode free_node;             // Free한 위치를 수정하기 위한 변수
    free_node.next = head.free_loc; // 제일 최근에 삭제한 노드는 Next를 -1로 하여 연결리스트의 마지막임을 알림

    int is_end = 0; // 삭제하는 노드가 파일의 마지막인지 확인하는 변수

    while (1) // 삭제할 노드를 찾는 과정
    {
        if (loc == -1) // 노드가 존재하지 않음
        {
            printf("There is no such node!\n");
            fclose(tree);
            return FAIL;
        }

        fseek(tree, loc * Node_size, SEEK_SET);
        fread(&now_node, Node_size, 1, tree);

        if (now_node.num == num) // 삭제할 노드를 찾은 경우
        {
            if (loc + 1 >= size) // 삭제한 노드가 파일의 맨 마지막인 경우
            {
                is_end = 1;
            }
            if (now_node.left == -1 && now_node.right == -1) // 자식이 없는 경우
            {
                if (parent_loc != -1) // 부모 노드가 존재할 경우 부모노드 수정
                {
                    Node parent;
                    fseek(tree, parent_loc * Node_size, SEEK_SET);
                    fread(&parent, Node_size, 1, tree);

                    if (parent.left == loc)
                        parent.left = -1;
                    else
                        parent.right = -1;

                    fseek(tree, parent_loc * Node_size, SEEK_SET);
                    fwrite(&parent, Node_size, 1, tree);
                }
                else // 루트 노드를 지우면 그냥 초기화
                {
                    initialize_tree();
                    break;
                }

                if (is_end) // 파일의 맨 마지막 노드를 삭제하는 경우
                {
                    int fd = fileno(tree);
                    ftruncate(fd, (size - 1) * Node_size); // 노드를 파일에서 삭제하기
                }
                else
                {
                    /* 삭제 연결 리스트에 노드 추가 */
                    // 삭제한 위치에 free node 저장하면서 삭제 연결리스트의 첫번째 노드로 삽입
                    fseek(tree, loc * Node_size, SEEK_SET);
                    fwrite(&free_node, sizeof(FreeNode), 1, tree);

                    head.free_loc = loc; // 연결리스트의 첫번째 노드를 방금 삭제한 노드의 위치로 변경
                }
                break;
            }
            else if (now_node.left != -1 && now_node.right != -1) // 자식이 둘 다 있는 경우
            {
                // 우측 서브트리에서 가장 작은 노드 찾기
                Node smallest;
                int loc2 = now_node.right; // 가장 작은 노드의 위치
                int loc2_parent = loc;     // 가장 작은 노드의 부모의 위치

                while (1)
                {
                    fseek(tree, loc2 * Node_size, SEEK_SET);
                    fread(&smallest, Node_size, 1, tree);

                    if (smallest.left == -1)
                        break;

                    loc2_parent = loc2;
                    loc2 = smallest.left;
                }

                // 가장 작은 노드의 부모 노드 수정
                Node s_parent;
                fseek(tree, loc2_parent * Node_size, SEEK_SET);
                fread(&s_parent, Node_size, 1, tree);

                if (s_parent.left == loc2) // 부모의 왼쪽노드가 smallest 일때
                {
                    // 부모 노드의 edge 수정
                    s_parent.left = smallest.right;
                    fseek(tree, loc2_parent * Node_size, SEEK_SET);
                    fwrite(&s_parent, Node_size, 1, tree);

                    // 삭제할 노드에 가장 작은 노드의 값 복사
                    now_node.num = smallest.num;
                    fseek(tree, loc * Node_size, SEEK_SET);
                    fwrite(&now_node, Node_size, 1, tree);
                }
                else // 부모의 오른쪽 노드가 smallest일때 ( 우측 서브트리에 노드가 단 하나일때 ) 삭제하는 노드와 가장 작은 노드의 부모노드가 같음
                {
                    // 삭제할 노드에 가장 작은 노드의 값 복사
                    now_node.num = smallest.num;
                    strcpy(now_node.name, smallest.name);
                    now_node.right = -1;
                    fseek(tree, loc * Node_size, SEEK_SET);
                    fwrite(&now_node, Node_size, 1, tree);
                }

                if (is_end) // 파일의 맨 마지막 노드를 삭제하는 경우
                {
                    int fd = fileno(tree);
                    ftruncate(fd, (size - 1) * Node_size); // 노드를 파일에서 삭제하기
                }
                else
                {
                    /* 삭제 연결 리스트에 노드 추가 */
                    // 삭제한 위치에 free node 저장하면서 삭제 연결리스트의 첫번째 노드로 삽입
                    fseek(tree, loc2 * Node_size, SEEK_SET);
                    fwrite(&free_node, sizeof(FreeNode), 1, tree);

                    head.free_loc = loc2; // 연결리스트의 첫번째 노드를 방금 삭제한 노드의 위치로 변경
                }
                break;
            }
            else // 자식이 하나만 있는 경우
            {
                int child_loc = (now_node.left != -1) ? now_node.left : now_node.right;

                if (parent_loc != -1) // 부모 노드가 존재할 경우
                {
                    // parent 변수에 부모 노드 fread
                    Node parent;
                    fseek(tree, parent_loc * Node_size, SEEK_SET);
                    fread(&parent, Node_size, 1, tree);
                    // 부모노드 수정
                    if (parent.left == loc)
                        parent.left = child_loc;
                    else
                        parent.right = child_loc;
                    // 수정한거 fwrite
                    fseek(tree, parent_loc * Node_size, SEEK_SET);
                    fwrite(&parent, Node_size, 1, tree);
                }
                else // 루트 노드를 삭제하면 head.root_loc를 수정해야 함
                    head.root_loc = child_loc;

                if (is_end) // 파일의 맨 마지막 노드를 삭제하는 경우
                {
                    int fd = fileno(tree);
                    ftruncate(fd, (size - 1) * Node_size); // 노드를 파일에서 삭제하기
                }
                else
                {
                    /* 삭제 연결 리스트에 노드 추가 */
                    // 삭제한 위치에 free node 저장하면서 삭제 연결리스트의 첫번째 노드로 삽입
                    fseek(tree, loc * Node_size, SEEK_SET);
                    fwrite(&free_node, sizeof(FreeNode), 1, tree);

                    head.free_loc = loc; // 연결리스트의 첫번째 노드를 방금 삭제한 노드의 위치로 변경
                }
                break;
            }
        }
        else if (now_node.num < num) // 오른쪽으로 이동
        {
            parent_loc = loc;
            loc = now_node.right;
        }
        else // 왼쪽으로 이동
        {
            parent_loc = loc;
            loc = now_node.left;
        }
    }

    fseek(tree, 0, SEEK_SET);
    fwrite(&head, Node_size, 1, tree);

    fclose(tree);
    return OK;
}

/*------------------------------------------------------------------------------
 Function: depth first search in binary seach tree
 Interface: int dfs()
 Parameter: None
 return: if a tree doesn't exist, return FIAL
         otherwise. return OK
------------------------------------------------------------------------------*/
int dfs()
{
    FILE *tree = fopen(FILENAME, "r+b"); // 읽기+쓰기 모드 기존의 데이터 유지
    if (tree == NULL)
    {
        printf("Error in dfs()\n");
        exit(1);
    }

    HeadNode head; // 헤더 노드 값 읽기
    fread(&head, Node_size, 1, tree);

    fseek(tree, 0, SEEK_END);           // 파일 끝으로 이동
    int size = ftell(tree) / Node_size; // 파일 크기

    if (size == EMPTY) // 파일 크기 ( EMPTY 이면 파일이 비어있다)
    {
        printf("There is no tree\n");
        return FAIL;
    }

    init_stack();
    int loc = head.root_loc;
    Node cur;

    if (push(loc) == STACKOVERFLOW)
    {
        printf("StackOverflow in dfs()\n");
        return FAIL;
    }

    printf("------------dfs------------\n");
    while (is_stack_empty() != 1)
    {
        loc = pop();
        if (loc == STACKUNDERFLOW)
        {
            printf("StackUnderflow in dfs()\n");
            return FAIL;
        }

        fseek(tree, loc * Node_size, SEEK_SET);
        fread(&cur, Node_size, 1, tree);

        printf("\t%d\t%s\n", cur.num, cur.name);

        if (cur.right != -1)
        {
            loc = cur.right;
            if (push(loc) == STACKOVERFLOW)
            {
                printf("StackOverflow in dfs()\n");
                return FAIL;
            }
        }
        if (cur.left != -1)
        {
            loc = cur.left;
            if (push(loc) == STACKOVERFLOW)
            {
                printf("StackOverflow in dfs()\n");
                return FAIL;
            }
        }
    }
    printf("---------------------------\n");

    fclose(tree);
    return OK;
}

/*------------------------------------------------------------------------------
 Function: breath first search in binary seach tree
 Interface: int bfs()
 Parameter: None
 return: if a tree doesn't exist, return FIAL
         otherwise. return OK
------------------------------------------------------------------------------*/
int bfs()
{
    FILE *tree = fopen(FILENAME, "r+b"); // 읽기+쓰기 모드 기존의 데이터 유지
    if (tree == NULL)
    {
        printf("Error in bfs()\n");
        exit(1);
    }

    HeadNode head; // 헤더 노드 값 읽기
    fread(&head, Node_size, 1, tree);

    fseek(tree, 0, SEEK_END);           // 파일 끝으로 이동
    int size = ftell(tree) / Node_size; // 파일 크기

    if (size == EMPTY) // 파일 크기 ( EMPTY 이면 파일이 비어있다)
    {
        printf("There is no tree\n");
        return FAIL;
    }

    init_queue();
    int loc = head.root_loc;
    Node cur;

    if (enqueue(loc) == QUEUE_OVERFLOW)
    {
        printf("QueueOverflow in bfs()\n");
        return FAIL;
    }

    printf("------------bfs------------\n");
    while (is_queue_empty() != 1)
    {
        loc = dequeue();
        if (loc == QUEUE_UNDERFLOW)
        {
            printf("QueueUnderflow in bfs()\n");
            return FAIL;
        }
        fseek(tree, loc * Node_size, SEEK_SET);
        fread(&cur, Node_size, 1, tree);
        printf("\t%d\t%s\n", cur.num, cur.name);

        if (cur.left != -1)
        {
            if (enqueue(cur.left) == QUEUE_OVERFLOW)
            {
                printf("QueueOverflow in bfs()\n");
                return FAIL;
            }
        }
        if (cur.right != -1)
        {
            if (enqueue(cur.right) == QUEUE_OVERFLOW)
            {
                printf("QueueOverflow in bfs()\n");
                return FAIL;
            }
        }
    }
    printf("---------------------------\n");

    fclose(tree);
    return OK;
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
    FILE *tree = fopen(FILENAME, "r+b");

    Node node;

    HeadNode head; // 헤더 노드 값 읽기
    fread(&head, Node_size, 1, tree);

    while (fread(&node, Node_size, 1, tree) == 1)
    {
        printf("%d\t%d\t%d\n", node.num, node.left, node.right);
    }
    printf("insert location: %d\nroot location: %d\n", head.free_loc, head.root_loc);
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

            if (retrieve(num) == FAIL)
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