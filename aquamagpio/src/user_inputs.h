#ifndef USER_INPUTS_H

#include "globals.h"

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

#endif