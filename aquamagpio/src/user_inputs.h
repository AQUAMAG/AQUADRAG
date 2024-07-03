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
  while(DRIVER.version() != 0x21) {
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
      stop_motor();
      print_menu();
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

    // // Check for 'CURRENT' command
    // else if (command.startsWith(F("current"))) {
    //   set_current(command);
    // }

    // Check for 'STEPS' command
    else if (command.startsWith(F("steps"))) {
      steps(command);
    }

    // Check for 'MOVE' command
    else if (command.startsWith(F("move"))) {
      move(command);
    }


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

    // Check for 'CORRECT' command
    else if (command.startsWith(F("correct"))) {
      correct();
    }
    // Check for 'NUDGE' command
    else if (command.startsWith(F("nudge"))) {
      nudge_mm(command);
    }

    // Check for 'MM' command
    else if (command.startsWith(F("mm"))) {
      move_mm(command);
    }

    // todo setCurrentPosition(currentPosition);
    else {
      Serial.println(F("UNKNOWN COMMAND"));
      Serial.println(F("     _____"));
      Serial.println(F("    /     \\"));
      Serial.println(F("   | () () |"));
      Serial.println(F("    \\  ^  /"));
      Serial.println(F("     |||||"));
      Serial.println(F("     |||||"));
      
      return;
    }
    print_debug_log();
    return;
}

#endif