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
#define PRINT 5
#define QUIT 6

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

Linked_List *list;

/*------------------------------------------------------------------------
 Function: Insert a new node into the doubly linked list sorted by id
 Interface: int insert(int id, char name[])
 Parameters: int id: ID of the node to insert
             char name[]: Name of the node to insert
 Return: if the inserting is complete, return OK
         Otherwise, exit()
------------------------------------------------------------------------*/
int insert(int id, char name[])
{
    Node *new_node = (Node *)malloc(sizeof(Node)); // node to insert

    if (new_node == NULL)
    {
        printf("Memory allocation was failed in insert() \n");
        exit(1);
    }

    new_node->id = id;
    strcpy(new_node->name, name); // Set the input value

    Node *cur; // node for searching
    Node *pre;

    if (list->head == NULL) // in case of the list is empty
    {
        new_node->right = NULL;
        new_node->left = NULL;
        list->head = new_node;
    }
    else if (id < list->head->id) // head의 id보다 작을때
    {
        list->head->left = new_node;
        new_node->right = list->head;
        new_node->left = NULL;
        list->head = new_node;
    }
    else // head의 id보다 값이 클때
    {
        for (cur = list->head; cur != NULL; pre = cur, cur = cur->right)
        // 삽입할 위치를 찾을때까지
        {
            if (cur->id > id)
            {
                break;
            }
        }

        if (cur == NULL) // 노드를 마지막에 삽입할때
        {
            new_node->left = pre;
            pre->right = new_node;
            new_node->right = NULL;
        }
        else // 노드를 중간에 삽입할때
        {
            new_node->right = pre->right;
            new_node->left = pre;
            pre->right->left = new_node;
            pre->right = new_node;
        }
    }

    return OK;
}

/*------------------------------------------------------------------------
 Function: Delete a node in the doubly linked list.
 Interface: void delete(int id)
 Parameters: int id: ID of the node to delete
 Return: void
------------------------------------------------------------------------*/
void delete(int id)
{
    Node *cur = list->head; // node for searching

    if (list->head == NULL) // in case that list is empty
    {
        printf("list is empty!\n");
    }
    else if (cur->id == id) // 삭제할 노드가 head 노드일때
    {
        if(cur->right == NULL) // 헤드 노드만 있는 경우
        {
            list->head = NULL;
            free(cur);
        }
        else // 헤드 노드와 연결된 노드가 있는 경우
        {
            list->head = cur->right;
            list->head->left = NULL;
            free(cur);
        }
    }
    else // 삭제할 노드가 head 노드가 아닌 경우
    {
        for (; cur != NULL; cur = cur->right) // 삭제할 노드 찾기
        {
            if (cur->id >= id) // 삭제할 노드를 찾거나 없으면 break
            {
                break;
            }
        }

        if (cur == NULL) // 노드가 존재하지 않을때
        {
            printf("There is no such node!\n");
        }
        else if (cur->id == id) // 삭제할 노드가 존재하는 경우
        {
            if (cur->right != NULL && cur->left != NULL)
            // 삭제할 노드의 양쪽에 노드가 있을때
            {
                cur->left->right = cur->right;
                cur->right->left = cur->left;
                free(cur);
            }
            else // 왼쪽에만 있을때
            {
                cur->left->right = NULL;
                free(cur);
            }
        }
        else // 삭제할 노드가 존재하지 않는 경우
        {
            printf("There is no such node!\n");
        }
    }
}

/*------------------------------------------------------------------------
 Function: Update the name of a node with a specified ID.
 Interface: void update(int id, char name[])
 Parameters: int id: ID of the node to update
             char name[]: New name to assign to the node
 Return: void
------------------------------------------------------------------------*/
void update(int id, char name[])
{
    Node *cur = list->head; // node for searching

    if (cur == NULL) // in case that list is empty
    {
        printf("List is empty!\n");
    }
    else if (cur->id == id) // update 할 노드가 head 노드일때
    {
        strcpy(cur->name, name);
    }
    else // update 할 노드가 head노드가 아닌 경우
    {
        for (; cur != NULL; cur = cur->right) // 노드 찾기
        {
            if (cur->id >= id) // 노드를 찾거나 없는 경우 break
            {
                break;
            }
        }

        if (cur == NULL) // 노드가 존재하지 않을때
        {
            printf("There is no such node!\n");
        }
        else if (cur->id == id) // update 할 노드가 존재하면
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
 Function: Retrieve and prints the name of a node with the specified ID.
 Interface: char* retrieve(int id)
 Parameters: int id: ID of the node to retrieve.
 Return: if a node is successfully found, return name of a node
         Otherwise return FAIL
------------------------------------------------------------------------*/
char *retrieve(int id)
{
    Node *cur = list->head; // node for searching

    if (cur == NULL) // in case of the list is empty
    {
        printf("List is empty!\n");
    }
    else if (cur->id == id)
    { // retrieve 할 노드가 head 노드일때
        return cur->name;
    }
    else // Retrieve 할 노드가 head 노드가 아닌 경우
    {
        for (; cur != NULL; cur = cur->right) // 노드 찾기
        {
            if (cur->id >= id) // 노드를 찾거나 없는 경우 break
            {
                break;
            }
        }

        if (cur == NULL)
        {
            printf("There is no such node!\n");
        }
        else if (cur->id == id) // Retrieve할 노드가 존재하면
        {
            return cur->name;
        }
        else // retrieve할 노드가 존재하지 않으면
        {
            printf("There is no such node!\n");
        }
    }

    return FAIL;
}

/*------------------------------------------------------------------------
 Function: Print all nodes in the doubly linked list in ascending order.
 Interface: void print_linked_list())
 Parameters: None
 Return: void
------------------------------------------------------------------------*/
void print_linked_list()
{
    if (list->head == NULL) // list가 비어있으면
    {
        printf("list is empty!\n");
    }
    else
    {
        Node *cur = list->head; // list를 탐색할 노드

        for (; cur != NULL; cur = cur->right) // 오른쪽 끝에 닿을 때까지 출력
        {
            printf("%d\t%s\n", cur->id, cur->name);
        }
    }
}

/*------------------------------------------------------------------------
 Function: Free all nodes in the doubly linked list and releases memory.
 Interface: void free_list()
 Parameters: None
 Return: void
------------------------------------------------------------------------*/
void free_list(Linked_List *list)
{
    if (list->head == NULL) // list가 비어있을때
    {
        free(list);
    }
    else
    {
        Node *cur = list->head; // 탐색용 노드
        Node *pre = NULL;       // free할 노드

        for (; cur != NULL; pre = cur, cur = cur->right) // 오른쪽으로 이동하면서 free
        {
            free(pre);
        }
        // 리스트 구조체 해제
        free(pre);
        free(list);
    }
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
        printf(" Print         = 5           Quit          = 6 \n");
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
            print_linked_list();

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

            delete(num);
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
            char *name;

            printf("Input id: ");
            while (scanf("%d", &id) != 1) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput id again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            name = retrieve(id);

            if (name == FAIL)
            {
                printf("retrieve() was failed\n");
            }
            else
            {
                printf("Name: %s\n", name);
            }

            break;
        }
        case PRINT: // 5
            print_linked_list();
            break;
        case QUIT: // 6
            free_list(list);
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != QUIT);
}