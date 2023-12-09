#include <LiquidCrystal.h>
#include <LedControl.h>
#define matrixDIN 10
#define matrixCS 9
#define matrixCLK 8
#define displayD7 7
#define displayD6 6
#define displayD5 5
#define displayD4 4
#define displayBacklight 3 //PWM controllable
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

byte matrixBrightness = 1;
byte lcdBrightness = 255;

LiquidCrystal lcd(displayRS, displayEN, displayD4, displayD5, displayD6, displayD7);
LedControl matrix = LedControl(matrixDIN, matrixCLK, matrixCS, 2);

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
  matrix.shutdown(0, false);
  matrix.setIntensity(0, matrixBrightness);
  matrix.clearDisplay(0);
  matrix.shutdown(1, false);
  matrix.setIntensity(1, matrixBrightness);
  matrix.clearDisplay(1);
  lcd.print("hi");
}

void loop() {
  int lightValue = analogRead(ambientLightSensor);
  matrixBrightness = map(lightValue, 0, 1023, 1, 15);
  //Serial.println(matrixBrightness);
  Serial.println(lightValue);
  matrix.setIntensity(0, matrixBrightness);
  matrix.setIntensity(1, matrixBrightness);
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
  for (int row = 0; row < matrixColumns; row++) {
    for (int col = 0; col < matrixColumns; col++) {
      matrix.setLed(0, row, col, true);
      matrix.setLed(1, row, col, true);  // turns on LED at col, row
      delay(25);
    }
  }
  for (int row = 0; row < matrixColumns; row++) {
    for (int col = 0; col < matrixColumns; col++) {
      matrix.setLed(0, row, col, false);  // turns off LED at col, row
      matrix.setLed(1, row, col, false);
      delay(25);
    }
  }
  delay(1000);
}
