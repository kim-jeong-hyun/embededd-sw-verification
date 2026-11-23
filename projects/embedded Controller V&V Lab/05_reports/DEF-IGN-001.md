# 결함 보고서 — DEF-IGN-001

가상 관찰 데이터를 기반으로 작성했으며, 실제 SW 실행 결과가 아니다.

| 항목 | 내용 |
|---|---|
| 문서 ID | ECVV-DEF-IGN-001 |
| 버전 | 1.2 |
| 날짜 | 2026-11-23 |
| 승인 상태 | 문서 리뷰 승인 |
| 작성 | 학습자 작성, Codex 리뷰 및 Markdown 정리 |

## 1. 결함 보고

| 항목 | 작성 내용 |
|---|---|
| 결함 ID | DEF-IGN-001 |
| 제목 | PWR_REQ의 DISABLE 출력값이 1,000 ms 동안 유지되지 않음. |
| 관련 요구사항·TC ID | SWR-IGN-002 / [TC-SWR-IGN-002](../03_test_design/SWE6_TC.md#tc-swr-ign-002) |
| 기준 문서 | [SW-INPUT-001 v1.0](../01_requirements/SWE6_IGN_Requirements.md) |
| 사전 조건 | IGN = ON & PWR_REQ = ENABLE. 공통 시험 전제는 관련 TC의 사전 조건을 참조한다. |
| 재현 절차 | IGN을 OFF로 변경한 시점을 t0로 정하고, t0+1,000 ms까지 OFF를 유지하며 출력을 1 ms 주기로 관찰한다. |
| 기대 결과 | t0+100 ms 이내(100 ms 포함)에 DISABLE로 전환한 후, 시험 종료 시점인 t0+1,000 ms까지 DISABLE을 유지한다. |
| 실제 결과 | DEF-IGN-001 관찰 데이터에서 t0+80 ms에 DISABLE로 전환한 후 t0+500 ms에 ENABLE이 관찰됐으며, t0+501 ms부터 다시 DISABLE이 유지됐다. |
| 요구사항 위반 근거 | IGN이 OFF인 동안 ENABLE이 관찰됐다. |
| 예상 영향 | t0+500 ms에 관찰된 ENABLE 요청으로 인해 불필요한 전원 공급 및 전력 소모가 발생할 가능성이 있음. 실제 장치 전원은 미확인. |
| 증거·추가 확보할 정보 | 현재 자료: 아래 DEF-IGN-001 관찰 데이터.<br>추가 확보 필요: 대상 SW 버전, IGN 및 PWR_REQ를 같은 시간 기준으로 비교할 수 있는 로그. |

## 2. DEF-IGN-001 관찰 데이터

출처는 대화에서 제공된 관찰 데이터다. IGN은 t0에 ON에서 OFF로 변경되어 t0+1,000 ms까지 OFF를 유지하고, 출력은 1 ms 주기로 기록한 것으로 가정한다.

| 데이터명 | PWR_REQ 관찰 결과 | 응답 조건 판정 | 유지 조건 판정 | 최종 판정 |
|---|---|---|---|---|
| DEF-IGN-001 관찰 데이터 | t0+80 ms에 DISABLE로 전환, t0+500 ms에 ENABLE 관찰, t0+501 ms부터 t0+1,000 ms까지 다시 DISABLE 유지 | Pass — 100 ms 이내 전환 | Fail — IGN OFF 유지 중 ENABLE 관찰 | Fail — 유지 조건 불만족 |

- 1,000 ms는 시험자가 정한 관찰 구간이며 요구사항에 정해진 유지 시간이 아니다.
- 500 ms와 501 ms의 관찰값만으로 그 사이 전체의 출력값이나 실제 전원 공급 여부를 단정하지 않는다.
- 원인, 수정 방법, 실제 재현 여부는 확인되지 않았다.
- 문서 리뷰 승인과 TC 실행 판정은 구분한다. 실제 TC 실행 판정은 Not Run을 유지한다.

## 3. 재검증 계획

### 변경 정보와 시작 조건

계획 수립을 위한 변경 정보는 “IGN ON/OFF 처리에서 함께 사용하는 PWR_REQ 출력 갱신 로직을 수정했다”는 가정이다. 실제 수정 코드나 SW는 제공되지 않았다.

재검증 시작 전 수정된 SW 버전을 전달받고, 반영 이슈와 변경 내역을 확보하여 DEF-IGN-001 및 공통 출력 갱신 로직의 변경 범위를 확인한다.

### 검증 범위

| 구분 | 선정할 기존 TC | 선정 이유 | 확인할 조건 |
|---|---|---|---|
| 결함 수정 확인 | [TC-SWR-IGN-002](../03_test_design/SWE6_TC.md#tc-swr-ign-002) | PWR_REQ DISABLE 출력 유지에 대한 결함이 있었음. | 1. 100 ms 이내 PWR_REQ가 DISABLE로 전환.<br>2. PWR_REQ가 DISABLE로 전환된 후 t0+1,000 ms까지 DISABLE 출력 유지. |
| 변경에 따른 회귀 검증 | [TC-SWR-IGN-001](../03_test_design/SWE6_TC.md#tc-swr-ign-001)<br>[TC-SWR-IGN-002](../03_test_design/SWE6_TC.md#tc-swr-ign-002) | IGN ON/OFF 처리에서 PWR_REQ 출력 갱신 로직을 수정했기 때문에, IGN ON인 경우에도 PWR_REQ가 ENABLE을 유지하는지 확인해야 함. | 1. IGN ON인 경우 100 ms 이내 PWR_REQ가 ENABLE로 전환된 후 t0+10,000 ms까지 유지.<br>2. t0+10,000 ms까지 IGN ON 상태 유지.<br>3. IGN OFF인 경우 100 ms 이내 PWR_REQ가 DISABLE로 전환된 후 t0+1,000 ms까지 유지.<br>4. t0+1,000 ms까지 IGN OFF 상태 유지. |

ON 시험과 OFF 시험은 각각 해당 입력 변경이 관찰된 시점을 t0로 사용한다. 100 ms 이내는 100 ms를 포함하며, 관찰 주기와 사전 조건은 각 TC를 따른다.

### 상태

- 재검증 계획 리뷰: 승인.
- 실행 상태: Not Run.
- 수정 완료 여부: 실행 증거 미확보로 판단하지 않음. 개발자의 수정 전달이나 계획 승인만으로 결함을 종료하지 않는다.
