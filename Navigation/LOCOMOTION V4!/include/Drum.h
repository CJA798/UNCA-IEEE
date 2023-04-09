#include "Arduino.h"
#include <TeensyStep.h>
#include <math.h>
#include <macros.h>
#include <Bounce2.h>
#include <Ports.h>
// #include <Sensors.h>
/*I need to store the current angle of the bot in every move that I make
    CurrAngle
    Angle of all items
    Angle Needed

    I need to worry about cases when I move there from any angle.
    If their angle is greater than curr angle then I just need to
    move that number of steps there. If their angle is less then I
    need to find the amount to zero then the amount to new angle.

    Need to make a helper function to change steps to radians

    The Drum will also need to keep track of all the items inside of the drum
    It can hold two yellow ducks in different positions
    One Pink duck
    Two White columns
    */

#define InYellowDuck1 ('a')
#define InYellowDuck2 ('f')
#define InPinkDuck ('b')
#define InWhiteColumn ('c')
#define InGreenColumn ('d')
#define InRedColumn ('e')

#define OutYellowDuck1 ('A')
#define OutYellowDuck2 ('F')
#define OutPinkDuck ('B')
#define OutWhiteColumn ('C')
#define OutGreenColumn ('D')
#define OutRedColumn ('E')

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Making the angle for all of the objects being loaded. Then we can deal with it using the helper method
// TODO: Fill out the appropriate angle for all of the items.
#define InYellowAng1 (1)
#define InYellowAng2 (2)
#define InPinkAng (3)
#define InWhiteAng (4)
#define InGreenAng (5)
#define InRedAng (6)
#define FOUR_PI (12.566370614)
// These are the Values in radians for positions
#define Position_In_1 (0)
#define Position_In_2 (41)
#define Position_In_3 (116)
#define Position_In_4 (190)
#define Position_In_5 (269)
#define Position_In_6 (340)

#define POSITION_OUT_1 (0)
#define POSITION_OUT_2 (43)
#define POSITION_OUT_3 (63)
#define POSITION_OUT_4 (176)
#define POSITION_OUT_5 (213)
#define POSITION_OUT_6 (320)

#define OutYellowAng1 (7)
#define OutYellowAng2 (8)
#define OutPinkAng (9)
#define OutWhiteAng (10)
#define OutGreenAng (11)
#define OutRedAng (12)
#define STEPS_PER_RAD_DRUM 2069
#define DRUM_STEPPER_MAX_SPEED 2000
bool Homed = 0;
char DrumState = 0;
char item = 0;
Bounce LimitSwitch;
int NewPosition = 0;
class StorageDrum
{
private:
    int CurrSteps;
    int CurrAngle;
    Stepper DrumStepper;
    StepControl DrumController;
    int DegreeCount;
    int RadToSteps()
    {
        /*Calculation for drum steps per rad
        StepsPerRotation = (200 * 32)%steps/rot
        StepsPerRad = StepsPerRotation / (2pi)
        GearRatio = 324/40
        StepsPerBotRad = ((StepsPerRad/(2pi)))*GearRatio
        Steps Per Bot Rad = 26262*/
        int GearRatio = 324 / 40;
        int StepsPerRotation = (200 * 4);
        int StepsPerRad = StepsPerRotation / (2 * M_PI) * GearRatio;
        return StepsPerRad;
    }

    // modify this function for loading only
    // int doRotation(int SlotAng, bool rotChoice)
    int doRotation(int NewPosition, int pillar_or_duck)
    {
        // The plan is that if we aren't currently spun to this angle then we move the amount to get there.
        // If we are already passed this angle then we have to spin all the way to reset and go back.
        // if (rotChoice)
        // {

        int CurrAngle = Position_In_6 - NewPosition;
        int steps = RadToSteps() * CurrAngle;
        // rotate to this angle by giving steps to Chase
        DrumStepper.setTargetAbs(steps);
        DrumController.moveAsync(DrumStepper);

        // Controller.isMoving();

        // int angle = SlotAng - CurrAngle;
        // int steps = RadToSteps() * angle;
        //  TODO: DrumStepper move to steps
        //}
        /*
        else
        {
            int angle = (2 * M_PI - CurrAngle) + SlotAng;
            int steps = RadToSteps() * angle;
            // TODO: DrumStepper move to steps
        }
        */
        return 0;
    };

public:
    StorageDrum() : DrumStepper(STORAGE_MTRSTEP, STORAGE_MTRDIR) // CONDSTRUCTOR

    {
        LimitSwitch = Bounce();
        pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
        LimitSwitch.attach(LIMIT_SWITCH_PIN, INPUT_PULLUP);
        LimitSwitch.interval(5);

        DrumStepper
            .setMaxSpeed(500)
            .setAcceleration(10000);

        // Add whatever code u want to run when the drum object is constructed
    };
    void DrumProcess(void)

    {
        DrumStepper.setMaxSpeed(DRUM_STEPPER_MAX_SPEED);
        Serial.print("Drum State: ");
        Serial.println(DrumState);
        // switch (DrumState)
        //{
        // case 0:
        //     HomeDrumStepper();
        //     DrumState = 1;
        //     item = InYellowDuck1;
        //     break;
        // case 1:
        // Move the drum to the correct position
        doRotation();
        if (!DrumController.isRunning())
        {
            DrumState = 2;
        };

        //     break;
        // default:
        //    break;
        //};
        // delay(50);
    };

    void HomeDrumStepper(void)
    { // Call and I will home the stepper. Blocking (returns after stepper is homed)
        if (Homed)
        {
            return;
        };
        DrumStepper.setMaxSpeed(DRUM_STEPPER_MAX_SPEED);
        Serial.println("Homing Drum");
        DrumStepper.setTargetRel(-FOUR_PI * STEPS_PER_RAD_DRUM);
        DrumController.moveAsync(DrumStepper);
        bool LimitSwitchState;
        delay(50);
        LimitSwitch.update();
        LimitSwitchState = LimitSwitch.read();
        Serial.print("Limit Switch State: ");
        Serial.println(LimitSwitchState);
        while (LimitSwitchState == 1)
        {
            LimitSwitch.update();
            LimitSwitchState = LimitSwitch.read();
            Serial.print("Limit Switch State: ");
            Serial.println(LimitSwitchState);
        };
        Serial.print("Drum Impacted");
        DrumController.emergencyStop();
        delay(200);
        DrumStepper.setTargetRel(.5 * STEPS_PER_RAD_DRUM);
        DrumController.moveAsync(DrumStepper);
        delay(100);
        DrumStepper.setMaxSpeed(300);
        DrumStepper.setTargetRel(-75 * STEPS_PER_RAD_DRUM);
        DrumController.moveAsync(DrumStepper);
        while (LimitSwitchState == 1)
        {
            LimitSwitch.update();
            LimitSwitchState = LimitSwitch.read();
            Serial.print("Limit Switch State: ");
            Serial.println(LimitSwitchState);
        };

        DrumController.emergencyStop();
        DrumStepper.setPosition(0);
        DrumStepper.setMaxSpeed(DRUM_STEPPER_MAX_SPEED);
        Homed = 1;
        Serial.println("Drum Homed");
        delay(500);
    };
    void TestRotation()

    {
        char Input = '0';
        // Serial.begin(9600);
        if (Serial.available())
        {
            Input = Serial.read();
            Serial.print("Input: ");
            Serial.println(Input);
        }

        switch (Input)
        {
        case '0':
            break;
        case '1':
            DrumStepper.setTargetRel((PI / 180) * STEPS_PER_RAD_DRUM);
            DrumController.move(DrumStepper);
            Serial.println("+1 degree");
            DegreeCount++;
            Serial.print("Degree Count: ");
            Serial.println(DegreeCount);
            break;
        case '2':
            DrumStepper.setTargetRel(-(PI / 180) * STEPS_PER_RAD_DRUM);
            DrumController.move(DrumStepper);
            DegreeCount--;
            Serial.println("-1 degree");
            Serial.print("Degree Count: ");
            Serial.println(DegreeCount);
            break;
        default:
            break;
        }
    }

    int doRotation(void)
    {
        Serial.println("doRotation");
        Serial.print("item: ");
        Serial.println((char)item);

        // int position = 0;
        switch (item)
        {

        case InYellowDuck1:

            Serial.print("entering swith");
            moveDrumToLocation(Position_In_4);
            break;

        case InYellowDuck2:
            DrumStepper.setTargetAbs(9700);
            DrumController.move(DrumStepper);
            // moveDrumToLocation(300);
            break;

        case InPinkDuck:
            moveDrumToLocation(Position_In_5);
            break;

        case InWhiteColumn:
            moveDrumToLocation(Position_In_3);
            break;

        case InGreenColumn:

            moveDrumToLocation(Position_In_2);
            break;

        case InRedColumn:
            moveDrumToLocation(Position_In_1);

            break;

        case OutYellowDuck1:
            moveDrumToLocation(POSITION_OUT_3);

            break;

        case OutYellowDuck2:
            moveDrumToLocation(POSITION_OUT_5);
            break;

        case OutPinkDuck:
            moveDrumToLocation(POSITION_OUT_4);
            break;

        case OutWhiteColumn:
            moveDrumToLocation(POSITION_OUT_3);
            break;

        case OutGreenColumn:
            moveDrumToLocation(POSITION_OUT_1);
            break;

        case OutRedColumn:
            moveDrumToLocation(POSITION_OUT_2);
            break;

        default:
            break;
        }
        item = 0;
        // The plan is that if we aren't currently spun to this angle then we move the amount to get there.
        // If we are already passed this angle then we have to spin all the way to reset and go back.
        // if (rotChoice)
        // {

        return 0;
    };

    void moveDrumToLocation(int NewPosition)
    {

        // int position = 0;

        delay(100);
        Serial.print("NewPosition :");
        Serial.println(NewPosition);
        NewPosition = NewPosition * (PI / 180);
        NewPosition = NewPosition * STEPS_PER_RAD_DRUM;
        DrumStepper.setTargetRel(NewPosition);
        DrumController.move(DrumStepper);
        //   break;
    }
    bool isDone()
    {
        return DrumController.isRunning();
    }

    /*
    #include "Arduino.h"
    #include <TeensyStep.h>
    #include <math.h>
    #include <macros.h>
    #include <Ports.h>

    I need to store the current angle of the bot in every move that I make
        CurrAngle
        Angle of all items
        Angle Needed

        I need to worry about cases when I move there from any angle.
        If their angle is greater than curr angle then I just need to
        move that number of steps there. If their angle is less then I
        need to find the amount to zero then the amount to new angle.

        Need to make a helper function to change steps to radians

        The Drum will also need to keep track of all the items inside of the drum
        It can hold two yellow ducks in different positions
        One Pink duck
        Two White columns


    #define InYellowDuck1 ('a')
    #define InYellowDuck2 ('f')
    #define InPinkDuck ('b')
    #define InWhiteColumn ('c')
    #define InGreenColumn ('d')
    #define InRedColumn ('e')

    #define OutYellowDuck1 ('A')
    #define OutYellowDuck2 ('F')
    #define OutPinkDuck ('B')
    #define OutWhiteColumn ('C')
    #define OutGreenColumn ('D')
    #define OutRedColumn ('E')

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Making the angle for all of the objects being loaded. Then we can deal with it using the helper method
    // TODO: Fill out the appropriate angle for all of the items.
    #define InYellowAng1 (1)
    #define InYellowAng2 (2)
    #define InPinkAng (3)
    #define InWhiteAng (4)
    #define InGreenAng (5)
    #define InRedAng (6)

    // These are the Values in degrees for positions
    #define Position_In_1 (0)
    #define Position_In_2 (41)
    #define Position_In_3 (116)
    #define Position_In_4 (190)
    #define Position_In_5 (269)
    #define Position_In_6 (322)

    #define POSITION_OUT_1 (0)
    #define POSITION_OUT_2 (43)
    #define POSITION_OUT_3 (63)
    #define POSITION_OUT_4 (176)
    #define POSITION_OUT_5 (213)
    #define POSITION_OUT_6 (320)

    #define OutYellowAng1 (7)
    #define OutYellowAng2 (8)
    #define OutPinkAng (9)
    #define OutWhiteAng (10)
    #define OutGreenAng (11)
    #define OutRedAng (12)
    #define STEPS_PER_RAD_DRUM 2069

    class Drum
    {
    private:
        int CurrPosition = 0;

        Stepper DrumStepper;
        StepControl DrumController;
        int DegreeCount;
        int RadToSteps()
        {
            Calculation for drum steps per rad
            StepsPerRotation = (200 * 32)%steps/rot
            StepsPerRad = StepsPerRotation / (2pi)
            GearRatio = 324/40
            StepsPerBotRad = ((StepsPerRad/(2pi)))*GearRatio
            Steps Per Bot Rad = 26262
            int GearRatio = 324 / 40;
            int StepsPerRotation = (200 * 4);
            int StepsPerRad = StepsPerRotation / (2 * M_PI) * GearRatio;
            // int position = 0;
            return StepsPerRad;
        }

        // modify this function for loading only
        // int doRotation(int SlotAng, bool rotChoice)
        int doRotation(char item)
        {
            // int position = 0;
            switch (item)
            {

            case InYellowDuck1:
                moveDrumToLocation(Position_In_4);
                break;

            case InYellowDuck2:
                moveDrumToLocation(Position_In_6);
                break;

            case InPinkDuck:
                moveDrumToLocation(Position_In_5);
                break;

            case InWhiteColumn:
                moveDrumToLocation(Position_In_3);
                break;

            case InGreenColumn:

                moveDrumToLocation(Position_In_2);
                break;

            case InRedColumn:
                moveDrumToLocation(Position_In_1);

                break;

            case OutYellowDuck1:
                moveDrumToLocation(POSITION_OUT_3);

                break;

            case OutYellowDuck2:
                moveDrumToLocation(POSITION_OUT_5);
                break;

            case OutPinkDuck:
                moveDrumToLocation(POSITION_OUT_4);
                break;

            case OutWhiteColumn:
                moveDrumToLocation(POSITION_OUT_3);
                break;

            case OutGreenColumn:
                moveDrumToLocation(POSITION_OUT_1);
                break;

            case OutRedColumn:
                moveDrumToLocation(POSITION_OUT_2);
                break;

            default:
                break;
            }

            // The plan is that if we aren't currently spun to this angle then we move the amount to get there.
            // If we are already passed this angle then we have to spin all the way to reset and go back.
            // if (rotChoice)
            // {

        int CurrAngle = Position_In_6 - NewPosition;
        int steps = RadToSteps() * CurrAngle;
        // rotate to this angle by giving steps to Chase
        DrumStepper.setTargetAbs(steps);
        DrumController.moveAsync(DrumStepper);


            // Controller.isMoving();

            // int angle = SlotAng - CurrAngle;
            // int steps = RadToSteps() * angle;
            //  TODO: DrumStepper move to steps
            //}

            else
            {
                int angle = (2 * M_PI - CurrAngle) + SlotAng;
                int steps = RadToSteps() * angle;
                // TODO: DrumStepper move to steps
            }

            return 0;
        };

    public:
        Drum() : DrumStepper(STORAGE_MTRSTEP, STORAGE_MTRDIR) // CONDSTRUCTOR

        {
            DrumStepper
                .setMaxSpeed(1000)
                .setAcceleration(1000);

            // Add whatever code u want to run when the drum object is constructed
        };
        void HomeDrumStepper(void)
        { // Call and I will home the stepper. Blocking (returns after stepper is homed)
            DrumStepper.setTargetRel(262);
            DrumController.move(DrumStepper);
        }
        void TestRotation()
        {
            char Input = '0';
            // Serial.begin(9600);
            if (Serial.available())
            {
                Input = Serial.read();
                Serial.print("Input: ");
                Serial.println(Input);
            }

            switch (Input)
            {
            case 'f':
                doRotation('f');

                break;

            case 'F':
                doRotation('F');

                break;

            case 'a':
                DrumStepper.setTargetRel((PI / 180) * STEPS_PER_RAD_DRUM);
                DrumController.move(DrumStepper);
                Serial.println("+1 degree");
                DegreeCount++;
                Serial.print("Degree Count: ");
                Serial.println(DegreeCount);
                break;
            case '2':
                DrumStepper.setTargetRel(-(PI / 180) * STEPS_PER_RAD_DRUM);
                DrumController.move(DrumStepper);
                DegreeCount--;
                Serial.println("-1 degree");
                Serial.print("Degree Count: ");
                Serial.println(DegreeCount);
                break;
            default:
                break;
            }
        }

        // FOR LOADING ONLY
        void moveDrumToLocation(int NewPosition)
        {

            // int position = 0;

            // switch(position){
            //    case InYellowAng1:
            CurrPosition = NewPosition - CurrPosition;
            DrumStepper.setTargetRel(((CurrPosition) * (PI / 180)) * STEPS_PER_RAD_DRUM);
            DrumController.move(DrumStepper);
            //   break;


            case InYellowAng2:
            CurrPosition = NewPosition - CurrPosition;
             DrumStepper.setTargetRel((CurrPosition)*(PI / 180) * STEPS_PER_RAD_DRUM);
             DrumController.move(DrumStepper);
                break;



                default:
                break;
                }
            /


            switch (move)
            {
            case InYellowDuck1:
                // Inside of all the cases we have two different cases.
                // Either the rotation will be less than required for the next item
                // Or it will be more than.
                // If the item is more than then we will zero out the drum and move to
                // Next item. If not then we just move to the next item.
                doRotation(Position_In_2);
                // CurrAngle = InYellowAng1;
                break;

            case InYellowDuck2:
                doRotation(Position_In_4);

                break;

            case InPinkDuck:
                doRotation(Position_In_3);

                break;

            case InWhiteColumn:
                doRotation(Position_In_1);
                break;

            case InGreenColumn:
                doRotation(Position_In_5);
                break;

            case InRedColumn:
                doRotation(Position_In_6);

                break;
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // This is the out cases of the case statement
            // Dont worry about this right now
            case OutYellowDuck1:
                doRotation(OutYellowAng1, CurrAngle <= OutYellowAng1);
                break;

            case OutYellowDuck2:
                doRotation(OutYellowAng2, CurrAngle <= InYellowAng2);

                break;

            case OutPinkDuck:
                doRotation(OutPinkAng, CurrAngle <= OutPinkAng);

                break;

            case OutWhiteColumn:
                doRotation(OutWhiteAng, CurrAngle <= OutWhiteAng);

                break;

            case OutGreenColumn:
                doRotation(OutGreenAng, CurrAngle <= OutGreenAng);

                break;

            case OutRedColumn:
                doRotation(OutRedAng, CurrAngle <= OutRedAng);

                break;

            }

        }
    };
    */
};