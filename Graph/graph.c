/*---------------------------------------------------------------------
File Name: graph.c
Programmed by: Hangyeol Lee
Affiliation: Chungbuk University
Functions: dfs(), bfs(), mst() in graph
Copyright (c) 2025 Hangyeol Lee. All rights reserved.
---------------------------------------------------------------------*/
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
    AdjNode *pre;
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
        for (cur = start->adjList; cur != NULL; pre = cur, cur = cur->next) // 노드를 삽입할 위치 찾기
        {
            if (cur->dest_node->id > e)
            {
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
 Function: depth first search in graph
 Interface: void dfs(int id)
 Parameters: int id: the starting node ID for searching
 Return: void
------------------------------------------------------------------------*/
void dfs(int id)
{
    int visited[MAX_VERTEX + 1] = {0};
    Node *start = find_node(id); // dfs 탐색 시잘할 노드 불러오기
    push(start);                 // 스택에 노드 Push

    Node *cur;    // 노드를 가르키는 변수
    AdjNode *now; // 노드에 연결된 간선들을 참조할 변수

    while (top > -1) // 스택이 빌때까지
    {
        cur = pop();

        if (visited[cur->id] == 0) // 방문한 적 없으면
        {
            visited[cur->id] = 1; // 방문하니까 1로 변환
            printf("%d ", cur->id);

            for (now = cur->adjList; now != NULL; now = now->next) // 연결된 간선들을 참조하면서 스택에 노드 추가
            {
                if (visited[now->dest_node->id] == 1) // 방문했으면 넘어가기
                    continue;
                else
                    push(now->dest_node); // 방문하지 않았으면 스택에 Push
            }
        }
    }
    printf("\n");
}

/*------------------------------------------------------------------------
 Function: breadth first search in graph
 Interface: void bfs(int id)
 Parameters: int id: the starting node ID for searching
 Return: void
------------------------------------------------------------------------*/
void bfs(int id)
{
    int visited[MAX_VERTEX + 1] = {0}; // 모든 노드들의 visited 변수 0으로 초기화
    front = rear = 0;                  // 큐 초기화

    Node *start = find_node(id); // bfs탐색을 시작할 노드 불러오기
    visited[start->id] = 1;      // 첫번째 노드는 무조건 방문하니 visited 변수 1로 초기화
    enqueue(start);              // 큐에 첫번째 노드 Enqueue

    Node *cur;    // 노드를 가르키는 변수
    AdjNode *now; // 노드에 연결된 간선들을 가르키는 변수

    while (front != rear) // 큐가 빌때까지
    {
        top = -1; // 스택을 사용하므로 스택 초기화
        cur = dequeue();
        printf("%d ", cur->id);

        for (now = cur->adjList; now != NULL; now = now->next) // 연결된 간선들을 확인하면서 스택에 Push
        {
            if (visited[now->dest_node->id] == 1) // 방문한 적 있으면 넘어가기
                continue;
            else
            {
                push(now->dest_node);            // 방문한 적 없으면 Push
                visited[now->dest_node->id] = 1; // 여기서 visited 값을 1로 변경해야 같은 값이 큐에 들어가지 않음.
            }
        }

        /* 내림차순으로 쌓아올린 스택에서 Pop한 값을 큐에 넣으면 오름차순으로 변경됨. */
        while (top > -1)
        {
            cur = pop();
            enqueue(cur);
        }
    }
    printf("\n");
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

int main()
{
    g = malloc(sizeof(Graph));
    g->head = NULL;
    g->tail = NULL;

    add_edge(1, 2, 3);
    add_edge(2, 1, 3);
    add_edge(1, 2, 1);
    add_edge(2, 1, 1);
    add_edge(2, 3, 4);
    add_edge(3, 2, 4);
    add_edge(3, 4, 5);
    add_edge(4, 3, 5);

    dfs(1);
    bfs(1);

    mst();
    return 0;
}