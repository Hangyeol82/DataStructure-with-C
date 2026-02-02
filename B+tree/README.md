# B+ Tree (파일 기반 구현)

 **C로 구현한 파일 기반 B+트리**  
- **트리 노드**는 `B+tree.bin`, **실제 데이터(이름)**는 `data.bin`에 저장됩니다.  
- **Free List**(삭제된 공간의 단일 연결 리스트)를 각각 유지하여 파일 공간을 재사용합니다.  
- 차수(`ORDER`)는 3으로 설정되어 있으며, **삽입/삭제/검색/갱신/범위검색/DFS/BFS**를 지원합니다.  
- DFS/BFS에 쓰는 **스택/큐 유틸은 별도 파일(`stack_queue.c/.h`)**에 있습니다 → **컴파일 시 함께 빌드**해야 합니다.

---

## 파일/메모리 레이아웃

### 트리 파일 `B+tree.bin`
- **HeadNode** (파일 0번 슬럿에 1개 저장, 크기는 `Node_size`)
  - `int root_loc` : 루트 노드의 파일 위치(슬럿 인덱스)
  - `int insert_loc`: 트리 노드용 Free List의 **첫 노드 위치** (없으면 -1)
  - `padding`: `Node_size` 정렬을 위한 패딩
- **Node** (내부/리프 공용)
  - `bool tag` : **1=리프**, **0=내부**
  - `short cnt`: **키의 개수** (내부/리프 모두 동일 의미)
  - `int parent`: 부모 노드 위치 (루트는 -1)
  - `int keys[62]` : 키 배열 (정렬 유지)
  - `int value[63]`:
    - **내부 노드**: 자식 노드 위치(자식 수 = `cnt+1`)
    - **리프 노드**: **데이터 파일(`data.bin`)의 레코드 위치**
  - `int next`: **리프 노드끼리의 오른쪽 형제** (리프에만 의미 있음, 내부에서는 사용 안 함)

> `Node_size = sizeof(Node)` 이며, 소스에서 512바이트 정렬을 염두에 둔 필드 크기로 구성됩니다.

- **FreeNode** (삭제된 위치를 Free List로 돌리기 위한 덮어쓰기용 구조)
  - `int next` + `padding` (크기는 `Node_size`와 동일)
  - 삭제 시 해당 노드 자리에 `FreeNode`를 써 넣고, `HeadNode.insert_loc` 앞에 붙입니다.

### 데이터 파일 `data.bin`
- **HeadData**
  - `int insert_loc` : 데이터용 Free List의 첫 노드 위치 (없으면 -1)
- **Data**
  - `char name[10]` : 학생 이름
- **FreeData**
  - `int next` + `padding` (크기는 `Data_size`와 동일)

---

## 상수와 불변식

- `ORDER = 3` (차수 3)
  - **최대 키 개수** = `ORDER - 1 = 2`
  - 코드에서는 `cnt >= ORDER`일 때 **오버플로우**로 보고 분할합니다.
- `UNDERFLOW = (ORDER + 2 - 1) / 2 - 1`
  - `ORDER=3`이면 `UNDERFLOW = 1`  
  - 즉, **최소 키 개수**는 1 (루트 특례 제외)
- 루트 특례
  - 루트는 내부/리프 모두 **더 관대**합니다. 내부 루트의 `cnt==0`이 되면 자식 하나를 **새 루트**로 승격합니다.

---

## 시간 복잡도 (대략)

| 연산 | 복잡도 |
|---|---|
| `insert` | `O(log N)` (분할/부모 전파 포함) |
| `delete` | `O(log N)` (차용/병합/부모 재귀 포함) |
| `retrieve` | `O(log N)` |
| `update` | `O(log N)` |
| `range_search(s,e)` | `O(log N + K)` (K = 출력 개수) |
| `dfs`, `bfs` | `O(N)` 노드 수 기준 |

---

## 함수별 설명

### `initialize_tree()`
- **역할**: B+tree.bin과 data.bin을 새로 생성하고 헤더를 초기화.
- **트리 헤더**: `root_loc=1`, `insert_loc=-1` 기록.
- **데이터 헤더**: `insert_loc=-1` 기록.
- **호출 시점**: 프로그램 시작 시 1회 호출(메인에서 호출됨).

---

### `insert_data(char name[]) → int`
- **역할**: data.bin에 이름을 1건 저장하고, 저장한 슬럿 인덱스를 반환.
- **동작**:
  1. `HeadData.insert_loc == -1` → 파일 끝에 append.
  2. 아니면 Free List의 첫 노드 자리에 덮어쓰기 후, 헤더를 다음 Free로 갱신.
- **반환값**: 저장된 데이터의 슬럿 인덱스 → 리프 노드의 `value[i]`로 들어감.

---

### `insert_node(FILE *tree, Node node) → int`
- **역할**: 트리 노드를 파일에 저장하고 위치를 반환.
- **동작**:
  1. `HeadNode.insert_loc == -1` → 파일 끝에 append.
  2. 아니면 Free List 첫 노드 자리에 덮어쓰고, 헤더의 `insert_loc` 갱신.
- **반환값**: 저장된 노드 슬럿 인덱스.

---

### `insert_into_parent(FILE *tree, int parent_loc, int left_child_loc, int right_child_loc, int key) → int`
- **역할**: 분할 결과 올라온 Key를 부모에 삽입. 필요하면 부모도 분할하고 새 루트 생성.
- **케이스**:
  - `parent_loc == -1` (부모 없음, 루트에서 분할 발생)  
    → 새 루트를 만들고 `[ left | key | right ]`로 세팅 후, `HeadNode.root_loc` 갱신.
  - 부모 존재  
    → 부모의 키/포인터 배열에서 key의 정렬 위치(index)를 찾아 밀어넣기.  
    → `parent.cnt++` 후 `cnt >= ORDER`면 내부노드를 분할:  
      - 왼쪽(원래 parent)의 cnt를 ORDER/2로 낮춤  
      - 오른쪽 노드를 새로 만들어 나머지 키/자식을 이관  
      - 오른쪽에 딸려간 자식들의 parent 포인터 갱신  
      - 승격 키를 상위로 insert_into_parent(...)에 재귀 전파
- **반환값**: 새로 생긴 부모 위치 (자식의 parent를 갱신할 때 필요)

---

### `insert(int num, char name[]) → int`
- **역할**: `(num, name)`을 B+트리에 삽입.
- **리프 탐색**: `root_loc`에서 시작, 내부 노드면 `num <= keys[i]`가 되는 첫 분기로 내려감.
- **리프 삽입**:
  - 키 배열에서 정렬 위치(index)를 찾아 keys/value를 뒤로 밀고 삽입.
  - `value[index]`에 `insert_data()`로 받은 데이터 위치 저장.
- **오버플로우 처리** (`cnt >= ORDER`)
  - 리프를 분할하고 오른쪽 리프를 파일에 저장.
  - 왼쪽의 최대 키(또는 분기 키)를 `insert_into_parent()`로 부모에 승격.
  - 리프 간 연결리스트(`next`) 갱신.
- **반환**: OK

---

### `delete_node(FILE *tree, int loc)`
- **역할**: 트리 파일의 `loc` 위치를 Free List에 연결.
- **방법**: FreeNode{ next = head.insert_loc }를 써 넣고, 헤더의 insert_loc = loc로 갱신.

---

### `delete_from_parent(FILE *tree, int parent_loc, int index) → int`
- **역할**: 부모 노드에서 분기 키 삭제 후, 언더플로우면 차용/병합을 재귀적으로 처리.
- **루트 특례**: 루트이고 `cnt == 0`이면 한 자식을 새 루트로 승격, 기존 루트는 `delete_node()`.
- **언더플로우 처리 순서 (내부 노드)**:
  1. 오른쪽 형제에서 차용 (가능하면 부모 키 조정)
  2. 왼쪽 형제에서 차용 (가능하면 부모 키 조정)
  3. 안 되면 형제와 병합 (부모 키를 내려 병합, 자식 parent 포인터 갱신)
- **반환값**: 새로운 부모 위치 (자식들의 parent 갱신용)

---

### `delete(int num) → int`
- **역할**: 키 `num`을 리프에서 삭제 후 언더플로우 복구.
- **데이터 삭제**: data.bin의 해당 레코드를 FreeData로 돌리고 HeadData.insert_loc 갱신.
- **리프 특례**:
  - 루트 리프이고 cnt==0 → `initialize_tree()`로 전체 초기화.
- **언더플로우 처리**:
  - 오른쪽 형제에서 차용 → 부모 키 조정
  - 왼쪽 형제에서 차용 → 부모 키 조정
  - 병합 → 부모 키 삭제를 `delete_from_parent()`에 위임
- **보정**: 리프의 마지막 키가 삭제되면 부모 키도 업데이트.

---

### `retrieve(int num) → int`
- **역할**: 키 `num`의 이름을 출력.
- **과정**: 내부 노드에서 분기를 따라 내려가 리프 도달 → 리프에서 탐색 → data.bin에서 읽어 출력.
- **실패 시**: `"There is no such node"`

---

### `update(int num, char name[]) → int`
- **역할**: 키 `num`의 이름을 갱신.
- **과정**: 리프 탐색 후 data.bin에서 해당 레코드를 읽어 이름을 덮어씀.
- **출력**: `"old -> new"`

---

### `dfs() → int`
- **역할**: 트리를 깊이 우선 순회하며 출력 (디버깅용).
- **방법**: `stack_queue`의 스택 사용.  
  내부 노드는 오른쪽 자식부터 push → 왼쪽부터 방문.
- **출력 예시**:
  - `Tag: 0 key: ...` (내부)
  - `Tag: 1 key: ...` (리프)

---

### `bfs() → int`
- **역할**: 트리를 너비 우선 순회하며 출력 (디버깅용).
- **방법**: `stack_queue`의 큐 사용.  
  내부 노드는 모든 자식(cnt+1)을 좌→우로 enqueue.
- **출력**: DFS와 동일 포맷.

---

### `range_search(int s, int e) → void`
- **역할**: 구간 [s, e]에 포함되는 모든 데이터(키, 이름)를 출력.
- **과정**:
  1. 루트에서 시작 → s 이상이 처음 나오는 리프로 내려감.
  2. 해당 리프에서 시작 인덱스 찾기 → e 이하 출력.
  3. e가 남아 있으면 `next` 타고 오른쪽 리프를 계속 스캔.
- **복잡도**: `O(log N + K)` (K = 출력 수)

---

### `print_tree() → void`
- **역할**: 파일 저장 순서대로 노드를 덤프 (디버깅).
- **출력**:
  - 노드의 cnt, parent, tag
  - 내부 → 자식 위치와 키
  - 리프 → 키와 (data_loc, 실제 데이터)
  - 마지막에 헤더(`root_loc`, `insert_loc`) 출력
