#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define FAIL 0

#define START 8

typedef struct Node
{
    int num;          // 학번
    char name[10];    // 이름
    int right;        // 우측 노드 
    int left;         // 좌측 노드  
} Node;

/* 바이너리 파일:
    .bin 확장자로 끝나는 파일, 이진 데이터를 저장하여 그냥 열면 읽을 수 없다
    바이너리 파일엔 int, char, 구조체 노드 등 다양한 정보를 저장할 수 있다.  */

/* 사용하는 함수 정리:
    fseek(): 언하는 위치로 파일 포인터를 옮겨준다
    fwrite(): 파일 포인터가 가르키는 위치에 값을 쓴다
    fread(): 파일 포인터가 가르키는 위치를 읽어 온다    */


int insert(char *tree_name, int num, char name[])
{
    FILE *tree = fopen(tree_name, "r+b");     // 읽기+쓰기 모드 기존의 데이터 유지
    if (tree == NULL)
    {
        printf("Error in insert()\n");
        exit(1);
    }

    fseek(tree, 0, SEEK_END); // 파일 끝으로 이동
    int size = ftell(tree);   // 파일 크기 (0이면 파일이 비어있다)
    
    Node new_node; // 삽입할 노드
    new_node.num = num;
    strcpy(new_node.name, name);
    new_node.right = -1;
    new_node.left = -1;
    
    Node now_node;   // edge를 추가할 노드

    int loc = 0;     // 시작 위치
    int insert_loc = 0;  // 노드를 삽입할 위치
    int tmp = 0;       // insert_loc값을 바꿔줄 변수

    fseek(tree, 0, SEEK_SET);       // isnert_loc 위치로 이동
    fread(&insert_loc ,sizeof(int), 1, tree);        // insert_loc 읽기
    fseek(tree,sizeof(Node) * insert_loc + START, SEEK_SET);    // insert_loc에 해당하는 위치로 이동
    fread(tmp, sizeof(int), 1,tree);                // insert_loc에 위치한 값이 가르키는 위치 tmp에 저장
    fwrite(&new_node, sizeof(Node), 1, tree);       // 새로운 노드 추가
    fseek(tree, 0, SEEK_SET);                       // insert_loc 위치로 이동
    fwrite(&tmp, sizeof(int), 1, tree);             // insert_loc 수정
    
    if (size == START*sizeof(int))
    {
        fclose(tree);
        return OK;
    }
    else
    {
        while (1)
        {
            fseek(tree, loc * sizeof(Node) + START, SEEK_SET);       // loc의 위치의 노드 찾기
            fread(&now_node, sizeof(Node), 1, tree);        // 그 위치의 노드 구조체 now_node에 읽어오기
            
            /* fread를 하면 자동으로 파일포인터는 다음노드를 가르킨다 그러므로 
               fwrite함수 전에 fseek함수로 파일포인터를 재조정 해야한다.     */
            if (now_node.num <= num)        // 우측 노드 방향
            {
                if (now_node.right == -1)
                {
                    now_node.right = insert_loc;
                    fseek(tree, loc * sizeof(Node) + START, SEEK_SET);
                    fwrite(&now_node, sizeof(Node), 1, tree); // 덮어쓰기
                    break;
                }
                loc = now_node.right;       // 탐색할 위치 오른쪽으로 바꾸기
            }
            else                            // 좌측 노드 방향
            {
                if (now_node.left == -1)
                {
                    now_node.left = insert_loc;
                    fseek(tree, loc * sizeof(Node) + START, SEEK_SET);
                    fwrite(&now_node, sizeof(Node), 1, tree); // 덮어쓰기
                    break;
                }
                loc = now_node.left;        // 탐색할 위치 왼쪽으로 비꾸기
            }
        }
        fclose(tree);
        return OK;
    }
}

int delete(char *tree_name, int num)
{
    // 필요한거: delete_node의 위치, 부모노드 위치
    FILE *tree = fopen(tree_name, "r+b");

    fseek(tree, 0, SEEK_END); // 파일 끝으로 이동
    int size = ftell(tree);   // 파일 크기( 0이면 파일이 비어있다 )

    Node now_node;            // 탐색할 노드의 정보를 저장할 노드

    int free_loc;
    fseek(tree, 4, SEEK_END);
    fread(&free_loc, sizeof(int), 1, tree);

    int loc = START;    // 탐색할 노드의 위치
    int parent_loc = NULL;  // 삭제할 노드의 부모 노드

    if (size == START)
    {
        printf("There is no such node!\n");
        fclose(tree);
        return OK;
    }
    else
    {
        while (1)
        {
            fseek(tree, loc * sizeof(Node) + START, SEEK_SET);      // loc의 위치의 노드 찾기
            fread(&now_node, sizeof(Node), 1, tree);        // 그 위치의 노드 구조체 now_node에 읽어오기
            
            if (now_node.num == num)
            {
                if (now_node.left == -1 && now_node.right == -1)        // 삭제할 노드가 자식이 없을때
                {
                    fseek(tree, loc * sizeof(Node) + START, SEEK_SET);  // 삭제할 노드의 위치로 이동
                    fwrite(NULL, sizeof(Node), 1, tree);                // NULL로 덮어 씌우기
                    fseek(tree, free_loc * sizeof(Node) + START, SEEK_SET);  // free_loc의 위치로 이동
                    fwrite(&loc, sizeof(int), 1, tree);          // loc으로 덮어 씌우기 (원래는 NULL)
                    if (parent_loc != NULL)
                    {
                        fseek(tree, parent_loc * sizeof(Node) + START, SEEK_SET);   // 부모노드 위치로 이동
                        fread(&now_node, sizeof(Node), 1, tree);                    // 부모노드 읽기
                        if(now_node.left == loc)
                        {
                            now_node.left = -1;
                        }
                        else{
                            now_node.right = -1;                                    
                        }
                        fseek(tree, parent_loc * sizeof(Node) + START, SEEK_SET);
                        fwrite(&now_node, sizeof(Node), 1, tree);               // 부모노드의 left or right 수정
                    }
                }
                else if (now_node.left != -1 && now_node.right != -1)       // 삭제할 노드가 자식이 둘다 있을때
                {
                }
                else if (now_node.left != -1)       // 왼쪽만 자식이 있을때
                {
                }
                else                                // 오른쪽만 자식이 있을때
                {
                }
            }
            else if (now_node.num < num)
            {
                parent_loc = loc;
                loc = now_node.right;
            }
            else
            {
                parent_loc = loc;
                loc = now_node.left;
            }
        }
        fclose(tree);
        return OK;
    }
}

void print_tree(char *tree_name)
{
    FILE *tree = fopen(tree_name, "r+b");

    Node node;

    int line = 0;

    while (fread(&node, sizeof(Node), 1, tree) == 1)
    {
        printf("%d\t%d\t%d\n", node.num, node.left, node.right);
        line++;
    }

    fclose(tree);
}

void initialize_tree()
{
    FILE* tree = fopen("bst.bin", "wb");
    int insert_loc = START;
    int free_loc = NULL;

    fwrite(insert_loc, sizeof(int), 1, tree);
    fwrite(free_loc, sizeof(int), 1, tree);
}

int main()
{
    initialize_tree();
    insert("bst.bin", 10, "한결");
    insert("bst.bin", 12, "장우");
    insert("bst.bin", 9, "태경");
    insert("bst.bin", 13, "선유");
    print_tree("bst.bin");
}