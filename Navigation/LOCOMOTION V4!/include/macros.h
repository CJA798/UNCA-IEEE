// NO MAGIC NUMBERS!!!! KEEP CARLSON PROUD!!!!!!!!!
//--------STATES--------------------------------------------------------------------
#define IDLE (0)
#define TRANSLATING (1)
#define ROTATING (2)
#define COMPLEX_MOVE (3)


//--------ADDRESSES AND OTHER CONSTANTS---------------------------------------------
//Addresses for the sensors
#define L1X1_ADDRESS 0x29
#define L1X2_ADDRESS 0x2B
#define L1X3_ADDRESS 0x2D
#define L1X4_ADDRESS 0x2F
#define LOX1_ADDRESS 0x31
#define LOX2_ADDRESS 0x33
#define LOX3_ADDRESS 0x35
#define LOX4_ADDRESS 0x37

#define IRQ_PIN 2

//Temp pins until we decide actual pins for the shutdown of all the sensors
//These are for the big sensors L1s
#define XSHUT_PIN1 10
#define XSHUT_PIN2 12
#define XSHUT_PIN3 25
#define XSHUT_PIN4 28

//These are for the small sensors L0s
#define SHT_LOX1 9
#define SHT_LOX2 11
#define SHT_LOX3 26
#define SHT_LOX4 27

#define RESET_TIME 100

//-------KINEMATICS CONSTANTS-------------------------------------------------------
#define ONE_OVR_RT_2 (0.707106)     // (1/sqrt(2))
#define STEPS_PER_ROTATION (400)    // (200steps per rotation) * (16 microsteps per step)
#define STEPS_PER_CM (25.46)        // Steps per inch in arclength any given wheel travels = StepsPerWheelRotation/WheelCircumference
#define WHEEL_RADIUS (2.5)          // Radius of wheel(cm) (1in)
#define STEPS_PER_BOT_RAD (1315)    // = ((BotCircumference*StepsPerCm*)/(2*Pi))*3 (gear ratio)
#define STEPS_PER_DIST_MULT (51.32) // = (1/sqrt(2))*StepsPerCM*3 (gear ratio) WAS 54.02; UPPED TO
#define MAX_MTR_SPEED (800)        // Steps/s maximum is 300000
#define MAX_MTR_ACCEL (400)         // Steps/s^2 skips at around 15000

//-------Navigation Stuff------------------------------------------------------------
#define mtx_type float // Matrix type definition
#define BLOCKING (1)
#define NON_BLOCKING (0)
#define ROTATIONAL (1)
#define LINEAR (0)
#define MOTOR_SATURATION_SPEED (4000)
#define COMPLEX_MOVE_ACCEL (10000)
#define INPUT_VECT_PROPORTIONAL_RADUIS (1)
#define MOVE_TIME_PERIOD (200) // ms
#define PROPORTIONAL_DIVIDER (.1)