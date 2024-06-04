#include "motor_functions.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(max_speed);
  stepper.setAcceleration(max_speed / 2);  // steps per second^2
  stepper.setPinsInverted(true, false, false); // invert direction

  // Set initial speed
  stepper.setSpeed(mm_to_steps(motor_speed_mms));  // steps per second
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);
  print_debug_log();
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  // Read the input string
    command.toLowerCase();
    Serial.println("---------");
    Serial.println(command);
    Serial.println("---------");

    // Check for 'STOP' command
    if (command.equals("stop")) {
      stop();
      Serial.println("Motor stopped.");
    }
    // Check for 'START' command
    else if (command.equals("start")) {
      start();
      Serial.println("Motor started.");
    }

    // Check for 'SPEED' command
    else if (command.startsWith("speed")) {
      int indexOfSpace = command.indexOf(' ');
      if (indexOfSpace != -1) {
        String speedString = command.substring(indexOfSpace + 1);
        float speedValue = speedString.toFloat();
        if (speedValue != 0.0 || speedString == "0" || speedString == "0.0") {
          set_speed(speedValue);
        } else {
          Serial.print(speedString);
          Serial.println(" is an invalid speed value.");
        }
      } else {
        Serial.println("Invalid command format. Use 'SPEED <value>'.");
      }
    }

    // Check for 'MOVE' command
    else if (command.startsWith("move")) {
      int indexOfSpace = command.indexOf(' ');
      if (indexOfSpace != -1) {
        String positionString = command.substring(indexOfSpace + 1);
        long position = positionString.toInt();
        move_to(position);
        // stepper.setSpeed(motor_speed);
      } else {
        Serial.println("Invalid command format. Use 'MOVE <value>'.");
      }
    }

    // Check for 'SET' command
    else if (command.startsWith("set")) {
      int indexOfSpace = command.indexOf(' ');
      if (indexOfSpace != -1) {
        String distanceString = command.substring(indexOfSpace + 1);
        float distanceValue = distanceString.toFloat();
        if (distanceValue != 0.0 || distanceString == "0" || distanceString == "0.0") {
          set_home(distanceValue);
        } else {
          Serial.print(distanceString);
          Serial.println(" is an invalid distance value.");
        }
      } else {
        set_home(0);
      }
    }
    

    // todo setCurrentPosition(currentPosition);
    else {
      Serial.println("Unknown command.");
    }
    print_debug_log();
  }

  switch (current_state) {
    case RUNNING:
    // Move the stepper motor continuously at the current speed
      stepper.runSpeed();
      break;
      
    case MOVE_POSITION:
      // Move the motor until it reaches position
      if (stepper.distanceToGo() == 0) {
        stop();
      } else {
        stepper.runSpeedToPosition();
      }
      break;
      
    case STOPPED:
      // Motor is stopped, do nothing or handle other tasks
      break;
  }
}

