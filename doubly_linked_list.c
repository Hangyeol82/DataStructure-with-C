/*---------------------------------------------------------------------------------------------
File Name: doubly_linked_list.c
Programmed by: Hangyeol Lee
Affiliation: Chungbuk University
Functions: insert(), delete(), print() in doubly linked list
Copyright (c) 2025 Hangyeol Lee. All rights reserved.
---------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#define INSERT 1
#define DELETE 2
#define PRINT 3
#define QUIT 4

typedef struct Node // struct of node
{
    int data;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Doubly_Linked_List // struct of Doubly linked list
{
    Node *head;
} Linked_List;

/*----------------------------------------------------------------------------
 insert(): inserting a node in the doubly linked list
 parameter: Linked_list (doubly linked list), int (data of the inserting node)
 return: X
----------------------------------------------------------------------------*/
void insert(Linked_List *list, int value)
{
    Node *new_node = (Node *)malloc(sizeof(Node)); // node to insert
    new_node->data = value;
    new_node->left = NULL;
    new_node->right = NULL;

    if (list->head == NULL) // when tree is empty
    {
        list->head = new_node;
        return;
    }

    Node *now_node = list->head; // node for searching

    if (value < now_node->data) // 왼쪽으로 탐색, head의 data보다 작을때
    {
        while (now_node->left != NULL && now_node->left->data > value) // 삽입할 위치를 찾을때까지
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
    else // 오른쪽으로 탐색, head의 data보다 값이 클때
    {
        while (now_node->right != NULL && now_node->right->data <= value) // 삽입할 위치를 찾을때까지
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
}

/*----------------------------------------------------------------------------
 delete(): deleting a node in the doubly linked list
 parameter: Linked_list (doubly linked list), int (data of the deleting node)
 return: X
----------------------------------------------------------------------------*/
void delete(Linked_List *list, int value)
{
    if (list->head == NULL) // when tree is empty
    {
        printf("Tree is empty!\n");
        return;
    }

    Node *now_node = list->head; // node for searching

    if (now_node->data == value)
    {                                                          // 삭제할 노드가 head 노드일때
        if (now_node->right != NULL && now_node->left != NULL) // head 양쪽에 둘다 노드가 있을때
        {
            list->head = now_node->right;
            list->head->left = now_node->left;
            now_node->left->right = list->head;
            free(now_node);
        }
        else if (now_node->left == NULL && now_node->right != NULL) // 오른쪽에만 존재할때
        {
            list->head = now_node->right;
            list->head->left = NULL;
            free(now_node);
        }
        else if (now_node->left != NULL && now_node->right == NULL) // 왼쪽에만 존재할때
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
        return;
    }

    if (value < now_node->data) // 삭제할 노드 찾기
    {
        while (now_node->left != NULL && now_node->data != value)
            now_node = now_node->left;
    }
    else
    {
        while (now_node->right != NULL && now_node->data != value)
            now_node = now_node->right;
    }

    if (now_node->data == value)
    {
        if (now_node->right != NULL && now_node->left != NULL) // 삭제할 노드의 양쪽에 노드가 있을때
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
    }
    else
        printf("There is no such node!\n");
}

/*----------------------------------------------------------------------------
 print_linked_list(): printing nodes in the doubly linked list
 parameter: Linked_list (doubly linked list)
 return: X
----------------------------------------------------------------------------*/
void print_linked_list(Linked_List *list)
{
    if (list->head == NULL)
    {
        printf("Tree is emptry!\n");
        return;
    }
    Node *now_node = list->head;
    while (now_node->left != NULL)
        now_node = now_node->left;

    while (now_node != NULL)
    {
        printf("%d ", now_node->data);
        now_node = now_node->right;
    }
    printf("\n");
}

/*----------------------------------------------------------------------------
 free_list(): free all nodes in list and list
 parameter: Linked_list (doubly linked list)
 return: X
----------------------------------------------------------------------------*/
void free_list(Linked_List *list)
{
    if (list->head == NULL)
    {
        free(list);
        return;
    }

    Node *now_node = list->head;
    Node *next_node;

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

int main()
{
    Linked_List *list = (Linked_List *)malloc(sizeof(Linked_List));
    list->head = NULL;
    int command;
    int ret;        // scanf에 잘못 입력됨을 확인하는 변수
    do
    {
        printf("----------------------------------------------------------------\n");
        printf("                    doubly linked list                         \n");
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