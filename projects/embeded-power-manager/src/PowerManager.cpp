#include "PowerManager.h"

PowerManager::PowerManager()
{
    Init();
}

// 상태 초기화
void PowerManager::Init()
{
    currentState = PowerState::POWER_OFF;

    ignSignal = false;
    wakeupSignal = false;
    batVoltage = 0.0f;
    batteryFault = false;
    canTimeout = false;
    internalFault = false;

    sleepTimer = 0;
    initFinished = false;

    peripheralPowerEnable = false;
    cameraPowerEnable = false;

    canMessageId = CanMessageId::NONE;
}

// 상태 업데이트
void PowerManager::Update()
{
    ProcessCanMessage(); // CAN 메시지 처리

    switch (currentState)
    {
    case PowerState::POWER_OFF:
        peripheralPowerEnable = false;
        cameraPowerEnable = false;

        if (ignSignal)
        {
            currentState = PowerState::INIT;
        }
        break;
    case PowerState::INIT:
        peripheralPowerEnable = false;
        cameraPowerEnable = false;

        if (initFinished)
        {
            currentState = PowerState::ACTIVE;
            initFinished = false; // 초기화 완료 후 플래그 초기화
        }
        break;
    case PowerState::ACTIVE:
        peripheralPowerEnable = true; // 주변 장치 전원 활성화
        cameraPowerEnable = true;     // 카메라 전원 활성화

        if (internalFault)
        {
            currentState = PowerState::FAULT;        
        }
        else if (batVoltage >= 10.0f && batVoltage < 11.5f)
        {
            currentState = PowerState::LOW_POWER;
        }
        else if (!ignSignal)
        {
            currentState = PowerState::SLEEP;
            sleepTimer = 0;
        }
        break;
    case PowerState::LOW_POWER:
        peripheralPowerEnable = true;
        cameraPowerEnable = false;

        if (batVoltage >= 11.5f)
        {
            currentState = PowerState::ACTIVE;
        }
        break;
    case PowerState::SLEEP:
        peripheralPowerEnable = false;
        cameraPowerEnable = false;
        // GPIO, Wake Pin 등의 H/W Wake up 신호 감지 시 INIT 상태로 전환
        if (wakeupSignal || ignSignal)
        {
            currentState = PowerState::INIT;
            sleepTimer = 0;
        }
        else
        {
            sleepTimer += TASK_PERIOD_MS;
            if (sleepTimer >= SLEEP_TIMEOUT_MS)
            {
                currentState = PowerState::POWER_OFF;
                sleepTimer = 0;
            }
        }
        break;
    case PowerState::FAULT:
        peripheralPowerEnable = false;
        cameraPowerEnable = false;
        break;
    default:
        break;
    }
}

// IGN 신호 설정
void PowerManager::SetIgnSignal(bool on)
{
    ignSignal = on;
}

// WAKEUP 신호 설정
void PowerManager::SetWakeupSignal(bool on)
{
    wakeupSignal = on;
}

// 배터리 전압 설정
void PowerManager::SetBatteryVoltage(float voltage)
{
    batVoltage = voltage;
}

// CAN 메시지 수신
void PowerManager::RecvCanMessage(CanMessageId messageId)
{
    canMessageId = messageId;
}

// CAN 메시지 처리
void PowerManager::ProcessCanMessage()
{
    switch (canMessageId)
    {
    case CanMessageId::WAKEUP_REQUEST:
        // Wake up Signal | Can Wakeup Request INIT 상태로 전환
        if (currentState == PowerState::POWER_OFF || currentState == PowerState::SLEEP)
        {
            currentState = PowerState::INIT;
            sleepTimer = 0; 
        }
        break;
    case CanMessageId::SLEEP_REQUEST:
        break;
    case CanMessageId::RESET_FAULT:
        // Fault 조건 해제 시 INIT 상태로 전환
        if (currentState == PowerState::FAULT)
        {
            ClearFault();
            currentState = PowerState::INIT;
        }
        break;
    default:
        break;
    }
    canMessageId = CanMessageId::NONE; // 메시지 처리 후 초기화 
}

// 내부 진단 설정
void PowerManager::SetInternalFault(bool fault)
{
    internalFault = fault;
}

// 내부 진단 해제
void PowerManager::ClearFault()
{
    batteryFault = false;
    canTimeout = false;
    internalFault = false;
}

// 현재 전원 상태 반환
PowerState PowerManager::GetPowerState() const
{
    return currentState;
}

// 초기화 완료 설정
void PowerManager::SetInitFinished(bool finished)
{
    initFinished = finished;
}

// 주변 장치 제어 상태 반환
bool PowerManager::GetPeripheralPowerEnable() const
{
    return peripheralPowerEnable;
}

// 카메라 전원 제어 상태 반환(일부 장치 제한용)
bool PowerManager::GetCameraPowerEnable() const
{
    return cameraPowerEnable;
}
