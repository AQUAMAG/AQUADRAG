#include "motor_functions.h"

void stop_motor(AccelStepper* stepper) {
  //reset_direction(stepper);
  stepper->stop();
  CURRENT_STATE = STOPPED;
  Serial.println("Motor stopped.");
}

void start_motor(AccelStepper* stepper, TMC2209Stepper* driver) {
  // print_debug_log();
  reset_to_last_speed(stepper);
  CURRENT_STATE = RUNNING;
  Serial.println("Motor started.");
  print_debug_log(stepper, driver);
  
}

void speed(AccelStepper* stepper, String command) {
  //Serial.println(command);
  int indexOfSpace = command.indexOf(' ');
      if (indexOfSpace != -1) {
        String speedString = command.substring(indexOfSpace + 1);
        float speedValue = speedString.toFloat();
        if (speedValue != 0.0 || speedString == "0" || speedString == "0.0") {
          set_speed(stepper, speedValue);
        } else {
          Serial.print(speedString);
          Serial.println(" is an invalid speed value.");
        }
      } else {
        Serial.println("Invalid command format. Use 'SPEED <value>'.");
    }
}

void move(AccelStepper* stepper, String command) {
  int indexOfSpace = command.indexOf(' ');
  if (indexOfSpace != -1) {
      String positionString = command.substring(indexOfSpace + 1);
      long position = positionString.toInt();
      move_to(stepper, position);
      Serial.println("new position to move to is: ");
      Serial.println(position);
      // stepper.setSpeed(motor_speed);
  } else {
      Serial.println("Invalid command format. Use 'MOVE <value>'.");
    }
}

void set(AccelStepper* stepper, String command) {
  int indexOfSpace = command.indexOf(' ');
  if (indexOfSpace != -1) {
    String distanceString = command.substring(indexOfSpace + 1);
    float distanceValue = distanceString.toFloat();
    if (distanceValue != 0.0 || distanceString == "0" || distanceString == "0.0") {
      set_home(stepper, distanceValue);
    } else {
      Serial.print(distanceString);
      Serial.println(" is an invalid distance value.");
    }
  } else {
    set_home(stepper, 0);
  }
}

void set_microsteps(TMC2209Stepper* driver, String command) {
  int indexOfSpace = command.indexOf(' ');
  if (indexOfSpace != -1) {
    String microstepsString = command.substring(indexOfSpace + 1);
    long microstepsValue = microstepsString.toInt();
    if (microstepsValue != 0 || microstepsString == "0" || microstepsString == "0.0") {
      driver->microsteps(microstepsValue);
    } else {
      Serial.print(microstepsString);
      Serial.println(" is an invalid microstep value.");
    }
  } else {
    Serial.println("Invalid command format. Use 'micro <value>'.");
  }
}