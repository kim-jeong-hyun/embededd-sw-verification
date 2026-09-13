# Software Design Description

## 1. Overview

### 1.1 Purpose
- 본 문서는 Embedded Power Manager의 소프트웨어 설계를 정의한다.

### 1.2 Scope
- 시스템 전원 상태 관리
- 주변 장치 전원 제어
- 시스템 상태 모니터링
- Fault 보호 및 복구

## 2. Software Architecture

### 2.1 Block Diagram
              +-----------------------+
              |     Input Layer       |
              +-----------------------+
              |       IGN Signal      |
              |     Wake-up Signal    |
              |     Battery Voltage   |
              |      CAN Message      |
              |       Fault Flag      |
              +-----------------------+
                         |
                         v
              +-----------------------+
              |     PowerManager      |
              +-----------------------+
              |    State Management   |
              |    Transition Logic   |
              |    Fault Management   |
              +-----------------------+
                         |
                         v
              +-----------------------+
              |     Output Layer      |
              +-----------------------+
              |     System State      |
              |    Peripheral Power   |
              |     Sleep Request     |
              |   Diagnostic Status   |
              +-----------------------+
              
### 2.2 Module Description
- Power Manager는 입력 신호를 기반으로 시스템 상태를 관리한다.
- 상태 변경에 따라 주변 장치 전원을 제어한다.
- Fault 발생 시 보호 및 복구를 수행한다.

## 3. Interface Design

### 3.1 Input Interface
| Input | Description |
|-------|-------------|
| IGN Signal | 시동 신호 |
| Wake-up Signal | 시스템 기동 요청 |
| Battery Voltage | 배터리 전압 |
| CAN Message | CAN 통신 메시지 |
| Fault Flag | 내부 Fault 정보 |

### 3.2 Output Interface
| Output | Description |
|--------|-------------|
| System State | 현재 시스템 상태 |
| Peripheral Power Enable | 주변 장치 전원 제어 |
| Sleep Request | Sleep 요청 |
| Diagnostic Status | 진단 상태 |

## 4. Data Definition

### 4.1 Enumeration

#### 4.1.1 PowerState

| Enumeration | Description |
|-------------|-------------|
| POWER_OFF | 시스템 전원 OFF 상태 |
| INIT | 시스템 초기화 상태 |
| ACTIVE | 정상 동작 상태 |
| LOW_POWER | 저전력 동작 상태 |
| SLEEP | 절전 상태 |
| FAULT | Fault 보호 상태 |

---

#### 4.1.2 CanMessageId

| Enumeration | Value | Description |
|-------------|------:|-------------|
| NONE | 0x000 | CAN Message 없음 |
| WAKEUP_REQUEST | 0x100 | Wake-up 요청 |
| SLEEP_REQUEST | 0x101 | Sleep 요청 |
| RESET_FAULT | 0x102 | Fault Reset 요청 |

### 4.2 Const Definition

#### 4.2.1 Timing Constant

| Constant | Value | Description |
|----------|------:|-------------|
| TASK_PERIOD_MS | 10 ms | Update 주기 |
| SLEEP_TIMEOUT_MS | 30000 ms | Sleep Timeout |

---

#### 4.2.2 Battery Threshold

| Constant | Value | Description |
|----------|------:|-------------|
| BATTERY_FAULT_THRESHOLD | 10.0 V | Battery Fault 판단 기준 |
| LOW_BATTERY_THRESHOLD | 11.5 V | LOW_POWER 복귀 기준 |

## 5. Class Design

### 5.1 Public Interface

| Function | Description |
|----------|-------------|
| Init() | 시스템 초기화 |
| UpdateState() | 상태 전이 수행 |
| UpdateOutputs() | 출력 갱신 |
| SetIgnSignal(bool) | IGN 입력 설정 |
| SetWakeupSignal(bool) | Wake-up Signal 설정 |
| SetBatteryVoltage(float) | Battery Voltage 설정 |
| SetCanTimeout(bool) | CAN Timeout 설정 |
| SetInternalFault(bool) | Internal Fault 설정 |
| RecvCanMessage() | CAN 메시지 수신 |
| ProcessCanMessage() | CAN 메시지 처리 |
| ClearFault() | Fault 초기화 |
| SetInitFinished(bool) | 초기화 완료 설정 

---

### 5.2 Function Description

#### SDD-EPM-001 : Init()

| Item | Description |
|------|-------------|
| Purpose | PowerManager를 초기 상태로 초기화한다. |
| Input | None |
| Output | None |
| Processing | - 상태 변수를 초기화한다.
<br>- 입력 변수를 초기화한다.
<br>- Fault Flag를 초기화한다.
<br>- 출력 변수를 초기화한다.
<br>- Timer를 초기화한다.
<br>- Current State를 POWER_OFF로 설정한다. |

---

#### SDD-EPM-002 : UpdateState()

| Item | Description |
|------|-------------|
| Purpose | 현재 입력 조건을 평가하여 시스템 상태를 전이한다. |
| Input | IGN Signal, Wake-up Signal, Battery Voltage, CAN Message, Fault Flag |
| Output | Current State |
| Processing | CAN 메시지를 처리한 후 현재 상태에 따라 상태 전이 조건을 평가하여 다음 상태를 결정한다. |

---

#### SDD-EPM-003 : UpdateOutputs()

| Item | Description |
|------|-------------|
| Purpose | 현재 시스템 상태에 따라 출력 신호를 설정한다. |
| Input | Current State |
| Output | Peripheral Power Enable, Camera Power Enable |
| Processing | 현재 상태에 따라 주변 장치 및 카메라 전원을 ON/OFF 한다. |

---

#### SDD-EPM-004 : SetIgnSignal()

| Item | Description |
|------|-------------|
| Purpose | IGN 입력 신호를 저장한다. |
| Input | bool on |
| Output | None |
| Processing | IGN 입력 상태를 내부 변수에 저장한다. |

---

#### SDD-EPM-005 : SetWakeupSignal()

| Item | Description |
|------|-------------|
| Purpose | Wake-up Signal을 저장한다. |
| Input | bool on |
| Output | None |
| Processing | Wake-up 요청 상태를 내부 변수에 저장한다. |

---

#### SDD-EPM-006 : SetBatteryVoltage()

| Item | Description |
|------|-------------|
| Purpose | Battery Voltage를 저장하고 Battery Fault를 판단한다. |
| Input | float voltage |
| Output | None |
| Processing | Battery Voltage를 저장하고 Fault Threshold 미만인 경우 Battery Fault를 설정한다. |

---

#### SDD-EPM-007 : SetCanTimeout()

| Item | Description |
|------|-------------|
| Purpose | CAN Timeout 상태를 저장한다. |
| Input | bool timeout |
| Output | None |
| Processing | CAN Timeout 상태를 내부 변수에 저장한다. |

---

#### SDD-EPM-008 : RecvCanMessage()

| Item | Description |
|------|-------------|
| Purpose | CAN 메시지를 수신한다. |
| Input | CanMessageId |
| Output | None |
| Processing | 수신한 CAN Message ID를 내부 변수에 저장한다. |

---

#### SDD-EPM-009 : ProcessCanMessage()

| Item | Description |
|------|-------------|
| Purpose | 수신된 CAN 메시지를 처리한다. |
| Input | CAN Message ID |
| Output | Current State |
| Processing | CAN Message 종류에 따라 Wake-up, Fault Reset 등의 상태 전이를 수행한 후 메시지를 초기화한다. |

---

#### SDD-EPM-010 : SetInternalFault()

| Item | Description |
|------|-------------|
| Purpose | Internal Fault 상태를 저장한다. |
| Input | bool fault |
| Output | None |
| Processing | Internal Fault Flag를 설정한다. |

---

#### SDD-EPM-011 : ClearFault()

| Item | Description |
|------|-------------|
| Purpose | 모든 Fault Flag를 초기화한다. |
| Input | None |
| Output | None |
| Processing | Battery Fault, CAN Timeout, Internal Fault를 모두 해제한다. |

---

#### SDD-EPM-012 : GetPowerState()

| Item | Description |
|------|-------------|
| Purpose | 현재 시스템 상태를 반환한다. |
| Input | None |
| Output | PowerState |
| Processing | 현재 State를 반환한다. |

---

#### SDD-EPM-013 : SetInitFinished()

| Item | Description |
|------|-------------|
| Purpose | 초기화 완료 여부를 설정한다. |
| Input | bool finished |
| Output | None |
| Processing | Initialization 완료 상태를 저장한다. |

---

#### SDD-EPM-014 : GetPeripheralPowerEnable()

| Item | Description |
|------|-------------|
| Purpose | 주변 장치 전원 상태를 반환한다. |
| Input | None |
| Output | bool |
| Processing | Peripheral Power Enable 상태를 반환한다. |

---

#### SDD-EPM-015 : GetCameraPowerEnable()

| Item | Description |
|------|-------------|
| Purpose | 카메라 전원 상태를 반환한다. |
| Input | None |
| Output | bool |
| Processing | Camera Power Enable 상태를 반환한다. |

---

#### SDD-EPM-016 : GetBatteryVoltage()

| Item | Description |
|------|-------------|
| Purpose | 현재 Battery Voltage를 반환한다. |
| Input | None |
| Output | float |
| Processing | 저장된 Battery Voltage를 반환한다. |

---

#### SDD-EPM-017 : GetPowerStateString()

| Item | Description |
|------|-------------|
| Purpose | 현재 Power State를 문자열 형태로 반환한다. |
| Input | None |
| Output | const char* |
| Processing | 현재 Power State에 해당하는 문자열을 반환한다. |

### 5.3 Class Relationship
```text
                +--------------------+
                |   SimulationInput  |
                +---------+----------+
                          |
                          v
                  +---------------+
                  | PowerManager  |
                  +---------------+
                  | UpdateState() |
                  | UpdateOutputs()|
                  +-------+--------+
                          |
                          v
            +------------------------------+
            | Peripheral / Camera Output   |
            +------------------------------+
```

## 6. State Machine

상태 천이 및 각 상태의 상세 동작은 StateMachine.md 참조
