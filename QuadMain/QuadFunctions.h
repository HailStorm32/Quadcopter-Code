int time = 0;

//VVVVVVV---------------------------SingleChan------------------------------------------------------------------------------VVVVV
void SingleChan(int escPin, int speed, int millisec)
{
  int stoptime = 0;

  time = millis();

  stoptime = time + millisec;
  
  while(true) //Needs to be in loop to provide complete wave forms
  {
    digitalWrite(escPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(escPin, LOW);
    delay(20);
   
    time = millis();
   
    if(time > stoptime)
    {
      break;
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












