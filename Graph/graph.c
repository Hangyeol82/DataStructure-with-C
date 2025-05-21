// bfs 고쳐야 됨.

#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define FALSE 0

#define STACK_OVERFLOW -1
#define STACK_UNDERFLOW -2

#define QUEUE_OVERFLOW -1
#define QUEUE_UNDERFLOW -2

#define MAX_STACK_SIZE 100
#define MAX_QUEUE_SIZE 100

// 간선 노드
typedef struct AdjNode
{
    int dest;               // 연결된 정점 id
    struct Node *dest_node; // 연결된 정점 주소
    int weight;             // 간선의 w
    struct AdjNode *next;   // 다음 간선
} AdjNode;

// 정점 노드
typedef struct Node
{
    int id;            // 정점의 고유 id
    int visited;       // 방문했는지 확인하는 변수
    AdjNode *adjList;  // 이 정점의 인접 리스트
    struct Node *next; // 다음 정점
} Node;

// 그래프 전체
typedef struct
{
    Node *vertices; // 정점 리스트(head)
} Graph;

Graph *g; // 그래프 선언

Node *stack[MAX_STACK_SIZE];
int top = -1;

int push(Node *node)
{
    if (top + 1 >= MAX_STACK_SIZE)
        return STACK_OVERFLOW;
    else
    {
        stack[++top] = node;
        return OK;
    }
}
Node *pop()
{
    if (top <= -1)
        return (Node *)STACK_UNDERFLOW;
    else
    {
        return stack[top--];
    }
}

Node *queue[MAX_QUEUE_SIZE];
int front = 0;
int rear = 0;

int enqueue(Node *node)
{
    if (rear > MAX_QUEUE_SIZE)
        return QUEUE_OVERFLOW;
    else
    {
        queue[rear++] = node;
        return OK;
    }
}

Node *dequeue()
{
    if (front == rear)
        return (Node *)QUEUE_UNDERFLOW;
    else
    {
        return queue[front++];
    }
}

// dfs, bfs 끝나고 모든 노드들의 visited 변수를 0 으로 초기화 해야함
void init_visited()
{
    for (Node *cur = g->vertices; cur; cur = cur->next)
    {
        cur->visited = 0;
    }
}

Node *find_node(int id)
{
    Node *cur;
    for (cur = g->vertices; cur != NULL; cur = cur->next)
    {
        if (cur->id == id)
        {
            return cur;
        }
    }
    return NULL;
}

// 노드 추가하거나 있으면 그 노드 반환하는 함수
Node *add_node(int id)
{
    Node *new_node = find_node(id);
    if (new_node == NULL) // 노드가 없으면 새로 만들기
    {
        new_node = malloc(sizeof(Node));
        new_node->id = id;
        new_node->visited = 0;
        new_node->adjList = NULL;
        new_node->next = g->vertices;
        g->vertices = new_node;
    }
    // 노드가 있으면 존재하는 노드 리턴 없으면 만들고 리턴
    return new_node;
}

// 간선 연결
int add_edge(int s, int e, int w)
{
    Node *start = add_node(s);
    Node *end = add_node(e);

    AdjNode *new_node = (AdjNode *)malloc(sizeof(AdjNode));
    if (new_node == NULL)
    {
        printf("Memory allocation was failed in add_edge()");
        exit(1);
    }
    new_node->dest = e;
    new_node->dest_node = end;
    new_node->weight = w;

    AdjNode *cur;
    AdjNode *pre;
    /* 인접리스트로 구현된 간선들의 정보는 내림차순으로 정렬한다 왜냐하면,
       dfs할 때 내림차순으로 push해야지 가장 작은 노드먼저 확인한다   */
    if (start->adjList == NULL) // 간선이 하나도 없을 때 (인접 리스트가 비어있음)
    {
        start->adjList = new_node;
    }
    else if (start->adjList->dest < e) // 첫번째 인접 리스트의 dest보다 새로 추가하는 간선의 e가 클 때
    {
        new_node->next = start->adjList;
        start->adjList = new_node;
    }
    else
    {
        for (cur = start->adjList; cur != NULL; pre = cur, cur = cur->next)
        {
            if (cur->dest > e)
            {
                break;
            }
        }

        if (cur == NULL)
        {
            pre->next = new_node;
            new_node->next = NULL;
        }
        else
        {
            pre->next = new_node;
            new_node->next = cur;
        }
    }

    return OK;
}

void dfs(int id)
{
    init_visited();
    Node *start = find_node(id);
    push(start);

    Node *cur;
    AdjNode *now;

    while (top > -1)
    {
        cur = pop();
        if (cur->visited == 0)
        {
            cur->visited = 1;
            printf("%d ", cur->id);
            for (now = cur->adjList; now != NULL; now = now->next)
            {
                if (now->dest_node->visited == 1)
                    continue;
                else
                {
                    push(now->dest_node);
                }
            }
        }
    }
}

void bfs(int id)
{
    init_visited();
    front = rear = 0;

    Node *start = find_node(id);
    enqueue(start);

    Node *cur;
    Node *tmp;
    AdjNode *now;

    while (front != rear)
    {
        top = -1;
        cur = dequeue();
        if (cur->visited == 0)
        {
            cur->visited = 1;
            printf("%d ", cur->id);
            for (now = cur->adjList; now != NULL; now = now->next)
            {
                if (now->dest_node->visited == 1)
                    continue;
                else
                {
                    push(now->dest_node);
                }
            }
            // 간선이 내림차순으로 정렬돼 있기 때문에 큐에는 오름차순으로 다시 정렬하기 위한 로직.
            while (top > -1)
            {
                tmp = pop();
                tmp->visited = 1;
                enqueue(tmp);
            }
        }
    }
}

int main()
{
    g = malloc(sizeof(Graph));
    g->vertices = NULL;

    // 1) 노드 삽입 (사실 add_edge()가 자동으로 생성해 주지만,
    //    별도 확인을 위해 명시적으로 찍어볼 수도 있습니다)
    add_node(1);
    add_node(2);
    add_node(3);
    add_node(4);
    add_node(5);
    add_node(6);

    // 2) 간선 추가 (dest 순서에 따라 adjList가 내림차순 정렬됨)
    //    1 ↔ 2, 1 ↔ 3
    add_edge(1, 2, 0);
    add_edge(1, 3, 0);

    //    2 ↔ 4, 2 ↔ 5
    add_edge(2, 4, 0);
    add_edge(2, 5, 0);

    //    3 ↔ 6
    add_edge(3, 6, 0);

    //    예상 출력: 1 2 4 5 3 6
    dfs(1);

    bfs(1);

    return 0;
}