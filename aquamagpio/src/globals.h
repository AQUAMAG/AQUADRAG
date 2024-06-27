#ifndef GLOBALS_H
#define GLOBALS_H
#include <AccelStepper.h>
#include <ezButton.h>
#include <math.h>
#include <SoftwareSerial.h>
#include <TMCStepper.h>

#define DEBUG

float THETA = 0.0;

float MOTOR_SPEED_STEPS = 102400; // steps per second
int MICROSTEPS = 256;

// Define the stepper and the pins it will use
#define DIR_PIN 5
#define ENABLE_PIN 8
#define STEP_PIN 2
#define RX_PIN 3 // Step Pulse Y-axis pin per CNC shield pin-out
#define TX_PIN 6 // Direction Y-axis pin per CNC shield pin-out


ezButton homeLimitSwitch(10); // Home limit switch attached to pin 9
ezButton endLimitSwitch(9); // End limit switch attached to pin 10;

// Create an AccelStepper object
AccelStepper STEPPER(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Create a SoftwareSerial object for UART communication
SoftwareSerial UART(RX_PIN, TX_PIN); // RX, TX

#define driverA_ADDRESS 0b00 //Pins MS1 and MS2 connected to GND.

// Create a TMC2209Stepper object
TMC2209Stepper DRIVER(&UART, static_cast<double>(0.11), driverA_ADDRESS); // Use SoftwareSerial



enum MotorState {
  HOME_LIMIT,
  END_LIMIT,
  RUNNING,
  MOVE_POSITION,
  STOPPED
};

MotorState CURRENT_STATE = STOPPED;


constexpr float FULL_STEPS_PER_ROTATION = 200.0;
constexpr float MM_PER_ROTATION = 5.0;
constexpr long MAX_SPEED = 10240.0; // steps per second when actuator is moving at 1 mm/s with 256 microsteps 

long get_steps_per_rotation() {
  long steps;
  uint16_t microsteps = DRIVER.microsteps();
  if(microsteps == 0) {
    steps = FULL_STEPS_PER_ROTATION;
  } else {
    steps = FULL_STEPS_PER_ROTATION * microsteps;
  }
  return steps;
}

float mm_to_steps(float mm) {
  return mm * static_cast<float>(get_steps_per_rotation()) / MM_PER_ROTATION;
}

float steps_to_mm(float steps) {
  return steps * MM_PER_ROTATION / static_cast<float>(get_steps_per_rotation());
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

float get_actuator_speed() {
  return steps_to_mm(MOTOR_SPEED_STEPS);
}

float get_peel_speed() {
  return (get_actuator_speed() / (1 + cos(degrees_to_radians(THETA))));
}

void print_debug_log() {
  Serial.println(F("---------"));
  Serial.print(F("Position           : ")); Serial.println(STEPPER.currentPosition());
  Serial.print(F("Target             : ")); Serial.println(STEPPER.targetPosition());
  Serial.print(F("Motor speed actual : ")); Serial.print(STEPPER.speed());             Serial.println(F(" (steps/sec)"));
  Serial.print(F("Motor speed global : ")); Serial.print(MOTOR_SPEED_STEPS);           Serial.println(F(" (steps/sec)"));
  Serial.print(F("Actuator speed     : ")); Serial.print(get_actuator_speed());        Serial.println(F(" (mm/sec)"));
  Serial.print(F("Pull     speed     : ")); Serial.print(get_peel_speed());            Serial.println(F(" (mm/sec)"));
  Serial.print(F("Current            : ")); Serial.print(DRIVER.rms_current());        Serial.println(F(" (mA)"));
  Serial.print(F("Microsteps global  : ")); Serial.println(MICROSTEPS);
  Serial.print(F("Microsteps driver  : ")); Serial.println(DRIVER.microsteps());
  Serial.print(F("Max speed          : ")); Serial.println(MAX_SPEED);
  Serial.print(F("Block angle        : ")); Serial.print(THETA);                       Serial.println(F(" (degrees)"));
  Serial.print(F("Peel  angle        : ")); Serial.print(get_peel_angle());            Serial.println(F(" (degrees)"));
  Serial.print(F("Free memory        : ")); Serial.print(get_free_memory());           Serial.println(F(" (bytes)"));
  Serial.print(F("Driver version     : ")); Serial.println(DRIVER.version(), HEX);
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

#endif