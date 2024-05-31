#include <AccelStepper.h>

#define steps_per_rotation 20000
#define mm_per_rotation 2

// Define the stepper and the pins it will use
#define dirPin 8
#define stepPin 9
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

int motor_speed = 30;

void set_speed_mms(float mms) {
  int steps_per_second = mms * steps_per_rotation / mm_per_rotation;
  stepper.setSpeed(steps_per_second);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(300);  // this doesn't seem to limit speed set by setSpeed() 
  stepper.setAcceleration(500);  // steps per second^2

  // Set initial speed
  stepper.setSpeed(motor_speed);  // steps per second
  Serial.print("Current Position: ");
  Serial.println(stepper.currentPosition());
}

  bool motor_on = false;

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  // Read the input string
    command.toLowerCase();
    // Check for 'STOP' command
    if (command.equals("stop")) {
      motor_on = false;
      Serial.println("Motor stopped.");
      // Print the current position
      Serial.print("Current Position: ");
      Serial.println(stepper.currentPosition());
    }
    // Check for 'START' command
    else if (command.equals("start")) {
      motor_on = true;
      Serial.println("Motor started.");
    }
    else if (command.startsWith("speed")) {
      int indexOfSpace = command.indexOf(' ');
      if (indexOfSpace != -1) {
        String speedString = command.substring(indexOfSpace + 1);
        float speedValue = speedString.toFloat();
        if (speedValue != 0.0 || speedString == "0" || speedString == "0.0") {
          motor_speed = speedValue;
          Serial.print("Motor speed set to: ");
          Serial.println(motor_speed);
          stepper.setSpeed(static_cast<int>(motor_speed));
        } else {
          Serial.println("Invalid speed value.");
        }
      } else {
        Serial.println("Invalid command format. Use 'SPEED <value>'.");
      }
    }
    else if (command.startsWith("move")) {
      int indexOfSpace = command.indexOf(' ');
      if (indexOfSpace != -1) {
        String positionString = command.substring(indexOfSpace + 1);
        float position = positionString.toFloat();
        stepper.moveTo(static_cast<int>(position));
        while(stepper.distanceToGo() != 0) {
          stepper.run();
        };
        Serial.print("Current Position: ");
        Serial.println(stepper.currentPosition());
      } else {
        Serial.println("Invalid command format. Use 'MOVE <value>'.");
      }
    }
    else {
      Serial.println("Unknown command.");
    }
  }

  if (motor_on) {
    // Move the stepper motor continuously at the current speed
    stepper.runSpeed();
  }
}

