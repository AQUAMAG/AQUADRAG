#include "globals.h"
#include "user_inputs.h"

void setup() {
  homeLimitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
  endLimitSwitch.setDebounceTime(50);
  Serial.begin(115200);
  UART.begin(57600);

  DRIVER.begin();

  verify_UART_connection();

  // Enable the microPlyer feature
  DRIVER.en_spreadCycle(false); // Disable spreadCycle to enable StealthChop (which uses microPlyer)
 // DRIVER.pwm_autoscale(true); //not sure what this does
  DRIVER.microsteps(MICROSTEPS); // Set microstepping resolution to 16
  DRIVER.rms_current(1800); // Set the current limit in mA


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
          print_menu();
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
        print_menu();
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