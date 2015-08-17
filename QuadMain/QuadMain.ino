// =======VVVVVV===========================VVVVVV==================
// ===           START GYRO DECLARATIONS/INCLUDES               ===
// =======VVVVVV===========================VVVVVV==================
#include <Wire.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"

// specific I2C addresses may be passed as a parameter here
MPU6050 mpu;


bool blinkState = true;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}
// =======^^^^^^===========================^^^^^^==================
// ===            END GYRO DECLARATIONS/INCLUDES                ===
// =======^^^^^^===========================^^^^^^==================

//NOTE: Connect both Ground AND Signal wires to ESC input
int Chan1 = 3;  //Channel: 1 -- Aileron
int Chan2 = 9;  //Channel: 2 -- Elevator
int Chan3 = 10; //Channel: 3 -- Throttle
int Chan4 = 11; //Channel: 4 -- Rudder
int arm = 1000; // pulse width in microseconds for arming
int currentSpeed = 1090; //Currenly not is use. Could be deleted later
int Yaw = 0;  //Holds yaw axis value
int Pitch = 0; //Holds Pitch axis value
int Roll = 0;  //Holds Roll axis value
int Skip = 0;  
int Yaw_max = 0; //Max allowable value for Yaw axis
int Yaw_min = 0; //Min allowable value for Yaw axis
int Pitch_max = 0; //Max allowable value for Pitch axis
int Pitch_min = 0; //Min allowable value for Pitch axis
int Roll_max = 0;  //Max allowable value for Roll axis           
int Roll_min = 0;  //Min allowable value for Roll axis



//#include "Arduino.h"  //Code will complain without it

#include "QuadFunctions.h"





void setup() 
 {
   pinMode(Chan1, OUTPUT);
   pinMode(Chan2, OUTPUT);
   pinMode(Chan3, OUTPUT);
   pinMode(Chan4, OUTPUT);
   pinMode(12 , OUTPUT);
   pinMode(8 , OUTPUT);
   pinMode(13, OUTPUT);
   pinMode(4, OUTPUT);


   
// ==========VVVVVV========================VVVVVV==================
// ===              START GYRO INITIAL SETUP                    ===
// ===         CREDIT TO Jeff Rowberg FOR GYRO CODE             ===
// ===         https://github.com/jrowberg/i2cdevlib            ===
// ==========VVVVVV========================VVVVVV==================

// join I2C bus (I2Cdev library doesn't do this automatically)
//    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
//        Wire.begin();
//        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
//    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
//        Fastwire::setup(400, true);
//    #endif

    Serial.begin(115200);

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

// ==========^^^^^^========================^^^^^^=================
// ===              END GYRO INITIAL SETUP                     ===
// ===         CREDIT TO Jeff Rowberg FOR GYRO CODE            ===
// ===         https://github.com/jrowberg/i2cdevlib           ===
// ==========^^^^^^========================^^^^^^=================



//VVVV----------USE ONLY WITH JUST ESC (TESTING ONLY)----------VVVV
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
//^^^^----------USE ONLY WITH JUST ESC (TESTING ONLY)----------^^^^

}


void loop()
  {

// ==========VVVVVV========================VVVVVV==================
// ===               START READ GYRO VALUES                     ===
// ===         CREDIT TO Jeff Rowberg FOR GYRO CODE             ===
// ===         https://github.com/jrowberg/i2cdevlib            ===
// ==========VVVVVV========================VVVVVV==================    
      // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);           
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print(", ");             
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print(", ");                         
            Serial.println(ypr[2] * 180/M_PI);

            
            Yaw = ypr[0] * 180/M_PI;
            Pitch = ypr[1] * 180/M_PI;
            Roll = ypr[2] * 180/M_PI;
// ==========^^^^^^========================^^^^^^=================
// ===               STOP READ GYRO VALUES                    ===
// ===         CREDIT TO Jeff Rowberg FOR GYRO CODE            ===
// ===         https://github.com/jrowberg/i2cdevlib           ===
// ==========^^^^^^========================^^^^^^=================


////////VVVVVVVV//////////////RUNS ONCE (Calibration of Gyro)/////////////VVVVVVVV//////////////////////          
         
         line249:
         
         if(Skip == 10)
         {
           //Set min & max range for the axis
            Yaw_max = Yaw + 10;
            Yaw_min = Yaw - 10;
            Pitch_max = Pitch + 10;
            Pitch_min = Pitch - 10;
            Roll_max = Roll + 10;             
            Roll_min = Roll - 10;             
            
            //print min & max values
            Serial.print(Yaw_max);
            Serial.print(", ");             
            Serial.print(Yaw_min);
            Serial.print(", ");                         
            Serial.println(Pitch_max);            
            Serial.print(", ");                         
            Serial.print(Pitch_min);
            Serial.print(", ");             
            Serial.print(Roll_max);
            Serial.print(", ");                         
            Serial.println(Roll_min);            
            
            
            digitalWrite(4, LOW);   //turn off red led (Status led)
            digitalWrite(13, HIGH);  //turn on green led (Status led)      

            Skip = 5; //set skip to 5 as not to run these process again
         }
         
         if(Skip == 0)
          { 
            //Set timer for 20 sec
            int millisec = 20000;
           
            int stoptime = 0;

            time = millis();

            stoptime = time + millisec;
            Serial.println("Calibrating Gyro....");                         
            
            //Start timer
            //would use the default timmer but the gyro doesn't seem to like
            while(true)
            {   
               digitalWrite(4, HIGH); //turn on red led (Status led)
               
               time = millis();
               
               if(time > stoptime)
               {
                BoardArm(Chan3, Chan4); //Arm the board
                Skip = 10;
                goto line249;             
                
               }
             }
          }
///////////^^^^^^^////////////RUNS ONCE (Calibration of Gyro)//////////////^^^^^^^^//////////////////                     


////////VVVVVVVV////////////// YAW AXIS CORRECTION /////////////VVVVVVVV//////////////////////          
          
            //If yaw axis is greater than the max value, output yaw PWM to compensate
            if(Yaw > Yaw_max)
            {
              
            SingleChan(Chan4, 1665, 2); //Write PWM singal with a pulse width of 1665 us for 2 milliseconds to channel 4 (Rudder)          
           
            //These have no purpose other than to act as a debugger tool              
            digitalWrite(8, LOW);   
            digitalWrite(12, HIGH); 
               
            }
            
            //If yaw axis is less than the min value, output yaw PWM to compensate            
            else if(Yaw < Yaw_min)
            {
              
            SingleChan(Chan4, 1265, 2); //Write PWM singal with a pulse width of 1265 us for 2 milliseconds to channel 4 (Rudder)
            
            //These have no purpose other than to act as a debugger tool                
            digitalWrite(8, HIGH);   
            digitalWrite(12, LOW); 
              
            }
            
            //Else do nothing
            else
            {             
            
            //These have no purpose other than to act as a debugger tool  
            digitalWrite(8, LOW);   
            digitalWrite(12, LOW); 
             
            }
            
///////////^^^^^^^//////////// YAW AXIS CORRECTION ///////////////^^^^^^^^//////////////////            
         
         
            
////////VVVVVVVV////////////// PITCH AXIS CORRECTION /////////////VVVVVVVV//////////////////////          
            
            //If pitch axis is greater than the min value, output yaw PWM to compensate
            if(Pitch > Pitch_max)
            {
              
             //CODE HERE              
              
            }
            
            //If pitch axis is less than the min value, output yaw PWM to compensate
            else if(Pitch < Pitch_min)
            {
              
             //CODE HERE  
             
            }

            //Else do nothing
            else
            {
              
             //CODE HERE   
             
            }  
            
///////////^^^^^^^//////////// YAW AXIS CORRECTION ///////////////^^^^^^^^//////////////////



////////VVVVVVVV////////////// ROLL AXIS CORRECTION /////////////VVVVVVVV//////////////////////
            //If roll axis is greater than the max value, output yaw PWM to compensate
            if(Roll > Roll_max)
            {
              
             //CODE HERE   
             
            }
            
            //If roll axis is less than the min value, output yaw PWM to compensate
            else if(Roll < Roll_min)
            {  
              
             //CODE HERE   
             
            }
            
            //Else do nothing
            else
            {
              
             //CODE HERE 
             
            }
///////////^^^^^^^//////////// ROLL AXIS CORRECTION ///////////////^^^^^^^^//////////////////

  }            
}
    
