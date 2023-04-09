#include <macros.h>
#include <Bounce2.h>
#include <math.h>
#include "Adafruit_VL53L1X.h"
#include "Adafruit_VL53L0X.h"
#include <ports.h>
// #include <Adafruit_I2CDevice.h>
// #include <Adafruit_Sensor.h>
// #include <Wire.h>
#define X_MAX (122)
#define Y_MAX (122)
#define X_MIN (-122)
#define Y_MIN (0)
#define FRONT_PRESSED (0)
#define RIGHT_PRESSED (1)
#define BACK_PRESSED (2)
#define LEFT_PRESSED (3)
#define NONE_PRESSED (4)
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
char SwitchesState = 0;
bool ChangeInSwitchState = 0;
double SwitchAngle = 0;
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
    pinMode(LEFTSIDE_RIGHT_SWITCH, INPUT_PULLUP);
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

    if (!(FrontLeftSw.read() && FrontRightSw.read() ==0))
    {

      SwitchesState = FRONT_PRESSED;
    } else
    if (!(BackLeftSw.read() && BackRightSw.read()))
    {

      SwitchesState = BACK_PRESSED;
    } else
    if (!(RightSideLeftSw.read() && RightSideRightSw.read()))
    {

      SwitchesState = RIGHT_PRESSED;
    } else
    if (!(LeftSideLeftSw.read() && LeftSideRightSw.read()))
    {

      SwitchesState = LEFT_PRESSED;
    } else
    {
      SwitchesState = NONE_PRESSED;
    }
  };
  void PrintPosition()
  {
    Serial.println("SwitchesState: ");
    Serial.println(" ");
    Serial.print("FrontRightSw: ");
    Serial.println(FrontRightSw.read());
    Serial.print("FrontLeftSw: ");
    Serial.println(FrontLeftSw.read());
    Serial.print("BackLeftSw: ");
    Serial.println(BackLeftSw.read());
    Serial.print("BackRightSw: ");
    Serial.println(BackRightSw.read());
    Serial.print("RightSideLeftSw: ");
    Serial.println(RightSideLeftSw.read());
    Serial.print("RightSideRightSw: ");
    Serial.println(RightSideRightSw.read());
    Serial.print("LeftSideLeftSw: ");
    Serial.println(LeftSideLeftSw.read());
    Serial.print("LeftSideRightSw: ");
    Serial.println(LeftSideRightSw.read());
  }

};

class TOFArray
{
private:
  struct TOFData_mm
  {
    uint16_t FrontBigTOFMeasurementmm = 0;
    uint16_t BackTOFMeasurementmm = 0;
    uint16_t RightTOFMeasurementmm = 0;
    uint16_t LeftTOFMeasurementmm = 0;
    VL53L0X_RangingMeasurementData_t FrontSmallTOFMeasurementmm;
    VL53L0X_RangingMeasurementData_t BackSmallTOFMeasurementmm;
    VL53L0X_RangingMeasurementData_t RightSmallTOFMeasurementmm;
    VL53L0X_RangingMeasurementData_t LeftSmallTOFMeasurementmm;
  };
  Adafruit_VL53L1X BigTOF_Front;
  Adafruit_VL53L1X BigTOF_Back;
  Adafruit_VL53L1X BigTOF_Right;
  Adafruit_VL53L1X BigTOF_Left;
  Adafruit_VL53L0X SmallTOF_Front;
  Adafruit_VL53L0X SmallTOF_Back;
  Adafruit_VL53L0X SmallTOF_Right;
  Adafruit_VL53L0X SmallTOF_Left;
  TOFData_mm TOFData;

public:
  TOFArray()
  { // CONSTRUCTOR

    //   Wire.begin();
    BigTOF_Front = Adafruit_VL53L1X(XSHUT_PIN1, 4);
    BigTOF_Back = Adafruit_VL53L1X(XSHUT_PIN2, 5);
    BigTOF_Right = Adafruit_VL53L1X(XSHUT_PIN3, 6);
    BigTOF_Left = Adafruit_VL53L1X(XSHUT_PIN4, 7);

    SmallTOF_Front = Adafruit_VL53L0X();
    SmallTOF_Back = Adafruit_VL53L0X();
    SmallTOF_Right = Adafruit_VL53L0X();
    SmallTOF_Left = Adafruit_VL53L0X();
  };

  void setIDL0()
  {
    Serial.println("Secodn set of sensors");
    // all reset
    digitalWrite(SHT_LOX1, LOW);
    digitalWrite(SHT_LOX2, LOW);
    digitalWrite(SHT_LOX3, LOW);
    digitalWrite(SHT_LOX4, LOW);
    delay(10);
    // all unreset
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, HIGH);
    digitalWrite(SHT_LOX3, HIGH);
    digitalWrite(SHT_LOX4, HIGH);
    delay(10);

    // activating LOX1
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, LOW);
    digitalWrite(SHT_LOX3, LOW);
    digitalWrite(SHT_LOX4, LOW);

    // initing LOX1 (SMALL SENSORS)
    if (!SmallTOF_Front.begin(LOX1_ADDRESS))
    {
      Serial.println(F("Failed to boot first VL53L0X"));
      while (1)
        ;
    };
    delay(10);

    // activating LOX2
    digitalWrite(SHT_LOX2, HIGH);
    digitalWrite(SHT_LOX3, LOW);
    digitalWrite(SHT_LOX4, LOW);
    delay(10);

    // initing LOX2
    if (!SmallTOF_Back.begin(LOX2_ADDRESS))
    {
      Serial.println(F("Failed to boot second VL53L0X"));
      while (1)
        ;
    };

    digitalWrite(SHT_LOX3, HIGH);
    digitalWrite(SHT_LOX4, LOW);
    delay(10);
    // Initizlizing LOX3
    if (!SmallTOF_Right.begin(LOX3_ADDRESS))
    {
      Serial.println(F("Failed to boot second VL53L0X"));
      while (1)
        ;
    };
    delay(10);

    // activating LOX4
    digitalWrite(SHT_LOX4, HIGH);
    delay(10);
    // Initializig LOX4
    if (!SmallTOF_Left.begin(LOX4_ADDRESS))
    {
      Serial.println(F("Failed to boot second VL53L0X"));
      while (1)
        ;
    };
  };

  void read_L0x_sensors()
  {

    SmallTOF_Front.rangingTest(&TOFData.FrontSmallTOFMeasurementmm, false); // pass in 'true' to get debug data printout!
    SmallTOF_Back.rangingTest(&TOFData.BackSmallTOFMeasurementmm, false);   // pass in 'true' to get debug data printout!
    SmallTOF_Right.rangingTest(&TOFData.RightSmallTOFMeasurementmm, false); // pass in 'true' to get debug data printout!
    SmallTOF_Left.rangingTest(&TOFData.LeftSmallTOFMeasurementmm, false);   // pass in 'true' to get debug data printout!

    // print sensor one reading
    Serial.print(F("1: "));
    if (TOFData.FrontSmallTOFMeasurementmm.RangeStatus != 4)
    { // if not out of range
      Serial.println(TOFData.FrontSmallTOFMeasurementmm.RangeMilliMeter);
    }
    else
    {
      Serial.println(F("Out of range"));
    };

    Serial.print(F(" "));

    // print sensor two reading
    Serial.print(F("2: "));
    if (TOFData.BackSmallTOFMeasurementmm.RangeStatus != 4)
    {
      Serial.println(TOFData.BackSmallTOFMeasurementmm.RangeMilliMeter);
    }
    else
    {
      Serial.println(F("Out of range"));
    };

    Serial.print(F(" "));
    // print sensor three reading
    Serial.print(F("3: "));
    if (TOFData.RightSmallTOFMeasurementmm.RangeStatus != 4)
    {
      Serial.println(TOFData.RightSmallTOFMeasurementmm.RangeMilliMeter);
    }
    else
    {
      Serial.println(F("Out of range"));
    };

    // print sensor four reading
    Serial.print(F("4: "));
    if (TOFData.LeftSmallTOFMeasurementmm.RangeStatus != 4)
    {
      Serial.println(TOFData.LeftSmallTOFMeasurementmm.RangeMilliMeter);
    }
    else
    {
      Serial.println(F("Out of range"));
    };
  };

  // Initializing the L1x sensors (BIG SENSORS)
  void setIDL1()
  {
    Serial.println("First set of sensors");
    Wire.begin();
    delay(100);
    Serial.println("Wire I2C initialized");
    if (!BigTOF_Front.begin(L1X1_ADDRESS, &Wire))
    {
      Serial.print(F("Error on init of VL sensor: "));
      Serial.println(BigTOF_Front.vl_status);
      while (1)
        delay(10);
    };
    Serial.print("First Sensor OK");

    if (!BigTOF_Back.begin(L1X2_ADDRESS, &Wire))
    {
      Serial.print(F("Error on init of VL sensor: "));
      Serial.println(BigTOF_Back.vl_status);
      while (1)
        delay(10);
    };
    if (!BigTOF_Right.begin(L1X3_ADDRESS, &Wire))
    {
      Serial.print(F("Error on init of VL sensor: "));
      Serial.println(BigTOF_Right.vl_status);
      while (1)
        delay(10);
    };
    if (!BigTOF_Left.begin(L1X4_ADDRESS, &Wire))
    {
      Serial.print(F("Error on init of VL sensor: "));
      Serial.println(BigTOF_Left.vl_status);
      while (1)
        delay(10);
    };

    Serial.println(F("VL53L1X sensors OK!"));

    Serial.print(F("BigFront Sensor ID: 0x"));
    Serial.println((BigTOF_Front).sensorID(), HEX);
    Serial.print(F("BigBack Sensor ID: 0x"));
    Serial.println((BigTOF_Back).sensorID(), HEX);
    Serial.print(F("BigRight Sensor ID: 0x"));
    Serial.println((BigTOF_Right).sensorID(), HEX);
    Serial.print(F("BigLeft Sensor ID: 0x"));
    Serial.println((BigTOF_Left).sensorID(), HEX);

    if (!BigTOF_Front.startRanging())
    {
      Serial.print(F("Couldn't start ranging: "));
      Serial.println(BigTOF_Front.vl_status);
      while (1)
        delay(10);
    };
    if (!BigTOF_Back.startRanging())
    {
      Serial.print(F("Couldn't start ranging: "));
      Serial.println(BigTOF_Back.vl_status);
      while (1)
        delay(10);
    };

    if (!BigTOF_Right.startRanging())
    {
      Serial.print(F("Couldn't start ranging: "));
      Serial.println(BigTOF_Right.vl_status);
      while (1)
        delay(10);
    };
    if (!BigTOF_Left.startRanging())
    {
      Serial.print(F("Couldn't start ranging: "));
      Serial.println(BigTOF_Left.vl_status);
      while (1)
        delay(10);
    };
    Serial.println(F("Ranging started"));

    // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
    BigTOF_Front.setTimingBudget(50);
    BigTOF_Back.setTimingBudget(50);
    BigTOF_Right.setTimingBudget(50);
    BigTOF_Left.setTimingBudget(50);
    Serial.print(F("Timing budget (ms): "));
    Serial.println(BigTOF_Front.getTimingBudget());
    Serial.println(BigTOF_Back.getTimingBudget());
    Serial.println(BigTOF_Right.getTimingBudget());
    Serial.println(BigTOF_Left.getTimingBudget());
  };
  void TOFSensorTest()
  {
    int16_t distance;
    int16_t distance1;

    if (BigTOF_Front.dataReady())
    {
      // new measurement for the taking!
      distance = BigTOF_Front.distance();
      if (distance == -1)
      {
        // something went wrong!
        Serial.print(F("Couldn't get distance: "));
        Serial.println(BigTOF_Front.vl_status);
        return;
      }
      Serial.print(F("Distance: "));
      Serial.print(distance);
      Serial.println(" mm");

      // data is read out, time for another reading!
      BigTOF_Front.clearInterrupt();
    };
    if (BigTOF_Back.dataReady())
    {
      // new measurement for the taking!
      distance1 = BigTOF_Back.distance();
      if (distance1 == -1)
      {
        // something went wrong!
        Serial.print(F("Couldn't get distance: "));
        Serial.println(BigTOF_Back.vl_status);
        return;
      }
      Serial.print(F("Distance: "));
      Serial.print(distance1);
      Serial.println(" mm");

      // data is read out, time for another reading!
      BigTOF_Back.clearInterrupt();
    };
    if (BigTOF_Right.dataReady())
    {
      // new measurement for the taking!
      distance1 = BigTOF_Right.distance();
      if (distance1 == -1)
      {
        // something went wrong!
        Serial.print(F("Couldn't get distance: "));
        Serial.println(BigTOF_Right.vl_status);
        return;
      }
      Serial.print(F("Distance: "));
      Serial.print(distance1);
      Serial.println(" mm");

      // data is read out, time for another reading!
      BigTOF_Right.clearInterrupt();
    };
    if (BigTOF_Left.dataReady())
    {
      // new measurement for the taking!
      distance1 = BigTOF_Left.distance();
      if (distance1 == -1)
      {
        // something went wrong!
        Serial.print(F("Couldn't get distance: "));
        Serial.println(BigTOF_Left.vl_status);
        return;
      }
      Serial.print(F("Distance: "));
      Serial.print(distance1);
      Serial.println(" mm");

      // data is read out, time for another reading!
      BigTOF_Left.clearInterrupt();
    };
  };

  void InitializeTOFs()
  {
    // Serial.begin(115200);

    Serial.println(F("Adafruit VL53L1X sensor demo"));
    setIDL1();
    setIDL0();
    read_L0x_sensors();

    byte count = 0;

    for (byte i = 1; i < 120; i++)
    {

      Wire.beginTransmission(i);
      if (Wire.endTransmission() == 0)
      {
        Serial.print("Found address: ");
        Serial.print(i, DEC);
        Serial.print(" (0x");
        Serial.print(i, HEX);
        Serial.println(")");
        count++;
        delay(1); // maybe unneeded?
      };          // end of good response
    };            // end of for loop
    Serial.println("Done.");
    Serial.print("Found ");
    Serial.print(count, DEC);
    Serial.println(" device(s).");

    /*
    vl.VL53L1X_SetDistanceThreshold(100, 300, 3, 1);
    vl.VL53L1X_SetInterruptPolarity(0);
*/
  };
};
// SMALL TOF SENSORS NEED TO BE IMPLEMENTED
/*
   };
   Original test code from dennis
   #include "Adafruit_VL53L1X.h"
   #include "Adafruit_VL53L0X.h"

   #define IRQ_PIN 2\

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

   //Addresses for the sensors
   #define L1X1_ADDRESS 0x29
   #define L1X2_ADDRESS 0x2B
   #define L1X3_ADDRESS 0x2D
   #define L1X4_ADDRESS 0x2F
   #define LOX1_ADDRESS 0x31
   #define LOX2_ADDRESS 0x33
   #define LOX3_ADDRESS 0x35
   #define LOX4_ADDRESS 0x37


   //Objects for the L1x
   Adafruit_VL53L1X sens1 = Adafruit_VL53L1X(XSHUT_PIN1, IRQ_PIN);
   Adafruit_VL53L1X sens2 = Adafruit_VL53L1X(XSHUT_PIN2, IRQ_PIN);
   Adafruit_VL53L1X sens3 = Adafruit_VL53L1X(XSHUT_PIN3, IRQ_PIN);
   Adafruit_VL53L1X sens4 = Adafruit_VL53L1X(XSHUT_PIN4, IRQ_PIN);

   // objects for the vl53l0x
   Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
   Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
   Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
   Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();

   // this holds the measurement
   VL53L0X_RangingMeasurementData_t measure1;
   VL53L0X_RangingMeasurementData_t measure2;
   VL53L0X_RangingMeasurementData_t measure3;
   VL53L0X_RangingMeasurementData_t measure4;

   void setIDL0() {
     Serial.println("Secodn set of sensors");
     // all reset
     digitalWrite(SHT_LOX1, LOW);
     digitalWrite(SHT_LOX2, LOW);
     digitalWrite(SHT_LOX3, LOW);
     digitalWrite(SHT_LOX4, LOW);
     delay(10);
     // all unreset
     digitalWrite(SHT_LOX1, HIGH);
     digitalWrite(SHT_LOX2, HIGH);
     digitalWrite(SHT_LOX3, HIGH);
     digitalWrite(SHT_LOX4, HIGH);
     delay(10);

     // activating LOX1
     digitalWrite(SHT_LOX1, HIGH);
     digitalWrite(SHT_LOX2, LOW);
     digitalWrite(SHT_LOX3, LOW);
     digitalWrite(SHT_LOX4, LOW);

     // initing LOX1
     if(!lox1.begin(LOX1_ADDRESS)) {
       Serial.println(F("Failed to boot first VL53L0X"));
       while(1);
     }
     delay(10);

     // activating LOX2
     digitalWrite(SHT_LOX2, HIGH);
     digitalWrite(SHT_LOX3, LOW);
     digitalWrite(SHT_LOX4, LOW);
     delay(10);

     //initing LOX2
     if(!lox2.begin(LOX2_ADDRESS)) {
       Serial.println(F("Failed to boot second VL53L0X"));
       while(1);
     }

     digitalWrite(SHT_LOX3, HIGH);
     digitalWrite(SHT_LOX4, LOW);
     delay(10);
     if(!lox1.begin(LOX3_ADDRESS)) {
       Serial.println(F("Failed to boot first VL53L0X"));
       while(1);
     }
     delay(10);

     // activating LOX2
     digitalWrite(SHT_LOX4, HIGH);
     delay(10);

     //initing LOX2
     if(!lox2.begin(LOX4_ADDRESS)) {
       Serial.println(F("Failed to boot second VL53L0X"));
       while(1);
     }
   }

   void read_L0x_sensors() {

     lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
     lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
     lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!
     lox4.rangingTest(&measure4, false); // pass in 'true' to get debug data printout!

     // print sensor one reading
     Serial.print(F("1: "));
     if(measure1.RangeStatus != 4) {     // if not out of range
       Serial.print(measure1.RangeMilliMeter);
     } else {
       Serial.print(F("Out of range"));
     }

     Serial.print(F(" "));

     // print sensor two reading
     Serial.print(F("2: "));
     if(measure2.RangeStatus != 4) {
       Serial.print(measure2.RangeMilliMeter);
     } else {
       Serial.print(F("Out of range"));
     }
       Serial.print(F(" "));

     Serial.print(F("3: "));
     if(measure1.RangeStatus != 4) {     // if not out of range
       Serial.print(measure3.RangeMilliMeter);
     } else {
       Serial.print(F("Out of range"));
     }

     Serial.print(F(" "));

     // print sensor two reading
     Serial.print(F("4: "));
     if(measure2.RangeStatus != 4) {
       Serial.print(measure4.RangeMilliMeter);
     } else {
       Serial.print(F("Out of range"));
     }

     Serial.println();
   }


   //Initializing the L1x sensors
   void setIDL1(){
     Serial.println("First set of sensors");
     Wire.begin();
     if (! sens1.begin(L1X1_ADDRESS, &Wire)) {
       Serial.print(F("Error on init of VL sensor: "));
       Serial.println(sens1.vl_status);
       while (1)       delay(10);
     }
     if (! sens2.begin(L1X2_ADDRESS, &Wire)) {
       Serial.print(F("Error on init of VL sensor: "));
       Serial.println(sens2.vl_status);
       while (1)       delay(10);
     }
     if (! sens3.begin(L1X3_ADDRESS, &Wire)) {
       Serial.print(F("Error on init of VL sensor: "));
       Serial.println(sens3.vl_status);
       while (1)       delay(10);
     }
     if (! sens4.begin(L1X4_ADDRESS, &Wire)) {
       Serial.print(F("Error on init of VL sensor: "));
       Serial.println(sens4.vl_status);
       while (1)       delay(10);
     }
     Serial.println(F("VL53L1X sensors OK!"));

     Serial.print(F("Sensor ID: 0x"));
     Serial.println(sens1.sensorID(), HEX);
     Serial.println(sens2.sensorID(), HEX);
     Serial.println(sens3.sensorID(), HEX);
     Serial.println(sens4.sensorID(), HEX);

     if (! sens1.startRanging()) {
       Serial.print(F("Couldn't start ranging: "));
       Serial.println(sens1.vl_status);
       while (1)       delay(10);
     }
     if (! sens2.startRanging()) {
       Serial.print(F("Couldn't start ranging: "));
       Serial.println(sens2.vl_status);
       while (1)       delay(10);
     }
     if (! sens3.startRanging()) {
       Serial.print(F("Couldn't start ranging: "));
       Serial.println(sens3.vl_status);
       while (1)       delay(10);
     }
     if (! sens4.startRanging()) {
       Serial.print(F("Couldn't start ranging: "));
       Serial.println(sens4.vl_status);
       while (1)       delay(10);
     }
     Serial.println(F("Ranging started"));

     // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
     sens1.setTimingBudget(50);
     sens2.setTimingBudget(50);
     sens3.setTimingBudget(50);
     sens4.setTimingBudget(50);
     Serial.print(F("Timing budget (ms): "));
     Serial.println(sens1.getTimingBudget());
     Serial.println(sens2.getTimingBudget());
     Serial.println(sens3.getTimingBudget());
     Serial.println(sens4.getTimingBudget());
   }

   void setup() {
     Serial.begin(115200);
     while (!Serial) delay(10);

     Serial.println(F("Adafruit VL53L1X sensor demo"));
     setIDL1();
     setIDL0();
     read_L0x_sensors();


     byte count = 0;


     for (byte i = 1; i < 120; i++)
     {

       Wire.beginTransmission (i);
       if (Wire.endTransmission () == 0)
       {
         Serial.print ("Found address: ");
         Serial.print (i, DEC);
         Serial.print (" (0x");
         Serial.print (i, HEX);
         Serial.println (")");
         count++;
         delay (1);  // maybe unneeded?
       } // end of good response
     } // end of for loop
     Serial.println ("Done.");
     Serial.print ("Found ");
     Serial.print (count, DEC);
     Serial.println (" device(s).");


     vl.VL53L1X_SetDistanceThreshold(100, 300, 3, 1);
     vl.VL53L1X_SetInterruptPolarity(0);


   }

   void loop()
   {
       int16_t distance;
       int16_t distance1;

       if (sens1.dataReady())
       {
           // new measurement for the taking!
           distance = sens1.distance();
           if (distance == -1)
           {
               // something went wrong!
               Serial.print(F("Couldn't get distance: "));
               Serial.println(sens1.vl_status);
               return;
           }
           Serial.print(F("Distance: "));
           Serial.print(distance);
           Serial.println(" mm");

           // data is read out, time for another reading!
           sens1.clearInterrupt();
       }
       if (sens2.dataReady())
       {
           // new measurement for the taking!
           distance1 = sens2.distance();
           if (distance1 == -1)
           {
               // something went wrong!
               Serial.print(F("Couldn't get distance: "));
               Serial.println(sens2.vl_status);
               return;
           }
           Serial.print(F("Distance: "));
           Serial.print(distance1);
           Serial.println(" mm");

           // data is read out, time for another reading!
           sens2.clearInterrupt();
       }
   }

   */