/*
  defining led and buttons
*/

#define LED1 19
#define LED2 42
#define BUTTON1 20
#define BUTTON2 2

/*
  state variables of current LED flashing state,
  last button state, and non-blocking timing variables
  for instant response
*/
bool led1Flash = true;
bool led2Flash = true;
bool lastButton1 = HIGH;
bool lastButton2 = HIGH;
unsigned long previousMilli1 = 0;
unsigned long previousMilli2 = 0;
const long interval = 1000;

/*
  setting up pinmode for LEDs as output
  and buttons and input (note INPUT_PULLUP is used so
  the signal read is inverted)
*/

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
}

void loop() {

  /*
  The two if statements below check if the buttons (for both)
  was from a state of unclicked to clicked, in that case 
  then turn the blinking led state to invert, and if the blinking
  state turns off after inverting, then output off to that led.
  */

  unsigned long currentMillis = millis();
  bool currentButton1 = digitalRead(BUTTON1);
  if (lastButton1 == HIGH && currentButton1 == LOW) {
    led1Flash = !led1Flash;
    if (!led1Flash) {
      digitalWrite(LED1, LOW);
    }
  }
  lastButton1 = currentButton1;

  bool currentButton2 = digitalRead(BUTTON2);
  if (lastButton2 == HIGH && currentButton2 == LOW) {
    led2Flash = !led2Flash;
    if (!led2Flash) {
      digitalWrite(LED2, LOW);
    }
  }
  lastButton2 = currentButton2;

/*
  for both if statements, if the led flash state variable 
  is on, then write to that led that invert of its current
  reading state to simulate the blinking behaviour. There 
  is also non-blocking timer math to assure instant response.
*/

  if (led1Flash) {
    if (currentMillis - previousMilli1 >= interval) {
      previousMilli1 = currentMillis;
      digitalWrite(LED1, !digitalRead(LED1));
    }
  }

  if (led2Flash) {
    if (currentMillis - previousMilli2 >= interval) {
      previousMilli2 = currentMillis;
      digitalWrite(LED2, !digitalRead(LED2));
    }
  }
}

