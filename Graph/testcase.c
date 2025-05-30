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

// 예상 출력: 1 2 3 4 5 6
bfs(1);

/*------------------------------------------------
Kruskal test case

add_edge(1, 2, 4); add_edge(2, 1, 4);
add_edge(2, 3, 3); add_edge(3, 2, 3);
add_edge(3, 4, 2); add_edge(4, 3, 2);
add_edge(4, 5, 1); add_edge(5, 4, 1);

anwer: 10
---------------------------

add_edge(1, 2, 1); add_edge(2, 1, 1);
add_edge(2, 3, 2); add_edge(3, 2, 2);
add_edge(3, 1, 3); add_edge(1, 3, 3);  // ← 싸이클 유발

add_edge(3, 4, 4); add_edge(4, 3, 4);
add_edge(4, 5, 5); add_edge(5, 4, 5);

answer: 12
---------------------------
add_edge(1, 2, 10); add_edge(2, 1, 10);
add_edge(1, 3, 1);  add_edge(3, 1, 1);
add_edge(2, 3, 1);  add_edge(3, 2, 1);
add_edge(3, 4, 1);  add_edge(4, 3, 1);

answer: 3
---------------------------
add_edge(1, 2, 1);  add_edge(2, 1, 1);
add_edge(2, 3, 1);  add_edge(3, 2, 1);
add_edge(4, 5, 1);  add_edge(5, 4, 1);

// 연결부
add_edge(3, 4, 100); add_edge(4, 3, 100);  // ← weight는 크지만 필요함

answer: 103
---------------------------
add_edge(1, 2, 3);  add_edge(2, 1, 3);
add_edge(1, 2, 1);  add_edge(2, 1, 1);  // ← 중복 간선
add_edge(2, 3, 4);  add_edge(3, 2, 4);
add_edge(3, 4, 5);  add_edge(4, 3, 5);

answer: 10
-------------------------------------------------*/
