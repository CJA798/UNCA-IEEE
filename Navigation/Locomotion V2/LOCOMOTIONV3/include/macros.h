// Limit Switch Input Pins
//     vvvvv- BOT MICROSWITCH PIN OUT
#define FRONT_RIGHT_SWITCH 30     //     _______________________
#define FRONT_LEFT_SWITCH 31      //     |  31             30  |
#define RIGHTSIDE_RIGHT_SWITCH 28 //     |32                 29|
#define RIGHTSIDE_LEFT_SWITCH 29  //     |                     |
#define LEFTSIDE_RIGHT_SWITCH 32  //     |                     |
#define LEFTSIDE_LEFT_SWITCH 25   //     |25                 28|
#define BACK_RIGHT_SWITCH 27      //     |__26_____________27__|
#define BACK_LEFT_SWTICH 26       //
#define DEBOUNCE_TIME 10
/// TOF SENSOR/////////////////////////////////////////////////////////////////////
#define IRQ_PIN 2
#define RESET_TIME 100
// Temp pins until we decide actual pins for the shutdown of all the sensors
// These are for the big sensors L1s
#define XSHUT_PIN1 33
#define XSHUT_PIN2 35
#define XSHUT_PIN3 37
#define XSHUT_PIN4 39

// These are for the small sensors L0s
#define SHT_LOX1 3
#define SHT_LOX2 4
#define SHT_LOX3 9
#define SHT_LOX4 29

// Addresses for the sensors
#define L1X1_ADDRESS 0x29
#define L1X2_ADDRESS 0x2B
#define L1X3_ADDRESS 0x2D
#define L1X4_ADDRESS 0x2F
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x35
#define LOX4_ADDRESS 0x37

#define MTR1STEP 43
#define MTR1DIR 42
#define MTR2STEP 51
#define MTR2DIR 46
#define MTR3STEP 45
#define MTR3DIR 44
#define MTR4STEP 53
#define MTR4DIR 52
#define STORAGE_MTRSTEP 40
#define STORAGE_MTRDIR 41

#define BN0_RST 25
#define BNO_ADR 24

#define mtx_type double            // Matrix type definition
#define ONE_OVR_RT_2 (0.707106)    // (1/sqrt(2))
#define STEPS_PER_ROTATION (3200)  // (200steps per rotation) * (16 microsteps per step)
#define STEPS_PER_CM (25.46)       // Steps per inch in arclength any given wheel travels = StepsPerWheelRotation/WheelCircumference
#define WHEEL_RADIUS (2.5)         // Radius of wheel(cm) (1in)
#define STEPS_PER_BOT_RAD (859.44) // = ((BotCircumference*StepsPerCm*)/(2*Pi))*3 (gear ratio)
#define STEPS_PER_DIST_MULT (27)   // = (1/sqrt(2))*StepsPerCM*3 (gear ratio)

// Stepper Settings
#define MAX_MTR_SPEED (7500) // Steps/s maximum is 300000
#define MAX_MTR_ACCEL (50)   // Steps/s^2 skips at around 15000
