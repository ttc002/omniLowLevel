#include "GyverStepper.h"

GStepper< STEPPER2WIRE> stepper1(800, 39, 33, 14);
GStepper< STEPPER2WIRE> stepper2(800, 43, 37, 14);
GStepper< STEPPER2WIRE> stepper3(800, 35, 41, 14);

int speed1 = 0;    //speed for 1st stepper
int speed2 = 0;    //speed for 2nd stepper
int speed3 = 0;    //speed for 3rd stepper
long time = 0;      //time, ms
long timer = 0;
void setup() 
{
  stepper1.setRunMode(KEEP_SPEED);
  stepper2.setRunMode(KEEP_SPEED);
  stepper3.setRunMode(KEEP_SPEED);
  
  stepper1.invertEn(true);
  // stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(0);
  stepper1.autoPower(true);

  stepper2.invertEn(true);
  // stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(0);
  stepper2.autoPower(true);
  
  stepper3.invertEn(true);
  // stepper3.setMaxSpeed(1000);
  stepper3.setAcceleration(0);
  stepper3.autoPower(true);
  stepper1.setSpeed(-speed1);
  stepper2.setSpeed(-speed2);
  stepper3.setSpeed(-speed3);
  
}

void loop()
{

  if(millis()-timer>=time)
  {
    
    stepper1.break();
    stepper2.break();
    stepper3.break();
  }
}
