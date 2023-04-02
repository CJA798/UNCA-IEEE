#include <macros.h>
#include <Bounce2.h>
#include <math.h>
#include "Adafruit_VL53L1X.h"
#include "Adafruit_VL53L0X.h"
// #include <Adafruit_I2CDevice.h>
// #include <Adafruit_Sensor.h>
#include <Wire.h>
/* The Libraries for the magnetometer/gyroscope cause errors.

#include <Adafruit_SPIDevice.h>

#include <Adafruit_BNO055.h>
*/



class BumperSwitches
{

private:
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
    bool WallActivated = false;
    bool CornerActivated = false;
    int WallLocation[8] ={front_wall ,right_wall ,left_wall ,back_wall, TOP_LEFT, TOP_RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT};
    int wall_or_corners = 0;

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
        SwitchesState[0] = FrontLeftSw.read();//LeftSideRightSw.read();
        SwitchesState[1] = FrontRightSw.read();//FrontLeftSw.read();
        SwitchesState[2] = LeftSideLeftSw.read();//FrontRightSw.read();
        SwitchesState[3] = LeftSideRightSw.read();//RightSideLeftSw.read();
        SwitchesState[4] = RightSideRightSw.read();
        SwitchesState[5] = RightSideLeftSw.read();//BackRightSw.read();
        SwitchesState[6] = BackLeftSw.read();
        SwitchesState[7] = BackRightSw.read();//LeftSideLeftSw.read();

        for (int i = 0; i < 8; i++)
        {
            Serial.print(SwitchesState[i]);
            Serial.print("Switch: ");
            Serial.println(i);
        };

    //if switch is pressed, value is 1
    //if two of the switches of interest are pressed then, relocate to the origin(whitebox in the middle)
    //relocation has to avoid debris 
    switch(wall_or_corners) {
  case wait_case:

  //walls and corners
  for(int i = 0; i < 8; ++i){
    if((SwitchesState[i] && SwitchesState[i+1]) || (SwitchesState[i] && SwitchesState[i+4])){
        WallActivated = true;
    if(i + 4 == 4){
       wall_or_corners = WallLocation[i+5];
    }
    if(i + 4 == 7){
        wall_or_corners = WallLocation[i+3];
    }
        wall_or_corners = WallLocation[i+1];
        break;
    }
  }

    break;
  case front_wall: //walls start
    //go back to origin 
    //after that, reset and deactivate Wall
    WallActivated = false;

    // code block
    break;

    case right_wall:
    //go back to origin 
    //after that, reset and deactivate Wall
    WallActivated = false;
    break;

    case left_wall:
    //go back to origin 
    //after that, reset and deactivate Wall
    WallActivated = false;

    break;

    case back_wall:
    //go back to origin 
    //after that, reset and deactivate Wall
    WallActivated = false;

    break;

    case TOP_LEFT: //corners start
     //go back to origin 
    //after that, reset and deactivate Wall
    WallActivated = false;

    break;
    

    case TOP_RIGHT:
     //go back to origin 
    //after that, reset and deactivate Wall
    WallActivated = false;

    break;

    case BOTTOM_RIGHT:
     //go back to origin 
    //after that, reset and deactivate Wall
    WallActivated = false;

    break;

    case BOTTOM_LEFT:
     //go back to origin 
    //after that, reset and deactivate Wall
    WallActivated = false;

    break;



    


  default:
    // code block
    break;
}
    
        

  

        

    












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
