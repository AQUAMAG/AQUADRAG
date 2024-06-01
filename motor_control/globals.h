// globals.h

#include <AccelStepper.h>

constexpr float steps_per_rotation = 25000.0;
constexpr float mm_per_rotation = 6.0;
constexpr float max_speed = steps_per_rotation * 2;

float motor_speed_mms = 1.0; // mm per second

bool running = false;
bool move_position = false;

// Define the stepper and the pins it will use
#define dirPin 8
#define stepPin 9
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);
