#include <Servo.h>
#include "macros.h"


//Top Pusher pushes things out with exception of the yellow Duck
//Bottom Pusher pushes things into position, except for the yellow duch which it pushes out

PusherTopPin = 5;
PusherBottomPin = 6;

enum class PusherStatus {   
    RETRACTED   
    LOADING                 //Bot 
    LOADED
    RETRACTING  
    HALF_UNLOADING  
    HALF_UNLOADED   
    UNLOADING                      //top or bottom
    UNLOADED           

};


class Pusher {
  private:
    // Private attributes
    Servo PusherTopServo;
    Servo PusherBottomServo;
    

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


    PusherSet(){
        status = PusherStatus::RETRACTED;
        PusherTopServo.attach(PusherTopPin);
        PusherBottomServo.attach(PusherBottomPin);
        RetractPusher(2,0); //initial condition 
    }

    void RetractPusher(int how_many, Servo top_or_bottom){
        status = PusherStatus::RETRACTING;
        if(how_many == 2 && top_or_bottom == 0){
        PusherTopServo.write(0);
        PusherBottomServo.write(0);
        wait(2000);
        }

        else{
        top_or_bottom.write(0);
        wait(2000);
        }
    }


    void LoadPillar(int which_pillar){
        if(which_pillar == 1){
        status = PusherStatus::LOADING;
        PusherBottomServo.write(120);
        wait(2000);
        
        }

        else{
        status = PusherStatus::LOADING;
        PusherBottomServo.write(60);
        wait(2000);
        }

        status = PusherStatus::LOADED;

    }


    void LoadDuck(){
        status = PusherStatus::LOADING;
        PusherBottomServo.write(160);
        wait(2000);
        RetractPusher(1,PusherBottomServo);
    }

    void UnloadPillar(int which_pillar){
        if(which_pillar == 1){
        status = PusherStatus::HALF_UNLOADING;
        PusherTopServo.write(60);
        wait(2000);
        status.status = PusherStatus::HALF_UNLOADED;
        RetractPusher(1,PusherTopServo);
        }

        else{
        status.status = PusherStatus::UNLOADING;
        PusherTopServo.write(120);
        wait(2000);
        status.status = PusherStatus::UNLOADED;
        RetractPusher(1,PusherTopServo);
        }

    }

    void UnloadDuck(char color){
        if(color == y){
        status = PusherStatus::UNLOADING;
        PusherBottomServo.write(160);
        wait(2000);
        RetractPusher(1,PusherBottomServo);
        status = PusherStatus::UNLOADED;
        }

        else{
        status = PusherStatus::UNLOADING;
        PusherTopServo.write(160);
        wait(2000);
        status = PusherStatus::UNLOADED;
        RetractPusher(1,PusherTopServo);
        }
    }





}








PusherStatus getStatus(PusherStatus state) {
      return status;
    }
    
    void setStatus(PusherStatus state) {
      status = state;
    }


