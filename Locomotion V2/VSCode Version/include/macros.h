
#define mtx_type double                // Matrix type definition
#define ONE_OVR_RT_2 (0.707106)        // (1/sqrt(2))
#define STEPS_PER_ROTATION (3200)      // (200steps per rotation) * (16 microsteps per step)
#define STEPS_PER_CM (203.718327)      // Steps per inch in arclength any given wheel travels = StepsPerWheelRotation/WheelCircumference
#define WHEEL_RADIUS (2.5)             // Radius of wheel(cm) (1in)
#define STEPS_PER_BOT_RAD (13750.98)   // = ((BotCircumference*StepsPerCm*)/(2*Pi))*3 (gear ratio)
#define STEPS_PER_DIST_MULT (432.1518) // = (1/sqrt(2))*StepsPerCM*3 (gear ratio)

// Stepper Settings
#define MAX_MTR_SPEED (750000) // Steps/s maximum is 300000
#define MAX_MTR_ACCEL (7500)   // Steps/s^2 skips at around 15000

#define IRQ_PIN1 (2)
#define XSHUT_PIN1 (1)

//#define X_STP (14) // A0 from mega
//#define X_DIR (13)  // A1 "   "
/*
#define Y_EN (12)   // A2
#define Y_STEP (11) // A6
#define Y_DIR (10)  // A7
#define Z_EN ()    // A8
#define Z_STEP 8)  // D46
#define Z_DIR 7   // D48
*/
/*
#define E1_STEP 5 // D36
#define E1_DIR 4  // D34
#define E1_EN 3   // D30
#define E0_DIR 2  // D28
#define E0_STEP 1 // D26
#define E0_EN 0   // D24
*/