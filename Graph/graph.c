/*---------------------------------------------------------------------
File Name: graph.c
Programmed by: Hangyeol Lee
Affiliation: Chungbuk University
Functions: add_node(), add_edge(), delete_edge(),
           dfs(), bfs(), mst() in graph
Copyright (c) 2025 Hangyeol Lee. All rights reserved.
---------------------------------------------------------------------*/

/*
!!mst에서 그래프가 연결된 그래프가 아니면 안됨!!
*/

#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define FALSE 0
#define MAX_VERTEX 100
#define MAX_EDGE 200

#define MAX_STACK_SIZE 100
#define STACKOVERFLOW -1
#define STACKUNDERFLOW -2

#define MAX_QUEUE_SIZE 100
#define QUEUE_OVERFLOW -1
#define QUEUE_UNDERFLOW -2

#define ADD 1
#define DELETE 2
#define DFS 3
#define BFS 4
#define MST 5
#define QUIT 6

// 간선 노드
typedef struct AdjNode
{
    int source;             // 시작된 정점 id
    int weight;             // 간선의 w
    struct Node *dest_node; // dest 정점 주소
    struct AdjNode *next;   // 다음 간선
} AdjNode;

// 정점 노드
typedef struct Node
{
    int id;            // 정점의 고유 id
    AdjNode *adjList;  // 이 정점의 간선 인접 리스트
    struct Node *next; // 다음 정점
} Node;

// 그래프 전체
typedef struct
{
    Node *head; // 정점의 head노드
    Node *tail;
} Graph;

Graph *g; // 그래프 선언

Node *stack[MAX_STACK_SIZE];
int top = -1;

/*-------------------------------------------------------------------------------------
 Function: 스택에 한 원소를 추가한다
 Interface: int push()
 Parameter: int number = a student number to push
            char name[] = a name of student to push
 return:  if the pushing is complete, return OK
          if the stack is full, return STACKOVERFLOW
-------------------------------------------------------------------------------------*/
int push(Node *node)
{
    if (top + 1 >= MAX_STACK_SIZE)
        return STACKOVERFLOW;
    else
    {
        stack[++top] = node;
        return OK;
    }
}

/*-------------------------------------------------------------------------------------
 Function: 스택에서 top이 가르키는 최상위 원소의 주소를 리턴한다
 Interface: Node* pop()
 Paramete: None
 return: if error, return address of STACKUNDERFLOW
         otherwise return address of top node
-------------------------------------------------------------------------------------*/
Node *pop()
{
    if (top <= -1)
        return (Node *)STACKUNDERFLOW;
    else
    {
        return stack[top--];
    }
}

Node *queue[MAX_QUEUE_SIZE];
int front = 0;
int rear = 0;

/*------------------------------------------------------------------------------
 Function: 큐에 노드를 추가하고 rear의 값에 +1 연산을 수행한다
 Interface: int enqueue(Node *node)
 Parameter: Node *node = address of a node to enqueue
 return: if the queue is full, return QUEUE_OVERFLOW
         otherwise return OK.
------------------------------------------------------------------------------*/
int enqueue(Node *node)
{
    if (rear >= MAX_QUEUE_SIZE)
        return QUEUE_OVERFLOW;
    else
    {
        queue[rear++] = node;
        return OK;
    }
}

/*------------------------------------------------------------------------------
 Function: 큐의 front가 가르키는 노드의 주소를 리턴한다
 Interface: Node *dequeue()
 Parameter: None
 return: if the queue is empty, return address of QUEUE_UNDERFLOW
         otherwise return address of the first node in queue
------------------------------------------------------------------------------*/
Node *dequeue()
{
    if (front == rear)
        return (Node *)QUEUE_UNDERFLOW;
    else
    {
        return queue[front++];
    }
}

typedef struct UnionFindNode
{
    int id;                       // 노드 번호
    int rank;                     // 노드의 랭크
    struct UnionFindNode *parent; // 노드의 부모 주소
} UnionFindNode;

/*------------------------------------------------------------------------
 Function: make a node with id n and return address of that node
 Interface: UnionFindNode *init_UF_Node(int n)
 Parameters: int n: id of the node to be create
 Return: return an address of the created node
------------------------------------------------------------------------*/
UnionFindNode *init_UF_Node(int n)
{
    UnionFindNode *new_node = (UnionFindNode *)malloc(sizeof(UnionFindNode));
    new_node->id = n;
    new_node->rank = 0;
    new_node->parent = new_node;

    return new_node;
}

/*------------------------------------------------------------------------
 Function: find an address of the top parent
 Interface: UnionFindNode *UF_find(UnionFindNode *node)
 Parameters: UnionFindNode *node: address of a node
 Return: retrun the address of the most top parent
------------------------------------------------------------------------*/
UnionFindNode *UF_find(UnionFindNode *node) // 추가 사항 경로 압축
{
    UnionFindNode *cur = node;
    for (; cur->parent != cur; cur = cur->parent)
        ; // 부모 노드를 찾을때까지
    return cur;
}

/*------------------------------------------------------------------------
 Function: merge two Union Find structures
 Interface: void UF_set(UnionFindNode *a, UnionFindNode *b)
 Parameters: UnionFindNode *a: an address of a Union Find node
             UnionFindNode *b: another address of a Union Find node
 Return: void
------------------------------------------------------------------------*/
void UF_set(UnionFindNode *a, UnionFindNode *b)
{
    UnionFindNode *a_parent = UF_find(a);
    UnionFindNode *b_parent = UF_find(b);

    if (a_parent->rank > b_parent->rank) // a의 랭크가 b의 랭크보다 큰 경우
        b_parent->parent = a_parent;
    else if (b_parent->rank > a_parent->rank) // b의 랭크가 a의 랭크보다 큰 경우
        a_parent->parent = b_parent;
    else // 랭크가 같은 경우
    {
        b_parent->parent = a_parent;
        a_parent->rank++;
    }
}

/*------------------------------------------------------------------------
 Function: find an address of the node
 Interface: void *find_node(int id)
 Parameters: int id:
 Return: if the finding node is complete, return an address
         Otherwise return NULL
------------------------------------------------------------------------*/
Node *find_node(int id)
{
    Node *cur = NULL; // 탐색할 노드
    for (cur = g->head; cur != NULL; cur = cur->next)
    {
        if (cur->id == id) // id에 해당하는 노드 발견하면 break
        {
            break;
        }
    }
    return cur; // 노드 발견하지 못하면 NULL 발견하면 그 노드의 주소 return
}

/*------------------------------------------------------------------------
 Function: add a node and return an address of added node
 Interface: void *find_node(int id)
 Parameters: int id: ID of a node to be added
 Return: an address of added node
------------------------------------------------------------------------*/
Node *add_node(int id)
{
    Node *new_node = find_node(id);
    if (new_node == NULL) // 노드가 없으면 새로 만들기
    {
        new_node = malloc(sizeof(Node));
        new_node->id = id;
        new_node->adjList = NULL;
        new_node->next = NULL;

        if (g->head == NULL) // 첫번째 노드인 경우
            g->head = new_node;
        else
            g->tail->next = new_node; // 원래 꼬리의 next를 new_node
        g->tail = new_node;           // new_node를 꼬리로 변경
    }
    // 노드가 있으면 존재하는 노드 리턴 없으면 만들고 리턴
    return new_node;
}

/*------------------------------------------------------------------------
 Function: add an edge between nodes
 Interface: void add_edge(int s, int e, int w)
 Parameters: int s: source node ID of edge
             int e: destination node ID of edge
             int w: weight of the adding edge
 Return: if the adding edge is complete, return OK
         Otherwiser exit(1)
------------------------------------------------------------------------*/
int add_edge(int s, int e, int w)
{
    Node *start = add_node(s); // 간선 시작 노드 주소
    Node *end = add_node(e);   // 간선 끝 노드 주소

    AdjNode *new_edge = malloc(sizeof(AdjNode)); // 새로운 간선 노드
    if (new_edge == NULL)
    {
        printf("Memory allocation was failed in add_edge()");
        exit(1);
    }

    new_edge->source = s;
    new_edge->weight = w; // 새로운 간선 노드 초기화
    new_edge->dest_node = end;

    AdjNode *cur;
    AdjNode *pre = NULL;
    /* 인접리스트로 구현된 간선들의 정보는 내림차순으로 정렬한다 왜냐하면,
       dfs할 때 내림차순으로 push해야지 가장 작은 노드를 먼저 확인한다   */
    if (start->adjList == NULL) // 간선이 하나도 없을 때 (인접 리스트가 비어있음)
    {
        start->adjList = new_edge;
        new_edge->next = NULL;
    }
    else if (start->adjList->dest_node->id < e) // 첫번째 인접 리스트의 dest보다 새로 추가하는 간선의 e가 작을때
    {
        new_edge->next = start->adjList;
        start->adjList = new_edge;
    }
    else
    {
        // 노드를 삽입할 위치 찾기
        for (cur = start->adjList; cur != NULL; pre = cur, cur = cur->next)
        {
            if (cur->dest_node->id > e)
            {
                pre = cur;
                cur = cur->next;
                break;
            }
        }

        if (cur == NULL) // 맨 끝에 삽입하는 경우
        {
            pre->next = new_edge;
            new_edge->next = NULL;
        }
        else // 중간에 삽입하는 경우
        {
            pre->next = new_edge;
            new_edge->next = cur;
        }
    }

    return OK;
}

/*------------------------------------------------------------------------
 Function: delete an edge between nodes
 Interface: int delete_edge(int s, int e)
 Parameters: int s: source node ID of edge
             int e: destination node ID of edge
 Return: if there is no vertex or edge, return FALSE
         otherwise, return OK
------------------------------------------------------------------------*/
int delete_edge(int s, int e)
{
    Node *start = find_node(s);
    Node *end = find_node(e);
    if (start == NULL || end == NULL) // vertex가 존재하지 않으면 FALSE 리턴
    {
        printf("There is no such vertex\n");
        return FALSE;
    }

    AdjNode *cur;
    AdjNode *pre;

    // start vertex에서 삭제할 간선 찾기
    for (cur = start->adjList; cur != NULL; pre = cur, cur = cur->next)
    {
        if (cur->dest_node == end)
        {
            break;
        }
    }
    // start -> end 간선 삭제
    if (cur == NULL) // 간선이 존재하지 않으면 FALSE 리턴
    {
        printf("There is no such edge\n");
        return FALSE;
    }
    else
    {
        if (cur == start->adjList) // 첫번째 간선을 삭제하는 경우
        {
            start->adjList = cur->next;
        }
        else // 중간 혹은 마지막 간선 삭제하는 경우
        {
            pre->next = cur->next;
        }
        free(cur); // 간선 Free
    }

    // end vertex에서 삭제할 간선 찾기
    for (cur = end->adjList; cur != NULL; pre = cur, cur = cur->next)
    {
        if (cur->dest_node == start)
        {
            break;
        }
    }
    // end -> start 간선 삭제
    if (cur == NULL) // 간선이 존재하지 않으면 FALSE 리턴
    {
        printf("There is no such edge\n");
        return FALSE;
    }
    else
    {
        if (cur == end->adjList) // 첫번째 간선을 삭제하는 경우
        {
            end->adjList = cur->next;
        }
        else // 중간 혹은 마지막 간선 삭제하는 경우
        {
            pre->next = cur->next;
        }
        free(cur); // 간선 Free
    }

    return OK;
}

/*------------------------------------------------------------------------
 Function: depth first search in graph
 Interface: int dfs(int id)
 Parameters: int id: the starting node ID for searching
 Return: if stack is overflowed or underflowed, return FALSE
         otherwise, return OK
------------------------------------------------------------------------*/
int dfs(int id)
{
    int visited[MAX_VERTEX + 1] = {0};
    top = -1; // 스택 초기화

    Node *start = find_node(id);      // dfs 탐색 시잘할 노드 불러오기
    if (push(start) == STACKOVERFLOW) // 스택에 노드 Push
    {
        return FALSE;
    }

    Node *cur;    // 노드를 가르키는 변수
    AdjNode *now; // 노드에 연결된 간선들을 참조할 변수

    while (top > -1) // 스택이 빌때까지
    {
        cur = pop();
        if (cur == (Node *)STACKUNDERFLOW)
        {
            return FALSE;
        }

        if (visited[cur->id] == 0) // 방문한 적 없으면
        {
            visited[cur->id] = 1; // 방문하니까 1로 변환
            printf("%d ", cur->id);

            // 연결된 간선들을 참조하면서 스택에 노드 추가
            for (now = cur->adjList; now != NULL; now = now->next)
            {
                if (visited[now->dest_node->id] == 1) // 방문했으면 넘어가기
                    continue;
                else if (push(now->dest_node) == STACKOVERFLOW) // 방문하지 않았으면 스택에 Push
                {
                    return FALSE;
                }
            }
        }
    }
    printf("\n");

    return OK;
}

/*------------------------------------------------------------------------
 Function: breadth first search in graph
 Interface: int bfs(int id)
 Parameters: int id: the starting node ID for searching
 Return: if stack is overflowed or underflowed
         or queue is overflowed or underflowed, return FALSE
         otherwise, return OK
------------------------------------------------------------------------*/
int bfs(int id)
{
    int visited[MAX_VERTEX + 1] = {0}; // 모든 노드들의 visited 변수 0으로 초기화
    front = rear = 0;                  // 큐 초기화

    Node *start = find_node(id);          // bfs탐색을 시작할 노드 불러오기
    visited[start->id] = 1;               // 첫번째 노드는 무조건 방문하니 visited 변수 1로 초기화
    if (enqueue(start) == QUEUE_OVERFLOW) // 큐에 첫번째 노드 Enqueue
    {
        return FALSE;
    }

    Node *cur;    // 노드를 가르키는 변수
    AdjNode *now; // 노드에 연결된 간선들을 가르키는 변수

    while (front != rear) // 큐가 빌때까지
    {
        top = -1; // 스택을 사용하므로 스택 초기화
        cur = dequeue();
        if (cur == (Node *)QUEUE_UNDERFLOW)
        {
            return FALSE;
        }
        printf("%d ", cur->id);

        // 연결된 간선들을 확인하면서 스택에 Push
        for (now = cur->adjList; now != NULL; now = now->next)
        {
            if (visited[now->dest_node->id] == 1) // 방문한 적 있으면 넘어가기
                continue;
            else
            {
                if (push(now->dest_node) == STACKOVERFLOW) // 방문한 적 없으면 Push
                {
                    return STACKOVERFLOW;
                }
                // 여기서 visited 값을 1로 변경해야 같은 값이 큐에 들어가지 않음.
                visited[now->dest_node->id] = 1;
            }
        }

        /* 내림차순으로 쌓아올린 스택에서 Pop한 값을 큐에 넣으면 오름차순으로 변경됨. */
        while (top > -1)
        {
            cur = pop();
            if (cur == (Node *)STACKUNDERFLOW)
            {
                return STACKOVERFLOW;
            }
            if (enqueue(cur) == QUEUE_OVERFLOW)
            {
                return FALSE;
            }
        }
    }
    printf("\n");

    return OK;
}

/*------------------------------------------------------------------------
 Function: compare weights of two edges for library function qsort()
 Interface: int compare_edge(const void *a, const void *b)
 Parameters: const void *a: pointer of an edge
             const void *b: pointer of an another edge
 Return: if a < b, return -1
         if a > b, return +1
         if a == b. return 0
------------------------------------------------------------------------*/
int compare_edge(const void *a, const void *b)
{
    const AdjNode *edge_a = *(const AdjNode **)a;
    const AdjNode *edge_b = *(const AdjNode **)b;

    if (edge_a->weight < edge_b->weight)
        return -1;
    else if (edge_a->weight > edge_b->weight)
        return 1;
    else
        return 0;
}

/*------------------------------------------------------------------------
 Function: print all edges of a minimum spanning tree and total weight of mst
 Interface: void mst()
 Parameters: None
 Return: void
------------------------------------------------------------------------*/
void mst()
{
    int num_vertex = 0;                             // 전체 정점 노드 개수를 담을 변수
    int num_edge = 0;                               // 전체 간선 노드 개수
    Node *start = g->head;                          // 정점 노드 탐색할 변수
    AdjNode *cur;                                   // 간선 노드 탐색할 변수
    AdjNode *edge_arr[MAX_EDGE];                    // 간선 정보를 담을 배열
    UnionFindNode *unionfind_nodes[MAX_VERTEX + 1]; // Union Find 노드들을 담을 배열

    for (int i = 0; start != NULL; start = start->next) // 정점 노드 루프
    {
        num_vertex++; // 정점 노드의 개수

        UnionFindNode *unionfind_node = init_UF_Node(start->id); // Union Find노드 생성
        unionfind_nodes[start->id] = unionfind_node;             // 배열에 Union find 노드 저장

        for (cur = start->adjList; cur != NULL; cur = cur->next) // 간선 노드 루프
        {
            edge_arr[i++] = cur; // 간선 노드 배열에 넣기
            num_edge++;
        }
    }

    /* 파라미터 설명
    1. 정렬할 배열
    2. 원소 개수
    3. 원소의 크기 (포인터 배열이므로 포인터 크기)
    4. 비교함수 포인터 */
    qsort(edge_arr, num_edge, sizeof(AdjNode *), compare_edge); // qsort함수를 이용해서 오름차순 정렬

    AdjNode *mst_edge[num_vertex - 1]; // mst의 간선 정보를 담을 배열, 전체 정점 개수 -1 = mst 간선의 개수
    int edge_arr_i = 0;                // 정렬된 edge_arr의 Index 변수
    int mst_edge_i = 0;                // mst_edge의 index 변수
    int total_weight = 0;              // Mst의 전체 가중치 값

    while (mst_edge_i < num_vertex - 1 && edge_arr_i < num_edge) // mst_edge를 다 채울때 까지
    {
        if (UF_find(unionfind_nodes[edge_arr[edge_arr_i]->dest_node->id]) !=
            UF_find(unionfind_nodes[edge_arr[edge_arr_i]->source])) // Union find 자료구조를 활용해서 연결됐는지 확인
        {
            UF_set(unionfind_nodes[edge_arr[edge_arr_i]->dest_node->id],
                   unionfind_nodes[edge_arr[edge_arr_i]->source]); // UF_set함수로 두 정점 연결시키기
            mst_edge[mst_edge_i++] = edge_arr[edge_arr_i];         // edge_arr에 간선 추가.
            total_weight += edge_arr[edge_arr_i]->weight;          // total_Weight에 weight 덧셈
        }
        edge_arr_i++;
    }

    printf("---------------------------------------------\n");
    for (int i = 0; i < mst_edge_i; i++) // mst의 edge를 출력하고 total_weight을 출력한다.
    {
        printf("source: %d\t dest: %d\t weight: %d\n",
               mst_edge[i]->source, mst_edge[i]->dest_node->id, mst_edge[i]->weight);
    }
    printf("\nTotal weight: %d\n", total_weight);
    printf("---------------------------------------------\n");
}

/*------------------------------------------------------------------------
 Function: free all node in graph
 Interface: void free_graph()
 Parameters: None
 Return: void
------------------------------------------------------------------------*/
void free_graph()
{
    Node *vertex;
    Node *pre_vertex;

    AdjNode *cur;
    AdjNode *pre;
    // 모든 Vertex free
    for (vertex = g->head, pre_vertex = NULL; vertex != NULL; pre_vertex = vertex, vertex = vertex->next)
    {
        // vertex의 모든 edge free
        for (cur = vertex->adjList, pre = NULL; cur != NULL; pre = cur, cur = cur->next)
        {
            free(pre);
        }
        free(pre);

        free(pre_vertex);
    }
    free(pre_vertex);

    free(g);
}

int main()
{
    g = malloc(sizeof(Graph));
    g->head = NULL;
    g->tail = NULL;

    int command;
    int ret;

    do
    {
        printf("----------------------------------------------------------------\n");
        printf("                            graph                               \n");
        printf("----------------------------------------------------------------\n");
        printf(" Add Edge       = 1           Delete Edge    = 2 \n");
        printf(" Dfs            = 3           Bfs            = 4 \n");
        printf(" MST            = 5           Quit           = 6 \n");
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
        case ADD: // 1
        {
            int s; // 간선 시작 지점
            int d; // 간선 끝나는 지점
            int w; // weight

            printf("Input source, destination and weight :");
            while (scanf("%d %d %d", &s, &d, &w) != 3) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput command again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            add_edge(s, d, w);
            add_edge(d, s, w);
            break;
        }
        case DELETE: // 2
        {
            int s; // 간선 시작 지점
            int d; // 간선 끝나는 지점

            printf("Input source and destination :");
            while (scanf("%d %d", &s, &d) != 2) // 입력값이 정수가 아닐 경우
            {
                printf("Wrong Input!\nInput command again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (delete_edge(s, d) == FALSE)
            {
                printf("delete_edge() is failed\n");
            }
            else
            {
                printf("delete_edge() is complete\n");
            }
            break;
        }
        case DFS: // 3
        {
            int start; // dfs 시작 지점

            printf("Input the start point of dfs: ");
            while (scanf("%d", &start) != 1)
            {
                printf("Wrong Input!\nInput command again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (dfs(start) == FALSE)
            {
                printf("dfs() is failed\n");
            }
            else
            {
                printf("dfs() is complete\n");
            }
            break;
        }
        case BFS: // 4
        {
            int start; // dfs 시작 지점

            printf("Input the start point of bfs: ");
            while (scanf("%d", &start) != 1)
            {
                printf("Wrong Input!\nInput command again = ");
                while (getchar() != '\n')
                    ; // 입력 버퍼 비우기
            }

            if (bfs(start) == FALSE)
            {
                printf("bfs() is failed\n");
            }
            else
            {
                printf("bfs() is complete\n");
            }
            break;
        }
        case MST: // 5
        {
            mst();
            break;
        }
        case QUIT: // 6
            free_graph();
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<      \n");
            break;
        }
    } while (command != QUIT);
}