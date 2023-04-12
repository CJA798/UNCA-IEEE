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
#define HOME_DRUM ('H')
#define RUN_STATE_MACHINE ('R')
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
// load
#define IN_POSITION_GREEN_PILLAR (0)     // green
#define IN_POSITION_RED_PILLAR (1550)    //  red/green
#define IN_POSITION_WHITE_PILLAR (11580) // white
#define IN_POSITION_YELLOW_DUCK2 (9700)  // yellow duck 2
#define IN_POSITION_PINK_DUCK (6850)     // pink duck
#define IN_POSITION_YELLOW_DUCK1 (4200)  // yellow duck 1

#define OUT_POSITION_WHITE_PILLAR (0)
#define OUT_POSITION_GREEN_PILLAR (1400)
#define OUT_POSITION_RED_PILLAR (2900)
#define OUT_POSITION_YELLOW_DUCK1 (3500)
#define OUT_POSITION_PINK_DUCK (7780)
#define OUT_POSITION_YELOW_DUCK2 (9000)

#define OutYellowAng1 (7)
#define OutYellowAng2 (8)
#define OutPinkAng (9)
#define OutWhiteAng (10)
#define OutGreenAng (11)
#define OutRedAng (12)
#define STEPS_PER_RAD_DRUM 2069
#define DRUM_STEPPER_MAX_SPEED 2000
#define SPECIAL ('*')
void InitDrumAsSpeaker(void);
void note(int num, long dur);
char MegaState = 0;
// defines pins numbers
const int stepPin = 41;
const int dirPin = 40;

// here comes a bunch of 'useful' vars; dont mind
int coun;
bool dir = 0;
int del;
int c = 1912;
int cf = 1805;
int d = 1703;
int df = 1607;
int e = 1517;
int f = 1431;
int ff = 1351;
int g = 1275;
int gf = 1203;
int a = 1136;
int af = 1072;
int b = 1012;
int c1 = floor(c / 2);
int cf1 = floor(cf / 2);
int d1 = floor(d / 2);
int df1 = floor(df / 2);
int e1 = floor(e / 2);
int f1 = floor(1431 / 2);
int ff1 = floor(1351 / 2);
int g1 = floor(1275 / 2);
int gf1 = floor(1203 / 2);
int a1 = floor(1136 / 2);
int af1 = floor(1072 / 2);
int b1 = floor(1012 / 2);
int e0 = e * 2;
int g0 = g * 2;
int b0 = b * 2;
int af0 = af * 2;
int a0 = a * 2;
int f0 = f * 2;
int use = 180;
int tempo = 120;
int oct = 5;
bool Homed = false;
char DrumState = 0;
char item = 0;
Bounce LimitSwitch;
int NewPosition = 0;
double StepbyDeg = 36.15;
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
        if (Command > 0)
        {
            if (debug)
            {
                Serial.println("Command");
                Serial.print(Command);
            }

            switch (Command)
            {
            case InYellowDuck1:
                if (debug)
                {
                    Serial.println("InYellowDuck1");
                };
                NewPosition = IN_POSITION_YELLOW_DUCK1;
                break;
            case InYellowDuck2:
                NewPosition = IN_POSITION_YELLOW_DUCK2;
                break;
            case InPinkDuck:
                NewPosition = IN_POSITION_PINK_DUCK;
                break;
            case InWhiteColumn:
                NewPosition = IN_POSITION_WHITE_PILLAR;
                break;
            case InGreenColumn:
                NewPosition = IN_POSITION_GREEN_PILLAR;
                break;
            case InRedColumn:
                NewPosition = IN_POSITION_RED_PILLAR;
                break;
            case OutYellowDuck1:
                NewPosition = OUT_POSITION_YELOW_DUCK2;
                break;
            case OutYellowDuck2:
                NewPosition = OUT_POSITION_YELLOW_DUCK1;
                break;
            case OutPinkDuck:
                NewPosition = OUT_POSITION_PINK_DUCK;
                break;
            case OutWhiteColumn:
                NewPosition = OUT_POSITION_WHITE_PILLAR;
                break;
            case OutGreenColumn:
                NewPosition = OUT_POSITION_GREEN_PILLAR;
                break;
            case OutRedColumn:
                NewPosition = OUT_POSITION_RED_PILLAR;
                break;
            case HOME_DRUM:
                Homed = false;
                // HomeDrumStepper();
                break;
            case SPECIAL:
                StartTune();
                HomeDrumStepper();
                break;
            case RUN_STATE_MACHINE:

                break;
            default:
                break;
            }
            DrumStepper.setTargetAbs(NewPosition);
            DrumController.move(DrumStepper);
            Serial.print("finished");
            Serial.println(Command);
            Command = 0;
        };
    };

    void HomeDrumStepper(void)
    { // Call and I will home the stepper. Blocking (returns after stepper is homed)
        if (Homed)
        {
            return;
        };
        DrumStepper.setMaxSpeed(DRUM_STEPPER_MAX_SPEED);
        // Serial.println("Homing Drum");
        DrumStepper.setTargetRel(-FOUR_PI * STEPS_PER_RAD_DRUM);
        DrumController.moveAsync(DrumStepper);
        bool LimitSwitchState;
        delay(50);
        LimitSwitch.update();
        LimitSwitchState = LimitSwitch.read();
        //   Serial.print("Limit Switch State: ");
        //  Serial.println(LimitSwitchState);
        while (LimitSwitchState == 1)
        {
            LimitSwitch.update();
            LimitSwitchState = LimitSwitch.read();
            //      Serial.print("Limit Switch State: ");
            //     Serial.println(LimitSwitchState);
        };
        // Serial.print("Drum Impacted");
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
            //  Serial.print("Limit Switch State: ");
            //  Serial.println(LimitSwitchState);
        };

        DrumController.emergencyStop();
        DrumStepper.setPosition(0);
        DrumStepper.setMaxSpeed(DRUM_STEPPER_MAX_SPEED);
        Homed = true;
        // Serial.println("Drum Homed");
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
            item = InWhiteColumn;
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
    };
    void StartTune(void)
    {
        InitDrumAsSpeaker();
        note(c, 300);
        note(d, 300);
        note(f, 300);
        note(d, 300);
        note(a, 900);
        note(a, 900);
        note(g, 1800);
        
        note(c, 300);
        note(d, 300);
        note(f, 300);
        note(d, 300);
        note(g, 900);
        note(g, 900);
        note(f, 1800);
        
       /*
        note(e, 200);
        delay(100);
        note(e, 200);
        delay(100);
        note(e, 400);
        delay(100);
        note(c, 200);
        delay(100);
        note(d, 200);
        delay(100);
        note(g, 400);
        delay(100);
        */
    }
    bool isDone()
    {
        return DrumController.isRunning();
    }
    void InitDrumAsSpeaker()
    {
        // Sets the two pins as Outputs
        pinMode(41, OUTPUT);
        pinMode(40, OUTPUT);
    }

    void note(int num, long dur)
    {
        del = (num * oct) / 10;
        dir = !dir;
        digitalWrite(dirPin, dir);
        coun = floor((dur * 5 * tempo) / del);
        for (int x = 0; x < coun; x++)
        {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(del);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(del);
        }
    }

    void pa(int durp)
    {
        int ker = floor(durp / 100) * tempo;
        delay(ker);
    }
};
