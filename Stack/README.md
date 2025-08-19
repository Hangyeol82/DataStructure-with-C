# Stack (배열 기반 스택)

📌 **C 언어로 구현한 배열 기반 스택**  
최대 크기(`STACK_MAXSIZE`)를 지정해 정적 배열로 스택을 구현했습니다.  
삽입(Push), 삭제(Pop), 출력(Print), 초기화(Initialization) 기능을 제공합니다.  

---

## 📖 주요 기능
- **push(int number, char name[])**
  - 스택에 새 원소 삽입.
  - 꽉 차면 `STACKOVERFLOW` 리턴.
- **pop()**
  - 최상위 원소를 삭제하고 반환.
  - 비어있으면 `STACKUNDERFLOW` 리턴.
- **print_stack()**
  - 현재 스택 원소들을 순서대로 출력.
- **initialize_stack()**
  - 스택을 초기화 (top = -1).

---
