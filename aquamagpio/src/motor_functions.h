// motor_functions.h
#ifndef MOTOR_FUNCTIONS_H
#define MOTOR_FUNCTIONS_H
#include "globals.h"

void print_debug_log(AccelStepper* stepper, TMC2209Stepper* driver) {
  Serial.println("---------");
  Serial.print("Position: ");
  Serial.println(stepper->currentPosition());
  Serial.print("Target: ");
  Serial.println(stepper->targetPosition());
  Serial.println("Motor speed set to: ");
  Serial.print(stepper->speed());
  Serial.println(" (steps/sec)");
  Serial.print(motor_speed_mms);
  Serial.println(" (mm/sec)");
  Serial.print("Current: ");
  Serial.print(driver->rms_current());
  Serial.println(" (mA)");
  Serial.print("Microsteps global variable: ");
  Serial.println(driver->microsteps());
  Serial.print("Max speed: ");
  Serial.println(MAX_SPEED);
  Serial.print("Microsteps driver register: ");
  Serial.println(driver->microsteps());
  Serial.print("Driver version: ");
  auto version = driver->version();
  Serial.println(version, HEX);
  Serial.print("State: ");
  switch (CURRENT_STATE) {
    case HOME_LIMIT:
      Serial.println("HOME_LIMIT");
      break;
    case END_LIMIT:
      Serial.println("END_LIMIT");
      break;
    case RUNNING:
      Serial.println("RUNNING");
      break;

    case MOVE_POSITION:
      Serial.println("MOVE_POSITION");
      break;

    case STOPPED:
      Serial.println("STOPPED");
  }
  Serial.println("---------");
  Serial.println(" ");
}

void invert_direction(AccelStepper* stepper) {
  stepper->setSpeed(-stepper->speed());
}

void reset_to_last_speed(AccelStepper* stepper) {
  float steps = mm_to_steps(motor_speed_mms);
  stepper->setSpeed(steps); 
  //stepper.setSpeed(-steps);
  // print_debug_log();
}

void set_speed_mm_per_second(AccelStepper* stepper, float mm_per_second) {
  float steps = mm_to_steps(mm_per_second);
  stepper->setSpeed(steps);
  motor_speed_mms = mm_per_second;
}

void set_speed_steps_per_second(AccelStepper* stepper, float steps_per_second) {
  stepper->setSpeed(steps_per_second);
}

void move_to(AccelStepper* stepper, long position) {
  stepper->moveTo(position);
  reset_to_last_speed(stepper);
  CURRENT_STATE = MOVE_POSITION;
}

void set_home(AccelStepper* stepper, long position_mm) {
  float position_steps = mm_to_steps(-position_mm);
  stepper->setCurrentPosition(position_steps);
}


// void set_microsteps(AccelStepper* stepper, long microsteps) {
//   stepper->microsteps(microsteps);
// }

#endif






