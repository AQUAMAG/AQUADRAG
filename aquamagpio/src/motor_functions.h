#ifndef MOTOR_FUNCTIONS_H
#define MOTOR_FUNCTIONS_H
#include "globals.h"

int freeMemory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void print_debug_log(AccelStepper* stepper, TMC2209Stepper* driver) {
  char buffer[30]; // Ensure this is large enough to hold your string
  Serial.println("---------");
  strcpy_P(buffer, DEBUG_POSITION      ); Serial.print(buffer); Serial.println(stepper->currentPosition());
  strcpy_P(buffer, DEBUG_TARGET        ); Serial.print(buffer); Serial.println(stepper->targetPosition());
  strcpy_P(buffer, DEBUG_SPEED_ACTUAL  ); Serial.print(buffer); Serial.print(stepper->speed());  Serial.println(" (steps/sec)");
  strcpy_P(buffer, DEBUG_SPEED_GLOBAL  ); Serial.print(MOTOR_SPEED_STEPS); Serial.println(" (steps/sec)");
  strcpy_P(buffer, DEBUG_ACTUATOR_SPEED); Serial.print(steps_to_mm(MOTOR_SPEED_STEPS)); Serial.println(" (mm/sec)");
  strcpy_P(buffer, DEBUG_CURRENT       ); Serial.print(driver->rms_current()); Serial.println(" (mA)");
  strcpy_P(buffer, DEBUG_MCRSTPS_GLBL  ); Serial.println(MICROSTEPS);
  strcpy_P(buffer, DEBUG_MCRSTPS_DRVR  ); Serial.println(driver->microsteps());
  strcpy_P(buffer, DEBUG_MAX_SPEED     ); Serial.println(MAX_SPEED);
  strcpy_P(buffer, DEBUG_ANGLE         ); Serial.println(THETA);
  strcpy_P(buffer, DEBUG_DRIVER_VERSION); Serial.println(driver->version(), HEX);
  strcpy_P(buffer, DEBUG_STATE         );
  switch (CURRENT_STATE) {
    case HOME_LIMIT:    strcpy_P(buffer, DEBUG_HOME_LIMIT   );Serial.println(buffer); break;
    case END_LIMIT:     strcpy_P(buffer, DEBUG_END_LIMIT    );Serial.println(buffer); break;
    case RUNNING:       strcpy_P(buffer, DEBUG_RUNNING      );Serial.println(buffer); break;
    case MOVE_POSITION: strcpy_P(buffer, DEBUG_MOVE_POSITION);Serial.println(buffer); break;
    case STOPPED:       strcpy_P(buffer, DEBUG_STOPPED      );Serial.println(buffer); break;
  }
  Serial.println("---------");
  Serial.println(" ");
}

void invert_direction(AccelStepper* stepper) {
  stepper->setSpeed(-stepper->speed());
}

void reset_to_last_speed(AccelStepper* stepper) {
  stepper->setSpeed(MOTOR_SPEED_STEPS); 

  #ifdef DEBUG
  Serial.print("Setting speed to: ");
  Serial.println(MOTOR_SPEED_STEPS);
  Serial.print("motor_speed_mms: ");
  Serial.println(steps_to_mm(MOTOR_SPEED_STEPS));
  Serial.print("Motor Actual speed: ");
  Serial.println(stepper->speed());
  #endif
  //stepper.setSpeed(-steps);
  // print_debug_log();
}

void set_speed_mm_per_second(AccelStepper* stepper, float mm_per_second) {
  MOTOR_SPEED_STEPS = mm_to_steps(mm_per_second);
  stepper->setSpeed(MOTOR_SPEED_STEPS);
}

void set_speed_steps_per_second(AccelStepper* stepper, float steps_per_second) {
  MOTOR_SPEED_STEPS = steps_per_second;
  stepper->setSpeed(MOTOR_SPEED_STEPS);
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






