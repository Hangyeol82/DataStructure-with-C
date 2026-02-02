# Doubly Linked List (이중 연결 리스트)

 **C 언어로 구현한 이중 연결 리스트 (Doubly Linked List)**  
각 노드는 **왼쪽 포인터(`left`)**와 **오른쪽 포인터(`right`)**를 모두 저장하여,  
양방향 탐색과 삽입/삭제를 효율적으로 수행할 수 있습니다.  
이 구현에서는 **학번(`id`) 기준 오름차순 정렬**을 유지하도록 작성했습니다.  

---

## 주요 기능

### 1. `insert(int id, char name[])`
- **역할**: 새로운 노드를 정렬된 위치에 삽입.
- **알고리즘 단계**
  1. 새 노드 동적 할당 후 `id`, `name` 저장.
  2. 리스트가 비어 있으면 `head = new_node`.
  3. 새 노드가 `head->id`보다 작으면 맨 앞에 삽입.
  4. 그렇지 않으면 순회(`cur`, `pre`)하며 올바른 위치 탐색.
  5. 위치를 찾으면:
     - 맨 끝이면 → `pre->right = new_node`.
     - 중간이면 → `pre`와 `cur` 사이에 삽입.
- **시간 복잡도**: O(n)

---

### 2. `delete(int id)`
- **역할**: 특정 `id`를 가진 노드를 삭제.
- **알고리즘 단계**
  1. 리스트가 비어 있으면 `"list is empty!"`.
  2. 삭제 대상이 `head`이면:
     - 단일 노드일 경우 `head = NULL`.
     - 연결된 노드가 있으면 `head = head->right`, 그리고 `head->left = NULL`.
  3. 그 외에는 순회하며 대상 탐색.
  4. 대상이 존재하면:
     - 양쪽 연결 노드가 있을 경우: `cur->left->right = cur->right`, `cur->right->left = cur->left`.
     - 오른쪽이 없는 경우: `cur->left->right = NULL`.
     - 이후 `free(cur)`.
- **시간 복잡도**: O(n)
- **주의**: 반드시 포인터 갱신 후 `free()`.

---

### 3. `update(int id, char name[])`
- **역할**: 특정 `id`의 노드 이름을 새로운 값으로 변경.
- **알고리즘 단계**
  1. 리스트가 비어 있으면 `"List is empty!"`.
  2. `head`부터 순회하며 대상 탐색.
  3. 발견 시 `strcpy(cur->name, name)`.
  4. 없으면 `"There is no such node!"`.
- **시간 복잡도**: O(n)

---

### 4. `retrieve(int id)`
- **역할**: 특정 `id`의 노드 이름을 반환.
- **알고리즘 단계**
  1. 리스트가 비어 있으면 `"List is empty!"`.
  2. `head`부터 순회하며 대상 탐색.
  3. 발견 시 `cur->name` 반환.
  4. 없으면 `"There is no such node!"`.
- **시간 복잡도**: O(n)

---

### 5. `print_linked_list()`
- **역할**: 전체 리스트를 **앞에서 뒤까지** 순서대로 출력.
- **출력 형식**:
- 2021041001   Alice
- 2021041055   Hangyeol
- **시간 복잡도**: O(n)

---

### 6. `free_list()`
- **역할**: 리스트 전체 노드 및 리스트 구조체 메모리 해제.
- **알고리즘 단계**
1. `head`부터 오른쪽으로 순회하면서 `free(cur)`.
2. 마지막에 `free(list)` 호출.
- **시간 복잡도**: O(n)

---
