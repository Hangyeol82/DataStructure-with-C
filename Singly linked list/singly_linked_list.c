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
#define PRINT 5
#define QUIT 6

#define MAX_NAME 10
#define OK 1
#define FAIL 0

typedef struct S_Node // struct of singly linked list node
{
    int id;
    char name[MAX_NAME];
    struct S_Node *next;
} Node;

Node *first = NULL;

/*----------------------------------------------------------------------------
  Function : insert a node in the singly linked list sorted by id
  Interface: int insert(int id, char name[])
  Parameter: int id = an inserted id
             char name[] = an inserted name
  return: if a node is successfully inserted, return OK
          Otherwise exit
----------------------------------------------------------------------------*/
int insert(int id, char name[])
{
    Node *new_node = (Node *)malloc(sizeof(Node)); // new node to insert

    if (new_node == NULL)
    {
        printf("Menory allocation was failed in insert() \n");
        exit(1);
    }

    new_node->id = id; // set the input values
    strcpy(new_node->name, name);

    if (first == NULL) // in case of the linked list is empty
    {
        first = new_node;
        new_node->next = NULL;
    }
    else if (first->id > id) // in case of the list node
    {
        new_node->next = first;
        first = new_node;
    }
    else // in case of the middle or end node
    {
        Node *cur = first; // node for searching
        Node *pre = NULL;  // previous node

        for (; cur != NULL; pre = cur, cur = cur->next)
        // in case of the middle location
        {
            if (cur->id > id)
            {
                pre->next = new_node;
                new_node->next = cur;
                break;
            }
        }

        if (cur == NULL) // in case of the end location
        {
            pre->next = new_node;
            new_node->next = NULL;
        }
    }

    return OK;
}

/*----------------------------------------------------------------------------
 Function: delete a node in the singly linked list
 Interface: void delete(int id)
 Parameter: int id = id of a node to be deleted
 return: void
----------------------------------------------------------------------------*/
void delete(int id)
{
    Node *cur; // node for searching
    Node *pre; // previous node of cur

    if (first == NULL) // in case the list is empty
    {
        printf("list is empty!\n");
    }
    else if (first->id == id) // in case of the list node
    {
        first = cur->next;
        free(cur);
        printf("Deleting is done!\n");
    }
    else // in case of the middle or end node
    {

        for (pre = first, cur = pre->next; cur != NULL; pre = cur, cur = cur->next)
        // finding a node to delete
        {
            if (cur->id >= id)
            {
                break;
            }
        }

        if (cur == NULL) // A target doesn't exist
        {
            printf("Target doesn't exist!\n");
        }
        else if (cur->id != id) // A target doesn't exist
        {
            printf("Target doesn't exist!\n");
        }
        else // A target was found
        {
            pre->next = cur->next;
            free(cur);
            printf("Deleting is done!\n");
        }
    }
}

/*----------------------------------------------------------------------------
 Function: update a name of a node with a certain id
 Interface: void update(int id, char name[])
 Parameter: int id: id of a node to be updated
            char name[]: name of a node to be updated
 return: void
----------------------------------------------------------------------------*/
void update(int id, char name[])
{
    if (first == NULL) // in case the list is empty
    {
        printf("list is empty!\n");
    }
    else
    {
        Node *cur; // node for searching

        for (cur = first; cur != NULL; cur = cur->next)
        {
            if (cur->id >= id)
            {
                break;
            }
        }

        if (cur == NULL) // the node doesn't exist
        {
            printf("There is no such node!\n");
        }
        else if (cur->id != id) // the node doesn't exist
        {
            printf("There is no such node!\n");
        }
        else // node was found
        {
            strcpy(cur->name, name);
            printf("Updating is done!\n");
        }
    }
}

/*----------------------------------------------------------------------------
 Function: retrieve a name of a node with a certain id
 Interface: void retrieve(int id)
 Parameter: int id: id of the node to be retrieved
 return: void
----------------------------------------------------------------------------*/
char *retrieve(int id)
{
    if (first == NULL) // in case the list is empty
    {
        printf("list is empty!\n");
    }
    else
    {
        Node *cur;

        for (cur = first; cur != NULL; cur = cur->next)
        {
            if (cur->id >= id)
            {
                break;
            }
        }

        if (cur == NULL) // the node doesn't exist
        {
            printf("There is no such node!\n");
        }
        else if (cur->id != id) // the node doesn't exist
        {
            printf("There is no such node!\n");
        }
        else // A node was found
        {
            return cur->name;
        }
    }

    return FAIL;
}

/*----------------------------------------------------------------------------
 Function: print all nodes in the singly linked list
 Interface: void print_linked_list()
 Parameter: None
 return: void
----------------------------------------------------------------------------*/
void print_linked_list()
{
    Node *cur = first;

    if (cur == NULL)
        printf("list is empty!\n");
    else
    {
        printf("\n");
        for (cur = first; cur != NULL; cur = cur->next)
        {
            printf("Id: %d      Name: %s\n", cur->id, cur->name);
        }
        printf("\n");
    }
}

/*----------------------------------------------------------------------------
 Function: free all nodes in list
 Interface: void free_list()
 Parameter: None
 return: void
----------------------------------------------------------------------------*/
void free_list()
{
    if (first == NULL) // list is empty
    {
        first = NULL;
    }
    else
    {
        Node *cur;
        Node *pre;

        for (cur = first; cur != NULL; cur = cur->next)
        {
            pre = cur;
            cur = cur->next;
            free(pre);
        }
        first = NULL;
    }
}

/* linked list simulator */
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
        printf(" Print          = 5           Quit           = 6 \n");
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
            delete(id);

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
            char *name;

            printf("Input Id: ");
            while (scanf("%d", &id) != 1) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput id again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            name = retrieve(id);

            if (name == FAIL)
            {
                printf("Retreive() was failed\n");
            }
            else
            {
                printf("Name: %s", name);
            }

            break;
        }
        case PRINT: // 5
            print_linked_list();
            break;
        case QUIT: // 6
            free_list();
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != QUIT);
}