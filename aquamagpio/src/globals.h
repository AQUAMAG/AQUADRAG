#include <AccelStepper.h>
#include <TMCStepper.h>

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

MotorState CURRENT_STATE = STOPPED;
MicroSteps MICROSTEPS = MS_16;

void set_microsteps(uint16_t ms) {
  // handle input between microstep values
  if(ms < 2) {
    ms = 0;
  } else if(ms < 4) {
    ms = 2;
  } else if(ms < 8) {
    ms = 4;
  } else if(ms < 16) {
    ms = 8;
  } else if(ms < 32) {
    ms = 16;
  } else if(ms < 64) {
    ms = 32;
  } else if(ms < 128) {
    ms = 64;
  } else if(ms < 256) {
    ms = 128;
  } else {
    ms = 256;
  }
  MICROSTEPS = static_cast<MicroSteps>(ms);
}

constexpr float full_steps_per_rotation = 200.0;
constexpr float mm_per_rotation = 8.0;

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

float motor_speed_mms = 1.0; // mm per second

// Define the stepper and the pins it will use
#define DIR_PIN 5
#define ENABLE_PIN 8
#define STEP_PIN 2
#define RX_PIN 12 //Spindle Enable pin per CNC shield pin-out
#define TX_PIN 13 //Spindle Direction pin per CNC shield pin-out
