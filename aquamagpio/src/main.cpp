#include "globals.h"
#include "motor_commands.h"

void setup() {
  homeLimitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
  endLimitSwitch.setDebounceTime(50);
  Serial.begin(115200);
  UART.begin(57600);

  DRIVER.begin();

  verify_UART_connection();

  // Enable the microPlyer feature
  DRIVER.en_spreadCycle(false); // Disable spreadCycle to enable StealthChop (which uses microPlyer)
  DRIVER.microsteps(MICROSTEPS); // Set microstepping resolution to 16
  //DRIVER.rms_current(1000); // Set the current limit in mA

  
  // Set the maximum speed and acceleration
  STEPPER.setMaxSpeed(MAX_SPEED);
  STEPPER.setAcceleration(MAX_SPEED / 2);       // steps per second^2
  STEPPER.setPinsInverted(true, false, true);  // invert direction

  // Set initial speed
  STEPPER.setSpeed(MOTOR_SPEED_STEPS);  // steps per second
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW); // Enable the STEPPER motor

  print_debug_log();

  stop_motor();
}

void process_command(){

    verify_UART_connection();

    String command = Serial.readStringUntil('\n');  // Read the input string
    command.toLowerCase();
    Serial.println(F("---------"));
    Serial.println(command);
    Serial.println(F("---------"));

    // Check for 'STOP' command
    if (command.equals(F("stop"))) {
      stop_motor();

    }

    // Check for 'Print' command
    else if (command.equals(F("print"))) {
      print_debug_log();
    }

    // Check for 'START' command
    else if (command.equals(F("start"))) {
      //Serial.println("Enter speed in mm/sec: ");
      //wait_for_input();
      command = Serial.readStringUntil('\n');
      Serial.println(command);
      speed(command);
      //todo make direction do something
      // Serial.println("Enter direction (fwd/back): ");
      // wait_for_input();
      start_motor();
    }

    // Check for 'SPEED' command
    else if (command.startsWith(F("speed"))) {
      speed(command);
    }

    // Check for 'STEPS' command
    else if (command.startsWith(F("steps"))) {
      steps(command);
    }

    // Check for 'MOVE' command
    else if (command.startsWith(F("move"))) {
      move(command);
    }
    //todo fix hardcoded move 0 for proper home command
    // Check for 'HOME' command
    else if (command.startsWith(F("home"))) {
      home_motor();
    }

    // Check for 'MICRO' command
    else if (command.startsWith(F("micro"))) {
      set_microsteps(command);
    }

    // Check for 'ANGLE' command
    else if (command.startsWith(F("angle"))) {
      set_angle(command);
    }

    // Check for 'PULL' command
    else if (command.startsWith(F("pull"))) {
      pull(command);
    }

    // todo setCurrentPosition(currentPosition);
    else {
      Serial.println(F("Unknown command."));
    }
    print_debug_log();
}

void loop() {
  if (Serial.available() > 0) {
	  process_command();
    }

  // Check if the limit switch is pressed
  homeLimitSwitch.loop(); 
  endLimitSwitch.loop(); 

  if(homeLimitSwitch.isPressed()) {
    #ifdef DEBUG
          Serial.println(F("Home limit is pressed."));
    #endif
    stop_motor(); //if home motor is
    CURRENT_STATE = HOME_LIMIT;
  }

  if(endLimitSwitch.isPressed()) {
    #ifdef DEBUG
          Serial.println(F("End limit is pressed."));
    #endif
    stop_motor(); //if home motor is
    CURRENT_STATE = END_LIMIT;
  }

  switch (CURRENT_STATE) {

    case HOME_LIMIT:
      set_speed_mm_per_second(0.5);
      STEPPER.runSpeed();
        if(homeLimitSwitch.isReleased()) {
          stop_motor();
          STEPPER.setCurrentPosition(0);
          DRIVER.microsteps(MICROSTEPS); //Set DRIVER microsteps back to original value

          #ifdef DEBUG
          Serial.println(F("Home limit is released."));
          #endif
        }
      break;

    case END_LIMIT:
    set_speed_mm_per_second(-0.5);//set motor speed to 0.25 mm/s
      STEPPER.runSpeed(); //start the motor and run until the limit switch is released
      if(endLimitSwitch.isReleased()) {
        stop_motor();

        #ifdef DEBUG
        Serial.println(F("End limit is released."));
        #endif
      }
      break;

    case RUNNING:
      // Move the STEPPER motor continuously at the current speed
      STEPPER.runSpeed();

      break;

    case MOVE_POSITION:
      // Move the motor until it reaches position
      if (STEPPER.distanceToGo() == 0) {
        stop_motor();
      } else {
        STEPPER.runSpeedToPosition();
      }
      break;

    case STOPPED:
      // Motor is stopped, do nothing or handle other tasks
      break;
  }
}