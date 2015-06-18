int time = 0;

void SingleMotor(int escPin, int speed, int millisec)
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


void FourMotor(int esc1, int esc2, int esc3, int esc4, int speed1, int speed2, int speed3, int speed4, int millisec)
{
  int stoptime = 0;

  time = millis();

  stoptime = time + millisec;
  
  while(true) //Needs to be in loop to provide complete wave forms
  {
    digitalWrite(esc1, HIGH);
    delayMicroseconds(speed1);
    digitalWrite(esc1, LOW);
    
    digitalWrite(esc2, HIGH);
    delayMicroseconds(speed2);
    digitalWrite(esc2, LOW);        
   
    digitalWrite(esc3, HIGH);
    delayMicroseconds(speed3);
    digitalWrite(esc3, LOW);
    
    digitalWrite(esc4, HIGH);
    delayMicroseconds(speed4);
    digitalWrite(esc4, LOW);
    
    time = millis();
   
    if(time > stoptime)
    {
      break;
    }
  }
}

void BoardArm(int esc1, int esc2)
{
  int millisec = 1500;
  
  int stoptime = 0;

  time = millis();

  stoptime = time + millisec;
  
  while(true) //Needs to be in loop to provide complete wave forms
  {
    digitalWrite(esc1, HIGH);
    delayMicroseconds(1086);
    digitalWrite(esc1, LOW);
    
    digitalWrite(esc2, HIGH);
    delayMicroseconds(1875);
    digitalWrite(esc2, LOW);        
   
    time = millis();
   
    if(time > stoptime)
    {
      break;
    }
    
  }
  
}
