
/*
  This setup block sets the pin 19 and pin 20 LED's
  as output, so we can configure their output states.
*/

void setup() {
  pinMode(19, OUTPUT);
  pinMode(20, OUTPUT);
}

/*
  This loop block calls the blink helper function for 
  both the LED's for pin 19 and pin 20 at different
  frequencies.
*/

void loop() {
  blink(19, 1000);
  blink(20, 500); 
}

/*
  The blink helper function takes in the pin number of the LED
  alongside the frequency interval between blinks, and creates
  a blinking pattern that turns the LED on, waits for the frequency,
  turns off, waits for the frequency, and then repeats.
*/

void blink(int pin, int frequency) {
  digitalWrite(pin, HIGH);
  delay(frequency);
  digitalWrite(pin, LOW);
  delay(frequency);
}
