#include <Wire.h>


// Ensure that the LiquidCrystal I2C library by Frank de Brabander is installed in your Arduino IDE Library Manager
#include <LiquidCrystal_I2C.h>

bool finishedPrint;


LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD
void setup() {
  finishedPrint = false;
  Serial.begin(115200);
  Wire.begin();
  lcd.init();

  
  resetScreen();
    

  delay(2);
}

// maybe have some state variable so each new time entered message, clear old message. 
// How to tell when finished printing all of message?

void loop() {
  // ====================> TODO:
  // Write code that takes Serial input and displays it to the LCD. Do NOT use any 
  // functions from the LiquidCrystal I2C library here.

  if (Serial.available()) {

  if (finishedPrint) {
    resetScreen();
    delay(2);
    finishedPrint = false;
  }

    // read in the text that is entered
    char incoming = Serial.read();

    if (incoming != '\n' && incoming != '\r') {
      // send to screen

      // all in the position for data
      uint8_t highNibble = (incoming & 0xF0); 
      uint8_t lowNibble = ((incoming << 4) & 0xF0);

      printLCD(highNibble);
      printLCD(lowNibble);

      


    }
    else {
      finishedPrint = true;
    }


  }

}

void printLCD(uint8_t data) {
  Wire.beginTransmission(0x27);
  Wire.write(data | 0x0D);
  Wire.write(data | 0x09);
  Wire.endTransmission();

}

void resetScreen() {
  Wire.beginTransmission(0x27);
  Wire.write(0x00 | 0x0C);
  Wire.write(0x00 | 0x08);

  Wire.write((1 << 4) | 0x0C);
  Wire.write((1 << 4) | 0x08);
  Wire.endTransmission();

  Wire.beginTransmission(0x27);
  Wire.write(0x80 | 0x0C);
  Wire.write(0x80 | 0x08);

  Wire.write(0x00 | 0x0C);
  Wire.write(0x00 | 0x08);

  Wire.endTransmission();
}

