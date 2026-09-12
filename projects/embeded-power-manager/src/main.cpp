#include <iostream>
#include <thread>
#include <chrono>

#include "PowerManager.h"

// 시뮬레이션 입력 함수
void SimulationInput(PowerManager &pm, uint32_t tick)
{
    switch (tick)
    {
    case 10:
        // IGN ON
        pm.SetIgnSignal(true);
        break;
    case 20:
        // Initalization complete
        pm.SetInitFinished(true);
        break;
    case 100:
        // battrery low
        pm.SetBatteryVoltage(10.8f);
        break;
    case 200:
        // battery fault
        pm.SetBatteryVoltage(9.9f);
        break;
    case 300:
        // reset fault
        pm.RecvCanMessage(CanMessageId::RESET_FAULT);
        break;
    case 310:
        // Initialization complete
        pm.SetInitFinished(true);
        break;
    case 400:
        // IGN OFF
        pm.SetIgnSignal(false);
        break;
    case 3500:
        // Wakeup Signal ON
        pm.SetWakeupSignal(true);
        break;
    case 3510:
        // Wakeup Signal OFF
        pm.SetWakeupSignal(false);
        break;
    case 3520:
        pm.SetInitFinished(true);
        break;
    default:
        break;
    }
}

int main()
{
    PowerManager pm;

    uint32_t tick = 0;

    PowerState prevState = pm.GetPowerState();

    // 60초만 실행하고 종료
    while (tick <= 6000)
    {
        SimulationInput(pm, tick);

        pm.UpdateState();
        pm.UpdateOutputs();

        if (prevState != pm.GetPowerState())
        {
            std::cout
                << "[Tick: " << tick << "] "
                << pm.GetPowerStateString()
                << " | Battery Voltage: " << pm.GetBatteryVoltage()
                << " | Peripheral Power: " << std::boolalpha << pm.GetPeripheralPowerEnable()
                << " | Camera : " << std::boolalpha << pm.GetCameraPowerEnable()
                << std::endl;
            prevState = pm.GetPowerState();
        }
        //메인 스레드 10ms 대기
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        tick++;
    }

    return 0;
}