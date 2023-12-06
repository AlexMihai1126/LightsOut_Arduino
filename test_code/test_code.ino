#include <LiquidCrystal.h>
#include <LedControl.h>
#define matrix1DIN 9
#define matrix1CS 8
#define matrix1CLK 7
#define matrix2DIN 13
#define matrix2CS 12
#define matrix2CLK 10
#define displayD7 6
#define displayD6 4
#define displayD5 2
#define displayD4 1
#define displayBacklight 5  //PWM controllable
#define displayEN 0
#define displayRS A5  //will be using it as digital out as I don't have enough pins for all the parts
#define buzzerPin 3
#define redPushbtn 11
#define joystickX A0
#define joystickY A1
#define ambientLightSensor A2
#define matrixRows 16
#define matrixColumns 8

byte matrixBrightness = 1;
byte lcdBrightness = 255;

LiquidCrystal lcd(displayRS, displayEN, displayD4, displayD5, displayD6, displayD7);
LedControl matrix1 = LedControl(matrix1DIN, matrix1CLK, matrix1CS, 1);
LedControl matrix2 = LedControl(matrix2DIN, matrix2CLK, matrix2CS, 1);  //using the matrix as 2 separate objects (parallel mode)

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(redPushbtn, INPUT_PULLUP);
  pinMode(joystickX, INPUT);
  pinMode(joystickY, INPUT);
  pinMode(ambientLightSensor, INPUT);
  pinMode(displayBacklight, OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  analogWrite(displayBacklight, lcdBrightness);
  matrix1.shutdown(0, false);
  matrix1.setIntensity(0, matrixBrightness);
  matrix1.clearDisplay(0);
  matrix2.shutdown(0, false);
  matrix2.setIntensity(0, matrixBrightness);
  matrix2.clearDisplay(0);
  lcd.print("ready");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
  for (int row = 0; row < matrixColumns; row++) {
    for (int col = 0; col < matrixColumns; col++) {
      matrix1.setLed(0, row, col, true);  // turns on LED at col, row
      matrix2.setLed(0, row, col, true);
      delay(25);
    }
  }
  for (int row = 0; row < matrixColumns; row++) {
    for (int col = 0; col < matrixColumns; col++) {
      matrix1.setLed(0, row, col, false);  // turns off LED at col, row
      matrix2.setLed(0, row, col, false);
      delay(25);
    }
  }
}
