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
    private:
        PowerState currentState;

        bool ignSignal;
        bool wakeupSignal;

        float batVoltage;

        bool batteryFault;
        bool canTimeout;
        bool internalFault;

        uint32_t sleepTimer;

        bool initFinished;
};

#endif // POWERMANAGER_H