int time = 0;

//VVVVVVV---------------------------BoardArm------------------------------------------------------------------------------VVVVV
void BoardArm(int chanVal1, int chanVal2)
{
  int millisec = 1500;
  
  int stoptime = 0;

  time = millis();

  stoptime = time + millisec;
  
  while(true) //Needs to be in loop to provide complete wave forms
  {
    digitalWrite(chanVal1, HIGH);
    delayMicroseconds(1086);
    digitalWrite(chanVal1, LOW);
    
    digitalWrite(chanVal2, HIGH);
    delayMicroseconds(1875);
    digitalWrite(chanVal2, LOW);        
   
    time = millis();
   
    if(time > stoptime)
    {
      break;
    }
    
  } 
}
//^^^^^^^---------------------------BoardArm------------------------------------------------------------------------------^^^^^


//VVVVVVV---------------------------SingleChan------------------------------------------------------------------------------VVVVV
void SingleChan(int chanVal, int speed, int millisec)
{
  
  if(millisec > 0)
  {
  
  int stoptime = 0;

  time = millis();

  stoptime = time + millisec;
  
  while(true) //Needs to be in loop to provide complete wave forms
  {
    digitalWrite(chanVal, HIGH);
    delayMicroseconds(speed);
    digitalWrite(chanVal, LOW);
    delay(20);
   
    time = millis();
   
    if(time > stoptime)
    {
      break;
    }
  }
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  else if(millisec == 0)
  {
   
    int Stop;
    
  while(true) //Needs to be in loop to provide complete wave forms
  {
     
    line73:    
    
    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }

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

////////VVVVVVVV//////////////RUNS ONCE/////////////VVVVVVVV//////////////////////          
         
         if(Skip == 10)
         {
            YavgMx = Yaw + 10;
            YavgMi = Yaw - 10;
            PavgMx = Pitch + 10;
            PavgMi = Pitch - 10;
            RavgMx = Roll + 10;             
            RavgMi = Roll - 10;             

            Serial.print(YavgMx);
            Serial.print(", ");             
            Serial.print(YavgMi);
            Serial.print(", ");                         
            Serial.println(PavgMx);            
            Serial.print(", ");                         
            Serial.print(PavgMi);
            Serial.print(", ");             
            Serial.print(RavgMx);
            Serial.print(", ");                         
            Serial.println(RavgMi);            

            digitalWrite(4, LOW);
            digitalWrite(13, HIGH);          

            Skip = 5;
         }
         
         if(Skip == 0)
          { 
            int millisec = 20000;
           
            int stoptime = 0;

            time = millis();

            stoptime = time + millisec;
            Serial.println("Calibrating Gyro....");                         
  
            while(true) 
            {   
               digitalWrite(4, HIGH);
               
               time = millis();
               
               if(time > stoptime)
               {
                BoardArm(Chan3, Chan4);
                Skip = 10;             
                goto line73;
                //break;
               }
             }
          }
          
///////////^^^^^^^////////////RUNS ONCE//////////////^^^^^^^^//////////////////   
 
    digitalWrite(chanVal, HIGH);
    delayMicroseconds(speed);
    digitalWrite(chanVal, LOW);
   // delay(300);   
   
 if(Yaw > YavgMx)
            {
              
            SingleChan(Chan4, 1665, 2);
              
            digitalWrite(8, LOW);   
            digitalWrite(12, HIGH); 
              
              
            }
            
            else if(Yaw < YavgMi)
            {
              
            SingleChan(Chan4, 1265, 2);
              
            digitalWrite(8, HIGH);   
            digitalWrite(12, LOW); 
              
            }
            
            else
            {             
              
            digitalWrite(8, LOW);   
            digitalWrite(12, LOW); 

              
            }
//////////////////////////////////////////////////////////////////////            
//            if(Pitch > PavgMx)
//            {
//              
//             //CODE HERE              
//              
//            }
//            
//            else if(Pitch < PavgMi)
//            {
//              
//            digitalWrite(8, LOW);   
//            digitalWrite(12, LOW); 
//              
//            }
//
//            else
//            {
//             //CODE HERE              
//            }            
/////////////////////////////////////////////////////////////////////            
//            if(Roll > RavgMx)
//            {
//              
//            digitalWrite(8, LOW);   
//            digitalWrite(12, LOW); 
//              
//            }
//            
//            else if(Roll < RavgMi)
//            {
//              
//             //CODE HERE              
//              
//            }
//            
//            else
//            {
//             //CODE HERE              
//            }
//   
//    if(Stop > 0)
//    {
//      break;
//    }
  }    
  }
  } 
  
}
//^^^^^^^---------------------------SingleChan------------------------------------------------------------------------------^^^^^



//VVVVVVV---------------------------FourChan------------------------------------------------------------------------------VVVVV
void FourChan(int chanVal1, int chanVal2, int chanVal3, int chanVal4, int speed1, int speed2, int speed3, int speed4, int millisec)
{
  int stoptime = 0;

  time = millis();

  stoptime = time + millisec;
  
  while(true) //Needs to be in loop to provide complete wave forms
  {
    digitalWrite(chanVal1, HIGH);
    delayMicroseconds(speed1);
    digitalWrite(chanVal1, LOW);
    
    digitalWrite(chanVal2, HIGH);
    delayMicroseconds(speed2);
    digitalWrite(chanVal2, LOW);        
   
    digitalWrite(chanVal3, HIGH);
    delayMicroseconds(speed3);
    digitalWrite(chanVal3, LOW);
    
    digitalWrite(chanVal4, HIGH);
    delayMicroseconds(speed4);
    digitalWrite(chanVal4, LOW);
    
    time = millis();
   
    if(time > stoptime)
    {
      break;
    }
  }
}
//^^^^^^^---------------------------FourChan------------------------------------------------------------------------------^^^^^


//VVVVVVV---------------------------unArm------------------------------------------------------------------------------VVVVV
void unArm(int chanVal1, int chanVal2)
{
  
  int millisec = 1500;
  
  int stoptime = 0;

  time = millis();

  stoptime = time + millisec;
  
  while(true) //Needs to be in loop to provide complete wave forms
  {
    digitalWrite(chanVal1, HIGH);
    delayMicroseconds(1086);
    digitalWrite(chanVal1, LOW);
    
    digitalWrite(chanVal2, HIGH);
    delayMicroseconds(1120);
    digitalWrite(chanVal2, LOW);        
   
    time = millis();
   
    if(time > stoptime)
    {
      break;
    }
    
  } 
}
//^^^^^^^---------------------------unArm------------------------------------------------------------------------------^^^^^



//VVVVVVV---------------------------ThrSet------------------------------------------------------------------------------VVVVV
void ThrSet(int Chan1, int Setspeed, int millisec)
{
  int stoptime = 0;

  if(Setspeed > 1750)
  {
    Setspeed = 1750;
  }

  if(Setspeed > currentSpeed)
  {
    
    time = millis();

    stoptime = time + millisec;
  
    while(true)
    {
     
      while(Setspeed > currentSpeed)
      {
        SingleChan(Chan3, currentSpeed, 230); 
      
        currentSpeed = currentSpeed + 20;
      }
     
      time = millis();
   
      if(time > stoptime)
      {
         break;
      }
    
    } 
    
  }
  
  else if(Setspeed < currentSpeed)
  {
    
    time = millis();

    stoptime = time + millisec;
  
    while(true)
    {
     
      while(Setspeed < currentSpeed)
      {
        SingleChan(Chan3, currentSpeed, 230); 
      
        currentSpeed = currentSpeed - 5;
      }
     
      time = millis();
   
      if(time > stoptime)
      {
         break;
      }
    
    } 
    
  }



}
//^^^^^^^---------------------------ThrSet------------------------------------------------------------------------------^^^^^

//VVVVVVV---------------------------Route------------------------------------------------------------------------------VVVVV
void Route()
{
  
  
  
   SingleChan(Chan3, 1150, 2);
  
  
  
  
}
//^^^^^^^---------------------------Route------------------------------------------------------------------------------^^^^^











