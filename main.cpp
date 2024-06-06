#include "motor_commands.h"
#include <AccelStepper.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(250000);
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
      stop_motor();
    }

    // Check for 'Print' command
    else if (command.equals("print")) {
      print_debug_log();
    }

    // Check for 'START' command
    else if (command.equals("start")) {
      start();

    }

    // Check for 'SPEED' command
    else if (command.startsWith("speed")) {
      speed(command);
      }


    // Check for 'MOVE' command
    else if (command.startsWith("move")) {
      move(command);
    }

    // Check for 'SET' command
    else if (command.startsWith("set")) {
      set(command);
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
        stop_motor();

      } else {
        stepper.runSpeedToPosition();
      }
      break;

    case STOPPED:
      // Motor is stopped, do nothing or handle other tasks
      break;
  }
}

