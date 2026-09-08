#ifndef POWERMANAGER_H
#define POWERMANAGER_H

#include <cstdint>
#include "State.h"

class PowerManager {
    public:
        PowerManager();

        void Init();
        void Update();

        void SetIgnSignal(bool on);
        void SetWakeupSignal(bool on);
        void SetBatteryVoltage(float voltage);
        void RecvCanMessage(uint32_t messageId);

        void SetInternalFault(bool fault);
        void ClearFault();

        PowerState GetPowerState() const;

        void SetInitFinished(bool finished);

        bool GetPeripheralPowerEnable() const;
        
    private:
        //시스템 전원 상태 관리 변수
        PowerState currentState;

        bool ignSignal;
        bool wakeupSignal;

        float batVoltage;

        bool batteryFault;
        bool canTimeout;
        bool internalFault;

        uint32_t sleepTimer;

        bool initFinished;

        // 주변 장치 제어 변수
        bool peripheralPowerEnable;
        bool cameraPowerEnable;

        //타이머 및 시간 관련 상수
        static constexpr uint32_t TASK_PERIOD_MS = 10;          //10ms
        static constexpr uint32_t SLEEP_TIMEOUT_MS = 30*1000;   //30초
};

#endif // POWERMANAGER_H