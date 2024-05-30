#include <AccelStepper.h>

// Define the stepper and the pins it will use
#define dirPin 3
#define stepPin 2
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

#todo // take mm/s as input and translate to stepper speed

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(1000);  // steps per second
  stepper.setAcceleration(500);  // steps per second^2

  // Set initial speed
  stepper.setSpeed(200);  // steps per second
}

bool keepRunning = true;

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  // Read the input string

    // Check for 'STOP' command
    if (command.equalsIgnoreCase("STOP")) {
      keepRunning = false;  // Stop the loop
      Serial.println("Loop interrupted.");
    }
    // Check for 'START' command
    else if (command.equalsIgnoreCase("START")) {
      keepRunning = true;  // Resume the loop
      Serial.println("Loop resumed.");
    }
  }

  if (keepRunning) {
    // Your loop code here
    Serial.println("Loop is running...");
    // Move the stepper motor continuously at the current speed
    stepper.runSpeed();
    delay(1000);
  } else {
    Serial.println("Loop is stopped. Type 'START' to resume.");
    delay(1000);  // Add a small delay to prevent spamming the Serial Monitor
  }
  // Move the stepper motor continuously at the current speed
  stepper.runSpeed();
}

