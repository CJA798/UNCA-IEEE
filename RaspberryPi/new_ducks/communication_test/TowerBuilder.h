////////////////////////////////////////////////////////////////////////////////
//  Description: This file contains the TowerBuilder class
//
//  Modified: Carlos Anzola (27 Mar 2023)
////////////////////////////////////////////////////////////////////////////////
// bottom servo controls first and second level bracing (bot to top)
// upper servo controls third level bracing
// upper servo probably controls additional duck support
// if not, its functionality will be split in two servos
#include <Servo.h>
#include "macros.h"

enum class Tower_Builder_State {
  BRACING_OFF,
  SMALL_TOWER_BRACING_ON,
  BIG_TOWER_BRACING_ON,
  WHITE_DONE,
  WHITE_GREEN_DONE,
  WHITE_GREEN_RED_DONE,
  TOWER_DONE,
  TOWER_BUILDER_ERROR_UNKNOWN
};

class TowerBuilder {
  private:
    // Private attributes
    // Servos

    // Private helper function to wait for a certain amount of time
    void wait(unsigned long duration) {
      unsigned long startTime = millis();
      while (millis() - startTime < duration) {
        // Do nothing while waiting
      }
    }


  public:
    // Public attributes
    Tower_Builder_State state;

    // Constructor to initialize the class attributes    
    TowerBuilder(/* Servo Pins */) {
      state = Tower_Builder_State::BRACING_OFF;   
      // Servo Attach
    }

    void setBracing(bool bracingSize) {
      if (bracingSize == SMALL_BRACING) {
        // TODO: code to enable small bracing
        state = Tower_Builder_State::SMALL_TOWER_BRACING_ON;
      } else {
        // TODO: code to enable big bracing
        state = Tower_Builder_State::BIG_TOWER_BRACING_ON;
      }
    }

    void removeBracing() {
        // TODO: code to remove any bracing
        state = Tower_Builder_State::BRACING_OFF;
    }
}