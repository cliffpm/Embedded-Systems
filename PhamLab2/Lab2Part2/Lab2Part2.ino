/*
Filename : Lab2Part2.ino
Author : Cliff Pham, Josh Corrigan
Date : 10/26/26
Description : This file controls a blinking LED with a nonblocking hardware timer
*/

// Includes
#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"
#include "soc/timer_group_reg.h"

// Macros
#define GPIO_PIN 5
#define PRESCALER_START 13
#define TIMER_DIVIDER 80
#define THRESHOLD 500000 

// Global Variables
uint32_t lastTick = 0;
bool ledState;


/*
Name: setup
Description: This setup function sets the LED pin as GPIO and output,
  and configures the hardware timer accordingly to how the lab handout
  wants it to be set up.
*/
void setup() {
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[GPIO_PIN], PIN_FUNC_GPIO);
  *((volatile uint32_t*)GPIO_ENABLE_REG) |= ( 1 << GPIO_PIN);

  *((volatile uint32_t*)TIMG_T0CONFIG_REG(0)) &= ~TIMG_T0_EN; // good practice to make sure timer off when config.
  

  *((volatile uint32_t*)TIMG_T0CONFIG_REG(0)) &= ~(0xFFFF << PRESCALER_START); // clears any old values in prescaler
  *((volatile uint32_t*)TIMG_T0CONFIG_REG(0)) |= (((uint32_t)TIMER_DIVIDER) << PRESCALER_START);
  
  *((volatile uint32_t*)TIMG_T0CONFIG_REG(0)) |= (TIMG_T0_EN | TIMG_T0_INCREASE);

  *((volatile uint32_t*) TIMG_T0UPDATE_REG(0)) = 1;
}


/*
Name: loop
Description: This loop function toggles between states of the
  LED for every threshold (defined above as a macro), reading
  the hardware timer to do so.
*/
void loop() {
  *((volatile uint32_t*)TIMG_T0UPDATE_REG(0)) = 1;
  uint32_t currentTick = *((volatile uint32_t*)TIMG_T0LO_REG(0));
  if (currentTick - lastTick >= THRESHOLD) {
    if (ledState) {
      *((volatile uint32_t*)GPIO_OUT_REG) &= ~(1 << GPIO_PIN);
    }
    else {
      *((volatile uint32_t*)GPIO_OUT_REG) |= (1 << GPIO_PIN);

    }
    lastTick = currentTick;
    ledState = !ledState;
  }
}
