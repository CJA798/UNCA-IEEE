#include <Servo.h>
#include "macros.h"


//Top Pusher pushes things out with exception of the yellow Duck
//Bottom Pusher pushes things into position, except for the yellow duch which it pushes out



enum class BraceStatus {   
    OPEN,
    CLOSE
};


class BraceINO {
  private:
    // Private attributes
    Servo BraceTopServo;
    Servo BraceBottomServo;
    Servo SwingServo; 
    
    const int BraceTopPin = 7;
    const int BraceBottomPin = 8;
    const int SwingServo = 9;

    // Private helper function to wait for a certain amount of time
    void wait(unsigned long duration) {
      unsigned long startTime = millis();
      while (millis() - startTime < duration) {
        // Do nothing while waiting
      }
    }

    public:
    // Public attributes
    BraceStatus status;
  

    BraceINO() :
        status(PusherStatus::OPEN)
        
        
    {
        BraceTopServo.attach(BraceTopPin);
        BraceBottomServo.attach(BraceBottomPin);
        SwingServo.attach(SwingServo);
       //open method here for initial condition 
       BraceOpen(BRACE_BOTH, OPEN_VAL);
    }

    void BraceAction(int top_bottom_swing, int open_or_close){  //3 servos to move, which servo, which direction 
        status = BraceStatus::OPEN;


        //initial condition
        if(top_bottom_swing == BRACE_BOTH){ //3 means top and bottom at the same time
            BraceTopServo.write(open_or_close);
            wait(2000);
            BraceBottomServo.write(open_or_close);
            wait(2000);
            
        }


        //top brace opens, 0 is top. User decides wheter open or close
        else  if(top_bottom_swing == BRACE_TOP){  
        BraceTopServo.write(open_or_close);
        wait(2000);
        }


        //bottom brace open or close
        else  if(top_bottom_swing == BRACE_BOTTOM){
        BraceBottomServo.write(open_or_close);
        wait(2000);
        }        

        //swing open or close
        else  if(top_bottom_swing == SWING){ 
        BraceBottomServo.write(open_or_close);
        wait(2000);
        }        

        
    }
    

    //once two pillars are loaded call this
    void Towers(int how_high){
        if(how_high == 2){
        //opens top brace
        BraceAction(BRACE_TOP,OPEN_VAL);
        wait(2000);
        //swings top brace, gets ready to push in duck therefore waits longer
        BraceAction(SWING, OPEN_VAL);
        wait(7000);
        //tower ready, bottom brace releases
        BraceAction(BRACE_BOTTOM, OPEN_VAL);
        wait(7000);
        //returns swing to initial position
        BraceAction(SWING, CLOSED_VAL);
        wait(5000);
        //returns braces to closed position, waits for next tower
        BraceAction(BRACE_BOTH, CLOSED_VAL);
        }

        else if(how_high == 3){
            //for 3 high once the duck is loaded, both braces have to open, wait extra time for testing
             BraceAction(BRACE_BOTH, OPEN_VAL);
             wait(2000);

             //then just close them back up and wait for next tower
              BraceAction(BRACE_BOTH, CLOSED_VAL);

        }
    }




};









   