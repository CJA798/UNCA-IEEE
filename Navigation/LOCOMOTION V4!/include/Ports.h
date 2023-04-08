//-----------PORTS----------------------------------------------------------------
// Limit Switch Input Pins
//                                     v- BOT MICROSWITCH PIN OUT-v
#define FRONT_RIGHT_SWITCH 30     //     _______________________
#define FRONT_LEFT_SWITCH 31      //     |  31    FRONT    30  |
#define RIGHTSIDE_RIGHT_SWITCH 28 //     |32                 29|
#define RIGHTSIDE_LEFT_SWITCH 29  //     |                     |
#define LEFTSIDE_RIGHT_SWITCH 32  //     |                     |
#define LEFTSIDE_LEFT_SWITCH 25   //     |25                 28|
#define BACK_RIGHT_SWITCH 27      //     |__26_____________27__|
#define BACK_LEFT_SWTICH 26       //
#define DEBOUNCE_TIME 10

/// TOF SENSORS
#define IRQ_PIN 2
// Temp pins until we decide actual pins for the shutdown of all the sensors
// These are for the big sensors L1s
#define XSHUT_PIN1 12 // may need updating
#define XSHUT_PIN2 11
#define XSHUT_PIN3 10
#define XSHUT_PIN4 9
// These are for the small sensors L0s
#define SHT_LOX1 20
#define SHT_LOX2 21
#define SHT_LOX3 22
#define SHT_LOX4 23

// STEPPER MOTOR PINOUT
#define MTR1STEP (43)
#define MTR1DIR (42)
#define MTR2STEP (51)
#define MTR2DIR (46)
#define MTR3STEP (45)
#define MTR3DIR (44)
#define MTR4STEP (53)
#define MTR4DIR (52)
#define STORAGE_MTRSTEP (40)
#define STORAGE_MTRDIR (41)

// MAGNETOMETER-GYROSCOPE
#define BN0_RST 25
#define BNO_ADR 24
