#ifndef MOTOR_FUNCTIONS_H
#define MOTOR_FUNCTIONS_H
#include "globals.h"

void invert_direction() {
  STEPPER.setSpeed(-STEPPER.speed());
}

void reset_to_last_speed() {
  STEPPER.setSpeed(MOTOR_SPEED_STEPS); 

  #ifdef DEBUG
  Serial.print(F("Setting speed to: "));
  Serial.println(MOTOR_SPEED_STEPS);
  Serial.print(F("motor_speed_mms: "));
  Serial.println(steps_to_mm(MOTOR_SPEED_STEPS));
  Serial.print(F("Motor Actual speed: "));
  Serial.println(STEPPER.speed());
  #endif
}

void set_speed_mm_per_second(float mm_per_second) {
  MOTOR_SPEED_STEPS = mm_to_steps(mm_per_second);
  STEPPER.setSpeed(MOTOR_SPEED_STEPS);
}

void set_speed_steps_per_second(float steps_per_second) {
  MOTOR_SPEED_STEPS = steps_per_second;
  STEPPER.setSpeed(MOTOR_SPEED_STEPS);
}

void set_effective_speed_mm_per_second(float mm_per_second) {
  float adjusted_speed = mm_per_second * (1 + cos(degrees_to_radians(THETA)));
  MOTOR_SPEED_STEPS = mm_to_steps(adjusted_speed);
  STEPPER.setSpeed(MOTOR_SPEED_STEPS);

  #ifdef DEBUG
  Serial.print(F("Angle Deg      : ")); Serial.println(THETA);
  Serial.print(F("Angle Rad      : ")); Serial.println(degrees_to_radians(THETA));
  Serial.print(F("Cos(angle)     : ")); Serial.println(cos(degrees_to_radians(THETA)));
  Serial.print(F("Speed          : ")); Serial.println(mm_per_second);
  Serial.print(F("Adjusted speed : ")); Serial.println(adjusted_speed);
  #endif
}

void move_to(long position) {
  STEPPER.moveTo(position);
  reset_to_last_speed();
  CURRENT_STATE = MOVE_POSITION;
}

void set_home(long position_mm) {
  float position_steps = mm_to_steps(-position_mm);
  STEPPER.setCurrentPosition(position_steps);
}

#endif






