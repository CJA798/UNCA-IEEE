#include <macros.h>
#include <Bounce2.h>
#include <math.h>
#include "Adafruit_VL53L1X.h"
#include "Adafruit_VL53L0X.h"
// #include <Adafruit_I2CDevice.h>
// #include <Adafruit_Sensor.h>
#include <Wire.h>
#define X_MAX (122)
#define Y_MAX (122)
#define X_MIN (-122)
#define Y_MIN (0)
#define FRONT_PRESSED (1)
#define BACK_PRESSED (2)
#define RIGHT_PRESSED (3)
#define LEFT_PRESSED (4)
/* The Libraries for the magnetometer/gyroscope cause errors.

#include <Adafruit_SPIDevice.h>

#include <Adafruit_BNO055.h>
*/
Bounce FrontLeftSw;
Bounce FrontRightSw;
Bounce BackRightSw;
Bounce BackLeftSw;
Bounce RightSideRightSw;
Bounce RightSideLeftSw;
Bounce LeftSideRightSw;
Bounce LeftSideLeftSw;
bool SwitchesState[8] = {0};
bool ChangeInSwitchState = 0;

class BumperSwitches
{ // This class will contain the functions to read the construct the bumpers switches, read from the switches, and set flags for when the X_min, X_max, Y_min, Y_max bounds are reached.

private:
public:
    BumperSwitches()
    {
        FrontLeftSw = Bounce();
        FrontRightSw = Bounce();
        BackRightSw = Bounce();
        BackLeftSw = Bounce();
        RightSideRightSw = Bounce();
        RightSideLeftSw = Bounce();
        LeftSideRightSw = Bounce();
        LeftSideLeftSw = Bounce();

        pinMode(FRONT_LEFT_SWITCH, INPUT_PULLUP);
        pinMode(FRONT_RIGHT_SWITCH, INPUT_PULLUP);
        pinMode(RIGHTSIDE_LEFT_SWITCH, INPUT_PULLUP);
        pinMode(RIGHTSIDE_RIGHT_SWITCH, INPUT_PULLUP);
        pinMode(LEFTSIDE_LEFT_SWITCH, INPUT_PULLUP);
        pinMode(RIGHTSIDE_RIGHT_SWITCH, INPUT_PULLUP);
        pinMode(BACK_RIGHT_SWITCH, INPUT_PULLUP);
        pinMode(BACK_LEFT_SWTICH, INPUT_PULLUP);
        FrontRightSw.attach(FRONT_RIGHT_SWITCH); // 1
        FrontRightSw.interval(DEBOUNCE_TIME);
        FrontLeftSw.attach(FRONT_LEFT_SWITCH); // 2
        FrontLeftSw.interval(DEBOUNCE_TIME);
        BackRightSw.attach(BACK_RIGHT_SWITCH); // 3
        BackRightSw.interval(DEBOUNCE_TIME);
        BackLeftSw.attach(BACK_LEFT_SWTICH); // 3
        BackLeftSw.interval(DEBOUNCE_TIME);
        RightSideRightSw.attach(RIGHTSIDE_RIGHT_SWITCH); // 4
        RightSideRightSw.interval(DEBOUNCE_TIME);
        RightSideLeftSw.attach(RIGHTSIDE_LEFT_SWITCH); // 5
        RightSideLeftSw.interval(DEBOUNCE_TIME);
        LeftSideRightSw.attach(LEFTSIDE_RIGHT_SWITCH); // 6
        LeftSideRightSw.interval(DEBOUNCE_TIME);
        LeftSideLeftSw.attach(LEFTSIDE_LEFT_SWITCH); // 6
        LeftSideLeftSw.interval(DEBOUNCE_TIME);
    }

    void SwitchesProcess(void)
    {
        // Serial.println("SwitchesProcess");
        FrontRightSw.update();     // 1
        FrontLeftSw.update();      // 2
        BackLeftSw.update();       // 3
        BackRightSw.update();      // 4
        RightSideLeftSw.update();  // 5
        RightSideRightSw.update(); // 6
        LeftSideLeftSw.update();   // 7
        LeftSideRightSw.update();  // 8
        SwitchesState[0] = LeftSideRightSw.read();
        SwitchesState[1] = FrontLeftSw.read();
        SwitchesState[2] = FrontRightSw.read();
        SwitchesState[3] = RightSideLeftSw.read();
        SwitchesState[4] = RightSideRightSw.read();
        SwitchesState[5] = BackRightSw.read();
        SwitchesState[6] = BackLeftSw.read();
        SwitchesState[7] = LeftSideLeftSw.read();

        for (int i = 0; i < 8; i++)
        {
            Serial.print(SwitchesState[i]);
            Serial.print("Switch: ");
            Serial.println(i);
        };
    };
    void CheckBumperState(char Side){
        // This function returns 0 if none of the switches are pressed, it returns 1 if the front is pressed, and so on.
        // The position  of the bot is accessed via ThetaXY[3][1];
        // To check that a switch is really "pressed", we will check if we are within SWITCH_THRESHOLD of X_MAX, X_MIN, Y_MAX or Y_MIN;
        // as well as if both switches on that side are pressed.

    };
};

class TOFArray
{
private:
    /*  SMALL TOF SENSORS NEED TO BE IMPLEMENTED
        typedef struct
        {
            Adafruit_VL53L0X *psensor; // pointer to object
            TwoWire *pwire;
            int id;            // id for the sensor
            int shutdown_pin;  // which pin for shutdown;
            int interrupt_pin; // which pin to use for interrupts.
            Adafruit_VL53L0X::VL53L0X_Sense_config_t
                sensor_config;     // options for how to use the sensor
            uint16_t range;        // range value used in continuous mode stuff.
            uint8_t sensor_status; // status from last ranging in continuous.
        } sensorList_t;


        Adafruit_VL53L0X SmallTOF_Front;
        Adafruit_VL53L0X SmallTOF_Back;
        Adafruit_VL53L0X SmallTOF_Right;
        Adafruit_VL53L0X SmallTOF_Left;
    */
    Adafruit_VL53L1X BigTOF_Front;
    Adafruit_VL53L1X BigTOF_Back;
    Adafruit_VL53L1X BigTOF_Right;
    Adafruit_VL53L1X BigTOF_Left;

    uint16_t FrontTOF = 0;
    uint16_t BackTOF = 0;
    uint16_t RightTOF = 0;
    uint16_t LeftTOF = 0;

public:
    TOFArray()
    {
        Wire.begin();
        BigTOF_Front = Adafruit_VL53L1X(XSHUT_PIN1);
        BigTOF_Back = Adafruit_VL53L1X(XSHUT_PIN2);
        BigTOF_Right = Adafruit_VL53L1X(XSHUT_PIN3);
        BigTOF_Left = Adafruit_VL53L1X(XSHUT_PIN4);
        /*
                SmallTOF_Front = Adafruit_VL53L0X();
                SmallTOF_Back = Adafruit_VL53L0X();
                SmallTOF_Right = Adafruit_VL53L0X();
                SmallTOF_Left = Adafruit_VL53L0X();
        */
        // SMALL TOF SENSORS NEED TO BE IMPLEMENTED
    }
    void TOF_InitBig(void)
    {

        // INITIALIZATION OF BIG TOF SENSORS
        Serial.println("TOF_Init");
        if (!BigTOF_Front.begin(L1X1_ADDRESS, &Wire))
        {
            Serial.println("Failed to boot front TOF");
            while (1)
                delay(2000);
            Serial.println("Failed to boot front TOF");
        };
        if (!BigTOF_Back.begin(L1X2_ADDRESS, &Wire))
        {
            Serial.println("Failed to boot back TOF");
            while (1)
                delay(2000);
            Serial.println("Failed to boot back TOF");
        };
        if (!BigTOF_Right.begin(L1X3_ADDRESS, &Wire))
        {
            Serial.println("Failed to boot right TOF");
            while (1)
                delay(2000);
            Serial.println("Failed to boot right TOF");
        };
        if (!BigTOF_Left.begin(L1X4_ADDRESS, &Wire))
        {
            Serial.println("Failed to boot left TOF");
            while (1)
                delay(2000);
            Serial.println("Failed to boot left TOF");
        };
        Serial.println("TOF_Init Done");
    }

    void TOF_InitSmall(void)
    {
        // INITIALIZATION OF SMALL TOF SENSORS
        // NEED TO BE IMPLEMENTED
    }
    void TOF_Process(void)
    {
        Serial.println("TOF_Process");

        FrontTOF = BigTOF_Front.distance();
        BackTOF = BigTOF_Back.distance();
        RightTOF = BigTOF_Right.distance();
        LeftTOF = BigTOF_Left.distance();
        Serial.print("FrontTOF: ");
        Serial.println(FrontTOF);
        Serial.print("BackTOF: ");
        Serial.println(BackTOF);
        Serial.print("RightTOF: ");
        Serial.println(RightTOF);
        Serial.print("LeftTOF: ");
        Serial.println(LeftTOF);
    }
};
