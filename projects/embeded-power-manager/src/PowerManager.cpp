#include "PowerManager.h"

PowerManager::PowerManager()
{
    Init();
}

// 상태 초기화
void PowerManager::Init()
{
    currentState = PowerState::POWER_OFF;

    ignSignalOnOff = false;
    isWakeupRequested = false;
    batVoltage = 0.0f;
    isBatteryFault = false;
    isCanTimeout = false;
    isInternalFault = false;

    sleepTimer = 0;
    isInitFinished = false;

    isPeripheralPowerEnable = false;
    isCameraPowerEnable = false;

    canMessageId = CanMessageId::NONE;
}

// 상태 업데이트
void PowerManager::Update()
{
    ProcessCanMessage(); // CAN 메시지 처리

    switch (currentState)
    {
    case PowerState::POWER_OFF:
        isPeripheralPowerEnable = false;
        isCameraPowerEnable = false;

        if (ignSignalOnOff || isWakeupRequested)
        {
            currentState = PowerState::INIT;
        }
        break;
    case PowerState::INIT:
        isPeripheralPowerEnable = false;
        isCameraPowerEnable = false;

        if (isInitFinished)
        {
            currentState = PowerState::ACTIVE;
            isInitFinished = false; // 초기화 완료 후 플래그 초기화
        }
        break;
    case PowerState::ACTIVE:
        isPeripheralPowerEnable = true; // 주변 장치 전원 활성화
        isCameraPowerEnable = true;     // 카메라 전원 활성화

        if (isBatteryFault || isCanTimeout || isInternalFault)
        {
            currentState = PowerState::FAULT;
        }
        else if (batVoltage >= BATTERY_FAULT_THRESHOLD && batVoltage < LOW_BATTERY_THRESHOLD)
        {
            currentState = PowerState::LOW_POWER;
        }
        else if (!ignSignalOnOff)
        {
            currentState = PowerState::SLEEP;
            sleepTimer = 0;
        }
        break;
    case PowerState::LOW_POWER:
        isPeripheralPowerEnable = true;
        isCameraPowerEnable = false;

        if (isBatteryFault || isCanTimeout || isInternalFault)
        {
            currentState = PowerState::FAULT;
        }
        else if (batVoltage >= LOW_BATTERY_THRESHOLD)
        {
            currentState = PowerState::ACTIVE;
        }
        break;
    case PowerState::SLEEP:
        isPeripheralPowerEnable = false;
        isCameraPowerEnable = false;

        // GPIO, Wake Pin 등의 H/W Wake up 신호 감지 시 INIT 상태로 전환
        if (isWakeupRequested || ignSignalOnOff)
        {
            currentState = PowerState::INIT;
            sleepTimer = 0;

            isWakeupRequested = false; // Wakeup 신호 처리 후 초기화
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
        isPeripheralPowerEnable = false;
        isCameraPowerEnable = false;
        break;
    default:
        break;
    }
}

// IGN 신호 설정
void PowerManager::SetIgnSignal(bool on)
{
    ignSignalOnOff = on;
}

// WAKEUP 신호 설정
void PowerManager::SetWakeupSignal(bool on)
{
    isWakeupRequested = on;
}

// 배터리 전압 설정
void PowerManager::SetBatteryVoltage(float voltage)
{
    batVoltage = voltage;

    if(batVoltage < BATTERY_FAULT_THRESHOLD)
    {
        isBatteryFault = true;
    }
}

// CAN Timeout 설정
void PowerManager::SetCanTimeout(bool timeout)
{
    isCanTimeout = timeout;
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
    isInternalFault = fault;
}

// 내부 진단 해제
void PowerManager::ClearFault()
{
    isBatteryFault = false;
    isCanTimeout = false;
    isInternalFault = false;
}

// 현재 전원 상태 반환
PowerState PowerManager::GetPowerState() const
{
    return currentState;
}

// 초기화 완료 설정
void PowerManager::SetInitFinished(bool finished)
{
    isInitFinished = finished;
}

// 주변 장치 제어 상태 반환
bool PowerManager::GetPeripheralPowerEnable() const
{
    return isPeripheralPowerEnable;
}

// 카메라 전원 제어 상태 반환(일부 장치 제한용)
bool PowerManager::GetCameraPowerEnable() const
{
    return isCameraPowerEnable;
}

float PowerManager::GetBatteryVoltage() const
{
    return batVoltage;
}

// 전원 상태 문자열 반환
const char *PowerManager::GetPowerStateString() const
{
    const char* curState = "UNKNOWN";

    switch (currentState)
    {
    case PowerState::POWER_OFF:
        curState = "POWER_OFF";
        break;  
    case PowerState::INIT:
        curState = "INIT";
        break;
    case PowerState::ACTIVE:
        curState = "ACTIVE";
        break;      
    case PowerState::LOW_POWER:
        curState = "LOW_POWER";
        break;
    case PowerState::SLEEP:
        curState = "SLEEP";
        break;
    case PowerState::FAULT:
        curState = "FAULT";
        break;
    default:
        curState = "UNKNOWN";
        break;
    }
    return curState;
}
