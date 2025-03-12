#include <stdio.h>
#include <stdlib.h>

#define INSERT 1
#define DELETE 2
#define PRINT 3
#define QUIT 4
#define INITIALIZATION 5

#define MAX_QUEUE_SIZE 10
#define LEFT -1
#define RIGHT 1

typedef struct BSTNode
{
    int key;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct BinarySearchTree
{
    Node *head;
} Tree;

void insert(Tree *tree, int value)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->key = value;
    new_node->left = NULL;
    new_node->right = NULL;

    if (tree->head == NULL)
    {
        tree->head = new_node;
        return;
    }

    Node *now_node = tree->head;
    Node *previous_node = NULL;

    while (now_node != NULL)
    {
        previous_node = now_node;
        if (now_node->key <= value)
            now_node = now_node->right;
        else
            now_node = now_node->left;
    }

    if (previous_node->key <= value)
        previous_node->right = new_node;
    else
        previous_node->left = new_node;
}

void delete(Tree *tree, int value)
{
    if(tree->head == NULL){
        printf("Tree is empty!\n");
        return;
    }

    Node *now_node = tree->head;
    Node *previous_node = NULL;
    
    while (now_node != NULL && now_node->key != value)
    {
        previous_node = now_node;
        if (now_node->key <= value)
            now_node = now_node->right;
        else
            now_node = now_node->left;
    }
    
    if(now_node == NULL){
        printf("There is no such node\n");
        return;
    }
    else{
        int way = 0;
        if(previous_node->key > value)
            way = LEFT;
        else
            way = RIGHT;
        if(now_node->left == NULL && now_node->right == NULL){      // 리프노드 일때
            free(now_node);
        }
        else if(now_node->left != NULL && now_node->right == NULL){     // 삭제하는 노드의 왼쪽만 노드가 잇는 경우
            if (way == LEFT){
                previous_node->left = now_node->left;
                free(now_node);
            }
            else{
                previous_node->right = now_node->left;
                free(now_node);
            }
        }
        else if(now_node->left == NULL && now_node->right != NULL){     // 삭제하는 노드의 오른족만 노드가 있는 경우
            if (way == LEFT){
                previous_node->left = now_node->right;
                free(now_node);
            }
            else{
                previous_node->right = now_node->right;
                free(now_node);
            }
        }
        else{

        }
    }
}

void delete_node(){

}

void postorder(Node* node){ // 후위 순회
    if(node->left != NULL)
        postorder(node->left);
    if(node->right != NULL)
        postorder(node->right);
    printf("%d ", node->key);
}

void preorder(Node* node){  // 전위 순회
    printf("%d ", node->key);
    if(node->left != NULL)
        preorder(node->left);
    if(node->right != NULL)
        preorder(node->right);
}

void inorder(Node* node){   // 중위 순회
    if(node->left != NULL)
        inorder(node->left);
    printf("%d ", node->key);
    if(node->right != NULL)
        inorder(node->right);
}

void print_tree(Tree *tree)
{
    if(tree->head == NULL){
        printf("Tree is empty!\n");
        return;
    }
    printf("Postorder: ");
    postorder(tree->head);
    printf("\n");
    printf("Preorder: ");
    preorder(tree->head);
    printf("\n");
    printf("Inorder: ");
    inorder(tree->head);
    printf("\n");
}

int main()
{
    Tree *tree = (Tree *)malloc(sizeof(Tree));
    tree->head = NULL;
    int command;
    do
    {
        printf("----------------------------------------------------------------\n");
        printf("                     binary search tree                         \n");
        printf("----------------------------------------------------------------\n");
        printf(" Insert        = 1           Delete        = 2 \n");
        printf(" Print         = 3           Quit          = 4 \n");
        printf(" Initialize    = 5 \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf("%d", &command);

        switch (command)
        {
        case PRINT: // 3;
        {
            print_tree(tree);
            break;
        }
        case INSERT: // 1
        {
            int num;
            int ret;
            printf("Input value: ");
            ret = scanf("%d", &num);
            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ;
            }
            else
            {
                insert(tree, num);
            }
            break;
        }
        case DELETE: // 2
        {
            int num;
            int ret;
            printf("Input value: ");
            ret = scanf("%d", &num);
            if (ret != 1)
            {
                printf("Wrong Input!\n");
                while (getchar() != '\n')
                    ;
            }
            else
            {
                delete (tree, num);
            }
            break;
        }
        case QUIT: // 4
            break;
        case INITIALIZATION: // 5
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != 4);
}