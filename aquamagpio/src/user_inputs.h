#ifndef USER_INPUTS_H

#include "globals.h"
#include "motor_commands.h"

// Clear the serial buffer by reading and discarding any data
// read from the serial buffer until it is empty
void clearSerialBuffer() {
  while (Serial.available() > 0) {
    Serial.read();
  }
  Serial.println("Serial buffer cleared.");
}
// Wait for user input by checking if there is any data available
// in the serial buffer. If there is no data available, the function
// will wait until there is data available.
void wait_for_input() {
  clearSerialBuffer();
  while (Serial.available() <= 0);
}

void print_menu(){
  Serial.println("Enter a command:");
  Serial.println("Start, Stop, Print, Speed <value>, Home, Move <target step>, Set <value>, help");
}

void verify_UART_connection(){
  while(driver.version() != 0x21) {
    Serial.println("UART communication failed. Retrying...");
    delay(2000);
  }
  Serial.println("UART communication verified.");
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
      stop_motor(&stepper);
      print_menu();
    }

    // Check for 'Print' command
    else if (command.equals(F("print"))) {
      print_debug_log(&stepper, &driver);
    }

    // Check for 'START' command
    else if (command.equals(F("start"))) {
      //Serial.println("Enter speed in mm/sec: ");
      //wait_for_input();
      command = Serial.readStringUntil('\n');
      Serial.println(command);
      speed(&stepper, command);
      //todo make direction do something
      // Serial.println("Enter direction (fwd/back): ");
      // wait_for_input();
      start_motor(&stepper, &driver);
    }

    // Check for 'SPEED' command
    else if (command.startsWith(F("speed"))) {
      speed(&stepper, command);
    }

    // Check for 'CURRENT' command
    else if (command.startsWith(F("current"))) {
      set_current(command);
    }

    // Check for 'STEPS' command
    else if (command.startsWith(F("steps"))) {
      steps(&stepper, command);
    }

    // Check for 'MOVE' command
    else if (command.startsWith(F("move"))) {
      move(&stepper, command);
    }


    // Check for 'HOME' command
    else if (command.startsWith(F("home"))) {
      home_motor();
    }

    // Check for 'MICRO' command
    else if (command.startsWith(F("micro"))) {
      set_microsteps(&driver, command);
    }

    // Check for 'ANGLE' command
    else if (command.startsWith(F("angle"))) {
      set_angle(command);
    }

    // Check for 'PULL' command
    else if (command.startsWith(F("pull"))) {
      pull(&stepper, &driver, command);
    }

    // todo setCurrentPosition(currentPosition);
    else {
      Serial.println(F("Unknown command."));
    }
    print_debug_log(&stepper, &driver);
}

#endif