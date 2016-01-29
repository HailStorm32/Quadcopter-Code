#pragma once

void Gyro_Calibration()
{
	//Set min & max range for the axis
	Yaw_max = Yaw + 5; //'2' is to low  //used to be 3
	Yaw_min = Yaw - 5; //'2' is to low  //used to be 3
	Pitch_max = Pitch + 10;
	Pitch_min = Pitch - 10;
	Roll_max = Roll + 10;
	Roll_min = Roll - 10;

	uint8_t count = 0;

	//Make sure max values for all axis aren't over 360
	while (yawFlag != true && pitchFlag != true && rollFlag != true)
	{

		//Abort loop and give error if stuck looping
		if (count > 4)
		{
			Serial.println("ERROR!! Stuck in loop... Aborting...");
			Error(1);
		}

		//Check Yaw_max
		if (Yaw_max > 360)
		{
			Val_hold = Yaw_max - 360;

			Yaw_max = Yaw_max - Val_hold;

			if (Yaw_max <= 360)
			{
				yawFlag = true; //Set flag to true so we know Yaw_max is valid
			}
		}

		else if (Yaw_max <= 360)
		{
			yawFlag = true; //Set flag to true so we know Yaw_max is valid
		}

		//Check Pitch_max
		if (Pitch_max > 360)
		{
			Val_hold = Pitch_max - 360;

			Pitch_max = Pitch_max - Val_hold;

			if (Pitch_max <= 360)
			{
				pitchFlag = true; //Set flag to true so we know Pitch_max is valid
			}
		}

		else if (Pitch_max <= 360)
		{
			pitchFlag = true; //Set flag to true so we know Pitch_max is valid
		}

		//Check Roll_max
		else if (Roll_max > 360)
		{
			Val_hold = Roll_max - 360;

			Roll_max = Roll_max - Val_hold;

			if (Roll_max <= 360)
			{
				rollFlag = true; //Set flag to true so we know Roll_max is valid
			}
		}

		else if (Roll_max <= 360)
		{
			rollFlag = true; //Set flag to true so we know Roll_max is valid
		}

		count = count + 1; //Add one to count so we can see how many times we have looped

	}

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
}
