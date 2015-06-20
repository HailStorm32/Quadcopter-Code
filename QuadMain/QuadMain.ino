// Connect both Ground AND Signal wires to ESC input
int Chan1 = 3;  //Channel: 1 -- Aileron
int Chan2 = 9;  //Channel: 2 -- Elevator
int Chan3 = 10; //Channel: 3 -- Throttle
int Chan4 = 11; //Channel: 4 -- Rudder
int arm = 1000; // pulse width in microseconds for arming
int speedvalue = 1000; // pulse width in microseconds for operation  // TESTED (5/19/15) Equal to 1000 is idol || Greater than (possibly lower) 1150 is motor movement
int currentSpeed = 1090;

#include "Arduino.h"
#include "QuadFunctions.h"

void setup() 
 {
   pinMode(Chan1, OUTPUT);
   pinMode(Chan2, OUTPUT);
   pinMode(Chan3, OUTPUT);
   pinMode(Chan4, OUTPUT);   

//VVVV----------USE ONLY WITH JUST ESC----------VVVV
/* It appears that the ESC will accept a range of values for the arming
sequence. This provides 10 pulses with a pulse width of 1000 us with
with a 20 ms delay between pulses. */

//   delay(8000); //Allow for battery to be pluged in
//
//
//   for (int count = 0; count < 10; count++){
//      digitalWrite(Chan1, HIGH);
//      delayMicroseconds(arm);
//      digitalWrite(Chan1, LOW);
//     delay(20);
//   }
//   
//      for (int count = 0; count < 10; count++){
//      digitalWrite(Chan2, HIGH);
//      delayMicroseconds(arm);
//      digitalWrite(Chan2, LOW);
//     delay(20);
//   }
//   
//      for (int count = 0; count < 10; count++){
//      digitalWrite(Chan3, HIGH);
//      delayMicroseconds(arm);
//      digitalWrite(Chan3, LOW);
//     delay(20);
//     
//      for (int count = 0; count < 10; count++){
//      digitalWrite(Chan4, HIGH);
//      delayMicroseconds(arm);
//      digitalWrite(Chan4, LOW);
//     delay(20);
//   }

//^^^^----------USE ONLY WITH JUST ESC----------^^^^   


}

void loop()
  {
    
    //FourChan(Chan1, Chan2, Chan3, Chan4, 1000, 1000, 1000, 1000, 2500); //"Prime" esc's (esc testing only) 
    
    delay(8000); //alow time to plug in battery
    
    BoardArm(Chan3, Chan4);

    ThrSet(Chan3, 1290, 1);
    

    

    
    unArm(Chan3, Chan4);
  }

