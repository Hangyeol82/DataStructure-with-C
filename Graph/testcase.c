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

    // 3) DFS 호출 (1번 노드부터)
    //    예상 출력: 1 2 4 5 3 6
    dfs(1);


