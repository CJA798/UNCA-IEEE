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
