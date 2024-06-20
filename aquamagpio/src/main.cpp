#include "motor_commands.h"
#include <AccelStepper.h>
#include <ezButton.h>
#include <SoftwareSerial.h>
#include <TMCStepper.h>


ezButton limitSwitch(9); // Create a button object that attach to pin 9;

// Create an AccelStepper object
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Create a SoftwareSerial object for UART communication
SoftwareSerial Serial1(RX_PIN, TX_PIN); // RX, TX

#define driverA_ADDRESS 0b00 //Pins MS1 and MS2 connected to GND.

// Create a TMC2209Stepper object
TMC2209Stepper driver = TMC2209Stepper(&Serial1, static_cast<double>(0.11), driverA_ADDRESS); // Use SoftwareSerial

void setup() {
  limitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds

  Serial.begin(115200);
  Serial1.begin(115200);

  driver.begin();

  // Enable the microPlyer feature
  driver.en_spreadCycle(false); // Disable spreadCycle to enable StealthChop (which uses microPlyer)
  driver.microsteps(MICROSTEPS); // Set microstepping resolution to 16
  // driver.rms_current(uint16_t mA)


  
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(get_max_speed());
  stepper.setAcceleration(get_max_speed() / 2);       // steps per second^2
  stepper.setPinsInverted(false, false, true);  // invert direction

  // Set initial speed
  stepper.setSpeed(mm_to_steps(motor_speed_mms));  // steps per second
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW); // Enable the stepper motor
  print_debug_log(&stepper, &driver);
}

void loop() {
  // Check if there is any input from the Serial Monitor
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  // Read the input string
    command.toLowerCase();
    Serial.println("---------");
    Serial.println(command);
    Serial.println("---------");

    // Check for 'STOP' command
    if (command.equals("stop")) {
      stop_motor(&stepper);
    }

    // Check for 'Print' command
    else if (command.equals("print")) {
      print_debug_log(&stepper, &driver);
    }

    // Check for 'START' command
    else if (command.equals("start")) {
      start_motor(&stepper, &driver);

    }

    // Check for 'SPEED' command
    else if (command.startsWith("speed")) {
      speed(&stepper, command);
    }


    // Check for 'MOVE' command
    else if (command.startsWith("move")) {
      move(&stepper, command);
    }

    // Check for 'SET' command
    else if (command.startsWith("set")) {
      set(&driver, command);
    }

    // Check for 'MICRO' command
    else if (command.startsWith("micro")) {
      set_microsteps(&driver, command);
    }

    // Check for 'MICRO' command
    else if (command.startsWith("micro")) {
      set_microsteps(&driver, command);
    }

    // todo setCurrentPosition(currentPosition);
    else {
      Serial.println("Unknown command.");
    }
    print_debug_log(&stepper, &driver);
  }

  // Check if the limit switch is pressed
  limitSwitch.loop(); // MUST call the loop() function first

  if(limitSwitch.isPressed()) {
    invert_direction(&stepper);
    CURRENT_STATE = RUNNING;
  }

  if(limitSwitch.isReleased()) {
    invert_direction(&stepper);
    stop_motor(&stepper);
  }

  // int state = limitSwitch.getState();
  // if(state == HIGH)
  //   Serial.println("The limit switch: UNTOUCHED");
  // else
  //   Serial.println("The limit switch: TOUCHED");

  switch (CURRENT_STATE) {
    case RUNNING:
      // Move the stepper motor continuously at the current speed
      stepper.runSpeed();

      break;

    case MOVE_POSITION:
      // Move the motor until it reaches position
      if (stepper.distanceToGo() == 0) {
        stop_motor(&stepper);

      } else {
        stepper.runSpeedToPosition();
      }
      break;

    case STOPPED:
      // Motor is stopped, do nothing or handle other tasks
      break;
  }
}
