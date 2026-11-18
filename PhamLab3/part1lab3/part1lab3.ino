/*
Filename: part1lab3.ino
Authors: Cliff Pham and Josh Corrigan
Date: 11/17/2026
Description: Reads text typed into the Serial Monitor and displays it on external
  LCD screen through I2C.
*/

// ================= Includes =================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================= Global Variables =================
bool finishedPrint;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD


// ================= Function Prototypes =================

/*
Name: printLCD
Description: Sends one 4-bit nibble of character data to the LCD over I2C.
*/
void printLCD(uint8_t data);

/*
Name: resetScreen
Description: Clears the LCD and returns the cursor to the first position
  using raw I2C writes.
*/
void resetScreen();

// ================= Setup and Loop =================

void setup() {
  finishedPrint = false;
  Serial.begin(115200);
  Wire.begin();
  lcd.init();  
  resetScreen();
  delay(2);
}

void loop() {
  if (Serial.available()) {

    if (finishedPrint) {
      resetScreen();
      delay(2);
      finishedPrint = false;
    }
  
    char incoming = Serial.read();

    if (incoming != '\n' && incoming != '\r') {

      // split incoming data to high and low nibble.
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
// ================= Function Implementations =================

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