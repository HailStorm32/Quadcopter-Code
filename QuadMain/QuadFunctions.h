int time = 0;

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

