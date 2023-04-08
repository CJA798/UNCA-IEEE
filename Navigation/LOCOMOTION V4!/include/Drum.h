#include "Arduino.h"
#include <TeensyStep.h>
#include <math.h>
#include <macros.h>
#include <Ports.h>
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

#define OutYellowAng1 (7)
#define OutYellowAng2 (8)
#define OutPinkAng (9)
#define OutWhiteAng (10)
#define OutGreenAng (11)
#define OutRedAng (12)

class Drum
{
private:
    int CurrSteps;
    int CurrAngle;
    Stepper DrumStepper;
    StepControl DrumController;
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

    void doRotation(int SlotAng, bool rotChoice)
    {
        // The plan is that if we aren't currently spun to this angle then we move the amount to get there.
        // If we are already passed this angle then we have to spin all the way to reset and go back.
        if (rotChoice)
        {
            int angle = SlotAng - CurrAngle;
            int steps = RadToSteps() * angle;
            // TODO: DrumStepper move to steps
        }
        else
        {
            int angle = (2 * M_PI - CurrAngle) + SlotAng;
            int steps = RadToSteps() * angle;
            // TODO: DrumStepper move to steps
        };
    }

public:
    Drum() : DrumStepper(STORAGE_MTRDIR, STORAGE_MTRSTEP) // CONDSTRUCTOR

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
    };
    void moveDrumToLocation(char move)
    {
        switch (move)
        {
        case InYellowDuck1:
            // Inside of all the cases we have two different cases.
            // Either the rotation will be less than required for the next item
            // Or it will be more than.
            // If the item is more than then we will zero out the drum and move to
            // Next item. If not then we just move to the next item.
            doRotation(InYellowAng1, CurrAngle <= InYellowAng1);
            break;

        case InYellowDuck2:
            doRotation(InYellowAng2, CurrAngle <= InYellowAng2);

            break;

        case InPinkDuck:
            doRotation(InPinkAng, CurrAngle <= InPinkAng);

            break;

        case InWhiteColumn:
            doRotation(InWhiteAng, CurrAngle <= InWhiteAng);
            break;

        case InGreenColumn:
            doRotation(InGreenAng, CurrAngle <= InGreenAng);
            break;

        case InRedColumn:
            doRotation(InRedAng, CurrAngle <= InRedAng);

            break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // This is the out cases of the case statement
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