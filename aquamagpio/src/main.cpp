#include <AccelStepper.h>
#include <SoftwareSerial.h>
#include <TMCStepper.h>
#include "motor_commands.h"



// Create an AccelStepper object
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Create a SoftwareSerial object for UART communication
SoftwareSerial Serial1(RX_PIN, TX_PIN); // RX, TX

#define driverA_ADDRESS 0b00 //Pins MS1 and MS2 connected to GND.

// Create a TMC2209Stepper object
TMC2209Stepper driver = TMC2209Stepper(&Serial1, static_cast<double>(0.11), driverA_ADDRESS); // Use SoftwareSerial

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  driver.begin();

  // Enable the microPlyer feature
  driver.en_spreadCycle(false); // Disable spreadCycle to enable StealthChop (which uses microPlyer)
  driver.microsteps(0); // Set microstepping resolution to 16
  
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(get_max_speed());
  stepper.setAcceleration(get_max_speed() / 2);       // steps per second^2
  stepper.setPinsInverted(false, false, true);  // invert direction

  // Set initial speed
  stepper.setSpeed(mm_to_steps(motor_speed_mms));  // steps per second
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);
  print_debug_log(&stepper);
  stop_motor(&stepper);
}



void process_command(){
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
      print_debug_log(&stepper);
    }

    // Check for 'START' command
    else if (command.equals("start")) {
      Serial.println("Enter speed in mm/sec: ");
      wait_for_input();
      command = Serial.readStringUntil('\n');
      Serial.println(command);
      speed(&stepper, command);
      //todo make direction do something
      // Serial.println("Enter direction (fwd/back): ");
      // wait_for_input();
      start_motor(&stepper);
    }

    // Check for 'SPEED' command
    else if (command.startsWith("speed")) {
      speed(&stepper, command);
    }
    // Check for 'MOVE' command
    else if (command.startsWith("move")) {
      move(&stepper, command);
    }
    //todo fix hardcoded move 0 for proper home command
    // Check for 'HOME' command
    else if (command.startsWith("home")) {
      move(&stepper, "move 0");
    }

    //todo fix set home command
    // Check for 'SET' command
    else if (command.startsWith("set")) {
      set(&stepper, command);
    }
    // todo setCurrentPosition(currentPosition);
    else {
      Serial.println("Unknown command.");
    }
    print_debug_log(&stepper);
    


}

void loop() {
  if (Serial.available() > 0) {
	  process_command();
    }

  switch (current_state) {
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