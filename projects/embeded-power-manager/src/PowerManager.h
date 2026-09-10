#ifndef POWERMANAGER_H
#define POWERMANAGER_H

#include <cstdint>
#include "State.h"

class PowerManager {
    public:
        PowerManager();

        void Update();
        
        void SetIgnSignal(bool on);
        void SetWakeupSignal(bool on);
        void SetBatteryVoltage(float voltage);
        void SetCanTimeout(bool timeout);
        void RecvCanMessage(CanMessageId messageId);
        void ProcessCanMessage();
        void SetInternalFault(bool fault);
        void ClearFault();
        
        PowerState GetPowerState() const;

        void SetInitFinished(bool finished);

        bool GetPeripheralPowerEnable() const;
        bool GetCameraPowerEnable() const;
        
    private:
        void Init();

        //시스템 전원 상태 관리 변수
        PowerState currentState;

        bool ignSignalOnOff;
        bool isWakeupRequested;

        float batVoltage;

        bool isBatteryFault;
        bool isCanTimeout;
        bool isInternalFault;

        uint32_t sleepTimer;

        bool isInitFinished;

        // 주변 장치 제어 변수
        bool isPeripheralPowerEnable;
        bool isCameraPowerEnable;

        // CAN message ID
        CanMessageId canMessageId;

        //타이머 및 시간 관련 상수
        static constexpr uint32_t TASK_PERIOD_MS = 10;          //10ms
        static constexpr uint32_t SLEEP_TIMEOUT_MS = 30*1000;   //30초

        //배터리 전압 관련 상수
        static constexpr float BATTERY_FAULT_THRESHOLD  = 10.0f; //배터리 전압 임계값
        static constexpr float LOW_BATTERY_THRESHOLD  = 11.5f; //배터리 전압 회복 임계값
};

#endif // POWERMANAGER_H