#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define FAIL 0

typedef struct Node
{
    int value;
    int right;
    int left;
} Node;

int insert(char *name, int value)
{
    FILE *tree = fopen(name, "r+b");
    if (tree == NULL)
    {
        printf("Error in insert()\n");
        exit(1);
    }
    Node new_node;
    new_node.value = value;
    new_node.right = -1;
    new_node.left = -1;

    fseek(tree, 0, SEEK_END); // 파일 끝으로 이동
    int size = ftell(tree);   // 파일 크기( 0이면 파일이 비어있다 )

    Node now_node;
    int loc = 0;
    fwrite(&new_node, sizeof(Node), 1, tree); // 노드 추가

    printf("size: %d\n", size);
    
    if (size == 0)
    {
        return OK;
    }
    else
    {
        int last_line = size / sizeof(Node);
        while (loc != -1)
        {
            fseek(tree, loc, SEEK_SET);              // loc의 위치의 노드 찾기
            fread(&now_node, sizeof(Node), 1, tree); // 그 위치의 노드 구조체 now_node에 읽어오기
            if (now_node.value <= value)
            {
                loc = now_node.right;
                if (loc == -1)
                {
                    Node change; // 덮어 쓸 구조체
                    change.value = now_node.value;
                    change.left = now_node.left;
                    change.right = last_line;

                    fwrite(&change, sizeof(Node), 1, tree); // 덮어쓰기

                    return OK;
                }
            }
            else
            {
                loc = now_node.left;
                if (loc == -1)
                {
                    Node change; // 덮어 쓸 구조체
                    change.value = now_node.value;
                    change.left = last_line;
                    change.right = now_node.right;

                    fwrite(&change, sizeof(Node), 1, tree); // 덮어쓰기

                    return OK;
                }
            }
        }
    }
}

void print_tree(char *name)
{
    FILE *tree = fopen(name, "r+b");

    Node node;

    int line = 0;

    while (fread(&node, sizeof(Node), 1, tree) == 1)
    {
        printf("%d\t%d\t%d\n", node.value, node.left, node.right);
        line++;
    }
}

int main()
{
    fopen("bst.txt", "wb");
    insert("bst.txt", 10);
    insert("bst.txt", 12);
    print_tree("bst.txt");
}