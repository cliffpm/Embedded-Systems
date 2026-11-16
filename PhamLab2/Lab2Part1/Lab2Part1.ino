/*
Filename : Lab2Part1Step1.ino
Author : Cliff Pham, Josh Corrigan
Date : 10/26/26
Description : This file controls an external LED using direct register access and manipulation
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
}

/*
Name : loop
Description: This loop function turns the LED on, waits 1 second,
  turns the LED off, waits 1 second and repeats using register access.
*/
void loop() {
  *((volatile uint32_t*)GPIO_OUT_REG) |= (1 << GPIO_PIN);
  delay(1000);
  *((volatile uint32_t*)GPIO_OUT_REG) &= ~(1 << GPIO_PIN);
  delay(1000);
}
