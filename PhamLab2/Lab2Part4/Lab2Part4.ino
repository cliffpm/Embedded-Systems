/*
Filename : Lab2Part4.ino
Author : Cliff Pham, Josh Corrigan
Date : 10/26/26
Description : 
*/

// Includes
#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"
#include "soc/timer_group_reg.h"

// Macros
#define PRESCALER_START 13
#define TIMER_DIVIDER 80
#define TIMER_THRESHOLD 2000000 
#define BRIGHTNESS_THRESHOLD 2000
#define RES 12
#define BUZZER_PIN 48
#define PHOTORESISTOR_PIN 1

// Global Variables
bool buzzing;
int current_buzz;
uint32_t lastTick;

/*
Name: setup
Description: This setup function configures the timer, configures PWM for the buzzer,
  configures the photoresistor as input and sets up global variables accordingly.
*/

void setup() {
  pinMode(PHOTORESISTOR_PIN, INPUT);
  *((volatile uint32_t*)TIMG_T0CONFIG_REG(0)) &= ~TIMG_T0_EN; // good practice to make sure timer off when config.
  *((volatile uint32_t*)TIMG_T0CONFIG_REG(0)) &= ~(0xFFFF << PRESCALER_START); // clears any old values in prescaler
  *((volatile uint32_t*)TIMG_T0CONFIG_REG(0)) |= (((uint32_t)TIMER_DIVIDER) << PRESCALER_START);
  *((volatile uint32_t*)TIMG_T0CONFIG_REG(0)) |= (TIMG_T0_EN | TIMG_T0_INCREASE);
  *((volatile uint32_t*) TIMG_T0UPDATE_REG(0)) = 1;

  ledcAttach(BUZZER_PIN, 200, RES);

  buzzing = false;
  current_buzz = 0;
  lastTick = 0;
}


/*
Name : loop
Description: This loop function will make the buzzer buzz if it is 
  reading ambient light levels above the threshold, and if it is
  in the proper scenario to buzz, it will call the helper function
  buzzerSounds, to loop through the three different frequency sounds.
  The timer will make sure we play each sound for TIMER_THRESHOLD amount.
*/
void loop() {
  *((volatile uint32_t*)TIMG_T0UPDATE_REG(0)) = 1;

  uint32_t currentTick = *((volatile uint32_t*)TIMG_T0LO_REG(0));
  if (analogRead(PHOTORESISTOR_PIN) > BRIGHTNESS_THRESHOLD && current_buzz <= 2) {
    buzzing = true;
    
  } else {
    buzzing = false;
    current_buzz = 0                                                                                                                                                                                                                                                                                                       ;
  }
  if (buzzing) {
    if ((currentTick - lastTick >= TIMER_THRESHOLD)){
      buzzerSounds();
      lastTick = currentTick;
    }
    ledcWrite(BUZZER_PIN, 512);
  }
  else {
    ledcWrite(BUZZER_PIN, 0);
  }

}

/*
Name : buzzerSounds
Description: This helper function will switch through
  three different sound at three different frequencies
  just as according to the lab handout.
*/
void buzzerSounds() {
  switch(current_buzz) {
    case 0:
      ledcChangeFrequency(BUZZER_PIN, 200, RES);
      current_buzz++;
      break;
    case 1:
      ledcChangeFrequency(BUZZER_PIN, 400, RES);
      current_buzz++;
      break;
    case 2:
      ledcChangeFrequency(BUZZER_PIN, 1000, RES);
      current_buzz++;
      break;
    default:
      ledcChangeFrequency(BUZZER_PIN, 200, RES);
      current_buzz++;
      break;
  }
}