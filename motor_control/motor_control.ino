#include <AccelStepper.h>

constexpr float steps_per_rotation = 25000.0;
constexpr float mm_per_rotation = 6.0;

// Define the stepper and the pins it will use
#define dirPin 8
#define stepPin 9
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

#define MAX_SPEED 500000

float motor_speed = 1.0; // mm per second

float mm_to_steps(float mm) {
  return mm * steps_per_rotation / mm_per_rotation;
}

void print_debug_log() {
  Serial.print("Current Position: ");
  Serial.println(stepper.currentPosition());
  Serial.println("Motor speed set to: ");
  Serial.print(mm_to_steps(motor_speed));
  Serial.println(" (steps/sec)");
  Serial.print(motor_speed);
  Serial.println(" (mm/sec)");
  Serial.print("Current Speed: ");
  Serial.print(stepper.speed());
  Serial.println(" steps/sec");
  Serial.print("Max speed: ");
  Serial.println(MAX_SPEED);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(500);  // steps per second^2

  // Set initial speed
  stepper.setSpeed(mm_to_steps(motor_speed));  // steps per second
  print_debug_log();
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
          float mm = speedValue;
          float steps = mm_to_steps(mm);
          Serial.println("*****");
          Serial.println(steps);
          stepper.setSpeed(steps);
          Serial.println("*****");
          Serial.println(steps);
          motor_speed = mm;
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
        stepper.setSpeed(motor_speed);
      } else {
        Serial.println("Invalid command format. Use 'MOVE <value>'.");
      }
    }
    else {
      Serial.println("Unknown command.");
    }
    print_debug_log();
  }

  if (motor_on) {
    // Move the stepper motor continuously at the current speed
    stepper.runSpeed();
  }
}

