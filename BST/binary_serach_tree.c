#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define FAIL 0
#define START 0
#define FILENAME "bst.bin"

int insert_loc = START;
int deleted_loc = -1;

typedef struct Node
{
    int num;       // 학번
    char name[10]; // 이름
    int right;     // 우측 노드
    int left;      // 좌측 노드
} Node;

const int Node_size = sizeof(Node); // 구조체 Node의 크기

/* 바이너리 파일:
    .bin 확장자로 끝나는 파일, 이진 데이터를 저장하여 그냥 열면 읽을 수 없다
    바이너리 파일엔 int, char, 구조체 노드 등 다양한 정보를 저장할 수 있다.  */

/*
   사용하는 함수 정리:
    size_t fread(void *ptr, size_t size, size_t count, FILE *stream)
           - 파일 포인터가 가르키는 위치에서 size 만큼 count 개수의 정보를 ptr에 읽어 온다
    int fseek(FILE *stream, long offset, int origin)
           - origin에서부터 원하는 offset만큼 떨어진 위치로 파일 포인터를 옮겨준다
    size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream)
           - 파일 포인터가 가르키는 위치에 size크기만큼 count개수의 정보를 덮어쓴다.
*/
int insert(int num, char name[])
{
    FILE *tree = fopen(FILENAME, "r+b"); // 읽기+쓰기 모드 기존의 데이터 유지
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

    Node now_node; // edge를 추가할 노드

    int tmp; // insert_loc값을 바꿔줄 변수 & deleted_loc의 값이 -1인지 확인. 만약 -1이면 삭제된 부분이 다 채워진다는 뜻
    /*
     노드를 삽입하는 과정
     inset_loc위치에 노드를 삽입하고 insert_loc의 값을 수정
    */
    fseek(tree, Node_size * insert_loc, SEEK_SET); // insert_loc에 해당하는 위치로 이동
    fread(&tmp, sizeof(int), 1, tree);             // tmp에 insert_loc가 가르키는 위치의 값 저장
    printf("tmp : %d\n", tmp);
    if (tmp == -1)
    {
        deleted_loc = -1; // 삭제된 부분이 다 채워졌으면 deleted_loc를 -1로 바꿈
    }

    fseek(tree, Node_size * insert_loc, SEEK_SET); // insert_loc의 위치로 이동
    fwrite(&new_node, Node_size, 1, tree);         // 새로운 노드 추가

    /*
     삽입한 노드의 부모 노드의 left or right를 수정.
    */

    if (size != START) // 트리가 비어있지 않을때
    {
        int loc = START; // 시작 위치
        while (1)
        {
            fseek(tree, loc * Node_size, SEEK_SET); // loc의 위치의 노드 찾기
            fread(&now_node, Node_size, 1, tree);   // 그 위치의 노드 구조체 now_node에 읽어오기

            /* fread를 하면 자동으로 파일포인터는 다음노드를 가르킨다 그러므로
               fwrite함수 전에 fseek함수로 파일포인터를 재조정 해야한다.     */
            if (now_node.num <= num) // 우측 노드 방향
            {
                if (now_node.right == -1)
                {
                    now_node.right = insert_loc;
                    fseek(tree, loc * Node_size, SEEK_SET);
                    fwrite(&now_node, Node_size, 1, tree); // 덮어쓰기
                    break;
                }
                loc = now_node.right; // 탐색할 위치 오른쪽으로 바꾸기
            }
            else // 좌측 노드 방향
            {
                if (now_node.left == -1)
                {
                    now_node.left = insert_loc;
                    fseek(tree, loc * Node_size, SEEK_SET);
                    fwrite(&now_node, Node_size, 1, tree); // 덮어쓰기
                    break;
                }
                loc = now_node.left; // 탐색할 위치 왼쪽으로 비꾸기
            }
        }
    }
    if (deleted_loc == -1) // deleted_loc가 -1이면 삭제된 부분이 다 채워졌다는 뜻
    {
        fseek(tree, 0, SEEK_END);             // 끝 쪽으로 이동
        insert_loc = ftell(tree) / Node_size; // 끝의 위치로 insert_loc 업데이트
    }
    else
    {
        insert_loc = tmp; // insert_loc 업데이트
    }
    fclose(tree);
    return OK;
}

int delete(int num)
{
    FILE *tree = fopen(FILENAME, "r+b");
    if (tree == NULL)
    {
        printf("Error opening file in delete()\n");
        return FAIL;
    }

    int none = -1; // 삭제 노드에 대신 저장할 값

    fseek(tree, 0, SEEK_END); // 파일 끝으로 이동
    int size = ftell(tree);   // 파일 크기 (START이면 파일이 비어있다)

    if (size == START) // 파일이 비어있을 경우
    {
        printf("There is no such node!\n");
        fclose(tree);
        return FAIL;
    }

    Node now_node;       // 탐색할 노드의 정보를 저장할 노드
    int loc = START;     // 탐색할 노드의 위치
    int parent_loc = -1; // 삭제할 노드의 부모 노드 위치

    while (1) // 삭제할 노드를 찾는 과정
    {
        if (loc == -1)
        {
            printf("There is no such node!\n");
            fclose(tree);
            return FAIL;
        }
        fseek(tree, loc * Node_size, SEEK_SET); // loc의 위치의 노드 찾기
        fread(&now_node, Node_size, 1, tree);   // 그 위치의 노드를 now_node에 읽어오기

        if (now_node.num == num) // 삭제할 노드를 찾은 경우
        {
            if (now_node.left == -1 && now_node.right == -1) // 자식이 없는 경우
            {
                if (parent_loc != -1) // 부모 노드가 존재할 경우
                {
                    Node parent;
                    fseek(tree, parent_loc * Node_size, SEEK_SET);
                    fread(&parent, Node_size, 1, tree);

                    if (parent.left == loc)
                        parent.left = -1;
                    else
                        parent.right = -1;

                    fseek(tree, parent_loc * Node_size, SEEK_SET);
                    fwrite(&parent, Node_size, 1, tree);
                }

                // 삭제 연결 리스트에 노드 추가
                fseek(tree, loc * Node_size, SEEK_SET);
                fwrite(&none, sizeof(int), 1, tree);
                fseek(tree, deleted_loc * Node_size, SEEK_SET);
                fwrite(&loc, sizeof(int), 1, tree);
                if (deleted_loc == -1)
                    insert_loc = loc;
                deleted_loc = loc;

                break;
            }
            else if (now_node.left != -1 && now_node.right != -1) // 자식이 둘 다 있는 경우
            {
                // 우측 서브트리에서 가장 작은 노드 찾기
                Node smallest;
                int loc2 = now_node.right; // 가장 작은 노드의 위치
                int loc2_parent = loc;     // 가장 작은 노드의 부모의 위치

                while (1)
                {
                    fseek(tree, loc2 * Node_size, SEEK_SET);
                    fread(&smallest, Node_size, 1, tree);

                    if (smallest.left == -1)
                        break;

                    loc2_parent = loc2;
                    loc2 = smallest.left;
                }

                // 가장 작은 노드의 부모 노드 수정
                Node s_parent;
                fseek(tree, loc2_parent * Node_size, SEEK_SET);
                fread(&s_parent, Node_size, 1, tree);

                if (s_parent.left == loc2) // 부모의 왼쪽노드가 smallest 일때
                {
                    // 부모 노드의 edge 수정
                    s_parent.left = smallest.right;
                    fseek(tree, loc2_parent * Node_size, SEEK_SET);
                    fwrite(&s_parent, Node_size, 1, tree);

                    // 삭제할 노드에 가장 작은 노드의 값 복사
                    now_node.num = smallest.num;
                    fseek(tree, loc * Node_size, SEEK_SET);
                    fwrite(&now_node, Node_size, 1, tree);
                }
                else // 부모의 오른쪽 노드가 smallest일때 ( 우측 서브트리에 노드가 단 하나일때 ) 삭제하는 노드와 가장 작은 노드의 부모노드가 같음
                {
                    // 삭제할 노드에 가장 작은 노드의 값 복사
                    now_node.num = smallest.num;
                    now_node.right = -1;
                    fseek(tree, loc * Node_size, SEEK_SET);
                    fwrite(&now_node, Node_size, 1, tree);
                }

                // 삭제 연결 리스트에 추가
                fseek(tree, loc2 * Node_size, SEEK_SET);
                fwrite(&none, sizeof(int), 1, tree);
                fseek(tree, deleted_loc * Node_size, SEEK_SET);
                fwrite(&loc2, sizeof(int), 1, tree);
                if (deleted_loc == -1)
                    insert_loc = loc2; // 위치기 바뀌었기에 Loc2로 초기화
                deleted_loc = loc2;

                break;
            }
            else // 자식이 하나만 있는 경우
            {
                int child_loc = (now_node.left != -1) ? now_node.left : now_node.right;

                if (parent_loc != -1) // 부모 노드가 존재할 경우
                {
                    Node parent;
                    fseek(tree, parent_loc * Node_size, SEEK_SET);
                    fread(&parent, Node_size, 1, tree);

                    if (parent.left == loc)
                        parent.left = child_loc;
                    else
                        parent.right = child_loc;

                    fseek(tree, parent_loc * Node_size, SEEK_SET);
                    fwrite(&parent, Node_size, 1, tree);
                }

                // 삭제 연결 리스트에 추가
                fseek(tree, loc * Node_size, SEEK_SET);
                fwrite(&none, sizeof(int), 1, tree);
                fseek(tree, deleted_loc * Node_size, SEEK_SET);
                fwrite(&loc, sizeof(int), 1, tree);
                if (deleted_loc == -1)
                    insert_loc = loc;
                deleted_loc = loc;

                break;
            }
        }
        else if (now_node.num < num) // 오른쪽으로 이동
        {
            parent_loc = loc;
            loc = now_node.right;
        }
        else // 왼쪽으로 이동
        {
            parent_loc = loc;
            loc = now_node.left;
        }
    }

    fclose(tree);
    return OK;
}

void print_tree()
{
    FILE *tree = fopen(FILENAME, "r+b");

    Node node;

    int line = 0;

    while (fread(&node, Node_size, 1, tree) == 1)
    {
        printf("%d\t%d\t%d\n", node.num, node.left, node.right);
        line++;
    }
    printf("%d %d\n", insert_loc, deleted_loc);
    fclose(tree);
}

void initialize_tree()
{
    FILE *tree = fopen(FILENAME, "wb");
    insert_loc = START;
    deleted_loc = -1;
    fclose(tree);
}

int main()
{
    initialize_tree();
}