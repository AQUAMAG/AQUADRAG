#include <AccelStepper.h>

enum MotorState {
  RUNNING,
  MOVE_POSITION,
  STOPPED
};

enum MicroSteps {
  MS_0 = 0,
  MS_2 = 2,
  MS_4 = 4,
  MS_8 = 8,
  MS_16 = 16,
  MS_32 = 32,
  MS_64 = 64,
  MS_128 = 128,
  MS_256 = 256
};

MotorState current_state = STOPPED;
MicroSteps microsteps = MS_16;

constexpr float full_steps_per_rotation = 200.0;
constexpr float mm_per_rotation = 8.0;

long get_steps_per_rotation() {
  long steps;
  if(microsteps == 0) {
    steps = 200;
  } else {
    steps = full_steps_per_rotation * microsteps;
  }
  return steps;
}

long get_max_speed() {
  return get_steps_per_rotation() * 2;
}

float mm_to_steps(float mm) {
  return mm * static_cast<float>(get_steps_per_rotation()) / mm_per_rotation;
}

float motor_speed_mms = 1.0; // mm per second

// Define the stepper and the pins it will use
#define DIR_PIN 5
#define ENABLE_PIN 8
#define STEP_PIN 2
#define RX_PIN 6
#define TX_PIN 7
