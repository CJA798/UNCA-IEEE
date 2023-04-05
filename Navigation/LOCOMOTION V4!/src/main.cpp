#include <main.h>
DriverObject Driver;
BumperSwitches Bumpers;

// #include <Wire.h>

int State = 1;
char InputBit = 0;

void setup()
{
  // Wire2.begin();
  Serial.begin(115200);
  Serial.println("START");
};

void loop()
{

      Serial.print(InputBit);

      Driver.ComputeTranslation(0, 20);
      Driver.ComputeRotation(-PI);
      Driver.ComputeTranslation(-90, -17.5); // were at -90, 2.5

      // Drop red Chips

      // Drop Green Chips
    

      Driver.ComputeTranslation(20, 10);
    

      Driver.ComputeRotation(-PI/4);
    

        Driver.ComputeTranslation(-10, 20); // were at -90, 90
        
    
      Driver.ComputeTranslation(50, 30);
    
   // Driver.ComputeRotation(-PI/2);
    // Driver.Cleanup();

    
   /*
    InputBit = Serial.read();
    if(InputBit > 0){
    Driver.WASD(InputBit);
    }
    InputBit = 0;
    */
  
};
