// motor_functions.h

#include "globals.h"

float mm_to_steps(float mm) {
  return mm * steps_per_rotation / mm_per_rotation;
}

void print_debug_log() {
  Serial.print("Current Position: ");
  Serial.println(stepper.currentPosition());
  Serial.println("Motor speed set to: ");
  Serial.print(mm_to_steps(motor_speed_mms));
  Serial.println(" (steps/sec)");
  Serial.print(motor_speed_mms);
  Serial.println(" (mm/sec)");
  Serial.print("Current Speed: ");
  Serial.print(stepper.speed());
  Serial.println(" steps/sec");
  Serial.print("Max speed: ");
  Serial.println(max_speed);
}

void stop() {
  currentState = STOPPED;
  // stepper.setPinsInverted(true, false, false);
}

void start() {
  currentState = RUNNING;
}

void set_speed(float mm_per_second) {
  float steps = mm_to_steps(mm_per_second);
  stepper.setSpeed(steps);
  motor_speed_mms = mm_per_second;
}

void move_to(long position) {
  stepper.moveTo(position);
  stepper.setSpeed(mm_to_steps(motor_speed_mms));
  current_state = MOVE_POSITION;
}
