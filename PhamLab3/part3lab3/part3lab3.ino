#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "soc/timer_group_reg.h"
#include "esp_timer.h"
#include <LiquidCrystal_I2C.h>
//===============> TODO:
// Generate random Service and Characteristic UUIDs: https://www.uuidgenerator.net/


#define SERVICE_UUID        "063ed6a1-0ae5-4118-8615-6746c234c6e5"
#define CHARACTERISTIC_UUID "e3b08038-1268-4b65-8450-990267206bde"
#define BUTTON 6
#define TIMER_DIVIDER 80
#define PRESCALER_START 13
#define TOGGLE_INTERVAL_US 1000000

volatile int counter;
volatile bool paused;
volatile bool buttonPressed;
volatile bool bluetoothTriggered;
volatile bool printCount;
volatile unsigned long pauseStartTimeMilli;
bool completedButtonOrBLE; // marks if we already printed bc. of ble or button already. so we dont print repeatedly.

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD
esp_timer_handle_t periodic_timer;



class MyCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic) {
     // =========> TODO: This callback function will be invoked when signal is
     // 		     received over BLE. Implement the necessary functionality that
     //		     will trigger the message to the LCD.
    //  lcd.print("New Message!");
     paused = true;
     bluetoothTriggered = true;
     pauseStartTimeMilli = millis();
    return;
   }
};


// ==============> TODO: Write your timer ISR here.
void IRAM_ATTR timer_isr(void* pv) {
 // esp_timer_stop(periodic_timer);
  if (!paused) counter++;
  printCount = true;
  return;
}

void IRAM_ATTR button_isr() {
  // lcd.print("Button Pressed");
  //esp_timer_stop(periodic_timer);
  buttonPressed = true;
  paused = true;
  pauseStartTimeMilli = millis();
  return;
}

// ==============> TODO: Create an ISR function to handle button press here.


const esp_timer_create_args_t timer_args = {
  .callback = &timer_isr,
  .arg = NULL,
  .name = "timer"
};


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
  printCount= false;
  pauseStartTimeMilli = 0;
  completedButtonOrBLE = false;

   // =========> TODO: Initialize LCD display
   //
  lcd.backlight();
  lcd.init();
  lcd.clear();
   // =========> TODO: create a timer, attach an interrupt, set an alarm which will
   //                  update the counter every second.
   //
    esp_timer_create(&timer_args, &periodic_timer);
    esp_timer_start_periodic(periodic_timer, TOGGLE_INTERVAL_US); // fires interupt every 1 second

    // ========> TODO: Set button pin as input and attach an interrupt
    pinMode(BUTTON, INPUT_PULLUP);
    attachInterrupt(BUTTON, button_isr, FALLING); // input_pullup, so falling = pressed
}


void loop() {
  unsigned long currTimeMilli = millis();

  if (paused) { // either bluetooth or button ISR was pressed
    if (currTimeMilli - pauseStartTimeMilli >= 2000) { // 2 seconds have passed
      paused = !paused;
      // completedButtonOrBLE = false;
    }
    if (!completedButtonOrBLE) {
      if (bluetoothTriggered) {
        lcd.clear();
        lcd.print("New Message!");
      }
      else {
        lcd.clear();
        lcd.print("Button Pressed");
      }
      completedButtonOrBLE = true;
    }
  }

  else { // default case. print the incremented counter value to lcd
    //esp_timer_start_periodic(periodic_timer, TOGGLE_INTERVAL_US);
    completedButtonOrBLE = false;
    lcd.clear();
    lcd.print(counter);
  }
 // =========> TODO: Print out an incrementing counter to the LCD.
 //                  If a signal has been received over BLE, print out “New
 //                  Message!” on the LCD.
 //                  If the button has been pressed, print out "Button Pressed"
 //                  on the LCD.
  delay(100);
}
