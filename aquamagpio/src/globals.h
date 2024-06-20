#ifndef GLOBALS_H
#define GLOBALS_H
#include <AccelStepper.h>
#include <TMCStepper.h>

#define DEBUG

enum MotorState {
  RUNNING,
  MOVE_POSITION,
  STOPPED
};

int MICROSTEPS = 256;

MotorState CURRENT_STATE = STOPPED;


constexpr float full_steps_per_rotation = 200.0;
constexpr float mm_per_rotation = 5.0;

long get_steps_per_rotation() {
  long steps;
  if(MICROSTEPS == 0) {
    steps = 200;
  } else {
    steps = full_steps_per_rotation * MICROSTEPS;
  }
  return steps;
}


long get_max_speed() {
  return get_steps_per_rotation() * 2;
}

float mm_to_steps(float mm) {
  return mm * static_cast<float>(get_steps_per_rotation()) / mm_per_rotation;
}

bool is_valid_microsteps(int n) {
    // Array of valid values for microsteps
    const int validValues[] = {0, 2, 4, 8, 16, 64, 128, 256};
    const int size = sizeof(validValues) / sizeof(validValues[0]); // Calculate the size of the array

    // Use a loop to search for the value in the array
    for (int i = 0; i < size; ++i) {
        if (validValues[i] == n) {
            return true;
        }
    }
    return false;
}

float motor_speed_mms = 1.0; // mm per second

// Define the stepper and the pins it will use
#define DIR_PIN 5
#define ENABLE_PIN 8
#define STEP_PIN 2
#define RX_PIN 12 //Spindle Enable pin per CNC shield pin-out
#define TX_PIN 13 //Spindle Direction pin per CNC shield pin-out

#endif