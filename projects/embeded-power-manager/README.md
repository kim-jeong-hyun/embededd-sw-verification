# Embedded Power Manager

## OverView

Embedded Power Manager는 입력 조건을 기반으로 시스템의 전원 상태를 관리하고 안전하게 상태를 전환하고 Fault 발생 시 보호 및 복구를 수행하는 소프트웨어이다.

본 프로젝트의 목적은 소프트웨어 구현이 주 목적이 아니라 실제 임베디드 개발 프로세스를 기반으로 

- Requirement Specification
- Software Design
- Source Code
- Unit Test
- Traceability
- Coverage
- CI/CD

전체 개발 및 검증 과정을 경험하기 위함이다다.

---

## Features

### Input
-  Digital 신호
    - IGN Signal 
    - Wake up Signal
- Analog 신호
    - Battery Voltage 
- Communication
    - CAN Message
- Internal
    - Fault Flag
    - Timer

Input
   │
   ▼
Embedded Power Manager
   │
   ▼
Output

### Output
- System State
- Peripheral Power Enable
- Sleep Request
- Diagnostic Status

---

## System State
- POWER OFF 
- INIT 
- ACTIVE
- LOW POWER 
- SLEEP 
- FAULT 

---

## Directory Structure

```text
requirements/   # 요구사항
design/         # 설계
src/            # 소스 코드
test/           # 단위 테스트
docs/           # 기타 문서
traceability/   # 요구사항 추적성
```

---

## Development Status

- [x] Project Planning
- [x] Repository Struture
- [ ] Requirement Specification
- [ ] Software Design
- [ ] Implementation
- [ ] Unit Test
- [ ] Traceability
- [ ] Coverage
- [ ] CI/CD