/*---------------------------------------------------------------------
File Name: doubly_linked_list.c
Programmed by: Hangyeol Lee
Affiliation: Chungbuk University
Functions: insert(), delete(), print(),
           retrieve(), update() in doubly linked list
Copyright (c) 2025 Hangyeol Lee. All rights reserved.
---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INSERT 1
#define DELETE 2
#define UPDATE 3
#define RETRIEVE 4
#define INITIALIZE 5
#define PRINT 6
#define QUIT 7

#define MAX_NAME 10
#define OK 1
#define FAIL 0

typedef struct Node // struct of node
{
    int id;              // number of the student
    char name[MAX_NAME]; // name of the student
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Doubly_Linked_List // struct of Doubly linked list
{
    Node *head;
} Linked_List;

Linked_List *list = NULL;

/*  고쳐야함!!!!!!!!!!!!! insert빼고 다 void로 바꾸고 while문을 for문으로 바꾸기   */

/*------------------------------------------------------------------------
 Function: Inserting a new node into the doubly linked list sorted by id
 Interface: void insert(int id, char name[])
 Parameters: int id: ID of the node to insert
             char name[]: Name of the node to insert
 Return: if the inserting is complete, return OK
------------------------------------------------------------------------*/
int insert(int id, char name[])
{
    Node *new_node = (Node *)malloc(sizeof(Node)); // node to insert
    if (new_node == NULL)
    {
        printf("Menory allocation was failed in insert() \n");
        exit(1);
    }

    new_node->id = id;
    strcpy(new_node->name, name); // initialize new_node

    Node *cur; // node for searching

    if (list->head == NULL) // list is empty
    {
        list->head = new_node;
    }
    else if (id < cur->id) // 왼쪽으로 탐색, head의 id보다 작을때
    {
        for (cur = list->head; cur->left != NULL; cur = cur->left)
        // 삽입할 위치를 찾을때까지
        {
            if (cur->left->id <= id)
            {
                break;
            }
        }

        if (cur->left == NULL) // 노드를 마지막에 삽입할때
        {
            new_node->right = cur;
            cur->left = new_node;
            new_node->left = NULL;
        }
        else // 노드를 중간에 삽입할때
        {
            new_node->left = cur->left;
            new_node->right = cur;
            cur->left->right = new_node;
            cur->left = new_node;
        }
    }
    else // 오른쪽으로 탐색, head의 id보다 값이 클때
    {
        for (cur = list->head; cur->right != NULL; cur = cur->right)
        // 삽입할 위치를 찾을때까지
        {
            if (cur->right->id > id)
            {
                break;
            }
        }
        if (cur->right == NULL) // 노드를 마지막에 삽입할때
        {
            new_node->left = cur;
            cur->right = new_node;
            new_node->right = NULL;
        }
        else // 노드를 중간에 삽입할때
        {
            new_node->right = cur->right;
            new_node->left = cur;
            cur->right->left = new_node;
            cur->right = new_node;
        }
    }
    return OK;
}

/*------------------------------------------------------------------------
 Function: Deleting a node in the doubly linked list.
 Interface: void delete(int id)
 Parameters: int id: ID of the node to delete
 Return: void
------------------------------------------------------------------------*/
void delete(int id)
{
    Node *cur; // node for searching

    if (list->head == NULL) // when list is empty
    {
        printf("list is empty!\n");
    }
    else if (cur->id == id) // 삭제할 노드가 head 노드일때
    {
        if (cur->right != NULL && cur->left != NULL)
        // head 양쪽에 둘다 노드가 있을때
        {
            list->head = cur->right;
            list->head->left = cur->left;
            cur->left->right = list->head;
            free(cur);
        }
        else if (cur->left == NULL && cur->right != NULL)
        // 오른쪽에만 존재할때
        {
            list->head = cur->right;
            list->head->left = NULL;
            free(cur);
        }
        else if (cur->left != NULL)
        // 왼쪽에만 존재할때
        {
            list->head = cur->left;
            list->head->right = NULL;
            free(cur);
        }
        else // head만 존재할때
        {
            list->head = NULL;
            free(cur);
        }
    }
    else
    {
        if (id < list->head->id) // 삭제할 노드 찾기
        {
            for (cur = list->head; cur->left != NULL; cur = cur->left)
            {
                if (cur->id <= id)
                {
                    break;
                }
            }
        }
        else
        {
            for (cur = list->head; cur->right != NULL; cur = cur->right)
            {
                if (cur->id >= id)
                {
                    break;
                }
            }
        }
        if (cur->id == id)
        {
            if (cur->right != NULL && cur->left != NULL)
            // 삭제할 노드의 양쪽에 노드가 있을때
            {
                cur->left->right = cur->right;
                cur->right->left = cur->left;
                free(cur);
            }
            else if (cur->left == NULL) // 오른쪽에만 있을때
            {
                cur->right->left = NULL;
                free(cur);
            }
            else // 왼쪽에만 있을때
            {
                cur->left->right = NULL;
                free(cur);
            }
        }
        else
        {
            printf("There is no such node!\n");
        }
    }
}

/*------------------------------------------------------------------------
 Function: Updating the name of a node with a specified ID.
 Interface: int update(int id, char name[])
 Parameters: int id: ID of the node to update
             char name[]: New name to assign to the node
 Return: void
------------------------------------------------------------------------*/
void update(int id, char name[])
{
    Node *cur = list->head; // node for searching

    if (cur == NULL) // list is empty
    {
        printf("List is empty!\n");
    }
    else if (cur->id == id) // update 할 노드가 head 노드일때
    {
        strcpy(cur->name, name);
    }
    else
    {
        if (id < cur->id) // update 할 노드 찾기
        {
            for (cur = list->head; cur->left != NULL; cur = cur->left)
            {
                if (cur->id <= id)
                {
                    break;
                }
            }
        }
        else
        {
            for (cur = list->head; cur->right != NULL; cur = cur->right)
            {
                if (cur->id >= id)
                {
                    break;
                }
            }
        }
        if (cur->id == id) // update 할 노드가 존재하면
        {
            strcpy(cur->name, name);
        }
        else // update 할 노드가 존재하지 않으면
        {
            printf("There is no such node!\n");
        }
    }
}

/*------------------------------------------------------------------------
 Function: Retrieving and prints the name of a node with the specified ID.
 Interface: int retrieve(int id)
 Parameters: int id: ID of the node to retrieve.
 Return: void
------------------------------------------------------------------------*/
void retrieve(int id)
{
    Node *cur = list->head; // node for searching

    if (cur == NULL) // list is empty
    {
        printf("List is empty!\n");
    }
    else if (cur->id == id)
    { // retrieve 할 노드가 head 노드일때
        printf("Name: %s\n", cur->name);
    }
    else
    {
        if (id < cur->id) // retrieve 할 노드 찾기
        {
            for (cur = list->head; cur->left != NULL; cur = cur->left)
            {
                if (cur->id <= id)
                {
                    break;
                }
            }
        }
        else
        {
            for (cur = list->head; cur->right != NULL; cur = cur->right)
            {
                if (cur->id >= id)
                {
                    break;
                }
            }
        }

        if (cur->id == id) // Retrieve할 노드가 존재하면
        {
            printf("Name: %s\n", cur->name);
        }
        else // 삭제할 노드가 존재하지 않으면
        {
            printf("There is no such node!\n");
        }
    }
}

/*------------------------------------------------------------------------
 Function: Prints all nodes in the doubly linked list in ascending order.
 Interface: void print_linked_list())
 Parameters: None
 Return: void
------------------------------------------------------------------------*/
void print_linked_list()
{
    if (list->head == NULL) // list가 비어있으면
    {
        printf("list is emptry!\n");
    }
    else
    {
        Node *cur = list->head; // list를 탐색할 노드

        for (; cur->left != NULL; cur = cur->left)
        {
        } // 맨 왼쪽으로 이동

        for (; cur != NULL; cur = cur->right) // 오른쪽 끝에 닿을 때까지 출력
        {
            printf("%d\t%s\n", cur->id, cur->name);
        }
    }
}

/*------------------------------------------------------------------------
 Function: Frees all nodes in the doubly linked list and releases memory.
 Interface: void free_list()
 Parameters: None
 Return: void
------------------------------------------------------------------------*/
void free_list(Linked_List *list)
{
    Node *tmp;                        // Free할 노드
    Node *r_side = list->head->right; // 오른쪽 탐색할 노드
    Node *l_side = list->head->left;  // 왼쪽 탐색할 노드

    if (list->head == NULL) // list가 비어있을때
    {
        free(list);
    }
    else
    {
        // 왼쪽 리스트 free
        for (tmp = NULL; l_side != NULL; tmp = l_side, l_side = l_side->left)
        {
            free(tmp);
        }
        // 오른쪽 리스트 free
        for (tmp = NULL; r_side != NULL; tmp = r_side, r_side = r_side->right)
        {
            free(tmp);
        }
        // 리스트 구조체 해제
        Free(list->head);
        free(list);
    }
}

/*------------------------------------------------------------------------
 Function: initialize the doubly linked list
 Interface: void initialize_list()
 Parameters: None
 Return: void
------------------------------------------------------------------------*/
void initialize_list()
{
    Node *tmp;                        // Free할 노드
    Node *r_side = list->head->right; // 오른쪽 탐색할 노드
    Node *l_side = list->head->left;  // 왼쪽 탐색할 노드

    // 왼쪽 리스트 free
    for (tmp = NULL; l_side != NULL; tmp = l_side, l_side = l_side->left)
    {
        free(tmp);
    }
    // 오른쪽 리스트 free
    for (tmp = NULL; r_side != NULL; tmp = r_side, r_side = r_side->right)
    {
        free(tmp);
    }
    // head 노드 Free
    Free(list->head);

    list->head = NULL;

    printf("Initializing is complete!\n");
}

int main()
{
    list = (Linked_List *)malloc(sizeof(Linked_List));
    list->head = NULL;
    int command;
    int ret; // scanf에 잘못 입력됨을 확인하는 변수
    do
    {
        printf("----------------------------------------------------------------\n");
        printf("                    doubly linked list                         \n");
        printf("----------------------------------------------------------------\n");
        printf(" Insert        = 1           Delete        = 2 \n");
        printf(" Update        = 3           Retrieve      = 4 \n");
        printf(" Initialize    = 5           Print         = 6 \n");
        printf(" Quit          = 7 \n");
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
            int num;
            char name[10];

            printf("Input id: ");
            while (scanf("%d", &num) != 1) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput id again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            printf("Input name: ");
            while (scanf("%s", name) != 1) // 입력값이 문자열이 아닐 경우
            {
                printf("Wrong Input!\nInput name again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (insert(num, name) == OK)
            {
                printf("Inserting is complete\n");
            }
            else
            {
                printf("Inserting is failed\n");
            }

            break;
        }
        case DELETE: // 2
        {
            int num;

            printf("Input id: ");
            while (scanf("%d", &num) != 1) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput id again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            delete (num);
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

            printf("Input name: ");
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

            printf("Input id: ");
            while (scanf("%d", &id) != 1) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput id again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            retrieve(id);

            break;
        }
        case INITIALIZE: // 5
            initialize_list();
            break;
        case PRINT: // 6
            print_linked_list();
            break;
        case QUIT: // 7
            free_list(list);
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != QUIT);
}