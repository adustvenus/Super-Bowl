

/*
  Stepper Motor Demonstration 1
  Stepper-Demo1.ino
  Demonstrates 28BYJ-48 Unipolar Stepper with ULN2003 Driver
  Uses Arduino Stepper Library
 
  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/
 
//Include Header Files
#include <Stepper.h>
#include <IRremote.h>

int IR_RECEIVE_PIN = 7;                                   // the pin where you connect the output pin of IR sensor     

const float STEPS_PER_REV = 32;                           // Number of steps per internal motor revolution 

const float GEAR_RED = 64;                                //  Amount of Gear Reduction

const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED; // Number of steps per geared output rotation

int StepsRequired;

int STOP = 1;

const int echoPin = 2;

const int trigPin = 4;
// Create Instance of Stepper Class
// Specify Pins used for motor coils
// The pins used are 8,9,10,11 
// Connected to ULN2003 Motor Driver In1, In2, In3, In4 
// Pins entered in sequence 1-3-2-4 for proper step sequencing
 
Stepper steppermotor(STEPS_PER_REV, 8, 10, 9, 11);
 
void setup()
{
  pinMode(echoPin, INPUT);  // Register echoPin for receiving input
  pinMode(trigPin, OUTPUT);  // Register trigPin for sending output
  Serial.begin(9600);   
  IrReceiver.begin(IR_RECEIVE_PIN);
}
 
void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  double distance = duration / 29/ 2;
  
  if (duration < 500 && duration != 0)
  {
    if (STOP == 2)
      {
          Serial.println("DOWN");
          Serial.println(duration);
          STOP = 1;
          StepsRequired  =  STEPS_PER_OUT_REV / 4; 
          steppermotor.setSpeed(700);   
          steppermotor.step(StepsRequired);
        
      }
   
    else if (STOP == 1)
    {
        Serial.println("UP");
        Serial.println(duration);
        STOP = 2;
        StepsRequired  =  -STEPS_PER_OUT_REV / 4; 
        steppermotor.setSpeed(700);   
        steppermotor.step(StepsRequired);
    }
  }




  
  if (IrReceiver.decode())// Returns 0 if no data ready, 1 if data ready.     
  {     
    //4127850240 UP
    if (STOP == 1)
    {
      if(IrReceiver.decodedIRData.decodedRawData == 4127850240) 
      {
        STOP = 2;
        StepsRequired  =  -STEPS_PER_OUT_REV / 4; 
        steppermotor.setSpeed(700);   
        steppermotor.step(StepsRequired);
      }
    }
    //4161273600 DOWN
    if (STOP == 2)
    {
      if(IrReceiver.decodedIRData.decodedRawData == 4161273600) 
      {
        STOP = 1;
        StepsRequired  =  STEPS_PER_OUT_REV / 4; 
        steppermotor.setSpeed(700);   
        steppermotor.step(StepsRequired);
      }
    }
    
    delay(50);
    IrReceiver.resume();
  }   
}
