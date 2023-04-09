//-----------PORTS----------------------------------------------------------------
// Limit Switch Input Pins
//                                     v- BOT MICROSWITCH PIN OUT-v
#define FRONT_RIGHT_SWITCH 37     //     _______________________
#define FRONT_LEFT_SWITCH 27      //     | 27    FRONT     37  |
#define RIGHTSIDE_RIGHT_SWITCH 33 //     |28                 38|
#define RIGHTSIDE_LEFT_SWITCH 38  //     |                     |
#define LEFTSIDE_RIGHT_SWITCH 28  //     |                     |
#define LEFTSIDE_LEFT_SWITCH 31   //     |37                 33|
#define BACK_RIGHT_SWITCH 34      //     |__36_____________34__|
#define BACK_LEFT_SWTICH 32       //
#define DEBOUNCE_TIME 10
#define SCL_PIN 19
#define SDA_PIN 18

#define LIMIT_SWITCH_PIN (14)
// STEPPER MOTOR PINOUT
#define MTR1STEP (43)
#define MTR1DIR (42)
#define MTR2STEP (51)
#define MTR2DIR (46)
#define MTR3STEP (45)
#define MTR3DIR (44)
#define MTR4STEP (53)
#define MTR4DIR (52)
#define STORAGE_MTRSTEP (41)
#define STORAGE_MTRDIR (40)
#define IRQ_PIN 2

// Temp pins until we decide actual pins for the shutdown of all the sensors
// These are for the big sensors L1s
#define XSHUT_PIN1 25
#define XSHUT_PIN2 29
#define XSHUT_PIN3 36
#define XSHUT_PIN4 (A21)

// These are for the small sensors L0s
#define SHT_LOX1 26
#define SHT_LOX2 30
#define SHT_LOX3 35
#define SHT_LOX4 (A20)

// MAGNETOMETER-GYROSCOPE
#define BN0_RST 25
#define BNO_ADR 24
