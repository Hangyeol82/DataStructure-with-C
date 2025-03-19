/*---------------------------------------------------------------------------------------------
 File Name: singly_linked_list.c
 Programmed by: Hangyeol Lee
 Affiliation: Chungbuk University
 Functions: insert(), delete(), update(), retreve(), print() in singly linked list
 Copyright (c) 2025 Hangyeol Lee. All rights reserved.
---------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INSERT 1
#define DELETE 2
#define UPDATE 3
#define RETREVE 4
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

typedef struct S_List // struct of singly linked list head
{
    Node *head;
} Linked_List;

/*----------------------------------------------------------------------------
  Function : insert a node in the singly linked list
  Interface: void insert()
  Parameter: linked_list *list = a starting pointer for singly linked list
             int id = an inserted id
             char name[] = an inserted name
  return: OK = if the node was successfully inserted
----------------------------------------------------------------------------*/
int insert(Linked_List *list, int id, char name[])
{
    Node *new_node = (Node *)malloc(sizeof(Node)); // new node to insert
    if (new_node == NULL)
    {
        printf("Menory allocation was failed in insert() \n");
        exit(1);
    }

    new_node->id = id;
    strcpy(new_node->name, name);
    new_node->next = NULL;

    if (list->head == NULL) // the node of list is just one
    {
        list->head = new_node;
    }
    else if (list->head->id > id) // inserting id is less than id of head node
    {
        new_node->next = list->head;
        list->head = new_node;
    }
    else
    {
        Node *now_node = list->head; // node for searching

        while (now_node->next != NULL && now_node->next->id < id) // find the correct position to insert
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
 Interface: void delete()
 Parameter: Linked_list *list = a starting pointer for singly linked list
            int int = id of a node to be deleted
 return: OK = if the node was successfully deleted
         FAIL = if the node was not found
----------------------------------------------------------------------------*/
int delete(Linked_List *list, int id)
{
    Node *now_node = list->head; // node to serach
    Node *previous_node = NULL;

    if (list->head == NULL) // list is empty
    {
        printf("List is empty!\n");
        return FAIL;
    }
    else if (list->head->id == id) // the head node is target
    {
        list->head = list->head->next;
        free(now_node);
        return OK;
    }
    else
    {
        while (now_node->next != NULL && now_node->next->id <= id) // finding a node to delete
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
            return FAIL;
        }
    }
}

/*----------------------------------------------------------------------------
 Function: updating a name of a node with a certain id
 Interface: print_linked_list()
 Parameter: Linked_list *list: a starting pointer for singly linked list
            int id: id of a node to be updated
            char name[]: name of a node to be updated
 return: OK = if the name of node was successfully updated
         FAIL = if the list is empty or the node doesn't exist
----------------------------------------------------------------------------*/
int update(Linked_List *list, int id, char name[])
{
    if (list->head == NULL)
    {
        return FAIL;
    }
    else
    {
        Node *now_node = list->head;
        while (now_node->next != NULL && now_node->id != id)
        {
            now_node = now_node->next;
        }
        if (now_node->id == id)
        {
            strcpy(now_node->name, name);
            return OK;
        }
        else
        {
            printf("There is no such node!\n");
            return FAIL;
        }
    }
}

/*----------------------------------------------------------------------------
 Function: retreving a name of a node with a certain id
 Interface: print_linked_list()
 Parameter: Linked_list *list: a starting pointer for singly linked list
            int id: id of the node to be retreved
 return: OK = if the name of the node was retreved
         FAIL = if the list is empty or the node doesn't exist
----------------------------------------------------------------------------*/
int retreve(Linked_List *list, int id)
{
    if (list->head == NULL)
    {
        return FAIL;
    }
    else
    {
        Node *now_node = list->head;
        while (now_node->next != NULL && now_node->id != id)
        {
            now_node = now_node->next;
        }
        if (now_node->id == id)
        {
            printf("Name: %s\n", now_node->name);
            return OK;
        }
        else
        {
            printf("There is no such node!\n");
            return FAIL;
        }
    }
}

/*----------------------------------------------------------------------------
 Function: printing nodes in the singly linked list
 Interface: print_linked_list()
 Parameter: Linked_list *list: a starting pointer for singly linked list
 return: void
----------------------------------------------------------------------------*/
void print_linked_list(Linked_List *list)
{
    Node *now_node = list->head;

    while (now_node != NULL)
    {
        printf("%d\t %s\n", now_node->id, now_node->name);
        now_node = now_node->next;
    }
}

/*----------------------------------------------------------------------------
 Function: free all nodes in list
 Interfaace: free_list()
 Parameter: Linked_list *list: a starting pointer for singly linked list
 return: void
----------------------------------------------------------------------------*/
void free_list(Linked_List *list)
{
    if (list->head == NULL)
    {
        free(list);
    }
    else
    {
        Node *now_node = list->head;
        Node *previous_node = NULL;

        while (now_node->next != NULL)
        {
            previous_node = now_node;
            now_node = now_node->next;
            free(previous_node);
        }

        free(list);
    }
}

/* linked list simulator */
int main()
{
    Linked_List *list = (Linked_List *)malloc(sizeof(Linked_List));
    list->head = NULL;
    int command;
    int ret; // scanf에 잘못 입력됨을 확인하는 변수
    do
    {
        printf("----------------------------------------------------------------\n");
        printf("                  singly linked list                         \n");
        printf("----------------------------------------------------------------\n");
        printf(" Insert        = 1           Delete        = 2 \n");
        printf(" Update        = 3           Retreve       = 4 \n");
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
            if (insert(list, id, name) == OK)
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
            ret = scanf("%d", &id);

            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ;
            }

            if (delete (list, id) == OK)
            {
                printf("Deleting is compelte!\n");
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
        case RETREVE: // 4
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

            if (retreve(list, id) == OK)
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