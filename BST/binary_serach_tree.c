#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define FAIL 0

#define START 0

int insert_loc = START;
int deleted_loc = -1;

typedef struct Node
{
    int num;       // 학번
    char name[10]; // 이름
    int right;     // 우측 노드
    int left;      // 좌측 노드
} Node;

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
int insert(char *tree_name, int num, char name[])
{
    FILE *tree = fopen(tree_name, "r+b"); // 읽기+쓰기 모드 기존의 데이터 유지
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

    int loc = 0; // 시작 위치
    int tmp = 0; // insert_loc값을 바꿔줄 변수 & insert_loc의 값이 NULL인지 확인. 만약 NULL이면 삭제된 부분이 다 채워진다는 뜻
    /*
     노드를 삽입하는 과정
     inset_loc위치에 노드를 삽입하고 insert_loc의 값을 수정
    */
    fseek(tree, sizeof(Node) * insert_loc, SEEK_SET); // insert_loc에 해당하는 위치로 이동
    fread(&tmp, sizeof(int), 1, tree);                // tmp에 insert_loc가 가르키는 위치의 값 저장
    if (tmp == -1)
    {
        deleted_loc = -1; // 삭제된 부분이 다 채워졌으면 deleted_loc를 -1로 바꿈
    }
    fseek(tree, -sizeof(Node), SEEK_CUR);     // 현제 위치에서 -sizeof(Node)만큼 앞으로 이동. 원래 insert_loc위치
    fwrite(&new_node, sizeof(Node), 1, tree); // 새로운 노드 추가
    if (deleted_loc == -1)                    // deleted_loc가 -1이면 삭제된 부분이 다 채워졌다는 뜻
    {
        fseek(tree, 0, SEEK_END); // 끝 쪽으로 이동
        insert_loc = ftell(tree); // 끝의 위치로 insert_loc 업데이트
    }
    else
    {
        insert_loc = tmp; // insert_loc 업데이트
    }

    /*
     삽입한 노드의 부모 노드의 left or right를 수정.
    */
    if (size == START) // 트리가 비어있을때
    {
        fclose(tree);
        return OK;
    }
    else
    {
        while (1)
        {
            fseek(tree, loc * sizeof(Node), SEEK_SET); // loc의 위치의 노드 찾기
            fread(&now_node, sizeof(Node), 1, tree);   // 그 위치의 노드 구조체 now_node에 읽어오기

            /* fread를 하면 자동으로 파일포인터는 다음노드를 가르킨다 그러므로
               fwrite함수 전에 fseek함수로 파일포인터를 재조정 해야한다.     */
            if (now_node.num <= num) // 우측 노드 방향
            {
                if (now_node.right == -1)
                {
                    now_node.right = insert_loc;
                    fseek(tree, loc * sizeof(Node), SEEK_SET);
                    fwrite(&now_node, sizeof(Node), 1, tree); // 덮어쓰기
                    break;
                }
                loc = now_node.right; // 탐색할 위치 오른쪽으로 바꾸기
            }
            else // 좌측 노드 방향
            {
                if (now_node.left == -1)
                {
                    now_node.left = insert_loc;
                    fseek(tree, loc * sizeof(Node), SEEK_SET);
                    fwrite(&now_node, sizeof(Node), 1, tree); // 덮어쓰기
                    break;
                }
                loc = now_node.left; // 탐색할 위치 왼쪽으로 비꾸기
            }
        }
        fclose(tree);
        return OK;
    }
}

int delete(char *tree_name, int num)
{
    FILE *tree = fopen(tree_name, "r+b");

    int none = -1; // 삭제 노드에 대신 저장할 값

    fseek(tree, 0, SEEK_END); // 파일 끝으로 이동
    int size = ftell(tree);   // 파일 크기( START이면 파일이 비어있다 )

    Node now_node; // 탐색할 노드의 정보를 저장할 노드

    int deleted_loc; // 마지막으로 삭제된 노드 위치
    fseek(tree, 4, SEEK_END);
    fread(&deleted_loc, sizeof(int), 1, tree);

    int loc = START; // 탐색할 노드의 위치
    int parent_loc;  // 삭제할 노드의 부모 노드

    if (size == START) // 파일 크기가 START면 파일이 비어있다
    {
        printf("There is no such node!\n");
        fclose(tree);
        return FAIL;
    }
    else
    {
        while (1) // 삭제할 노드를 찾는 과정
        {
            fseek(tree, loc * sizeof(Node), SEEK_SET); // loc의 위치의 노드 찾기
            fread(&now_node, sizeof(Node), 1, tree);   // 그 위치의 노드를 now_node에 읽어오기

            if (now_node.num == num)
            {
                if (now_node.left == -1 && now_node.right == -1) // 삭제할 노드가 자식이 없을때
                {

                    if (parent_loc != NULL) // 삭제된 노드의 부모노드가 존재할 경우. 즉, 헤드노드가 아닐 경우
                    {
                        Node parent;
                        fseek(tree, parent_loc * sizeof(Node), SEEK_SET); // 부모노드 위치로 이동
                        fread(&parent, sizeof(Node), 1, tree);            // 부모노드 읽기
                        if (parent.left == loc)
                        {
                            parent.left = -1;
                        }
                        else
                        {
                            parent.right = -1;
                        }
                        fseek(tree, parent_loc * sizeof(Node), SEEK_SET);
                        fwrite(&parent, sizeof(Node), 1, tree); // 부모노드의 left or right 수정
                    }
                    /*
                     삭제 연결리스트에 노드 추가 & deleted_loc 업데이트
                    */
                    fseek(tree, loc * sizeof(Node), SEEK_SET);         // 삭제할 노드의 위치로 이동
                    fwrite(&none, sizeof(int), 1, tree);               // NULL로 덮어 씌우기
                    fseek(tree, deleted_loc * sizeof(Node), SEEK_SET); // deleted_loc가 가르키는 위치로 이동
                    fwrite(&loc, sizeof(int), 1, tree);                // loc으로 덮어 씌우기 (원래는 NULL)
                    deleted_loc = loc;                                 // deleted_loc 바꿔주기

                    break;
                }
                else if (now_node.left != -1 && now_node.right != -1) // 삭제할 노드가 자식이 둘다 있을때
                {
                    /*
                     삭제할 노드의 우측 트리에서 가장 작은 노드를 찾는 방법 선택
                    */
                    Node smallest;
                    Node s_parent;
                    int loc2_parent = loc; // 가장 작은 노드의 부모 노드 위치
                    int loc2 = now_node.right;

                    while (1) // 가장 작은 노드 탐색
                    {
                        fseek(tree, loc2, SEEK_SET);
                        fread(&smallest, sizeof(Node), 1, tree);
                        if (smallest.left == -1)
                        {
                            break;
                        }
                        loc2_parent = loc2;
                        loc2 = smallest.left;
                    }

                    /*
                     가장 작은 노드의 부모 노드 수정하는 작업
                    */
                    if (smallest.right == -1) // 가장 작은 노드가 자식이 있을 경우.
                    {
                        int child = smallest.right; // 가장 작은 노드의 자식의 위치를 child에 저장
                        fseek(tree, loc2_parent, SEEK_SET);
                        fread(&s_parent, sizeof(Node), 1, tree);
                        if (s_parent.right = loc2)
                        {
                            s_parent.right = child;
                        }
                        else
                        {
                            s_parent.left = child;
                        }
                        fseek(tree, -sizeof(Node), SEEK_CUR);
                        fwrite(&s_parent, sizeof(Node), 1, tree);
                    }
                    else
                    {
                        fseek(tree, loc2_parent, SEEK_SET);
                        fread(&s_parent, sizeof(Node), 1, tree);
                        if (s_parent.right = loc2)
                        {
                            s_parent.right = -1;
                        }
                        else
                        {
                            s_parent.left = -1;
                        }
                        fseek(tree, -sizeof(Node), SEEK_CUR);
                        fwrite(&s_parent, sizeof(Node), 1, tree);
                    }
                    /*
                     삭제 링크드 리스트에 smallest추가 & deleted_loc 업데이트
                    */
                    fseek(tree, loc2, SEEK_SET);
                    fwrite(none, sizeof(int), 1, tree);
                    fseek(tree, deleted_loc, SEEK_SET);
                    fwrite(&loc2, sizeof(int), 1, tree);
                    deleted_loc = loc2;

                    /*
                     원래 삭제 예정이던 노드를 smallest의 num으로 수정
                    */
                    now_node.num = smallest.num;
                    fseek(tree, loc, SEEK_SET);
                    fwrite(&now_node, sizeof(Node), 1, tree);
                }
                else // 삭제할 노드가 자식이 하나만 있을때
                {
                    int child_loc;
                    if (now_node.left != -1) // 왼쪽만 자식이 있을때
                    {
                        child_loc = now_node.left; // 왼쪽 자식의 위치
                    }
                    else
                    {
                        child_loc = now_node.right; // 오른쪽 자식의 위치
                    }

                    if (parent_loc != NULL) // 삭제하는 노드가 헤드노드가 아닌경우
                    {
                        Node parent;
                        fseek(tree, parent_loc * sizeof(Node), SEEK_SET);
                        fread(&parent, sizeof(Node), 1, tree);
                        if (parent.left == loc) // 삭제하는 노드가 부모노드의 왼쪽 자식일 경우
                        {
                            parent.left = child_loc; // 부모노드를 삭제하는 노드의 자식으로 위치를 수정
                        }
                        else // 삭제하는 노드가 부모노드의 오른쪽 자식일 경우
                        {
                            parent.right = child_loc; // 부모노드를 삭제하는 노드의 자식으로 위치를 수정
                        }
                        fseek(tree, -sizeof(Node), SEEK_CUR);   // parent위치로 이동
                        fwrite(&parent, sizeof(Node), 1, tree); // 수정된 parent로 덮어쓰기
                    }
                    /*
                    삭제 연결리스트에 노드 추가 & deleted_loc 업데이트
                    */
                    fseek(tree, loc * sizeof(Node), SEEK_SET);         // 삭제할 노드의 위치로 이동
                    fwrite(none, sizeof(int), 1, tree);                // NULL로 덮어 씌우기
                    fseek(tree, deleted_loc * sizeof(Node), SEEK_SET); // deleted_loc가 가르키는 위치로 이동
                    fwrite(&loc, sizeof(int), 1, tree);                // loc으로 덮어 씌우기 (원래는 NULL)
                    deleted_loc = loc;                                 // deleted_loc 바꿔주기

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
    FILE *tree = fopen("bst.bin", "wb");
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