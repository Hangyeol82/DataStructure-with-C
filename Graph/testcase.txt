/*
Test case
(0,1,7)
(0,3,5)
(1,2,8)
(1,3,9)
(1,4,7)
(2,4,5)
(3,4,15)
(3,5,6)
(4,5,8)
(4,6,9)
(5,6,11)
*/
/*
answer
---------------------------------------------
source: 0    dest: 3    weight: 5
source: 2    dest: 4    weight: 5
source: 3    dest: 5    weight: 6
source: 0    dest: 1    weight: 7
source: 1    dest: 4    weight: 7
source: 4    dest: 6    weight: 9

Total weight: 39
---------------------------------------------
*/
/*
Result
---------------------------------------------
source: 2        dest: 4         weight: 5
source: 0        dest: 3         weight: 5
source: 3        dest: 5         weight: 6
source: 0        dest: 1         weight: 7
source: 4        dest: 1         weight: 7
source: 6        dest: 4         weight: 9

Total weight: 39
---------------------------------------------
*/
int main()
{
    g = malloc(sizeof(Graph));
    g->head = NULL;
    g->tail = NULL;

    // 간선 추가 (양방향으로 두 번씩)
    add_edge(0, 1, 7);
    add_edge(1, 0, 7);

    add_edge(0, 3, 5);
    add_edge(3, 0, 5);

    add_edge(1, 2, 8);
    add_edge(2, 1, 8);

    add_edge(1, 3, 9);
    add_edge(3, 1, 9);

    add_edge(1, 4, 7);
    add_edge(4, 1, 7);

    add_edge(2, 4, 5);
    add_edge(4, 2, 5);

    add_edge(3, 4, 15);
    add_edge(4, 3, 15);

    add_edge(3, 5, 6);
    add_edge(5, 3, 6);

    add_edge(4, 5, 8);
    add_edge(5, 4, 8);

    add_edge(4, 6, 9);
    add_edge(6, 4, 9);

    add_edge(5, 6, 11);
    add_edge(6, 5, 11);

    // MST 수행
    mst();

    // 그래프 해제
    free_graph();

    return 0;
}

/* --------------------------------------------------------- */

// ===========================================================================
// 아래 코드를 기존 main 함수 대신 붙여넣고 실행하세요.
// ===========================================================================

// 각 테스트 케이스를 실행하고 그래프를 초기화/해제하는 헬퍼 함수
void run_test_case(int case_num, const char *description, void (*test_func)())
{
    // 1. 그래프 초기화
    g = malloc(sizeof(Graph));
    g->head = NULL;
    g->tail = NULL;

    // 2. 테스트 케이스 설명 출력 및 함수 실행
    printf("\n\n======================================================\n");
    printf("           Running Test Case #%d                     \n", case_num);
    printf("Description: %s\n", description);
    printf("======================================================\n");

    test_func(); // 실제 간선을 추가하는 함수 호출

    // 3. MST 실행 및 결과 출력
    printf("Running MST algorithm...\n");
    mst();

    // 4. 그래프 메모리 해제
    // free_graph() 함수에 일부 메모리 누수 가능성이 있어 수정된 버전을 사용합니다.
    Node *vertex = g->head;
    while (vertex != NULL)
    {
        Node *temp_vertex = vertex;
        vertex = vertex->next;

        AdjNode *edge = temp_vertex->adjList;
        while (edge != NULL)
        {
            AdjNode *temp_edge = edge;
            edge = edge->next;
            free(temp_edge);
        }
        free(temp_vertex);
    }
    free(g);

    printf("Test Case #%d Finished.\n", case_num);
    printf("======================================================\n");
}

// [테스트 케이스 1] 질문 주셨던 원본 데이터
// 노드 10개, 간선 24개 (양방향이므로 12*2)
void test_case_1()
{
    printf("Adding edges for Test Case 1...\n");
    // (source, destination, weight)
    add_edge(0, 1, 4);
    add_edge(1, 0, 4);
    add_edge(0, 7, 8);
    add_edge(7, 0, 8);
    add_edge(1, 2, 8);
    add_edge(2, 1, 8);
    add_edge(1, 7, 11);
    add_edge(7, 1, 11);
    add_edge(2, 3, 7);
    add_edge(3, 2, 7);
    add_edge(2, 5, 4);
    add_edge(5, 2, 4);
    add_edge(2, 8, 2);
    add_edge(8, 2, 2);
    add_edge(3, 4, 9);
    add_edge(4, 3, 9);
    add_edge(3, 5, 14);
    add_edge(5, 3, 14);
    add_edge(4, 5, 10);
    add_edge(5, 4, 10);
    add_edge(5, 6, 2);
    add_edge(6, 5, 2);
    add_edge(6, 7, 1);
    add_edge(7, 6, 1);
    add_edge(6, 8, 6);
    add_edge(8, 6, 6);
    add_edge(7, 8, 7);
    add_edge(8, 7, 7);
    add_edge(0, 2, 12);
    add_edge(2, 0, 12);
    add_edge(1, 3, 9);
    add_edge(3, 1, 9);
    add_edge(1, 5, 13);
    add_edge(5, 1, 13);
    add_edge(4, 6, 8);
    add_edge(6, 4, 8);
    add_edge(4, 7, 15);
    add_edge(7, 4, 15);
    add_edge(5, 9, 3);
    add_edge(9, 5, 3);
    add_edge(6, 9, 5);
    add_edge(9, 6, 5);
    add_edge(3, 8, 6);
    add_edge(8, 3, 6);
    add_edge(0, 6, 9);
    add_edge(6, 0, 9);
    add_edge(8, 9, 11);
    add_edge(9, 8, 11);
}

// [테스트 케이스 2] 동일 가중치 간선이 많은 복잡한 그래프
// 노드 7개. MST 가중치는 17이 되어야 함
void test_case_2()
{
    printf("Adding edges for Test Case 2...\n");
    // (source, destination, weight)
    add_edge(0, 1, 1);
    add_edge(1, 0, 1);
    add_edge(2, 3, 2);
    add_edge(3, 2, 2); // 가중치 2
    add_edge(4, 5, 2);
    add_edge(5, 4, 2); // 가중치 2
    add_edge(0, 2, 3);
    add_edge(2, 0, 3);
    add_edge(1, 3, 4);
    add_edge(3, 1, 4); // 사이클 형성 (0-1-3-2-0)
    add_edge(5, 6, 4);
    add_edge(6, 5, 4);
    add_edge(0, 4, 5);
    add_edge(4, 0, 5); // 가중치 5
    add_edge(3, 4, 5);
    add_edge(4, 3, 5); // 가중치 5, 사이클 형성 가능
    add_edge(1, 6, 7);
    add_edge(6, 1, 7);
}

// [테스트 케이스 3] 연결되지 않은 그래프 (Disconnected Graph)
// 두 개의 컴포넌트 {0,1,2} 와 {5,6,7} 로 구성
// MST가 아닌 '최소 신장 포레스트'를 찾아야 함. 총 가중치는 (1+2) + (4+5) = 12
void test_case_3()
{
    printf("Adding edges for Test Case 3...\n");
    // Component 1
    add_edge(0, 1, 1);
    add_edge(1, 0, 1);
    add_edge(1, 2, 2);
    add_edge(2, 1, 2);
    add_edge(0, 2, 3);
    add_edge(2, 0, 3); // 사이클 형성

    // Component 2
    add_edge(5, 6, 4);
    add_edge(6, 5, 4);
    add_edge(6, 7, 5);
    add_edge(7, 6, 5);
    add_edge(5, 7, 10);
    add_edge(7, 5, 10); // 사이클 형성
}

// [테스트 케이스 4] 고밀도 그래프 (Dense Graph)
// 6개의 노드에 많은 간선이 연결되어 사이클이 많이 발생하는 경우
// 총 가중치는 10 이 되어야 함.
void test_case_4()
{
    printf("Adding edges for Test Case 4...\n");
    add_edge(0, 1, 1);
    add_edge(1, 0, 1);
    add_edge(0, 2, 2);
    add_edge(2, 0, 2);
    add_edge(0, 3, 3);
    add_edge(3, 0, 3);
    add_edge(1, 3, 4);
    add_edge(3, 1, 4);
    add_edge(2, 3, 1);
    add_edge(3, 2, 1); // (0,2,2) 보다 우선 선택됨
    add_edge(1, 2, 2);
    add_edge(2, 1, 2); // 사이클
    add_edge(3, 4, 2);
    add_edge(4, 3, 2);
    add_edge(4, 5, 3);
    add_edge(5, 4, 3);
    add_edge(2, 5, 5);
    add_edge(5, 2, 5);
    add_edge(0, 5, 6);
    add_edge(5, 0, 6);
}

int main()
{
    // 테스트 케이스 1: 원본 문제
    run_test_case(1, "Original problem data provided by user", test_case_1);
    // 예상 결과: Total weight: 38

    // 테스트 케이스 2: 동일 가중치 간선이 많은 경우
    run_test_case(2, "Complex graph with multiple edges of the same weight", test_case_2);
    // 예상 결과: Total weight: 17

    // 테스트 케이스 3: 연결되지 않은 그래프
    run_test_case(3, "Disconnected graph (testing for Minimum Spanning Forest)", test_case_3);
    // 예상 결과: Total weight: 12

    // 테스트 케이스 4: 고밀도 그래프
    run_test_case(4, "Dense graph with many potential cycles", test_case_4);
    // 예상 결과: Total weight: 9

    printf("\nAll test cases completed.\n");

    return 0;
}