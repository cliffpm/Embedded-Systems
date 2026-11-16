/*
Filename : Lab2Part1Step2B.ino
Author : Cliff Pham, Josh Corrigan
Date : 10/26/26
Description : This file records the runtime of turning an LED on and off for 1000 iterations using register access.
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
Description: This setup function sets GPIO_PIN as a GPIO, and enables
  GPIO_PIN as an output.
*/
void setup() {
  
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[GPIO_PIN], PIN_FUNC_GPIO);
  *((volatile uint32_t*)GPIO_ENABLE_REG) |= ( 1 << GPIO_PIN);
  Serial.begin(9600);
}

/*
Name : loop
Description: This loop function turns the LED on, waits 1 second,
  turns the LED off, waits 1 second and repeats using register access,
  and is recorded the runtime for this sequence in 1000 iterations,
  and the runtime is printed to the serial monitor.
*/
void loop() {
  unsigned long startTime = micros();
  for (int i = 0; i < 1000; i++) {
    *((volatile uint32_t*)GPIO_OUT_REG) |= (1 << GPIO_PIN);
    *((volatile uint32_t*)GPIO_OUT_REG) &= ~(1 << GPIO_PIN);
  }
  unsigned long endTime = micros();
  unsigned long totalTime = endTime - startTime;
  Serial.print("Total Time :");
  Serial.println(totalTime);
}
