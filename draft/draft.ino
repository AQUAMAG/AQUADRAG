#include <AccelStepper.h>

// Define the stepper and the pins it will use
#define dirPin 8
#define stepPin 9
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void setup() {
  Serial.begin(9600);
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(1000);  // steps per second
  stepper.setAcceleration(500);  // steps per second^2
  
  // Set initial speed
  stepper.setSpeed(30);  // steps per second
}

int i = 0;
void loop() {
  // Move the stepper motor continuously at the current speed
  if(i < 100){
    Serial.println(i);
    stepper.runSpeed();
    i++;
  }
}