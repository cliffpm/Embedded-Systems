/*
  defining the LED and BUTTON pins
*/
#define LED 19
#define BUTTON 20

/*
  in this setup block, we are defining the modes
  for the LED (output) and the BUTTON (input).
  We are using the INPUT_PULLUP mode to assure
  a steady signal for the button when idle.
  Using INPUT_PULLUP causes a low reading when pressed
  and a high reading when not pressed when we are
  reading the signal from the button.
*/

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

/*
  In this loop block, we are simply saying to write
  to the LED according to the state of the button which
  aligns with the task for this section of the lab.
  Note we are inverting the digitalRead signal because
   we defined the BUTTON with INPUT_PULLUP
*/

void loop() {
  digitalWrite(LED, !digitalRead(BUTTON));
}
