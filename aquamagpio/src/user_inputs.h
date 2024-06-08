
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
  Serial.println("Start, Stop, Speed <value>, Move <target step>, Set <value>, Print, Move Set Distance, Go Home, Reset Home");
}