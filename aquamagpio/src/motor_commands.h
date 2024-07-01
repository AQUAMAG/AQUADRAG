#ifndef MOTOR_COMMANDS_H
#define MOTOR_COMMANDS_H

#include "globals.h"
#include "motor_functions.h"


void stop_motor() {
  STEPPER.stop();
  CURRENT_STATE = STOPPED;
  Serial.println(F("Motor stopped."));

}

void start_motor() {
  // print_debug_log();
  reset_to_last_speed();
  CURRENT_STATE = RUNNING;
  Serial.println(F("Motor started."));
  print_debug_log();
}


void home_motor(){
   MICROSTEPS = DRIVER.microsteps(); //Get current microsteps
      DRIVER.microsteps(0); //Set driver microsteps to 0 AT THIS MOMENT GLOBAL VARIABLE IS NOT UPDATED!
      set_speed_mm_per_second(-10.0); //set motor speed to 15 mm/s for quick homing
      start_motor();
}


//todo fix speed to take in just a number or full command
void speed(String command) {
  //Serial.println(command);
  int indexOfSpace = command.indexOf(' ');
      if (indexOfSpace != -1) {
        String speedString = command.substring(indexOfSpace + 1);
        float speedValue = speedString.toFloat();
        if (speedValue != 0.0 || speedString == "0" || speedString == "0.0") {
          set_speed_mm_per_second(speedValue);
        } else {
          Serial.print(speedString);
          Serial.println(F(" is an invalid speed value."));
        }
      } else {
        Serial.println(F("Invalid command format. Use 'SPEED <value>'."));
    }
}


void set_current(String command) {
  //Serial.println(command);
  int indexOfSpace = command.indexOf(' ');
      if (indexOfSpace != -1) {
        String currentString = command.substring(indexOfSpace + 1);
        uint16_t currentValue = static_cast<uint16_t>(currentString.toInt());
        if (currentValue > 0.0) {
          DRIVER.rms_current(currentValue);
        } else {
          Serial.print(currentString);
          Serial.println(F(" is an invalid current value."));
        }
      } else {
        Serial.println(F("Invalid command format. Use 'current <value in mA>'"));
    }
}

void steps(String command) {
  //Serial.println(command);
  int indexOfSpace = command.indexOf(' ');
      if (indexOfSpace != -1) {
        String speedString = command.substring(indexOfSpace + 1);
        float speedValue = speedString.toFloat();
        if (speedValue != 0.0 || speedString == "0" || speedString == "0.0") {
          set_speed_steps_per_second(speedValue);
        } else {
          Serial.print(speedString);
          Serial.println(F(" is an invalid speed value."));
        }
      } else {
        Serial.println(F("Invalid command format. Use 'STEPS <value>'."));
    }
}

void move(String command) {
  int indexOfSpace = command.indexOf(' ');
  if (indexOfSpace != -1) {
      String positionString = command.substring(indexOfSpace + 1);
      long position = positionString.toInt();
      move_to(position);
      Serial.println(F("new position to move to is: "));
      Serial.println(position);
  } else {
      Serial.println(F("Invalid command format. Use 'MOVE <value>'."));
    }
}

void set_microsteps(String command) {
  int indexOfSpace = command.indexOf(' ');
  if (indexOfSpace != -1) {
    String microstepString = command.substring(indexOfSpace + 1);
    int microstepValue = microstepString.toInt();
      #ifdef DEBUG
      Serial.print(F("Setting microsteps to: "));
      Serial.println(microstepValue);
      #endif
    if (is_valid_microsteps(microstepValue)) {
      MICROSTEPS = microstepValue;
      DRIVER.microsteps(MICROSTEPS);

      #ifdef DEBUG
      Serial.print(F("Microsteps driver actual value: "));
      Serial.println(DRIVER.microsteps());
      Serial.print(F("Microsteps global set value: "));
      Serial.println(MICROSTEPS);
      #endif
      
    } else {
      Serial.print(microstepString);
      Serial.println(F(" is an invalid microstep value."));
    }
  } else {
    Serial.println(F("Invalid command format. Use 'MICRO <Microstep value>'."));
  }
}

void set_angle(String command) {
  int indexOfSpace = command.indexOf(' ');
  if (indexOfSpace != -1) {
    String angleString = command.substring(indexOfSpace + 1);
    float angleValue = angleString.toDouble();
    if (angleValue != 0.0 || angleString == "0" || angleString == "0.0") {
      THETA = angleValue;
    } else {
      Serial.print(angleString);
      Serial.println(F(" is an invalid angle value."));
    }
  } else {
    Serial.println(F("Invalid command format. Use 'ANGLE <value>'."));
  }
}

void pull(String command) {
  int indexOfSpace = command.indexOf(' ');
  if (indexOfSpace != -1) {
    String speedString = command.substring(indexOfSpace + 1);
    float speedValue = speedString.toFloat();
    if (speedValue != 0.0 || speedString == "0" || speedString == "0.0") {
      set_effective_speed_mm_per_second(speedValue);
    } else {
      Serial.print(speedString);
      Serial.println(F(" is an invalid pull value."));
    }
  } else {
    Serial.println(F("Invalid command format. Use 'PULL <value>'."));
  }
}

\
void correct() {
  int max_steps_per_second = 1000;
  float mm_per_sec = get_actuator_speed();
  const int valid_values[] = {0, 2, 4, 8, 16, 64, 128, 256};
  const int size = sizeof(valid_values) / sizeof(valid_values[0]);
  for(int i = size - 1; i >= 0; i--) {
    MICROSTEPS = valid_values[i];
    DRIVER.microsteps(MICROSTEPS);
    set_speed_mm_per_second(mm_per_sec);
    if(MOTOR_SPEED_STEPS <= max_steps_per_second) {
      break;
    }
  }
  return;
  
void move_mm(String command) {
  int indexOfSpace = command.indexOf(' ');
  if (indexOfSpace != -1) {
    String distanceString = command.substring(indexOfSpace + 1);
    float distanceValue = distanceString.toFloat();
    if (distanceValue != 0.0 || distanceString == "0" || distanceString == "0.0") {
      move_to(mm_to_steps(distanceValue));
    } else {
      Serial.print(distanceString);
      Serial.println(F(" is an invalid mm value."));
    }
  } else {
    Serial.println(F("Invalid command format. Use 'MM <value>'."));
  }
}

void nudge_mm(String command) {
  int indexOfSpace = command.indexOf(' ');
  if (indexOfSpace != -1) {
    String distanceString = command.substring(indexOfSpace + 1);
    float distanceValue = distanceString.toFloat();
    if (distanceValue != 0.0 || distanceString == "0" || distanceString == "0.0") {
      float steps = mm_to_steps(distanceValue);
      move_to(STEPPER.currentPosition() + steps);
    } else {
      Serial.print(distanceString);
      Serial.println(F(" is an invalid nudge value."));
    }
  } else {
    Serial.println(F("Invalid command format. Use 'nudge <value>'."));
  }
}
#endif
