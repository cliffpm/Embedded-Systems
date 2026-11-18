/*
Filename: part2lab3.ino
Authors: Cliff Pham and Josh Corrigan
Date: 11/17/2026
Description: A priority-based task scheduler implementation. Four tasks
  (LED blinker, LCD counter, music player, alphabet printer) are stored in a
  task list of Task Control Blocks (TCBs). Each pass of loop() runs the
  incomplete task with the highest priority (lowest number) and marks it done.
  When every task has finished, all tasks are reset and their priorities are
  rotated by one so a different task goes first on the next cycle.
*/

// ================= Includes =================
#include "driver/gpio.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_reg.h"
#include "soc/gpio_periph.h"
#include <LiquidCrystal_I2C.h>

// ================= Macros =================
#define STATE_INCOMPLETE 0
#define STATE_DONE 1
#define N_MAX_TASKS 4
#define LED1_PIN 4
#define LED2_PIN 47
#define BUZZER_PIN 19
#define BUZZER_FREQ 200
#define BUZZER_RES 12
#define LED_FREQ 10000
#define LED_RES 10
#define TIMER_THRESHOLD 1000 

// Task Control Block (TCB) with relevant information the scheduler needs for each task
typedef struct {
  void (*fptr) (void);
  unsigned short int state;
  unsigned int priority;
  const char* name;
}TCBstruct;

// ================= Global Variables =================
TCBstruct TaskList[N_MAX_TASKS];
int currentBuzz;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD
uint32_t lastTick;
bool musicCycleDone;

// ================= Function Prototypes =================

/*
Name: resetTasks
Description: Marks every task as incomplete and
  rotates each task's priority by one so the task order
  changes every cycle.
*/
void resetTasks();


/*
Name: getHighestPriority
Description: Searches TaskList for the incomplete task with the highest priority (lowest value)
*/
int getHighestPriority();

/*
Name: blinkEight
Description: Blinks LED1 eight times in 1 second duration.
*/
void blinkEight();

/*
Name: printToTen
Description: Counts from 1 to 10 on the LCD, with 1-5 on the first row and
  6-10 on the second row, then clears the screen.
*/
void printToTen();

/*
Name: playNotes
Description: Plays ten notes on the buzzer, one second each. LED2's brightness is scaled to the
  current buzzer frequency so it gets brighter as the pitch rises. The buzzer
  and LED2 are turned off when finished.
*/
void playNotes();

/*
Name: printLetters
Description: Prints the lowercase letters a through z to the Serial Monitor
*/
void printLetters();

/*
Name : buzzerSounds
Description: This helper function will switch through
  three different sound at three different frequencies
  just as according to the lab handout.
*/
void buzzerSounds(int currentBuzz);

// ================= Setup and Loop =================

void setup() {
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[LED1_PIN], PIN_FUNC_GPIO);
  *((volatile uint32_t*) GPIO_ENABLE_REG) |= ( 1 << LED1_PIN);
  lcd.backlight();
  lcd.init();
  lcd.clear();
  delay(2);
  Serial.begin(115200);
  
  ledcAttach(BUZZER_PIN, BUZZER_FREQ, BUZZER_RES);
  ledcAttach(LED2_PIN, LED_FREQ, LED_RES);
  currentBuzz = 0;
  lastTick = 0;
  musicCycleDone = false;

  TaskList[0].fptr = blinkEight;
  TaskList[0].state = STATE_INCOMPLETE;
  TaskList[0].priority = 1;
  TaskList[0].name = "LED Blinker";

  TaskList[1].fptr = printToTen;
  TaskList[1].state = STATE_INCOMPLETE;
  TaskList[1].priority = 2;
  TaskList[1].name = "LCD Counter";


  TaskList[2].fptr = playNotes;
  TaskList[2].state = STATE_INCOMPLETE;
  TaskList[2].priority = 3;
  TaskList[2].name = "Music Player";


  TaskList[3].fptr = printLetters;
  TaskList[3].state = STATE_INCOMPLETE;
  TaskList[3].priority = 4;
  TaskList[3].name = "Alphabet Printer";
}

void loop() {
  int bestIndex = getHighestPriority(); // if bestIndex is -1, that means no more ready tasks
  if (bestIndex == -1) {
    resetTasks();
  }
  else {
    TaskList[bestIndex].fptr(); 
    TaskList[bestIndex].state = STATE_DONE;
    Serial.printf("%s : %d\n", TaskList[bestIndex].name, TaskList[bestIndex].priority);
  }
}

// ================= Function Implementations =================

void resetTasks() {
  for (int i = 0; i < N_MAX_TASKS; i++) {
    TaskList[i].state = STATE_INCOMPLETE;
    TaskList[i].priority = (TaskList[i].priority % 4) + 1; // wrap around
  }
  return;
}

int getHighestPriority() {
  int bestIndex = -1;
  int bestPriority = 5;
  for (int i = 0; i < N_MAX_TASKS; i++) {
    if (TaskList[i].state == STATE_INCOMPLETE) { // valid task
      if (TaskList[i].priority < bestPriority) {
        bestPriority = TaskList[i].priority;
        bestIndex = i;
      }
    }
  }
  return bestIndex;
}

void blinkEight () {
  for (int i = 0; i < 8; i ++) {
    *((volatile uint32_t*)GPIO_OUT_REG) |= (1 << LED1_PIN);
    delay(1000);
    *((volatile uint32_t*)GPIO_OUT_REG) &= ~(1 << LED1_PIN);
    delay(1000);
  }
  return;
}

void printToTen() {
  for (int i = 0; i < 10; i++) {
    if (i == 5) {
      lcd.setCursor(0,1); // print 6-10 on second row
    }
    lcd.print((i+1));
    lcd.print(" ");
    delay(300);
  }
  delay(500);
  lcd.clear(); 
  return;
}

void playNotes() {
  for (int i = 0; i < 10; i++) {
    buzzerSounds(i);
    ledcWrite(BUZZER_PIN, 512);
    int ledBrightness = map(ledcReadFreq(BUZZER_PIN), 200, 1200, 0, 1023);
    ledcWrite(LED2_PIN, ledBrightness);
    delay(TIMER_THRESHOLD);
  }
  ledcWrite(BUZZER_PIN, 0);
  ledcWrite(LED2_PIN, 0);
  return;
}

void printLetters() {
  for (int i = 97; i < 123; i++) { // loop from a to z ASCII value
    Serial.println(char(i));
  }
  return;
}


void buzzerSounds(int currentBuzz) {
  switch(currentBuzz) {
      case 0:
        ledcChangeFrequency(BUZZER_PIN, 200, BUZZER_RES);
        break;
      case 1:
        ledcChangeFrequency(BUZZER_PIN, 300, BUZZER_RES);
        break;
      case 2:
        ledcChangeFrequency(BUZZER_PIN, 400, BUZZER_RES);
        break;
      case 3:
        ledcChangeFrequency(BUZZER_PIN, 500, BUZZER_RES);
        break;
      case 4:
        ledcChangeFrequency(BUZZER_PIN, 600, BUZZER_RES);
        break;
      case 5:
        ledcChangeFrequency(BUZZER_PIN, 700, BUZZER_RES);
        break;
      case 6:
        ledcChangeFrequency(BUZZER_PIN, 800, BUZZER_RES);
        break;
      case 7:
        ledcChangeFrequency(BUZZER_PIN, 900, BUZZER_RES);
        break;
      case 8:
        ledcChangeFrequency(BUZZER_PIN, 1000, BUZZER_RES);
        break;
      case 9:
        ledcChangeFrequency(BUZZER_PIN, 1200, BUZZER_RES);
        break;
    }
}