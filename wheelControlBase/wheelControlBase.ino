#include <SPI.h>
#include "GyverStepper.h"


GStepper< STEPPER2WIRE> stepper1(800, 43, 37, 14);
GStepper< STEPPER2WIRE> stepper2(800, 39, 33, 14);
GStepper< STEPPER2WIRE> stepper3(800, 35, 41, 14);


byte stepper1Moving;
byte stepper2Moving;
byte stepper3Moving;

// SPI Setup
#define DATA_SIZE 40
byte data[DATA_SIZE];
int int_data[DATA_SIZE / 2];
byte sendData[DATA_SIZE];
volatile byte counter = 0;
volatile byte in_byte = 0;
volatile byte spiTranferEnd = 0;
volatile byte spiTranferStarted = 0;

// Default Mode in 0 - KEEP_SPEED
int controlModes[3] = {0, 0, 0};


ISR (SPI_STC_vect)
{
  in_byte = SPDR;
  if (in_byte == 240 and !spiTranferStarted) {
    spiTranferStarted = 1;
    counter = 0;
    SPDR = sendData[counter];
  }
  if (spiTranferStarted and counter > 0) {
    data[counter - 1] = in_byte;
    SPDR = sendData[counter];
  }
  counter++;

  if (counter == DATA_SIZE) {
    SPDR = sendData[counter - 1];
    counter = 0;
    spiTranferStarted = 0;
    spiTranferEnd = 1;
  }
}

void joinRecievedBytes() { 
  for (int i = 0; i < DATA_SIZE; i += 2) {
    int_data[i / 2] = data[i] << 8 | data[i + 1];
  }
  spiTranferEnd = 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPI.attachInterrupt();

  stepper1.setRunMode(KEEP_SPEED);
  stepper2.setRunMode(KEEP_SPEED);
  stepper3.setRunMode(KEEP_SPEED);
  
  stepper1.invertEn(true);
  // stepper1.setMaxSpeed(1000);
  // stepper1.setAcceleration(300);
  stepper1.autoPower(true);

  stepper2.invertEn(true);
  // stepper2.setMaxSpeed(1000);
  // stepper2.setAcceleration(300);
  stepper2.autoPower(true);
  
  stepper3.invertEn(true);
  // stepper3.setMaxSpeed(1000);
  // stepper3.setAcceleration(300);
  stepper3.autoPower(true);
  
}

void loop()
{
  // Publish move statuses
  sendData[0] = stepper1.tick();
  sendData[1] = stepper2.tick();
  sendData[2] = stepper3.tick();  

  // Publish control modes
  sendData[3] = controlModes[0];
  sendData[4] = controlModes[1];
  sendData[5] = controlModes[2];
	if (spiTranferEnd) {
		joinRecievedBytes();
    if (int_data[0] == 0){ // Switch control mode
      if (int_data[1] == 0){ // KEEP SPEED (speed)
        stepper1.setRunMode(KEEP_SPEED);
        stepper2.setRunMode(KEEP_SPEED);
        stepper3.setRunMode(KEEP_SPEED);
      }
      else { // FOLLOW POSE (steps)
        stepper1.setRunMode(FOLLOW_POS);
        stepper2.setRunMode(FOLLOW_POS);
        stepper3.setRunMode(FOLLOW_POS);
      } 
        
    } else if (int_data[0] == 1) {
      stepper1.enable();
      stepper2.enable();
      stepper3.enable();

      stepper1.setMaxSpeed(int_data[4]);
      stepper2.setMaxSpeed(int_data[5]);
      stepper3.setMaxSpeed(int_data[6]);
      stepper1.setTarget(int_data[1], RELATIVE);
      stepper2.setTarget(int_data[2], RELATIVE);
      stepper3.setTarget(int_data[3], RELATIVE);

      // stepper1.setAcceleration(int_data[4]);
      // stepper2.setAcceleration(int_data[5]);
      // stepper3.setAcceleration(int_data[6]);

      // Set speeds
    } else if (int_data[0] == 2){ // Velocity based target
      stepper1.setSpeed(int_data[1]);
      stepper2.setSpeed(int_data[2]);
      stepper3.setSpeed(int_data[3]);
    } else if (int_data[0] == 3) { // Motors Full stop with release
      stepper1.brake();
      stepper2.brake();
      stepper3.brake();

      stepper1.disable();
      stepper2.disable();
      stepper3.disable(); 
    }
  }
}
