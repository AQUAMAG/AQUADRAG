// motor_functions.h

#include "globals.h"

void print_debug_log(AccelStepper* stepper) {
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
  Serial.print("Current Speed: ");
  Serial.print(stepper->speed());
  Serial.println(" steps/sec");
  Serial.print("Max speed: ");
  Serial.println(get_max_speed());
  Serial.print("State: ");
  switch (current_state) {
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

void set_speed(AccelStepper* stepper, float mm_per_second) {
  float steps = mm_to_steps(mm_per_second);
  stepper->setSpeed(steps);
  motor_speed_mms = mm_per_second;
}

void move_to(AccelStepper* stepper, long position) {
  stepper->moveTo(position);
  reset_to_last_speed(stepper);
  current_state = MOVE_POSITION;
}

void set_home(AccelStepper* stepper, long position_mm) {
  float position_steps = mm_to_steps(-position_mm);
  stepper->setCurrentPosition(position_steps);
}

void reset_to_last_speed(AccelStepper* stepper) {
  float steps = mm_to_steps(motor_speed_mms);
  stepper->setSpeed(steps); 
  //stepper.setSpeed(-steps);
  // print_debug_log();
}




