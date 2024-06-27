#include "globals.h"
#include "user_inputs.h"

void setup() {
  homeLimitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
  endLimitSwitch.setDebounceTime(50);
  Serial.begin(115200);
  UART.begin(57600);

  driver.begin();

  verify_UART_connection();

  // Enable the microPlyer feature
  driver.en_spreadCycle(false); // Disable spreadCycle to enable StealthChop (which uses microPlyer)
  driver.microsteps(MICROSTEPS); // Set microstepping resolution to 16
  
  
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(MAX_SPEED / 2);       // steps per second^2
  stepper.setPinsInverted(true, false, true);  // invert direction

  // Set initial speed
  stepper.setSpeed(MOTOR_SPEED_STEPS);  // steps per second
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW); // Enable the stepper motor

  print_debug_log(&stepper, &driver);

  stop_motor(&stepper);
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
    stop_motor(&stepper); //if home motor is
    CURRENT_STATE = HOME_LIMIT;
  }

  if(endLimitSwitch.isPressed()) {
    #ifdef DEBUG
          Serial.println(F("End limit is pressed."));
    #endif
    stop_motor(&stepper); //if home motor is
    CURRENT_STATE = END_LIMIT;
  }

  switch (CURRENT_STATE) {

    case HOME_LIMIT:
      set_speed_mm_per_second(&stepper, 0.5);
      stepper.runSpeed();
        if(homeLimitSwitch.isReleased()) {
          stop_motor(&stepper);
          stepper.setCurrentPosition(0);
          driver.microsteps(MICROSTEPS); //Set driver microsteps back to original value

          #ifdef DEBUG
          Serial.println(F("Home limit is released."));
          #endif
          print_menu();
        }
      break;

    case END_LIMIT:
    set_speed_mm_per_second(&stepper, -0.5);//set motor speed to 0.25 mm/s
      stepper.runSpeed(); //start the motor and run until the limit switch is released
      if(endLimitSwitch.isReleased()) {
        stop_motor(&stepper);

        #ifdef DEBUG
        Serial.println(F("End limit is released."));
        #endif
        print_menu();
      }
      break;

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