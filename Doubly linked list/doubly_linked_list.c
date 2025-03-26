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

/*------------------------------------------------------------------------
 Function: Inserting a new node into the doubly linked list sorted by id
 Interface: void insert(Linked_List *list, int id, char name[])
 Parameters: Linked_List *list: Pointer to the doubly linked list
             int id: ID of the node to insert
             char name[]: Name of the node to insert
 Return: if the inserting is complete, return OK
------------------------------------------------------------------------*/
int insert(Linked_List *list, int id, char name[])
{
    Node *new_node = (Node *)malloc(sizeof(Node)); // node to insert
    if (new_node == NULL)
    {
        printf("Menory allocation was failed in insert() \n");
        exit(1);
    }

    new_node->id = id;
    strcpy(new_node->name, name);
    new_node->left = NULL;
    new_node->right = NULL; // initialize new_node

    Node *now_node = list->head; // node for searching

    if (list->head == NULL) // list is empty
    {
        list->head = new_node;
    }
    else if (id < now_node->id) // 왼쪽으로 탐색, head의 id보다 작을때
    {
        while (now_node->left != NULL && now_node->left->id > id)
        // 삽입할 위치를 찾을때까지
        {
            now_node = now_node->left;
        }

        if (now_node->left == NULL) // 노드를 마지막에 삽입할때
        {
            new_node->right = now_node;
            now_node->left = new_node;
        }
        else // 노드를 중간에 삽입할때
        {
            new_node->left = now_node->left;
            new_node->right = now_node;
            now_node->left->right = new_node;
            now_node->left = new_node;
        }
    }
    else // 오른쪽으로 탐색, head의 id보다 값이 클때
    {
        while (now_node->right != NULL && now_node->right->id <= id)
        // 삽입할 위치를 찾을때까지
        {
            now_node = now_node->right;
        }

        if (now_node->right == NULL) // 노드를 마지막에 삽입할때
        {
            new_node->left = now_node;
            now_node->right = new_node;
        }
        else // 노드를 중간에 삽입할때
        {
            new_node->right = now_node->right;
            new_node->left = now_node;
            now_node->right->left = new_node;
            now_node->right = new_node;
        }
    }
    return OK;
}

/*------------------------------------------------------------------------
 Function: Deleting a node in the doubly linked list.
 Interface: void delete(Linked_List *list, int id)
 Parameters: Linked_List *list: Pointer to the doubly linked list
             int id: ID of the node to delete
 Return: if the deleting is complete, return OK
         otherwise return FAIL
------------------------------------------------------------------------*/
int delete(Linked_List *list, int id)
{
    Node *now_node = list->head; // node for searching

    if (list->head == NULL) // when list is empty
    {
        printf("list is empty!\n");
        return FAIL;
    }
    else if (now_node->id == id) // 삭제할 노드가 head 노드일때
    {
        if (now_node->right != NULL && now_node->left != NULL)
        // head 양쪽에 둘다 노드가 있을때
        {
            list->head = now_node->right;
            list->head->left = now_node->left;
            now_node->left->right = list->head;
            free(now_node);
        }
        else if (now_node->left == NULL && now_node->right != NULL)
        // 오른쪽에만 존재할때
        {
            list->head = now_node->right;
            list->head->left = NULL;
            free(now_node);
        }
        else if (now_node->left != NULL && now_node->right == NULL)
        // 왼쪽에만 존재할때
        {
            list->head = now_node->left;
            list->head->right = NULL;
            free(now_node);
        }
        else // head만 존재할때
        {
            list->head = NULL;
            free(now_node);
        }
        return OK;
    }
    else
    {
        if (id < now_node->id) // 삭제할 노드 찾기
        {
            while (now_node->left != NULL && now_node->id != id)
                now_node = now_node->left;
        }
        else
        {
            while (now_node->right != NULL && now_node->id != id)
                now_node = now_node->right;
        }

        if (now_node->id == id)
        {
            if (now_node->right != NULL && now_node->left != NULL)
            // 삭제할 노드의 양쪽에 노드가 있을때
            {
                now_node->left->right = now_node->right;
                now_node->right->left = now_node->left;
                free(now_node);
            }
            else if (now_node->left == NULL) // 오른쪽에만 있을때
            {
                now_node->right->left = NULL;
                free(now_node);
            }
            else // 왼쪽에만 있을때
            {
                now_node->left->right = NULL;
                free(now_node);
            }
            return OK;
        }
        else
        {
            printf("There is no such node!\n");
            return FAIL;
        }
    }
}

/*------------------------------------------------------------------------
 Function: Updating the name of a node with a specified ID.
 Interface: int update(Linked_List *list, int id, char name[])
 Parameters: Linked_List *list: Pointer to the doubly linked list
             int id: ID of the node to update
             char name[]: New name to assign to the node
 Return: OK (if the name was successfully updated).
         FAIL (if the list is empty or the node does not exist).
------------------------------------------------------------------------*/
int update(Linked_List *list, int id, char name[])
{
    Node *now_node = list->head; // node for searching
    if (now_node == NULL)        // list is empty
    {
        printf("List is empty!\n");
        return FAIL;
    }
    else if (now_node->id == id) // update 할 노드가 head 노드일때
    {
        strcpy(now_node->name, name);
        return OK;
    }
    else
    {
        if (id < now_node->id) // update 할 노드 찾기
        {
            while (now_node->left != NULL && now_node->id != id)
                now_node = now_node->left;
        }
        else
        {
            while (now_node->right != NULL && now_node->id != id)
                now_node = now_node->right;
        }

        if (now_node->id == id) // update 할 노드가 존재하면
        {
            strcpy(now_node->name, name);
            return OK;
        }
        else // update 할 노드가 존재하지 않으면
        {
            printf("There is no such node!\n");
            return FAIL;
        }
    }
}

/*------------------------------------------------------------------------
 Function: Retrieving and prints the name of a node with a specified ID.
 Interface: int retrieve(Linked_List *list, int id)
 Parameters: Linked_List *list: Pointer to the doubly linked list.
             int id: ID of the node to retrieve.
 Return: OK (if the node was found and printed).
         FAIL (if the list is empty or the node does not exist).
------------------------------------------------------------------------*/
int retrieve(Linked_List *list, int id)
{
    Node *now_node = list->head; // node for searching

    if (now_node == NULL) // list is empty
    {
        printf("List is empty!\n");
        return FAIL;
    }
    else if (now_node->id == id)
    { // retrIeve 할 노드가 head 노드일때
        printf("Name: %s\n", now_node->name);
        return OK;
    }
    else
    {
        if (id < now_node->id) // 삭제할 노드 찾기
        {
            while (now_node->left != NULL && now_node->id != id)
                now_node = now_node->left;
        }
        else
        {
            while (now_node->right != NULL && now_node->id != id)
                now_node = now_node->right;
        }

        if (now_node->id == id) // 삭제할 노드가 존재하면
        {
            printf("Name: %s\n", now_node->name);
            return OK;
        }
        else // 삭제할 노드가 존재하지 않으면
        {
            printf("There is no such node!\n");
            return FAIL;
        }
    }
}

/*------------------------------------------------------------------------
 Function: Prints all nodes in the doubly linked list in ascending order.
 Interface: void print_linked_list(Linked_List *list)
 Parameters: Linked_List *list: Pointer to the doubly linked list.
 Return: void
------------------------------------------------------------------------*/
void print_linked_list(Linked_List *list)
{
    if (list->head == NULL) // list가 비어있으면
    {
        printf("list is emptry!\n");
    }
    else
    {
        Node *now_node = list->head; // list를 탐색할 노드

        while (now_node->left != NULL) // 맨 왼쪽으로 이동
            now_node = now_node->left;

        while (now_node != NULL) // 오른쪽 끝에 닿을 때까지 출력
        {
            printf("%d\t%s\n", now_node->id, now_node->name);
            now_node = now_node->right;
        }
    }
}

/*------------------------------------------------------------------------
 Function: Frees all nodes in the doubly linked list and releases memory.
 Interface: void free_list(Linked_List *list)
 Parameters: Linked_List *list: Pointer to the doubly linked list.
 Return: void
------------------------------------------------------------------------*/
void free_list(Linked_List *list)
{
    Node *now_node = list->head; // Free할 노드
    Node *next_node;             // list를 탐색할 노드

    if (list->head == NULL) // list가 비어있을때
    {
        free(list);
    }
    else
    {
        // 왼쪽 끝으로 이동
        while (now_node->left != NULL)
            now_node = now_node->left;

        // 모든 노드 해제
        while (now_node != NULL)
        {
            next_node = now_node->right;
            free(now_node);
            now_node = next_node;
        }

        // 리스트 구조체 해제
        free(list);
    }
}

int main()
{
    Linked_List *list = (Linked_List *)malloc(sizeof(Linked_List));
    list->head = NULL;
    int command;
    int ret; // scanf에 잘못 입력됨을 확인하는 변수
    do
    {
        printf("----------------------------------------------------------------\n");
        printf("                    doubly linked list                         \n");
        printf("----------------------------------------------------------------\n");
        printf(" Insert        = 1           Delete        = 2 \n");
        printf(" Update        = 3           RetrIeve      = 4 \n");
        printf(" Print         = 5           Quit          = 6 \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        ret = scanf("%d", &command);
        if (ret != 1)
        {
            printf("Wrong Input!\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (command)
        {
        case INSERT: // 1
        {
            int num;
            char name[10];

            printf("Input id: ");
            ret = scanf("%d", &num);

            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ;
            }

            printf("Input name: ");
            ret = scanf("%s", name);

            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ;
            }

            if (insert(list, num, name) == OK)
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
            ret = scanf("%d", &num);

            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ;
            }

            if (delete (list, num) == OK)
            {
                printf("Deleting is complete\n");
            }
            else
            {
                printf("Deleting is failed\n");
            }

            break;
        }
        case UPDATE: // 3
        {
            int id;
            char name[10];

            printf("Input id: ");
            ret = scanf("%d", &id);

            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ;
            }

            printf("Input Name: ");
            ret = scanf("%s", name);

            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ;
            }

            if (update(list, id, name) == OK)
            {
                printf("Updating is complete!\n");
            }
            else
            {
                printf("Updating is failed\n");
            }
            break;
        }
        case RETRIEVE: // 4
        {
            int id;
            printf("Input Id: ");

            ret = scanf("%d", &id);
            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ;
            }

            if (retrieve(list, id) == OK)
            {
                printf("Retreving is complete!\n");
            }
            else
            {
                printf("Retreving is failed\n");
            }

            break;
        }
        case PRINT: // 5
            print_linked_list(list);
            break;
        case QUIT: // 6
            free_list(list);
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != 6);
}