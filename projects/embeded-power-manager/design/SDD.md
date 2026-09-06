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

## 4. Class Design

### 4.1 PowerManager
+ Init()
+ Update()

+ SetIgnSignal()
+ SetWakeUpSignal()
+ SetBatteryVoltage()
+ ReceiveCanMessage()

+ GetState()

## 5. State Machine

상태 천이 및 각 상태의 상세 동작은 StateMachine.md 참조