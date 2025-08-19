# Circular Queue (원형 큐)

📌 **C 언어로 구현한 원형 큐 (Circular Queue)**  
배열을 이용해 원형 큐를 구현했습니다.  
`front`, `rear` 인덱스를 모듈러 연산(`% MAX_QUEUE_SIZE`)으로 관리하며, **마지막 한 칸을 비워서** 공백과 포화를 구분합니다.  

---

## 📖 주요 기능
- **enqueue(int id, char name[])**
  - 큐의 rear 위치에 원소 삽입.
  - 꽉 찼으면 `OVERFLOW` 반환.
- **dequeue()**
  - front 위치의 원소를 반환 및 삭제.
  - 비어있으면 `UNDERFLOW` 반환.
- **print_queue()**
  - 현재 큐 안의 모든 원소를 출력.
  - 원소 개수(size)도 함께 표시.
- **initialization_queue()**
  - 큐를 초기화 (`front = rear = 0`).

---
