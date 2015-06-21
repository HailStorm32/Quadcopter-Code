// =======VVVVVV===========================VVVVVV==================
// ===           START GYRO DECLARATIONS/INCLUDES               ===
// =======VVVVVV===========================VVVVVV==================
#include <Wire.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"

// specific I2C addresses may be passed as a parameter here
MPU6050 mpu;

#define LED_PIN 13 
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
   
// ==========VVVVVV========================VVVVVV==================
// ===              START GYRO INITIAL SETUP                    ===
// ==========VVVVVV========================VVVVVV==================

// join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

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
  //  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

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

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
// ==========^^^^^^========================^^^^^^=================
// ===               END GYRO INITIAL SETUP                    ===
// ==========^^^^^^========================^^^^^^=================


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

#include "Gyro.h"

void loop()
  {
    
    Data();
   
    //FourChan(Chan1, Chan2, Chan3, Chan4, 1000, 1000, 1000, 1000, 2500); //"Prime" esc's (esc testing only) 
    
//    delay(8000); //alow time to plug in battery
//    
//    BoardArm(Chan3, Chan4);
//
//    ThrSet(Chan3, 1290, 1);
//    
//    unArm(Chan3, Chan4);
  }

