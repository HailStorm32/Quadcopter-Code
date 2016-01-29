//==========================================================================\\
//                                                                          \\
//  MUST INSTALL "MPU6050" AND "I2Cdev" LIBRARIES BEFORE FIRST COMPILE!!!   \\
//                                                                          \\
//  FROM HERE (https://github.com/jrowberg/i2cdevlib/tree/master/Arduino)   \\
//                                                                          \\
//==========================================================================\\




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
uint8_t Chan1 = 3;  //Channel: 1 -- Aileron
uint8_t Chan2 = 9;  //Channel: 2 -- Elevator
uint8_t Chan3 = 10; //Channel: 3 -- Throttle
uint8_t Chan4 = 11; //Channel: 4 -- Rudder
int arm = 1000; // pulse width in microseconds for arming
int currentSpeed = 1090; //Currenly not is use. Could be deleted later
int Yaw = 0;  //Holds yaw axis value
int Pitch = 0; //Holds Pitch axis value
int Roll = 0;  //Holds Roll axis value
uint8_t Skip = 0;
int Yaw_max = 0; //Max allowable value for Yaw axis
int Yaw_min = 0; //Min allowable value for Yaw axis
int Pitch_max = 0; //Max allowable value for Pitch axis
int Pitch_min = 0; //Min allowable value for Pitch axis
int Roll_max = 0;  //Max allowable value for Roll axis
int Roll_min = 0;  //Min allowable value for Roll axis
bool yawFlag = false; //Makes sure code doesn't proceed if Yaw_max is incorrect
bool pitchFlag = false; //Makes sure code doesn't proceed if Pitch_max is incorrect
bool rollFlag = false; //Makes sure code doesn't proceed if Roll_max is incorrect
int Val_hold = 0; // Temp hold for axis max check
int X_off = 0; //Offset for Yaw axis
int Y_off = 0; //Offset for Pitch axis
int Z_off = 0; //Offset for Roll axis



#include "QuadFunctions.h"
#include "Gyro_Calibration.h"





void setup()
{
	pinMode(Chan1, OUTPUT);
	pinMode(Chan2, OUTPUT);
	pinMode(Chan3, OUTPUT);
	pinMode(Chan4, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(4, OUTPUT);

  pinMode(7, OUTPUT);



	// ==========VVVVVV========================VVVVVV==================
	// ===              START GYRO INITIAL SETUP                    ===
	// ===         CREDIT TO Jeff Rowberg FOR GYRO CODE             ===
	// ===         https://github.com/jrowberg/i2cdevlib            ===
	// ==========VVVVVV========================VVVVVV==================
	int TWBR;
	// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
	Wire.begin();
	TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
	Fastwire::setup(400, true);
#endif

	Serial.begin(115200); //115200

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
	}
	else {
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

line207:
	
	if (Skip == 30)
	{
		X_off = Yaw;
		Y_off = Pitch;
		Z_off = Roll;

		Skip = 10;
	}



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
	}
	else if (mpuIntStatus & 0x02) {
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

		//mpu.dmpGetQuaternion(&q, fifoBuffer);
		//mpu.dmpGetGravity(&gravity, &q);
		//mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		
		//Serial.print(ypr[0] * 360 / M_PI);
		//Serial.print(", ");
		//Serial.print(ypr[1] * 360 / M_PI);
		//Serial.print(", ");
		//Serial.println(ypr[2] * 360 / M_PI);


		Yaw = ypr[0] * 360 / M_PI;
		Pitch = ypr[1] * 360 / M_PI;
		Roll = ypr[2] * 360 / M_PI;

		Yaw = Yaw - X_off;
		Pitch = Pitch - Y_off ;
		Roll = Roll - Z_off;

		Serial.print(Yaw);
		Serial.print(", ");
		Serial.print(Pitch);
		Serial.print(", ");
		Serial.println(Roll);

	// ==========^^^^^^========================^^^^^^=================
	// ===               STOP READ GYRO VALUES                     ===
	// ===         CREDIT TO Jeff Rowberg FOR GYRO CODE            ===
	// ===         https://github.com/jrowberg/i2cdevlib           ===
	// ==========^^^^^^========================^^^^^^=================


		////////VVVVVVVV//////////////RUNS ONCE (Calibration of Gyro)/////////////VVVVVVVV//////////////////////

		if (Skip == 25)
		{
			Skip = 30;
			goto line207;
		}
		
		
		if (Skip == 10)
		{
			Gyro_Calibration(); //Contains more code for gyro calibration
			Skip = 5; //set skip to 5 as not to run these process again
		}


		if (Skip == 0)
		{
			//Set timer for 20 sec
			int millisec = 20000;

			int stoptime = 0;

			time = millis();

			stoptime = time + millisec;
			Serial.println("");
			Serial.println("Yaw Max: ");
			Serial.print(Yaw_max);
			Serial.println("Yaw Min: ");
			Serial.print(Yaw_min);
			Serial.println("Calibrating Gyro....");

			//Start timer
			//would use the default timmer but the gyro doesn't seem to like
			while (true)
			{
				digitalWrite(4, HIGH); //turn on red led (Status led)

				time = millis();

				if (time > stoptime)
				{

					digitalWrite(7, HIGH);
					
					BoardArm(Chan3, Chan4); //Arm the board
					Skip = 25;

					digitalWrite(7, LOW);
					goto line207;

				}
			}
		} 
		
		///////////^^^^^^^////////////RUNS ONCE (Calibration of Gyro)//////////////^^^^^^^^//////////////////





		////////VVVVVVVV////////////// YAW AXIS CORRECTION /////////////VVVVVVVV//////////////////////

		//If yaw axis is greater than the max value, output yaw PWM to compensate
		if (Yaw > Yaw_max)
		{

			//  SingleChan(Chan4, 1665, 1000); //Write PWM singal with a pulse width of 1665 us for 2 milliseconds to channel 4 (Rudder)

			//These have no purpose other than to act as a debugger tool
			digitalWrite(8, LOW); //Right LED
			digitalWrite(12, HIGH); //Left LED

		}

		//If yaw axis is less than the min value, output yaw PWM to compensate
		else if (Yaw < Yaw_min)
		{

			// SingleChan(Chan3, 1265, 250);  //NOT IMPORTANT

			SingleChan(Chan4, 1570, 10); //Write PWM singal with a pulse width of 1265 us for 2 milliseconds to channel 4 (Rudder)

		   //These have no purpose other than to act as a debugger tool
			digitalWrite(8, HIGH); //Right LED
			digitalWrite(12, LOW); //Left LED

		}

		//SingleChan(Chan3, 1200, 250);

		//Else do nothing
		else if (Yaw > Yaw_min && Yaw < Yaw_max)
		{

			SingleChan(Chan3, 1200, 10);

			//These have no purpose other than to act as a debugger tool
			digitalWrite(8, LOW); //Right LED
			digitalWrite(12, LOW); //Left LED

		}

		///////////^^^^^^^//////////// YAW AXIS CORRECTION ///////////////^^^^^^^^//////////////////



		////////VVVVVVVV////////////// PITCH AXIS CORRECTION /////////////VVVVVVVV//////////////////////

		//If pitch axis is greater than the min value, output yaw PWM to compensate
		if (Pitch > Pitch_max)
		{

			//CODE HERE

		}

		//If pitch axis is less than the min value, output yaw PWM to compensate
		else if (Pitch < Pitch_min)
		{

			//CODE HERE

		}

		//Else do nothing
		else
		{

			//CODE HERE

		}

		///////////^^^^^^^//////////// PITCH AXIS CORRECTION ///////////////^^^^^^^^//////////////////



		////////VVVVVVVV////////////// ROLL AXIS CORRECTION /////////////VVVVVVVV//////////////////////
		//If roll axis is greater than the max value, output yaw PWM to compensate
		if (Roll > Roll_max)
		{

			//CODE HERE

		}

		//If roll axis is less than the min value, output yaw PWM to compensate
		else if (Roll < Roll_min)
		{

			//CODE HERE

		}

		//Else do nothing
		else
		{

			//CODE HERE

		}
		///////////^^^^^^^//////////// ROLL AXIS CORRECTION ///////////////^^^^^^^^////////////////////

	}


}

