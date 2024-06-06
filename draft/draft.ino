// // Defining
// #define StepPin     2
// #define RX_PIN      0
// #define TX_PIN      1

// #define Current     1200
// #define MicroStep   0     // 0: Native 256 microstep setting.
//                           // 1: 128 microsteps per full step.
//                           // 2: 64 microsteps per full step.
//                           // 3: 32 microsteps per full step.
//                           // 4: 16 microsteps per full step.
//                           // 5: 8 microsteps per full step.
//                           // 6: 4 microsteps per full step.
//                           // 7: 2 microsteps per full step.
//                           // 8: Full step.

// // Including library
// #include <TMCStepper.h>                                // Include library
// TMC2208Stepper driver = TMC2208Stepper(RX_PIN, TX_PIN);    // Create driver and use

// // Setup
// void setup() {
//   driver.beginSerial(115200);    
//   driver.push();

//   // Prepare pins
//   pinMode(StepPin, OUTPUT);

//   driver.pdn_disable(1);              // Use PDN/UART pin for communication
//   driver.I_scale_analog(0);           // Adjust current from the registers
//   driver.rms_current(Current);        // Set driver current 
//   driver.toff(5);                     // Enable driver
//   driver.shaft(1);
//   driver.en_spreadCycle(0);           // 1: spreadCycle
//                                       // 0: stealthChop
//   driver.pwm_autoscale(1);            // 1: if stealthChop is chosen. Otherwise 0
//   driver.mstep_reg_select(1);
//   driver.mres(MicroStep);
// }

// // Looping
// void loop() {
//   digitalWrite(StepPin, !digitalRead(StepPin));  // Step
//   delayMicroseconds(100);
// }