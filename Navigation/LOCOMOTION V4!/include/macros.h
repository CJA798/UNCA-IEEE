// NO MAGIC NUMBERS!!!! KEEP CARLSON PROUD!!!!!!!!!
//--------STATES--------------------------------------------------------------------
#define IDLE (0)
#define TRANSLATING (1)
#define ROTATING (2)
#define COMPLEX_MOVE (3)

//--------ADDRESSES AND OTHER CONSTANTS---------------------------------------------
// Addresses for the sensors
#define L1X1_ADDRESS 0x29
#define L1X2_ADDRESS 0x2B
#define L1X3_ADDRESS 0x2D
#define L1X4_ADDRESS 0x2F
#define LOX1_ADDRESS 0x31
#define LOX2_ADDRESS 0x33
#define LOX3_ADDRESS 0x35
#define LOX4_ADDRESS 0x37

#define RESET_TIME 100
#define DEBOUNCE_TIME 100
//-------KINEMATICS CONSTANTS-------------------------------------------------------
#define ONE_OVR_RT_2 (0.707106)     // (1/sqrt(2))
#define STEPS_PER_ROTATION (400)    // (200steps per rotation) * (16 microsteps per step)
#define STEPS_PER_CM (25.46)        // Steps per inch in arclength any given wheel travels = StepsPerWheelRotation/WheelCircumference
#define WHEEL_RADIUS (2.5)          // Radius of wheel(cm) (1in)
#define STEPS_PER_BOT_RAD (1315)    // = ((BotCircumference*StepsPerCm*)/(2*Pi))*3 (gear ratio)
#define STEPS_PER_DIST_MULT (51.32) // = (1/sqrt(2))*StepsPerCM*3 (gear ratio) WAS 54.02; UPPED TO
#define MAX_MTR_SPEED (800)         // Steps/s maximum is 300000
#define MAX_MTR_ACCEL (400)         // Steps/s^2 skips at around 15000

#define NORTH ('N')
#define SOUTH ('S')
#define EAST ('E')
#define WEST ('W')
//-------Navigation Stuff------------------------------------------------------------
#define mtx_type float // Matrix type definition
#define BLOCKING (1)
#define NON_BLOCKING (0)
#define ROTATIONAL (1)
#define LINEAR (0)
#define NUDGE_DISTANCE 10
#define NUDGE_SPEED 50
#define NUDGE_ACCEL 1000
#define ROTATE_90_CW 1
#define ROTATE_180_CW 2
#define ROTATE_90_CCW 3
#define ROTATE_180_CCW 4
#define GOTO_NW_POS 5
#define GOTO_NE_POS 6
#define GOTO_SW_POS 7
#define GOTO_SE_POS 8
#define GOTO_POND_POS 9
#define DEGREE45 (0.785398)
#define DEGREE90 (1.570796)
#define DEGREE180 (3.141592)

// SWEEP STATE MACHINE DELAYS-----------------------------------------------
#define BOT_STOP_DELAY (150)
#define SERVO_DROP_DELAY (250)

#define NORTH_WALL (91)
#define SOUTH_WALL (0)   // prev 1
#define EAST_WALL (114)  // prev 112
#define WEST_WALL (-114) // prev 112
                         // SWEEEP CONSTANTS------
// Chip Dropper
#define CHIP_DROP_X (-105)
#define TOP_CHIP_DROP_Y (90)
#define BOTTOM_CHIP_DROP_Y (2)
#define CHIP_DROP_ZERO (90)
#define DROP_DIST (90)
#define SWEEP_COUNT (5)
#define SWEEP_DISTANCE_INCREMENT (10)
#define SWEEP_MAX_X (95)
#define SWEEP_START_X (-113)
#define SWEEP_START_Y (NORTH_WALL)

// Photoresistor
#define PHOTORESISTOR_PIN (22)
#define NUM_SAMPLES (50)
#define THRESHOLD (10000)
#define NEW_MOVE ('+')
#define WAITING (0)

#define LAST_CASE ('z')

// OE 57

#define RIGHT ('R')
#define LEFT ('L')
