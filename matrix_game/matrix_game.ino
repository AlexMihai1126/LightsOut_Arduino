#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>
#define matrixDIN 10
#define matrixCS 9
#define matrixCLK 8
#define displayD7 7
#define displayD6 6
#define displayD5 5
#define displayD4 4
#define displayBacklight 3  //PWM controllable
#define displayEN 12
#define displayRS 13
#define buzzerPin 2
#define redPushbtn A4
#define joystickX A1
#define joystickY A2
#define joystickSw A0
#define ambientLightSensor A3
#define matrixRows 16
#define matrixColumns 8
#define storedParametersCount 6
#define debounceTime 75
#define blinkInterval 250
#define maxCharsName 8

byte logicalMatrix[matrixRows][matrixColumns] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};


enum brightnessLevels {
  lowBrightness,
  midBrightness,
  highBrightness,
  ultraBrightness
};

enum menuStates {
  intro,
  startGame,
  highscores,
  settings,
  about,
  tutorial,
  inGame,
  endGame
};

enum settingsSubmenus {
  nameInput,
  brightnessControl,
  soundControl,
  gameLevel,
  resetHighscores
};

enum gameLevels {
  practice,
  easy,
  normal,
  hard
};

enum storedParameters {
  mtxBright,
  lcdBright,
  autoBright,
  soundOn,
  lastLevel,
  highscoreStartAddr
};

enum joystickState {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  STATIC
};

joystickState joyState = STATIC;

struct highscore {
  char name[maxCharsName] = "";
  int scoreValue = 0;
};

short adresses[storedParametersCount] = { 0, 1, 2, 3, 4, 5 };

byte matrixBrightness = 2;
byte lcdBrightness = 127;
bool autoBrightness = false;
bool isSoundOn = true;  //defaults

byte joySwReading = LOW;
byte joySwState = LOW;
byte joySwStateLastReading = LOW;

int minThreshold = 480;
int maxThreshold = 550;
int xValue = 0;
int yValue = 0;

unsigned long lastDebounceTime = 0;

highscore hs1;
highscore hs2;
highscore hs3;

LiquidCrystal lcd(displayRS, displayEN, displayD4, displayD5, displayD6, displayD7);
LedControl matrix = LedControl(matrixDIN, matrixCLK, matrixCS, 2);

void loadParameters() {
  matrixBrightness = EEPROM.read(adresses[mtxBright]);
  lcdBrightness = EEPROM.read(adresses[lcdBright]);
  autoBrightness = EEPROM.read(adresses[autoBright]);
  isSoundOn = EEPROM.read(adresses[soundOn]);
}

void loadHighscores() {
  short readAdress = adresses[highscoreStartAddr] + sizeof(highscore);
  EEPROM.get(readAdress, hs1);
  readAdress += sizeof(highscore);
  EEPROM.get(readAdress, hs2);
  readAdress += sizeof(highscore);
  EEPROM.get(readAdress, hs3);
}

void displayMatrix() {
  for (int row = 0; row < matrixRows; row++) {
    for (int col = 0; col < matrixColumns; col++) {
      matrix.setLed(1, 15-row, 7-col, logicalMatrix[row][col]);
      matrix.setLed(0, 15-row, 7-col, logicalMatrix[row][col]);  // Update each LED state
    }
  }
}

void autoBrightnessController() {
  //todo
}

void getJoystickState() {
  xValue = analogRead(joystickX);
  yValue = analogRead(joystickY);
  if (xValue > maxThreshold) {
    joyState = UP;
  }

  if (xValue < minThreshold) {
    joyState = DOWN;
  }

  if (yValue > maxThreshold) {
    joyState = RIGHT;
  }

  if (yValue < minThreshold) {
    joyState = LEFT;
  }

  if (xValue >= minThreshold && xValue < maxThreshold && yValue >= minThreshold && yValue < maxThreshold) {
    joyState = STATIC;
  }
}

void getJoySwitchState() {
  joySwReading = !digitalRead(joystickSw);
  if (joySwReading != joySwStateLastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) >= debounceTime) {
    if (joySwReading != joySwState) {
      joySwState = joySwReading;
      if (joySwReading == HIGH) {
        joySwState = HIGH;
      } else {
        joySwState = LOW;
      }
    }
  }
  joySwStateLastReading = joySwReading;
}


void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(redPushbtn, INPUT_PULLUP);
  pinMode(joystickX, INPUT);
  pinMode(joystickY, INPUT);
  pinMode(ambientLightSensor, INPUT);
  pinMode(displayBacklight, OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  loadParameters();
  loadHighscores();
  analogWrite(displayBacklight, lcdBrightness);
  matrix.shutdown(0, false);
  matrix.setIntensity(0, matrixBrightness);
  matrix.clearDisplay(0);
  matrix.shutdown(1, false);
  matrix.setIntensity(1, matrixBrightness);
  matrix.clearDisplay(1);
  randomSeed(analogRead(A5));
}

void loop() {
  displayMatrix();
  lcd.setCursor(0, 0);
  lcd.print(millis() / 1000);
}
