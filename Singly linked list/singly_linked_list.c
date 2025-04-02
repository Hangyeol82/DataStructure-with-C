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

typedef struct S_List // struct of singly linked list head
{
    Node *head;
} Linked_List;

Linked_List *list;

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
    strcpy(new_node->name, name);
    new_node->next = NULL; // initialize node

    if (list->head == NULL) // the node of list is just one
    {
        list->head = new_node;
    }
    else if (list->head->id > id) // id of new_node is less than id of head node
    {
        new_node->next = list->head;
        list->head = new_node;
    }
    else
    {
        Node *now_node = list->head; // node for searching

        while (now_node->next != NULL && now_node->next->id < id)
        // find the correct position to insert
        {
            now_node = now_node->next;
        }

        new_node->next = now_node->next;
        now_node->next = new_node;
    }
    return OK;
}

/*----------------------------------------------------------------------------
 Function: deleting a node in the singly linked list
 Interface: int delete(int id)
 Parameter: int int = id of a node to be deleted
 return: if the node was successfully deleted, return OK
         if the node was not found, return FAIL
----------------------------------------------------------------------------*/
int delete(int id)
{
    Node *now_node = list->head; // node to serach
    Node *previous_node = NULL;  // previous node of now_node

    if (list->head == NULL) // list is empty
    {
        printf("List is empty!\n");
        return FAIL;
    }
    else if (list->head->id == id) // when the head node is target
    {
        list->head = list->head->next;
        free(now_node);
        return OK;
    }
    else
    {
        while (now_node->next != NULL && now_node->id < id)
        // finding a node to delete
        {
            previous_node = now_node;
            now_node = now_node->next;
        }

        if (now_node->id == id) // target exists
        {
            previous_node->next = now_node->next;
            free(now_node);
            return OK;
        }
        else
        {
            printf("Target doesn't exist!\n");
            return FAIL;
        }
    }
}

/*----------------------------------------------------------------------------
 Function: updating a name of a node with a certain id
 Interface: int print_linked_list(int id, char name[])
 Parameter: int id: id of a node to be updated
            char name[]: name of a node to be updated
 return: if the name of node was successfully updated, return OK
         if the list is empty or the node doesn't exist, return FAIL
----------------------------------------------------------------------------*/
int update(int id, char name[])
{
    if (list->head == NULL) // list is empty
    {
        return FAIL;
    }
    else
    {
        Node *now_node = list->head; // node for searching

        while (now_node->next != NULL && now_node->id != id)
        // finding a node with a certain id
        {
            now_node = now_node->next;
        }
        if (now_node->id == id) // the node exists
        {
            strcpy(now_node->name, name);
            return OK;
        }
        else // the node doesn't exist
        {
            printf("There is no such node!\n");
            return FAIL;
        }
    }
}

/*----------------------------------------------------------------------------
 Function: retrieving the name of a node with a certain id
 Interface: int retrieve(int id)
 Parameter: int id: id of the node to be retrieved
 return: if the name of the node was retrieved, return OK
         if the list is empty or the node doesn't exist, return FAIL
----------------------------------------------------------------------------*/
int retrieve(int id)
{
    if (list->head == NULL) // list is empty
    {
        printf("List is empty!\n");
        return FAIL;
    }
    else
    {
        Node *now_node = list->head; // node for searching

        while (now_node->next != NULL && now_node->id != id)
        // finding a node with a certain id
        {
            now_node = now_node->next;
        }
        if (now_node->id == id) // the node exists
        {
            printf("Name: %s\n", now_node->name);
            return OK;
        }
        else // the node doesn't exist
        {
            printf("There is no such node!\n");
            return FAIL;
        }
    }
}

/*----------------------------------------------------------------------------
 Function: printing nodes in the singly linked list
 Interface: void print_linked_list()
 Parameter: None
 return: void
----------------------------------------------------------------------------*/
void print_linked_list()
{
    Node *now_node = list->head;
    if (now_node == NULL)
        printf("List is empty!\n");
    while (now_node != NULL)
    {
        printf("%d\t %s\n", now_node->id, now_node->name);
        now_node = now_node->next;
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
    if (list->head == NULL) // list is empty
    {
        free(list); // free struct of list
    }
    else
    {
        Node *now_node = list->head;
        Node *previous_node = NULL;

        while (now_node->next != NULL)
        {
            previous_node = now_node;
            now_node = now_node->next;
            free(previous_node); // free node of list
        }
        free(now_node); // free a last node
        free(list);     // free struct of list
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
    Node *now_node = list->head;
    Node *previous_node = NULL;

    while (now_node->next != NULL)
    {
        previous_node = now_node;
        now_node = now_node->next;
        free(previous_node); // free node of list
    }

    free(now_node);    // free a last node
    list->head = NULL; // initialize

    printf("Initializing is complete!\n");
}

/* linked list simulator */
int main()
{
    list = (Linked_List *)malloc(sizeof(Linked_List));
    list->head = NULL;
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
                printf("Inserting is complete!\n");
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

            if (delete (id) == OK)
            {
                printf("Deleting is complete!\n");
            }
            else
            {
                printf("Deleting is failed!\n");
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
            while (scanf("%d", &id) != 1) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput id again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (retrieve(id) == OK)
            {
                printf("Retrieving is complete!\n");
            }
            else
            {
                printf("Retrieving is failed\n");
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