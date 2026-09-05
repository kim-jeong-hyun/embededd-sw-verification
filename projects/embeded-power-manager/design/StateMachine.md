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
       Low        |     |
                  |     ▼
                  |  +-------------+
                  |  |   SLEEP     |
                  |  +-------------+
                  |     |       |
                  |     |       | 30초 경과
                  |     |       ▼
                  |     |  +-------------+
                  |     |  | POWER_OFF   |
                  |     |  +-------------+
                  |     |
                  | Wake-up
                  |     ▼
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
- 시스템 전원이 차단된 상태이다. 

#### Entry Condition
- SLEEP 상태에서 30초가 경과하여 시스템 전원이 차단된 경우

#### Exit Condition
- IGN Signal ON
- Wake up Signal 감지
- CAN Wake up Request 수신

### 2.2 INIT
- 시스템 초기화를 수행하는 상태이다.

#### Entry Condition
- POWER OFF 상태에서 IGN Signal ON
- Wake Up Signal
- CAN Wake Up Request 수신
- FAULT 상태에서 Fault Reset 완료

#### Exit Condition
- Initialization 완료

### 2.3 ACTIVE
- 시스템의 정상 동작 상태이다. 

#### Entry Condition
- INIT 완료

#### Exit Condition
- Battery Voltage Low
- IGN Signal OFF
- Fault 발생
