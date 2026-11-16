/*
Filename : Lab2Part1Step2A.ino
Author : Cliff Pham, Josh Corrigan
Date : 10/26/26
Description : This file compares the runtime from libary calls versus direct
  register access to control an external LED.
*/

// Includes
#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"

// Macros
#define GPIO_PIN 5


// Global Variables

// Function Prototypes

/*
Name : setup
Description: This setup function sets GPIO_PIN as an output
  using the Arduino library function pinMode.
*/
void setup() {
  pinMode(GPIO_PIN, OUTPUT);
  Serial.begin(9600);
}


/*
Name : loop
Description: This loop functions turns the LED on and off
  for 1000 iterations, and the runtime is then printed to the
  serial monitor of how long the 1000 iteration ran for.
*/
void loop() {


  unsigned long startTime = micros(); 


  for (int i = 0; i < 1000; i++) {
    digitalWrite(GPIO_PIN, HIGH);
    digitalWrite(GPIO_PIN, LOW);
  }
  unsigned long endTime = micros();
  unsigned long totalTime = endTime - startTime;

  Serial.print("Total time for 1000 reps : ");
  Serial.println(totalTime);
}
