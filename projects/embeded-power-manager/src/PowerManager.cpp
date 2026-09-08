#include "PowerManager.h"

PowerManager::PowerManager()
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
}

// 상태 초기화
void PowerManager::Init()
{
    currentState = PowerState::INIT;
}

// 상태 업데이트
void PowerManager::Update()
{
    switch (currentState)
    {
    case PowerState::POWER_OFF:
        if (ignSignal)
        {
            currentState = PowerState::INIT;
        }
        break;
    case PowerState::INIT:
        if (initFinished)
        {
            currentState = PowerState::ACTIVE;
            initFinished = false; // 초기화 완료 후 플래그 초기화
        }
        break;
    case PowerState::ACTIVE:
        if (batVoltage >= 10.0f && batVoltage < 11.5f)
        {
            currentState = PowerState::LOW_POWER;
        }
        else if (!ignSignal)
        {
            currentState = PowerState::SLEEP;
        }
        break;
    case PowerState::LOW_POWER:
        if (batVoltage >= 11.5f)
        {
            currentState = PowerState::ACTIVE;
        }
        break;
    case PowerState::SLEEP:
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
void PowerManager::RecvCanMessage(uint32_t messageId)
{
    // CAN 메시지 처리 로직 구현
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
