#include <Servo.h>
#include "macros.h"


//Top Pusher pushes things out with exception of the yellow Duck
//Bottom Pusher pushes things into position, except for the yellow duch which it pushes out



enum class PusherStatus {   
    RETRACTED,   
    LOADING,                 //Bot 
    LOADED,
    RETRACTING,  
    HALF_UNLOADING,  
    HALF_UNLOADED,   
    UNLOADING,                      //top or bottom
    UNLOADED           

};


class PusherINO {
  private:
    // Private attributes
    Servo PusherTopServo;
    Servo PusherBottomServo;
    const int PusherTopPin = 5;
    const int PusherBottomPin = 6;

    // Private helper function to wait for a certain amount of time
    void wait(unsigned long duration) {
      unsigned long startTime = millis();
      while (millis() - startTime < duration) {
        // Do nothing while waiting
      }
    }

    public:
    // Public attributes
    PusherStatus status;
  

    PusherINO() :
        status(PusherStatus::RETRACTED)
        
        
    {
      PusherTopServo.attach(PusherTopPin);
      PusherBottomServo.attach(PusherBottomPin);
      PusherTopServo.write(0);
      PusherBottomServo.write(130);
        wait(5000);
      RetractPusher(BOTH_PUSHERS); //initial condition 

    }

    void RetractPusher(int top_bottom_both){
        status = PusherStatus::RETRACTING;
        if(top_bottom_both == BOTH_PUSHERS){
          //good values so far might have to change the retraction
        PusherTopServo.write(0);
        wait(2000);
        PusherBottomServo.write(130);
        wait(2000);
        PusherTopServo.write(100);
        wait(2000);
        PusherBottomServo.write(-80);
        wait(2000);
        PusherTopServo.write(0);
        wait(2000);
        PusherBottomServo.write(130);
        wait(2000);
        }

        else  if(top_bottom_both == TOP_PUSHER){
        PusherTopServo.write(RETURN);
        wait(2000);
        }

        else  if(top_bottom_both == BOTTOM_PUSHER){
        PusherBottomServo.write(PUSH_ACTION);
        wait(2000);
        }        
        status = PusherStatus::RETRACTED;
    }


    void LoadPillar(int which_pillar){
        if(which_pillar == FIRST_PILLAR){
        status = PusherStatus::LOADING;
        PusherBottomServo.write(PUSH_ACTION);
        wait(2000);
        
        }

        else if(which_pillar == SECOND_PILLAR){
        status = PusherStatus::LOADING;
        PusherBottomServo.write(HALF_PUSH);
        wait(2000);
        }

        status = PusherStatus::LOADED;

    }


    void LoadDuck(){
        status = PusherStatus::LOADING;
        PusherBottomServo.write(PUSH_ACTION);
        wait(2000);
        RetractPusher(BOTTOM_PUSHER);
    }

    void UnloadPillar(int which_pillar){
        if(which_pillar ==  FIRST_PILLAR){
        status = PusherStatus::HALF_UNLOADING;
        PusherTopServo.write(60);
        wait(2000);
        status = PusherStatus::HALF_UNLOADED;
        RetractPusher(TOP_PUSHER);
        }

        else if(which_pillar == SECOND_PILLAR){
        status = PusherStatus::UNLOADING;
        PusherTopServo.write(120);
        wait(2000);
        status = PusherStatus::UNLOADED;
        RetractPusher(TOP_PUSHER);
        }

    }

    void UnloadDuck(char color){
        if(color == YELLOW_DUCK_P){
        status = PusherStatus::UNLOADING;
        PusherBottomServo.write(160);
        wait(2000);
        RetractPusher(BOTTOM_PUSHER);
        status = PusherStatus::UNLOADED;
        }

        else if(color == PINK_DUCK_P){
        status = PusherStatus::UNLOADING;
        PusherTopServo.write(160);
        wait(2000);
        status = PusherStatus::UNLOADED;
        RetractPusher(TOP_PUSHER);
        }
    }

};




