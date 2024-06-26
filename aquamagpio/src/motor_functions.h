#ifndef MOTOR_FUNCTIONS_H
#define MOTOR_FUNCTIONS_H
#include "globals.h"
#include <math.h>

int get_free_memory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

float get_peel_angle() {
  return 180.0 - THETA;
}

float degrees_to_radians(float degrees) {
  return degrees * PI / 180.0;
}

float radians_to_degrees(float radians) {
  return radians * 180.0 / PI;
}

float get_peel_speed() {
  float actuator_speed = steps_to_mm(MOTOR_SPEED_STEPS);
  return actuator_speed / (1 + cos(degrees_to_radians(THETA)));
}

void print_debug_log(AccelStepper* stepper, TMC2209Stepper* driver) {
  Serial.println(F("---------"));
  Serial.print(F("Position           : ")); Serial.println(stepper->currentPosition());
  Serial.print(F("Target             : ")); Serial.println(stepper->targetPosition());
  Serial.print(F("Motor speed actual : ")); Serial.print(stepper->speed());               Serial.println(F(" (steps/sec)"));
  Serial.print(F("Motor speed global : ")); Serial.print(MOTOR_SPEED_STEPS);              Serial.println(F(" (steps/sec)"));
  Serial.print(F("Actuator speed     : ")); Serial.print(steps_to_mm(MOTOR_SPEED_STEPS)); Serial.println(F(" (mm/sec)"));
  Serial.print(F("Pull     speed     : ")); Serial.print(get_peel_speed()); Serial.println(F(" (mm/sec)"));
  Serial.print(F("Current            : ")); Serial.print(driver->rms_current());          Serial.println(F(" (mA)"));
  Serial.print(F("Microsteps global  : ")); Serial.println(MICROSTEPS);
  Serial.print(F("Microsteps driver  : ")); Serial.println(driver->microsteps());
  Serial.print(F("Max speed          : ")); Serial.println(MAX_SPEED);
  Serial.print(F("Block angle        : ")); Serial.print(THETA);                          Serial.println(" (degrees)");
  Serial.print(F("Peel  angle        : ")); Serial.print(get_peel_angle());               Serial.println(" (degrees)");
  Serial.print(F("Free memory        : ")); Serial.print(get_free_memory());              Serial.println(" (bytes)");
  Serial.print(F("Driver version     : ")); Serial.println(driver->version(), HEX);
  Serial.print(F("State              : "));
  switch (CURRENT_STATE) {
    case HOME_LIMIT:    Serial.println(F("HOME_LIMIT"   )); break;
    case END_LIMIT:     Serial.println(F("END_LIMIT"    )); break;
    case RUNNING:       Serial.println(F("RUNNING"      )); break;
    case MOVE_POSITION: Serial.println(F("MOVE_POSITION")); break;
    case STOPPED:       Serial.println(F("STOPPED"      )); break;
  }
  Serial.println(F("---------"));
  Serial.println(F(" "));
}

void invert_direction(AccelStepper* stepper) {
  stepper->setSpeed(-stepper->speed());
}

void reset_to_last_speed(AccelStepper* stepper) {
  stepper->setSpeed(MOTOR_SPEED_STEPS); 

  #ifdef DEBUG
  Serial.print(F("Setting speed to: "));
  Serial.println(MOTOR_SPEED_STEPS);
  Serial.print(F("motor_speed_mms: "));
  Serial.println(steps_to_mm(MOTOR_SPEED_STEPS));
  Serial.print(F("Motor Actual speed: "));
  Serial.println(stepper->speed());
  #endif
}

void set_speed_mm_per_second(AccelStepper* stepper, float mm_per_second) {
  MOTOR_SPEED_STEPS = mm_to_steps(mm_per_second);
  stepper->setSpeed(MOTOR_SPEED_STEPS);
}

void set_speed_steps_per_second(AccelStepper* stepper, float steps_per_second) {
  MOTOR_SPEED_STEPS = steps_per_second;
  stepper->setSpeed(MOTOR_SPEED_STEPS);
}

void set_effective_speed_mm_per_second(AccelStepper* stepper, float mm_per_second) {
  float adjusted_speed = mm_per_second * (1 + cos(degrees_to_radians(THETA)));
  MOTOR_SPEED_STEPS = mm_to_steps(adjusted_speed);
  stepper->setSpeed(MOTOR_SPEED_STEPS);

  #ifdef DEBUG
  Serial.print(F("Angle Rad: "     )); Serial.println(degrees_to_radians(THETA));
  Serial.print(F("Cos angle: "     )); Serial.println(cos(degrees_to_radians(THETA)));
  Serial.print(F("Speed: "         )); Serial.println(mm_per_second);
  Serial.print(F("Adjusted speed: ")); Serial.println(adjusted_speed);
  Serial.print(F("Actuator speed: ")); Serial.println(steps_to_mm(MOTOR_SPEED_STEPS));
  #endif
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

#endif






