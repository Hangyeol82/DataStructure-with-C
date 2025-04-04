/*--------------------------------------------------------------------------
 File Name: singly_linked_list.c
 Programmed by: Hangyeol Lee
 Affiliation: Chungbuk University
 Functions: insert(), delete(), update(),
            retrieve(), print() in singly linked list
 Copyright (c) 2025 Hangyeol Lee. All rights reserved.
--------------------------------------------------------------------------*/
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

typedef struct S_Node // struct of singly linked list node
{
    int id;
    char name[MAX_NAME];
    struct S_Node *next;
} Node;

Node *list = NULL;

/*----------------------------------------------------------------------------
  Function : insert the node in the singly linked list sorted by id
  Interface: void insert(int id, char name[])
  Parameter: int id = an inserted id
             char name[] = an inserted name
  return: if the node was successfully inserted, return OK
----------------------------------------------------------------------------*/
int insert(int id, char name[])
{
    Node *new_node = (Node *)malloc(sizeof(Node)); // new node to insert
    if (new_node == NULL)
    {
        printf("Menory allocation was failed in insert() \n");
        exit(1);
    }
    new_node->id = id;
    strcpy(new_node->name, name); // initialize node

    if (list->next == NULL) // in case of empty
    {
        list->next = new_node;
        new_node->next = NULL;
    }
    else if (list->next->id > id) // in case of the first node
    {
        new_node->next = list->next;
        list->next = new_node;
    }
    else // in case of the middle or end node
    {
        Node *cur = list->next; // node for searching

        while (cur->next != NULL && cur->next->id < id)
        // find the correct position to insert
        {
            cur = cur->next;
        }

        new_node->next = cur->next;
        cur->next = new_node;
    }

    return OK;
}

/*----------------------------------------------------------------------------
 Function: deleting a node in the singly linked list
 Interface: int delete(int id)
 Parameter: int int = id of a node to be deleted
 return: if delete() is done, return OK
----------------------------------------------------------------------------*/
int delete(int id)
{
    Node *cur = list->next; // node for seraching
    Node *pre = NULL;       // previous node of cur

    if (list->next == NULL) // in case of empty
    {
        printf("List is empty!\n");
    }
    else if (list->next->id == id) // when the head node is target
    {
        list->next = list->next->next;
        free(cur);
    }
    else // in case of the middle or end node
    {
        while (cur->next != NULL && cur->id < id)
        // finding a node to delete
        {
            pre = cur;
            cur = cur->next;
        }

        if (cur->id == id) // target exists
        {
            pre->next = cur->next;
            free(cur);
        }
        else
        {
            printf("Target doesn't exist!\n");
        }
    }
    return OK;
}

/*----------------------------------------------------------------------------
 Function: updating a name of a node with a certain id
 Interface: int print_linked_list(int id, char name[])
 Parameter: int id: id of a node to be updated
            char name[]: name of a node to be updated
 return: if update() is done, return OK
----------------------------------------------------------------------------*/
int update(int id, char name[])
{
    if (list->next == NULL) // in case of empty
    {
        printf("List is empty!\n");
    }
    else
    {
        Node *cur = list->next; // node for searching

        while (cur->next != NULL && cur->id != id)
        // finding a node with a certain id
        {
            cur = cur->next;
        }
        if (cur->id == id) // the node exists
        {
            strcpy(cur->name, name);
        }
        else // the node doesn't exist
        {
            printf("There is no such node!\n");
        }
    }
    return OK;
}

/*----------------------------------------------------------------------------
 Function: retrieving the name of a node with a certain id
 Interface: int retrieve(int id)
 Parameter: int id: id of the node to be retrieved
 return: if retrieve() is done, return OK
----------------------------------------------------------------------------*/
int retrieve(int id)
{
    if (list->next == NULL) // list is empty
    {
        printf("List is empty!\n");
    }
    else
    {
        Node *cur = list->next; // node for searching

        while (cur->next != NULL && cur->id != id)
        // finding a node with a certain id
        {
            cur = cur->next;
        }
        if (cur->id == id) // the node exists
        {
            printf("Name: %s\n", cur->name);
        }
        else // the node doesn't exist
        {
            printf("There is no such node!\n");
        }
    }
    return OK;
}

/*----------------------------------------------------------------------------
 Function: printing nodes in the singly linked list
 Interface: void print_linked_list()
 Parameter: None
 return: void
----------------------------------------------------------------------------*/
void print_linked_list()
{
    Node *cur = list->next;
    if (cur == NULL)
        printf("List is empty!\n");
    while (cur != NULL)
    {
        printf("%d\t %s\n", cur->id, cur->name);
        cur = cur->next;
    }
}

/*----------------------------------------------------------------------------
 Function: free all nodes in list
 Interfaace: void free_list()
 Parameter: None
 return: void
----------------------------------------------------------------------------*/
void free_list()
{
    if (list->next == NULL) // list is empty
    {
        free(list); // free struct of list
    }
    else
    {
        Node *cur = list->next;
        Node *pre = NULL;

        while (cur->next != NULL)
        {
            pre = cur;
            cur = cur->next;
            free(pre); // free node of list
        }
        free(cur);  // free a last node
        free(list); // free struct of list
    }
}

/*----------------------------------------------------------------------------
 Function: initialize the singly linked list
 Interfaace: void initialize_list()
 Parameter: None
 return: void
----------------------------------------------------------------------------*/
void initialize_list()
{
    Node *cur = list->next;
    Node *pre = NULL;

    while (cur->next != NULL)
    {
        pre = cur;
        cur = cur->next;
        free(pre); // free node of list
    }

    free(cur);         // free a last node
    list->next = NULL; // initialize

    printf("Initializing is done!\n");
}

/* linked list simulator */
int main()
{
    list = (Node *)malloc(sizeof(Node));
    list->next = NULL;
    int command;
    int ret; // scanf에 잘못 입력됨을 확인하는 변수

    do
    {
        printf("----------------------------------------------------------------\n");
        printf("                     singly linked list                         \n");
        printf("----------------------------------------------------------------\n");
        printf(" Insert         = 1           Delete         = 2 \n");
        printf(" Update         = 3           Retrieve       = 4 \n");
        printf(" Initialize     = 5           Print          = 6 \n");
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
            print_linked_list();
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

            if (delete (id) == OK)
            {
                printf("Delete() is done!\n");
            }
            else
            {
                printf("Delete() is failed!\n");
            }

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

            if (update(id, name) == OK)
            {
                printf("Update() is done!\n");
            }
            else
            {
                printf("Update() is failed\n");
            }
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

            if (retrieve(id) == OK)
            {
                printf("Retrieve() is done!\n");
            }
            else
            {
                printf("Retrieve() is failed\n");
            }

            break;
        }
        case INITIALIZE: // 5
            initialize_list();
            break;
        case PRINT: // 6
            print_linked_list();
            break;
        case QUIT: // 7
            free_list();
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != QUIT);
}