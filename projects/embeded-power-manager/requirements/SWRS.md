# Software Requirements Specification

## 1. Overview

### 1.1 Purpose

- 이 문서는 Embedded Power Manager의 소프트웨어 요구사항 명세서이다.

### 1.2 Scope

Embedded Power Manager는 다음과 같은 기능을 수행한다.

1. 시스템 전원 상태 관리
2. 주변 장치 전원 제어
3. 시스템 상태 모니터링
4. Fault 보호 및 복구

---

## 2. Functional Requirements

### 2.1 시스템 전원 상태 관리

#### REQ-EPM-001
- IGN Signal이 OFF에서 ON으로 변경되면 본 시스템은 POWER OFF 상태에서 INIT 상태로 전환해야 한다.

#### REQ-EPM-002
- INIT 상태에서 초기화가 정상적으로 완료되면 ACTIVE 상태로 전환해야 한다.

#### REQ-EPM-003
- Battery Voltage가 10V 이상 11.5V 미만인 경우 LOW POWER 상태로 전환해야 한다.

#### REQ-EPM-004
- Battery Voltage가 11.5V 이상으로 복구되면 ACTIVE 상태로 전환해야 한다.

#### REQ-EPM-005
- IGN Signal이 ON에서 OFF로 변경되면 SLEEP 상태로 전환해야 한다.

#### REQ-EPM-006
- SLEEP 상태가 30초 동안 유지되면 POWER OFF 상태로 전환해야 한다.

#### REQ-EPM-007
- Wake up Signal이 감지되거나 CAN Wake up Request를 수신하면 SLEEP 상태에서 INIT 상태로 전환해야 한다.

---

### 2.2 주변 장치 전원 제어

#### REQ-EPM-008
- ACTIVE 상태에서는 주변 장치에 전원을 공급해야 한다.

#### REQ-EPM-009
- LOW POWER 상태에서는 주변 장치의 일부 기능을 제한하여 동작해야 한다.

#### REQ-EPM-010
- SLEEP 상태에서는 Wake up 기능을 제외한 주변 장치의 전원 공급을 차단해야 한다.

#### REQ-EPM-011
- POWER OFF 또는 FAULT 상태에서는 주변 장치의 전원 공급을 차단해야 한다.

---

### 2.3 시스템 상태 모니터링

#### REQ-EPM-012
- ACTIVE 상태에서는 Battery Voltage를 주기적으로 모니터링해야 한다.

#### REQ-EPM-013
- ACTIVE 상태에서는 수신된 CAN Message를 처리해야 한다.

#### REQ-EPM-014
- ACTIVE 상태에서는 Internal Fault Flag를 주기적으로 확인해야 한다.

---

### 2.4 Fault 보호 및 복구

#### REQ-EPM-015
- Battery Fault, CAN Timeout 또는 Internal Fault가 발생하면 본 시스템은 FAULT 상태로 전환해야 한다.

#### REQ-EPM-016
- Fault Reset 명령을 수신하고 Fault 조건이 해제되면 본 시스템은 INIT 상태로 전환해야 한다.