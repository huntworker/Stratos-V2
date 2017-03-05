

#define RRC_BAT_REG_DESIGN_VOLTAGE  0x19
#define RRC_BAT_REG_DESIGN_CAP      0x18
#define RRC_BAT_REG_DESIGN_N_CYCLES 0x17
#define RRC_BAT_REG_DESIGN_FULL_CAP 0x10
#define RRC_BAT_REG_TEMPERATURE     0x08
#define RRC_BAT_REG_VOLTAGE         0x09
#define RRC_BAT_REG_CURRENT         0x0A
#define RRC_BAT_REG_AVG_CURRENT     0x0B
#define RRC_BAT_REG_REL_CHARGE      0x0D
#define RRC_BAT_REG_ABS_CHARGE      0x0E
#define RRC_BAT_REG_REM_CAP         0x0F
#define RRC_BAT_REG_TIME_TO_EMPTY   0x11
#define RRC_BAT_REG_AVG_TIME_TO_EMPTY  0x12
#define RRC_BAT_REG_BATTERY_STATUS  0x16



int16_t RRC_Bat_get(uint8_t reg);
