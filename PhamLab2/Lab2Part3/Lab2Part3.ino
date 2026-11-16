/*
Filename : Lab2Part3.ino
Author : Cliff Pham, Josh Corrigan
Date : 10/26/26
Description : This file reads lighting in the room and adjust an LED
  based off that.
*/

// macros
#define LED_PIN 47
#define FREQ 10000
#define RES 10
#define PHOTORESISTOR_PIN 1


// function signatures / headers
int brightnessFunction(int reading);

// Name : setup
// Description: The setup function sets up the PWM for the LED, and 
//  sets up the photoresistor as an input.
void setup() {
  ledcAttach(LED_PIN, FREQ, RES);
  pinMode(PHOTORESISTOR_PIN, INPUT);
}

// Name : loop
// Description: This loop function reads in the photoresistor value, and 
//  controls the LED output based off the photoresistor reading with our
//  custom brightness function.
void loop() {
  int currentReading = analogRead(PHOTORESISTOR_PIN);
  ledcWrite(LED_PIN, brightnessFunction(currentReading));
}

// Name : brightnessFunction
// Description : This function takes in the reading value from the photoresistor,
//  and maps it such that the function outputs a high value when the photoresistor is dark
//  and low when the photoresistor reads bright, which is then used to control our LED.
int brightnessFunction(int reading) {
  return ((4000-reading)*5) / 12;
}