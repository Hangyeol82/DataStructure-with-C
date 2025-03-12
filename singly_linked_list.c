/*---------------------------------------------------------------------------------------------
File Name: singly_linked_list.c
Programmed by: Hangyeol Lee
Affiliation: Chungbuk University
Functions: insert(), delete(), print() in singly linked list
Copyright (c) 2025 Hangyeol Lee. All rights reserved.
---------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#define INSERT 1
#define DELETE 2
#define PRINT 3
#define QUIT 4

typedef struct Singly_Linked_List_Node // struct of singly linked list node
{
    int data;
    struct Singly_Linked_List_Node *next;
} Node;

typedef struct Singly_Linked_List // struct of singly linked list head
{
    Node *first_node;
} Linked_List;

/*----------------------------------------------------------------------------
 insert(): inserting a node in the singly linked list
 parameter: Linked_list (singly linked list), int (data of the inserting node)
 return: X
----------------------------------------------------------------------------*/
void insert(Linked_List *list, int value)
{
    Node *new_node = (Node *)malloc(sizeof(Node)); // new node to insert
    new_node->data = value;
    new_node->next = NULL;

    if (list->first_node == NULL)
    { // the node of list is just one
        list->first_node = new_node;
        return;
    }

    if (list->first_node->data > value)
    { // inserting value is less than data of first node
        new_node->next = list->first_node;
        list->first_node = new_node;
        return;
    }

    Node *now_node = list->first_node; // node for searching

    while (now_node->next != NULL && now_node->next->data < value)
    { // find the correct position to insert
        now_node = now_node->next;
    }

    new_node->next = now_node->next;
    now_node->next = new_node;
}

/*----------------------------------------------------------------------------
 delete(): deleting a node in the singly linked list
 parameter: Linked_list (singly linked list), int (data of the deleting node)
 return: X
----------------------------------------------------------------------------*/
void delete(Linked_List *list, int value)
{
    if (list->first_node == NULL) // list is empty
    {
        printf("List is empty!\n");
        return;
    }

    Node *now_node = list->first_node; // node to serach

    if (list->first_node->data == value) // the first node is target
    {
        list->first_node = list->first_node->next;
        free(now_node);
        return;
    }

    Node *previous_node = NULL;
    while (now_node->next != NULL && now_node->next->data <= value) // finding a node to delete
    {
        previous_node = now_node;
        now_node = now_node->next;
    }

    if (now_node->data == value) // target exists
    {
        previous_node->next = now_node->next;
        free(now_node);
    }
    else
    {
        printf("There is no such node!\n");
    }
}

/*----------------------------------------------------------------------------
 print_linked_list(): printing nodes in the singly linked list
 parameter: Linked_list (singly linked list)
 return: X
----------------------------------------------------------------------------*/
void print_linked_list(Linked_List *list)
{
    Node *now_node = list->first_node;

    while (now_node != NULL)
    {
        printf("%d ", now_node->data);
        now_node = now_node->next;
    }
    printf("\n");
}

/*----------------------------------------------------------------------------
 free_list(): free all nodes in list
 parameter: Linked_list (singly linked list)
 return: X
----------------------------------------------------------------------------*/
void free_list(Linked_List *list)
{
    if(list->first_node == NULL){
        free(list);
        return;
    }
    
    Node *now_node = list->first_node;
    Node *previous_node = NULL;

    while (now_node->next != NULL)
    {
        previous_node = now_node;
        now_node = now_node->next;
        free(previous_node);
    }

    free(list);
}

/* linked list simulator */
int main()
{
    Linked_List *list = (Linked_List *)malloc(sizeof(Linked_List));
    list->first_node = NULL;
    int command;
    int ret;            // scanf에 잘못 입력됨을 확인하는 변수
    do
    {
        printf("----------------------------------------------------------------\n");
        printf("                  singly linked list                         \n");
        printf("----------------------------------------------------------------\n");
        printf(" Insert        = 1           Delete        = 2 \n");
        printf(" Print         = 3           Quit          = 4 \n");
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
        case PRINT: // 3
            print_linked_list(list);
            break;
        case INSERT: // 1
        {
            int num;
            printf("Input value: ");
            ret = scanf("%d", &num);
            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ;
            }

            insert(list, num);
            break;
        }
        case DELETE: // 2
        {
            int num;
            printf("Input value: ");
            ret = scanf("%d", &num);
            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ;
            }

            delete (list, num);
            break;
        }
        case QUIT: // 4
            free_list(list);
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != 4);
}