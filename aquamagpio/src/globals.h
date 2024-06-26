#ifndef GLOBALS_H
#define GLOBALS_H
#include <AccelStepper.h>
#include <ezButton.h>
#include <SoftwareSerial.h>
#include <TMCStepper.h>

#define DEBUG

float THETA = 0.0;

float MOTOR_SPEED_STEPS = 10240; // steps per second
int MICROSTEPS = 256;

// Define the stepper and the pins it will use
#define DIR_PIN 5
#define ENABLE_PIN 8
#define STEP_PIN 2
#define RX_PIN 3 // Step Pulse Y-axis pin per CNC shield pin-out
#define TX_PIN 6 // Direction Y-axis pin per CNC shield pin-out


ezButton homeLimitSwitch(9); // Home limit switch attached to pin 9
ezButton endLimitSwitch(10); // End limit switch attached to pin 10;

// Create an AccelStepper object
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Create a SoftwareSerial object for UART communication
SoftwareSerial UART(RX_PIN, TX_PIN); // RX, TX

#define driverA_ADDRESS 0b00 //Pins MS1 and MS2 connected to GND.

// Create a TMC2209Stepper object
TMC2209Stepper driver = TMC2209Stepper(&UART, static_cast<double>(0.11), driverA_ADDRESS); // Use SoftwareSerial



enum MotorState {
  HOME_LIMIT,
  END_LIMIT,
  RUNNING,
  MOVE_POSITION,
  STOPPED
};

MotorState CURRENT_STATE = STOPPED;


constexpr float full_steps_per_rotation = 200.0;
constexpr float mm_per_rotation = 5.0;
constexpr long MAX_SPEED = 10240.0; // steps per second when actuator is moving at 1 mm/s with 256 microsteps

const char DEBUG_HOME_LIMIT[]       PROGMEM = "HOME_LIMIT";   
const char DEBUG_END_LIMIT[]        PROGMEM = "END_LIMIT";    
const char DEBUG_RUNNING[]          PROGMEM = "RUNNING";      
const char DEBUG_MOVE_POSITION[]    PROGMEM = "MOVE_POSITION";
const char DEBUG_STOPPED[]          PROGMEM = "STOPPED";      

long get_steps_per_rotation() {
  long steps;
  if(MICROSTEPS == 0) {
    steps = 200;
  } else {
    steps = full_steps_per_rotation * MICROSTEPS;
  }
  return steps;
}

float mm_to_steps(float mm) {
  return mm * static_cast<float>(get_steps_per_rotation()) / mm_per_rotation;
}

float steps_to_mm(float steps) {
  return steps * mm_per_rotation / static_cast<float>(get_steps_per_rotation());
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

#endif