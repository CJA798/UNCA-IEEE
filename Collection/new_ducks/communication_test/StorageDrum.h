////////////////////////////////////////////////////////////////////////////////
//  Description: This file contains the StorageDrum class and item structs
//
//  Modified: Carlos Anzola (27 Mar 2023)
////////////////////////////////////////////////////////////////////////////////
//  The rotating drum keeps track of the stored items.
//  Ideally, when an item needs to be added to the drum, it checks if there's
//  enough space. If there's space, the drum rotates to the respective storage
//  spot and pushes the object in. If there's no space, the drum returns an
//  error, so that the elevator platform proceeds with the respective
//  correction protocol.
////////////////////////////////////////////////////////////////////////////////
//  Encoded types:
//    0 -> Duck
//         0: Yellow Duck
//         1: Pink Duck
//
//    1 -> Pillar
//         2: White Pillar
//         3: Green Pillar
//         4: Red Pillar
//  ---------------------------
//    Slots:
//      0 -> Yellow Duck (FIRST_YELLOW_DUCK)
//      1 -> Pink Duck
//      2 -> Yellow Duck (SECOND_YELLOW_DUCK)
//      3 -> Two White Pillars
//      4 -> Two Green Pillars
//      5 -> Red and White Pillars
//
//    Positions:
//      0 -> Closer to building area
//      1 -> Closer to elevator area
////////////////////////////////////////////////////////////////////////////////

#include <Servo.h>
#include <TeensyStep.h>
#include "macros.h"

// Stepper Settings
#define MAX_MTR_SPEED (7500) // Steps/s maximum is 300000
#define MAX_MTR_ACCEL (50)   // Steps/s^2 skips at around 15000

struct Duck {
  int color; // 0: Yellow, 1: Pink
  bool inStorage;
  int slot; // 0-2 (pink is exclusive)
};

struct Pillar {
  int color; // 2: White, 3: Green, 4: Red
  bool inStorage;
  int slot; // 3-5
  int position; // 0-1 (two pillars can be in each slot)
};


class StorageDrum {
  private:
    Pillar pillars[7];
    Duck ducks[3];
    Servo pusherServo;
    StepControl controller; // Stepper controller object

  public:
    StorageDrum(int pusherServoPin, int stepPin, int dirPin) {      
      pusherServo.attach(pusherServoPin);
      Stepper rotationStepper(stepPin, dirPin)
      rotationStepper
        .setMaxSpeed(MAX_MTR_SPEED)      // steps/s
        .setAcceleration(MAX_MTR_ACCEL); // steps/s^2  

      ducks[0] = {
        YELLOW,     // color
        false,      // inStorage
        FIRST_SLOT  // slot
      };
      ducks[1] = {PINK, false, SECOND_SLOT};
      ducks[2] = {YELLOW, false, THIRD_SLOT}; 
        
      for (int i = 3; i < 6; i++) {
        for (int j = 0; j < 2; j++) {        
          pillars[2*i+j-6] = {
            -1,       // color NOTE: change -1 -> i-1 for colored-slots
            false,    // inStorage
            i,       // slot
            j,       // position
          };
        }
      }
      pillars[6] = {-1, false, -1, -1}; // initialize pillar that doesn't fit
    }
    

    int getDuckCount(int color) {
      int count = 0;
      for (int i = 0; i < 3; i++) {
        if (ducks[i].inStorage && (color == ALL || ducks[i].color == color)) {
          count++;
        }
      }
      return count;
    }

    int getPillarCount(int color) {
      int count = 0;
      for (int i = 0; i < 7; i++) {
        if (pillars[i].inStorage && (color == ALL || pillars[i].color == color)) {
          count++;
        }
      }
      return count;
    }

    int getItemCount() {
      return getDuckCount(ALL) + getPillarCount(ALL);
    }

    // Returns a 4 bit number where the first 3 bits from MSB to LSB are the slot number, and the LSB is the position
    int whereToStore(int itemType) {
      switch (itemType) {
        case YELLOW_DUCK:
          if (!ducks[FIRST_YELLOW_DUCK].inStorage) {
            return (FIRST_SLOT << 3) | FIRST_POSITION; // Slot 0, position 0
          } else if (!ducks[SECOND_YELLOW_DUCK].inStorage) {
            return (THIRD_SLOT << 3) | FIRST_POSITION; // Slot 1, position 1
          } else {
            return NO_STORAGE_AVAILABLE; // No available storage
          }
          break;
          
        case PINK_DUCK:
          if (!ducks[PINK_DUCK].inStorage) {
            return (SECOND_SLOT << 3) | FIRST_POSITION; // Slot 1, position 0
          } else {
            return NO_STORAGE_AVAILABLE; // No available storage
          }
          break;


        default:
          // check if itemType is a pillar
          if (itemType >= WHITE_PILLAR && itemType <= RED_PILLAR) {
            for (int i = 3; i < 6; i++) {
              for (int j = 0; j < 2; j++) {        
                if (!pillars[2*i+j-6].inStorage) {
                  // calculate the slot and position as a 4-bit number
                  int slot = i;
                  int position = j;
                  return (slot << 3) | position;
                }
              }
            }
          }
          return NO_STORAGE_AVAILABLE;
          break;
      }
    }
    
};
