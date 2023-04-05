#include "PusherINO.h"
#include "macros.h"

Pusher pusher;

void setup() {
  pinMode(7, INPUT); // set pin 7 as input
}

void loop() {
  int pin7State = digitalRead(7); // read the state of pin 7
  if (pin7State == HIGH) {
    pusher.RetractPusher(2, 0); // move servo to 180 degrees if pin 7 is high
  } else {
    pusher.LoadPillar(1); // move servo to 180 degrees if pin 7 is high
  }

}
