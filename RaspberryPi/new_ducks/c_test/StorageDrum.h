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
//      3 -> Pillars
//      4 -> Pillars
//      5 -> Pillars
//
//    Positions:
//      0 -> Closer to building area
//      1 -> Closer to elevator area
////////////////////////////////////////////////////////////////////////////////
// Everything is pushed in by the bottom servo
// The top servo pushes out the pink duck and cylinders
// The two yellow ducks are pushed out by the bottom servo

#include <Servo.h>
//#include <TeensyStep.h>
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

    // Private helper function to wait for a certain amount of time
    void wait(unsigned long duration) {
      unsigned long startTime = millis();
      while (millis() - startTime < duration) {
        // Do nothing while waiting
      }
    }

  public:
    StorageDrum(int pusherServoPin, int stepPin, int dirPin) {      
      pusherServo.attach(pusherServoPin);
      /*
      Stepper rotationStepper(stepPin, dirPin);
      rotationStepper
        .setMaxSpeed(MAX_MTR_SPEED)      // steps/s
        .setAcceleration(MAX_MTR_ACCEL); // steps/s^2  
      StepControl controller; // Stepper controller object
      */
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
            i-1,      // color NOTE: change -1 -> i-1 for colored-slots
            false,    // inStorage
            i,        // slot
            j,        // position
          };
        }
      }
      pillars[5] = {GREEN_PILLAR, false, SIXTH_SLOT, SECOND_POSITION}; // initialize green pillar in red pillar slot
      pillars[6] = {WHITE_PILLAR, false, BRACING_SLOT, FIRST_POSITION}; // initialize the pillar in the bracing slot
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
    uint8_t whereToStore(int itemType) {
      switch (itemType) {
        case YELLOW_DUCK:
          if (!ducks[FIRST_YELLOW_DUCK].inStorage) {
            return (FIRST_SLOT << 3) | FIRST_POSITION; // Slot 0, position 0
          } else if (!ducks[SECOND_YELLOW_DUCK].inStorage) {
            return (THIRD_SLOT << 3) | FIRST_POSITION; // Slot 2, position 0
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

        case WHITE_PILLAR:
          if (!pillars[0].inStorage) {
            return (THIRD_SLOT << 3) | FIRST_POSITION;  // Slot 3, position 0
          }else if (!pillars[1].inStorage) {
            return (THIRD_SLOT << 3) | SECOND_POSITION; // Slot 3, position 1
          }
          //TODO: else, activate bracing, push white pillar, and
          break;
        
        // NOTE FOR FUTURE DEBUGGING: if the towers are messing up green and red, the following cases might be the reason why
        case GREEN_PILLAR:
          if (!pillars[2].inStorage) {
            return (FOURTH_SLOT << 3) | FIRST_POSITION; // Slot 4, position 0
          } else if (!pillars[3].inStorage) {
            return (FOURTH_SLOT << 3) | SECOND_POSITION;  // Slot 4, position 1
          } else if (!pillars[5].inStorage && pillars[4].inStorage){
            return (FIFTH_SLOT << 3) | SECOND_POSITION;  // Slot 4, position 1
          } else {
            //TODO: figure out what to do when the green that shares with red comes before red and there's no bracing available
            return NO_STORAGE_AVAILABLE; // No available storage
          }
          break;

        case RED_PILLAR:
          if (!pillars[4].inStorage) {
            return (FIFTH_SLOT << 3) | FIRST_POSITION;  // Slot 5, position 0
          } else if (!pillars[5].inStorage) {
            return (FIFTH_SLOT << 3) | SECOND_POSITION; // Slot 5, position 1
          } else {
            return NO_STORAGE_AVAILABLE; // No available storage
          }
          break;

        default:
          return NO_STORAGE_AVAILABLE;
          break;
      }
    }


    void rotateToSlot(int slot){
      //TODO: rotate to slot
      //int steps = slot * STEPS_PER_SLOT;
      //controller.rotate(steps);
    }

    void pushItem(bool position){
      if(position == PUSH_IN){
        pusherServo.write(45);
        wait(500);
        pusherServo.write(0);
        wait(500);
      }else if(position == PUSH_OUT){
        pusherServo.write(90);
        wait(500);
        pusherServo.write(0);
        wait(500);
      }else{
        pusherServo.writeMicroseconds(1500);
      }
    }

    bool addDuck(int color) {
      uint8_t storageLocation = whereToStore(color);
      if(storageLocation == NO_STORAGE_AVAILABLE){
        // TODO: handle duck by kicking it out of system
        return CANT_ADD_ITEM;
      }else{
          int slot = storageLocation >> 1;         // shift bits to the right by 1 to get the 3 bits representing the slot
          //TODO: rotate stepper to respective slot
          rotateToSlot(slot);
          //TODO: use pushServo to put item in. if position is 0, push for longer. if position is 1, push for a shorter time
          pushItem(PUSH_IN);
          ducks[slot].inStorage = TRUE;
          return ITEM_ADDED;
      }
    }
    
    bool addPillar(int color) {
      uint8_t storageLocation = whereToStore(color);
      if(storageLocation == NO_STORAGE_AVAILABLE){
        // TODO: handle pillar by activating bracing and pushing another pillar out
        return CANT_ADD_ITEM;      
      }else{
          int position = storageLocation & 0x1;    // extract LSB (position)
          int slot = storageLocation >> 1;         // shift bits to the right by 1 to get the 3 bits representing the slot
          //TODO: rotate stepper to respective slot
          rotateToSlot(slot);
          //TODO: use pushServo to put item in. if position is 0, push for longer. if position is 1, push for a shorter time
          pushItem(PUSH_IN);
          // NOTE FOR FUTURE DEBUGGING: If the drum is not storing, the next line might be the reason why
          pillars[slot+position].inStorage = TRUE;
          return ITEM_ADDED;
      }
    }
    
    bool removeDuck(int color) {
      switch (color)
      {
        case YELLOW_DUCK:
          if(ducks[FIRST_YELLOW_DUCK].inStorage){
            rotateToSlot(FIRST_SLOT);
            pushItem(PUSH_OUT);
            ducks[FIRST_YELLOW_DUCK].inStorage = FALSE;
            return ITEM_REMOVED;
          }else if(ducks[SECOND_YELLOW_DUCK].inStorage){
            rotateToSlot(THIRD_SLOT);
            pushItem(PUSH_OUT);
            ducks[SECOND_YELLOW_DUCK].inStorage = FALSE;
            return ITEM_REMOVED;
          }
          return CANT_REMOVE_ITEM;
        break;

        case PINK_DUCK:
          if(ducks[PINK_DUCK].inStorage){
            rotateToSlot(SECOND_SLOT);
            pushItem(PUSH_OUT);
            ducks[PINK_DUCK].inStorage = FALSE;
            return ITEM_REMOVED;
          }
          return CANT_REMOVE_ITEM;
        break;
      
        default:
          return CANT_REMOVE_ITEM;
        break;
      }
    }

    bool removePillar(int color) {
      switch (color){
        case WHITE_PILLAR:
          if (pillars[0].inStorage) {
            rotateToSlot(FOURTH_SLOT);
            // TODO: enable bracing when needed
            pushItem(PUSH_OUT);
            if (pillars[1].inStorage){    // if position 1 is filled, move item to position 0 and clear position 1
              pillars[1].inStorage = FALSE;
            } else{
              pillars[0].inStorage = FALSE;
            }
            // TODO: add code for full white slot but white incoming 
            return ITEM_REMOVED;
          } else if (pillars[1].inStorage){   // ideally, this should never happen
            rotateToSlot(FOURTH_SLOT);
            // TODO: enable bracing when needed
            pushItem(PUSH_OUT);
            pillars[1].inStorage = FALSE;
            return ITEM_REMOVED;
          }
          return CANT_REMOVE_ITEM;
        break;
        
        case GREEN_PILLAR:
          if (pillars[2].inStorage) {
            rotateToSlot(FIFTH_SLOT);
            pushItem(PUSH_OUT);
            if (pillars[3].inStorage){    // if position 1 is filled, move item to position 0 and clear position 1
              pillars[3].inStorage = FALSE;
            } else{
              pillars[2].inStorage = FALSE;
            }
            // TODO: add code for full green slot but green incoming 
            return ITEM_REMOVED;
          } else if (pillars[3].inStorage){   // ideally, this should never happen
            rotateToSlot(FIFTH_SLOT);
            // TODO: enable bracing when needed
            pushItem(PUSH_OUT);
            pillars[3].inStorage = FALSE;
            return ITEM_REMOVED;
          } else if (!pillars[4].inStorage && pillars[5].inStorage){
            rotateToSlot(SIXTH_SLOT);
            // TODO: enable bracing when needed
            pushItem(PUSH_OUT);
            pillars[5].inStorage = FALSE;
            return ITEM_REMOVED;
          }
          return CANT_REMOVE_ITEM;
        break;

        case RED_PILLAR:
          if (pillars[4].inStorage) {
            rotateToSlot(SIXTH_SLOT);
            // TODO: enable bracing when needed
            pushItem(PUSH_OUT);
            pillars[4].inStorage = FALSE;
            return ITEM_REMOVED;
          } else if (pillars[5].inStorage) {
            // TODO: figure out what to do when green comes before red
            return CANT_REMOVE_ITEM;
          }
        break;
        
        default:
          return CANT_REMOVE_ITEM;
        break;
      }
    }
};
