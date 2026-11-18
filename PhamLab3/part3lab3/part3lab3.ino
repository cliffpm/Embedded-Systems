/*
Filename : part3lab3
Author : Cliff Pham and Josh Corrigan
Description : This file handles three asynchronous events (BLE write, button
  press, and a periodic timer) that interact with the external LCD screen.
  A 1 second timer increments a counter that is shown on the LCD. A button
  press or a BLE write pauses the counter for 2 seconds and shows a message
  ("Button Pressed" or "New Message!") instead.
*/

// ================= Includes =================
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "soc/timer_group_reg.h"
#include "esp_timer.h"
#include <LiquidCrystal_I2C.h>


// ================= Macros =================
#define SERVICE_UUID        "063ed6a1-0ae5-4118-8615-6746c234c6e5"
#define CHARACTERISTIC_UUID "e3b08038-1268-4b65-8450-990267206bde"
#define BUTTON 6
#define TOGGLE_INTERVAL_US 1000000 // 1 second in microsec.

// ================= Global Variables =================

volatile int counter;                       // number of seconds counted (only increments while not paused)
volatile bool paused;                       // true while the counter is paused (2 second pause after a button/BLE event)
volatile bool buttonPressed;                // set by the button ISR, cleared in loop() after the message is shown
volatile bool bluetoothTriggered;           // set by the BLE write callback, cleared in loop() after the message is shown
volatile unsigned long pauseStartTimeMilli; // millis() timestamp of when the most recent pause began


LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD
esp_timer_handle_t periodic_timer; // timer handle


// ================= Function Prototypes =================

/*
Name: timer_isr
Description: Callback for the 1 second periodic timer. Increments the counter
  only when not paused, so after a pause the count resumes from where it left off.
*/
void IRAM_ATTR timer_isr(void* pv);

/*
Name: button_isr
Description: Interrupt handler in the event of a button press. Pauses the
  counter and flags signal to display "Button Pressed" to the LCD screen. Pressing again during
  a pause restarts the 2 second pause window. After the message print to the screen, there is a 
  2 second delay before the counter increment ISR resumes.
*/

void IRAM_ATTR button_isr(void* pv);


   /*
   Name: onWrite
   Description: In the event BLE signal is detected, execute the function
    to print "New Message!" onto the LCD screen, and then wait 2 seconds afterwards
    until the counter increment ISR runs again.
   */
class MyCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic) {
     paused = true;
     bluetoothTriggered = true;
     pauseStartTimeMilli = millis();
    return;
   }
};




// argument for hardware timer handler
const esp_timer_create_args_t timer_args = {
  .callback = &timer_isr,
  .arg = NULL,
  .name = "timer"
};

// ================= Setup and Loop =================

void setup() {
  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                          CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_WRITE
                                      );


  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  paused = false;

  buttonPressed= false;
  bluetoothTriggered= false;
  pauseStartTimeMilli = 0;

  lcd.backlight();
  lcd.init();
  lcd.clear();

  esp_timer_create(&timer_args, &periodic_timer); // hardware timer hooks up to timer count incr. ISR
  esp_timer_start_periodic(periodic_timer, TOGGLE_INTERVAL_US); // fires interupt every 1 second

  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(BUTTON, button_isr, FALLING);  // attatch button ISR to button
}


void loop() {
  unsigned long currTimeMilli = millis();
  if (paused) { // either bluetooth or button ISR was pressed.
    if (currTimeMilli - pauseStartTimeMilli >= 2000) { // 2 second delay afterwards for BLE or button ISR
      paused = !paused;
    }
    if (bluetoothTriggered) {
      lcd.clear();
      lcd.print("New Message!");
      bluetoothTriggered = false;
    }
    else if (buttonPressed) {
      lcd.clear();
      lcd.print("Button Pressed");
      buttonPressed = false;
    }
  }

  else { 
    lcd.clear();
    lcd.print(counter);
  }
  delay(100);
}

// ================= Function Implementations =================

void IRAM_ATTR timer_isr(void* pv) {
  if (!paused) counter++; // Design implementation. Incrementing only when not paused so after pause, ensure counter's value is where it was left off.
  return;
}

void IRAM_ATTR button_isr(void* pv) {
  buttonPressed = true;
  paused = true;
  pauseStartTimeMilli = millis();
  return;
}