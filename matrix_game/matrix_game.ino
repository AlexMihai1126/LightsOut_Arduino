#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>
#define matrixDIN 12
#define matrixCS 9
#define matrixCLK 8
#define displayD7 7
#define displayD6 6
#define displayD5 5
#define displayD4 4
#define displayBacklight 10  //PWM controllable
#define displayEN 13
#define displayRS 2
#define buzzerPin 3
#define redPushbtn A0
#define joystickX A1
#define joystickY A2
#define joystickSw A4
#define ambientLightSensor A3
#define matrixSize 8
#define storedParametersCount 6
#define blinkInterval 250
#define maxCharsName 8
#define maxGameLevels 4
#define maxAccesibleMenuStates 5
#define maxAccesibleSubmenuStates 6
#define noOfMatrix 1
#define matrixId 0
#define matrixLowBrightnessValue 1
#define matrixMidBrightnessValue 3
#define matrixHighBrightnessValue 6
#define matrixUltraBrightnessValue 9
#define displayLowBrightnessValue 50
#define displayMidBrightnessValue 125
#define displayHighBrightnessValue 175
#define displayUltraBrightnessValue 255
#define noOfSymbols 36

byte upArrowChar[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};

byte downArrowChar[8] = {
  0b00000,
  0b00100,
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00000
};

byte logicalMatrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte deleteICO[matrixSize][matrixSize] = {
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 1, 0, 1, 0, 0, 1, 0, 1 },
  { 1, 0, 0, 1, 1, 0, 0, 1 },
  { 1, 0, 0, 1, 1, 0, 0, 1 },
  { 1, 0, 1, 0, 0, 1, 0, 1 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0 }
};

byte atSymbolICO[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 1, 1, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 1, 0, 0 },
  { 0, 1, 0, 1, 1, 1, 0, 0 },
  { 0, 1, 0, 1, 1, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
};

byte lightICO[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 1, 0, 1, 0, 1, 0 },
  { 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 1, 1, 1, 0, 1, 1, 1 },
  { 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 1, 0, 1, 0, 1, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0 },
};


byte musicICO[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 1, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 1, 1, 0, 0, 0, 0 },
  { 0, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 0, 0, 0 },
};

byte helpICO[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0 },
  { 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
};

byte smileyICO[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
};


byte settingsICO[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 1, 0, 1 },
  { 0, 0, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 1, 0, 0, 0, 0, 0 },
  { 1, 1, 0, 0, 0, 0, 0, 0 },
};

byte highscoreICO[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 1, 0, 0, 0, 1 },
  { 1, 1, 1, 1, 1, 0, 1, 1 },
  { 0, 1, 0, 1, 0, 0, 0, 1 },
  { 0, 1, 0, 1, 0, 0, 0, 1 },
  { 1, 1, 1, 1, 1, 0, 0, 1 },
  { 0, 1, 0, 1, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
};

enum brightnessLevels {
  lowBrightness,
  midBrightness,
  highBrightness,
  ultraBrightness
};

enum mainMenuStates {
  startGame,
  highscores,
  settings,
  about,
  tutorial,
  intro,
  inSubmenu
};

mainMenuStates currentState = inSubmenu;
mainMenuStates previousState = inSubmenu;  // using this state as a startup state to save memory or handing over control to another menu state

enum internalMenuStates {
  inGame,
  endGame,
  inOtherMenu
};

internalMenuStates currInternalMenuState = inOtherMenu;

enum submenuStates {
  nameInput,
  mtxBrightCtrl,
  lcdBrightCtrl,
  soundCtrl,
  gameTypeSelect,
  resetHighscores,
  inMainMenu
};

submenuStates currentSettingsSubmenu = inMainMenu;
submenuStates previousSubmenuState = inMainMenu;

enum gameType {
  randomBoard,
  freeDraw
};

gameType currentGameType = randomBoard;

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

enum arrowTypes {
  upArr,
  downArr,
  upDownArr,
  backArrow,
  forwardArrow
};

struct highscore {
  char name[maxCharsName] = "";
  int scoreValue = 0;
};

const short adresses[storedParametersCount] = { 0, 1, 2, 3, 4, 5 };

byte matrixBrightness = 2;
byte lcdBrightness = 127;
bool autoBrightness = false;
bool isSoundOn = true;  //defaults

bool introHasAppeared = false;
bool isInGame = false;
bool joystickMoved = false;
bool cmdExecuted = false;
bool isInSubmenu = false;

byte joySwReading = LOW;
byte joySwState = LOW;
byte joySwStateLastReading = LOW;

byte redSwReading = LOW;
byte redSwState = LOW;
byte redSwStateLastReading = LOW;

const short minThreshold = 480;
const short maxThreshold = 550;
int xValue = 0;
int yValue = 0;

const unsigned long debounceTime = 100;
const unsigned long introShowTime = 5000;

unsigned long startIntroTime = 0;
unsigned long lastDebounceTimeJoySw = 0;
unsigned long lastDebounceTimeRedSw = 0;

highscore hs1;
highscore hs2;
highscore hs3;
highscore currPlayer;

const char symbols[noOfSymbols] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

LiquidCrystal lcd(displayRS, displayEN, displayD4, displayD5, displayD6, displayD7);
LedControl matrix = LedControl(matrixDIN, matrixCLK, matrixCS, noOfMatrix);

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.createChar(1, upArrowChar);
  lcd.createChar(2, downArrowChar);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redPushbtn, INPUT_PULLUP);
  pinMode(joystickSw, INPUT_PULLUP);
  pinMode(joystickX, INPUT);
  pinMode(joystickY, INPUT);
  pinMode(ambientLightSensor, INPUT);
  pinMode(displayBacklight, OUTPUT);
  analogWrite(displayBacklight, lcdBrightness);
  lcd.print("STARTING UP...");
  loadParameters();
  loadHighscoresAndLevel();
  matrix.shutdown(matrixId, false);
  matrix.setIntensity(matrixId, matrixBrightness);
  matrix.clearDisplay(matrixId);
  randomSeed(analogRead(A5));
  currentState = intro;  //switch to intro state after the setup runs
  Serial.begin(9600);
  tone(buzzerPin, 500, 250);
  //clearLogicalMatrix();
}

void loop() {
  displayMatrix();
  getJoystickState();
  getJoySwitchState();
  getRedBtnState();
  if (!isInGame) {
    if (isInSubmenu) {
      handleSubmenuNavigation();  // Handle submenu navigation based on debounced joystick state
      handleSettingsSubmenus();   // Handle the current submenu state
    } else {
      handleMenuNavigation();  // Handle main menu navigation based on debounced joystick state
      handleMenu();            // Handle the current main menu state
    }
  }
}

//MAIN MENU FUNCTIONS START HERE
void handleMenu() {
  if (currentState != previousState) {
    lcd.clear();
    lcd.setCursor(0, 0);
  }
  switch (currentState) {
    case intro:
      handleIntro();
      break;
    case startGame:
      handleStartGame();
      break;
    case highscores:
      handleHighscores();
      break;
    case settings:
      handleSettings();
      break;
    case about:
      handleAbout();
      break;
    case tutorial:
      handleTutorial();
      break;
    default:
      break;
  }
  previousState = currentState;
}

void handleIntro() {
  if (introHasAppeared == false) {
    lcd.print("Welcome to");
    lcd.setCursor(0, 1);
    lcd.print("Lights OUT!");
    introHasAppeared = true;
    currentState = startGame;
    delay(1000);  //only for test purposes will be removed in the project
    handleMenu();
  }
}

void handleStartGame() {
  if (currentState != previousState) {
    lcd.print("Start game:");
    lcd.setCursor(0, 1);
    lcd.print("Press button!");
    copyByteMatrix(smileyICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
  }
}

void handleHighscores() {
  if (currentState != previousState) {
    lcd.print("Highscores:");
    lcd.setCursor(0, 1);
    lcd.print("Press joystick");
    copyByteMatrix(highscoreICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
  }
  if (joySwState == HIGH && !isInSubmenu) {
  }
}

void handleSettings() {
  if (currentState != previousState) {
    lcd.print("Settings:");
    lcd.setCursor(0, 1);
    lcd.print("Press joystick");
    copyByteMatrix(settingsICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
  }

  if (joySwState == HIGH && !isInSubmenu) {
    isInSubmenu = true;
    currentState = inSubmenu;
    currentSettingsSubmenu = nameInput;  // Set the initial submenu state
    handleSettingsSubmenus();            // Enter the submenu immediately
  }
}
void handleAbout() {
  if (currentState != previousState) {
    lcd.print("About the game:");
    lcd.setCursor(0, 1);
    lcd.print("Lights OUT");
    copyByteMatrix(atSymbolICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
  }
  //on joystick press get info about creator
}

void handleTutorial() {
  if (currentState != previousState) {
    lcd.print("How to play:");
    lcd.setCursor(0, 1);
    lcd.print("Press joystick");
    copyByteMatrix(helpICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
  }
  //scrollable menu with instructions
}

void handleMenuNavigation() {
  switch (joyState) {
    case UP:
      navigateMenuUp();
      break;
    case DOWN:
      navigateMenuDown();
      break;
    default:
      break;
  }
}

void navigateMenuUp() {
  if (cmdExecuted == false) {
    cmdExecuted = true;
    if (currentState > 0) {
      currentState = static_cast<mainMenuStates>(currentState - 1);
    } else {
      currentState = static_cast<mainMenuStates>(maxAccesibleMenuStates - 1);
    }
  }
}

void navigateMenuDown() {
  if (cmdExecuted == false) {
    cmdExecuted = true;
    currentState = static_cast<mainMenuStates>((currentState + 1) % maxAccesibleMenuStates);
  }
}


//SUBMENU FUNCTIONS START HERE
void handleSettingsSubmenus() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.clear();
    lcd.setCursor(0, 0);
  }
  switch (currentSettingsSubmenu) {
    case nameInput:
      handleNameInput();
      break;
    case mtxBrightCtrl:
      handleMtxBrightCtrl();
      break;
    case lcdBrightCtrl:
      handleLcdBrightCtrl();
      break;
    case soundCtrl:
      handleSoundCtrl();
      break;
    case gameTypeSelect:
      handleGameTypeSelect();
      break;
    case resetHighscores:
      handleResetHighscores();
      break;
    default:
      break;
  }
  previousSubmenuState = currentSettingsSubmenu;
}

void handleNameInput() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("Input name:");
    copyByteMatrix(atSymbolICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
  }
}

void handleMtxBrightCtrl() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("Mtx brightness:");
    copyByteMatrix(lightICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
  }
}

void handleLcdBrightCtrl() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("LCD Brightness:");
    copyByteMatrix(lightICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
  }
}

void handleSoundCtrl() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("Game sounds:");
    copyByteMatrix(musicICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
  }
}

void handleGameTypeSelect() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("Game type:");
    copyByteMatrix(smileyICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
  }
}

void handleResetHighscores() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("Reset highscores?");
    copyByteMatrix(deleteICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
  }
}

void handleSubmenuNavigation() {
  switch (joyState) {
    case UP:
      navigateSubmenuUp();
      break;
    case DOWN:
      navigateSubmenuDown();
      break;
    default:
      break;
  }
}

void navigateSubmenuUp() {
  if (cmdExecuted == false) {
    cmdExecuted = true;
    if (currentSettingsSubmenu > 0) {
      currentSettingsSubmenu = static_cast<submenuStates>(currentSettingsSubmenu - 1);
    } else {
      currentSettingsSubmenu = static_cast<submenuStates>(maxAccesibleSubmenuStates - 1);
    }
  }
}

void navigateSubmenuDown() {
  if (cmdExecuted == false) {
    cmdExecuted = true;
    currentSettingsSubmenu = static_cast<submenuStates>((currentSettingsSubmenu + 1) % maxAccesibleSubmenuStates);
  }
}

//AUXILIARY FUNCTIONS START HERE
void generateAutoscrollingText(const char text[], short lineToDisplay) {
  int textLength = strlen(text);
  int displayLength = 16;
  for (int i = 0; i < textLength - displayLength + 1; i++) {
    lcd.setCursor(0, lineToDisplay);
    lcd.print(text + i);
    delay(150);
  }
}

void addArrowsToDisplay(arrowTypes type) {
  switch (type) {
    case upArr:
      lcd.setCursor(15, 0);
      lcd.write((byte)1);
      lcd.setCursor(15, 1);
      lcd.write(" ");
      break;
    case downArr:
      lcd.setCursor(15, 0);
      lcd.write(" ");
      lcd.setCursor(15, 1);
      lcd.write((byte)2);
      break;
    case upDownArr:
      lcd.setCursor(15, 0);
      lcd.write((byte)1);
      lcd.setCursor(15, 1);
      lcd.write((byte)2);
      break;
    case backArrow:
      lcd.setCursor(15, 0);
      lcd.print(" ");
      lcd.setCursor(15, 1);
      lcd.print("<");
      break;
    case forwardArrow:
      lcd.setCursor(15, 0);
      lcd.print(" ");
      lcd.setCursor(15, 1);
      lcd.print(">");
      break;
    default:
      break;
  }
}

void displayMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      matrix.setLed(matrixId, row, col, logicalMatrix[row][col]);
    }
  }
}

void copyByteMatrix(byte matrixFrom[matrixSize][matrixSize], byte matrixTo[matrixSize][matrixSize]) {
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      matrixTo[i][j] = matrixFrom[i][j];
    }
  }
}

void clearLogicalMatrix() {
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      logicalMatrix[i][j] = 0;
    }
  }
}

// void testSymbols(){
//   copyByteMatrix(settingsICO, logicalMatrix);
//   displayMatrix();
//   delay(500);
//   copyByteMatrix(smileyICO, logicalMatrix);
//   displayMatrix();
//   delay(500);
//   copyByteMatrix(mainMenuICO, logicalMatrix);
//   displayMatrix();
//   delay(500);
//   copyByteMatrix(atSymbolICO, logicalMatrix);
//   displayMatrix();
//   delay(500);
//   copyByteMatrix(lightICO, logicalMatrix);
//   displayMatrix();
//   delay(500);
//   copyByteMatrix(musicICO, logicalMatrix);
//   displayMatrix();
//   delay(500);
//   copyByteMatrix(helpICO, logicalMatrix);
//   displayMatrix();
//   delay(500);
//   copyByteMatrix(highscoreICO, logicalMatrix);
//   displayMatrix();
//   delay(500);
//   clearLogicalMatrix();
// } useless function used for testing the symbols only

void loadParameters() {
  matrixBrightness = EEPROM.read(adresses[mtxBright]);
  lcdBrightness = EEPROM.read(adresses[lcdBright]);
  autoBrightness = EEPROM.read(adresses[autoBright]);
  isSoundOn = EEPROM.read(adresses[soundOn]);
}

void saveParameters() {
  EEPROM.update(adresses[mtxBright], matrixBrightness);
  EEPROM.update(adresses[lcdBright], lcdBrightness);
  EEPROM.update(adresses[autoBright], autoBrightness);
  EEPROM.update(adresses[soundOn], isSoundOn);
}

void loadHighscoresAndLevel() {
  short readAdress = adresses[highscoreStartAddr] + sizeof(highscore);
  EEPROM.get(readAdress, hs1);
  readAdress += sizeof(highscore);
  EEPROM.get(readAdress, hs2);
  readAdress += sizeof(highscore);
  EEPROM.get(readAdress, hs3);
  readAdress += sizeof(currentGameType);
  EEPROM.get(readAdress, currentGameType);
}

void saveHighscoresAndLevel() {
  short writeAdress = adresses[highscoreStartAddr] + sizeof(highscore);
  EEPROM.put(writeAdress, hs1);
  writeAdress += sizeof(highscore);
  EEPROM.put(writeAdress, hs2);
  writeAdress += sizeof(highscore);
  EEPROM.put(writeAdress, hs3);
  writeAdress += sizeof(gameType);
  EEPROM.put(writeAdress, currentGameType);
}

//HARDWARE CONTROL FUNCTIONS START HERE
void getJoystickState() {
  xValue = analogRead(joystickX);
  yValue = analogRead(joystickY);
  if (joystickMoved == false) {
    if (xValue > maxThreshold) {
      joyState = UP;
      joystickMoved = true;
    }

    if (xValue < minThreshold) {
      joyState = DOWN;
      joystickMoved = true;
    }

    if (yValue > maxThreshold) {
      joyState = RIGHT;
      joystickMoved = true;
    }

    if (yValue < minThreshold) {
      joyState = LEFT;
      joystickMoved = true;
    }
  }


  if (xValue >= minThreshold && xValue < maxThreshold && yValue >= minThreshold && yValue < maxThreshold) {
    joyState = STATIC;
    joystickMoved = false;
    cmdExecuted = false;
  }
}

void getJoySwitchState() {
  joySwReading = !digitalRead(joystickSw);
  if (joySwReading != joySwStateLastReading) {
    lastDebounceTimeJoySw = millis();
  }

  if ((millis() - lastDebounceTimeJoySw) >= debounceTime) {
    if (joySwReading != joySwState) {
      if (joySwReading == HIGH) {
        joySwState = HIGH;
      } else {
        joySwState = LOW;
      }
    }
  }
  joySwStateLastReading = joySwReading;
}

void getRedBtnState() {
  redSwReading = !digitalRead(redPushbtn);
  if (redSwReading != redSwStateLastReading) {
    lastDebounceTimeRedSw = millis();
  }

  if ((millis() - lastDebounceTimeJoySw) >= debounceTime) {
    if (redSwReading != redSwState) {
      if (redSwReading == HIGH) {
        redSwState = HIGH;
      } else {
        redSwState = LOW;
      }
    }
  }
  redSwStateLastReading = redSwReading;
}

void displayBrightnessController(brightnessLevels targetBrightness) {
  switch (targetBrightness) {
    case lowBrightness:
      analogWrite(displayBacklight, displayLowBrightnessValue);
      break;
    case midBrightness:
      analogWrite(displayBacklight, displayMidBrightnessValue);
      break;
    case highBrightness:
      analogWrite(displayBacklight, displayHighBrightnessValue);
      break;
    case ultraBrightness:
      analogWrite(displayBacklight, displayUltraBrightnessValue);
      break;
    default:
      break;
  }
}

void matrixBrightnessController(brightnessLevels targetBrightness) {
  switch (targetBrightness) {
    case lowBrightness:
      matrix.setIntensity(matrixId, matrixLowBrightnessValue);
      break;
    case midBrightness:
      matrix.setIntensity(matrixId, matrixMidBrightnessValue);
      break;
    case highBrightness:
      matrix.setIntensity(matrixId, matrixHighBrightnessValue);
      break;
    case ultraBrightness:
      matrix.setIntensity(matrixId, matrixUltraBrightnessValue);
      break;
    default:
      break;
  }
}

void autoBrightnessController() {
  if (autoBrightness == true) {
    //todo
  }
}
