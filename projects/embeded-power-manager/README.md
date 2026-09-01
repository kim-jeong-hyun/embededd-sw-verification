# Embedded Power Manager

## OverView

Embedded Power Manager는 입력 조건을 기반으로 시스템의 전원 상태를 관리하고 안전하게 상태를 전환하고 Fault 발생 시 보호 및 복구를 수행하는 소프트웨어이다.

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

### Output
- System State
- Peripheral Power Enable
- Sleep Request
- Diagnostic Status

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
- [x] Repository Structure
- [ ] Requirement Specification
- [ ] Software Design
- [ ] Implementation
- [ ] Unit Test
- [ ] Traceability
- [ ] Coverage
- [ ] CI/CD
