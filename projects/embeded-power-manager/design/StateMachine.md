# StateMachine.md

## 1. State Diagram

```text
                 +-------------+
                 | POWER_OFF   |
                 +-------------+
                        |
                 IGN ON / Wake-up
                        |
                        ▼
                 +-------------+
                 |    INIT     |
                 +-------------+
                        |
              Initialization Success
                        |
                        ▼
                 +-------------+
                 |   ACTIVE    |
                 +-------------+
                  |     |     |
                  |     |     |
       Battery    |     | IGN OFF
       Low        |     | Timer Start
                  |     ▼
                  |  +--------+
                  |  | SLEEP  |
                  |  +--------+
                  |      |
                  | Wake-up
                  |      ▼
                  |    INIT
                  ▼
            +-------------+
            | LOW_POWER   |
            +-------------+
                  |
       Battery Voltage Recovery
                  |
                  ▼
               ACTIVE

Fault 발생
    │
    ▼
+-------------+
|   FAULT     |
+-------------+
      |
 Fault Reset
      |
      ▼
    INIT
```

---

## 2. State Description

### 2.1 POWER OFF
- 시스템의 기본 대기 상태이다.

#### Entry Condition
- 시스템 초기 전원 인가
- Shutdown 완료

#### Exit Condition
- IGN Signal ON
- Wake up Signal
- CAN Wake up Request

### 2.2 INIT
- 시스템 초기화를 수행하는 상태이다.

#### Entry Condition
- POWER OFF 상태에서 IGN Signal ON
- Wake Up Signal
- CAN Wake Up Signal
- Fault Recovery

#### Exit Condition
- Initialization 완료

### 2.3 ACTIVE
- 시스템의 정상 동작 상태이다. 

#### Entry Condition
- INIT 완료

#### Exit Condition
- Battery Voltage Low
- IGN OFF
- Fault 발생
