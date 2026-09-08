//상태 관련 헤더파일

enum class PowerState {
    POWER_OFF,
    INIT,
    ACTIVE,
    LOW_POWER,
    SLEEP,
    FAULT
};

enum class CanMessageId : uint32_t 
{
    NONE                = 0x000,  
    WAKEUP_REQUEST      = 0x100,
    SLEEP_REQUEST       = 0x101,
    RESET_FAULT         = 0x102
};