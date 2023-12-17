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
#define blinkInterval 125
#define maxCharsName 8
#define maxGameLevels 4
#define maxAccesibleMenuStates 5
#define maxAccesibleSubmenuStates 6
#define noOfMatrix 1
#define matrixId 0
#define matrixLowBrightnessValue 1
#define matrixMidBrightnessValue 3
#define matrixHighBrightnessValue 5
#define matrixUltraBrightnessValue 8
#define displayLowBrightnessValue 75
#define displayMidBrightnessValue 150
#define displayHighBrightnessValue 200
#define displayUltraBrightnessValue 255
#define noOfSymbols 36
#define updateRate 3000
#define minThresholdValueLight 5
#define midThresholdValueLight 25
#define highThresholdValueLight 40
#define soundDuration 250
#define startupFreqSound 500
#define noOfBrightnessLevels 4  //autoB state IS DISABLED PERMANENTLY due to a bug affecting the display controller function (otherwise there are 5 total states)
#define soundOnAddr 0
#define highscoreStartAddr 100
#define highscoreShowTime 500
#define offsetIndex 1
#define noOfHighscores 3
#define noOfAboutPages 2
#define noOfTutorialPages 10
#define aboutPage1 0
#define aboutPage2 1
#define moveSoundHz 1000
#define moveSoundDuration 50
#define resetSoundHz 2500
#define msInSec 1000
#define secInMin 60
#define maxIndexFree 8
#define maxIndexBoard 5

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

byte enterSymbol[8] = {
  0b00001,
  0b00001,
  0b00001,
  0b00101,
  0b01001,
  0b11111,
  0b01000,
  0b00100
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

byte checkICO[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 0 },
  { 1, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 1, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 0 },
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
  ultraBrightness,
  autoB  //disabled currently
};

brightnessLevels currLcdBrightness = midBrightness;
brightnessLevels currMtxBrightness = midBrightness;

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
  freeDraw,
  demoGame
};

gameType currentGameType = randomBoard;

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
  forwardArrow,
  enterArrow
};

struct highscore {
  char name[maxCharsName] = "";
  int scoreValue = 0;
};

bool isSoundOn = true;  //defaults
bool autoBrightnessLCD = true;
bool autoBrightnessMtx = true;

bool introHasAppeared = false;
bool isInGame = false;
bool joystickMoved = false;
bool cmdExecuted = false;
bool isInSubmenu = false;
bool leftMovementEnabled = true;
bool upMovementEnabled = true;
bool downMovementEnabled = true;
bool cmdExecutedRedSw = false;
bool joySwCmdExec = false;
bool endGameLCDPrinted = false;
bool isMtxEnabled = true;

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
short upNeighbor = 0;
short downNeighbor = 0;
short leftNeighbor = 0;
short rightNeighbor = 0;

const unsigned long debounceTime = 100;
const unsigned long introShowTime = 2500;

unsigned long startIntroTime = 0;
unsigned long lastDebounceTimeJoySw = 0;
unsigned long lastDebounceTimeRedSw = 0;
unsigned long prevMillisRefresh = 0;
unsigned long prevMillisForDisplays = 0;
unsigned long startTime = 0;
unsigned long prevMillisBlink = 0;

highscore hsArr[noOfHighscores];
highscore currPlayer;
short hsIndex = 0;
int menuPage = 0;
short mtxIndexX = 0;
short mtxIndexY = 0;
int movesCount = 0;
short noOfOnLights = 0;

const char symbols[noOfSymbols] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

LiquidCrystal lcd(displayRS, displayEN, displayD4, displayD5, displayD6, displayD7);
LedControl matrix = LedControl(matrixDIN, matrixCLK, matrixCS, noOfMatrix);
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.createChar(1, upArrowChar);
  lcd.createChar(2, downArrowChar);
  lcd.createChar(3, enterSymbol);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redPushbtn, INPUT_PULLUP);
  pinMode(joystickSw, INPUT_PULLUP);
  pinMode(joystickX, INPUT);
  pinMode(joystickY, INPUT);
  pinMode(ambientLightSensor, INPUT);
  pinMode(displayBacklight, OUTPUT);
  loadParameters();
  loadHighscores();
  displayBrightnessController(currLcdBrightness);
  matrix.shutdown(matrixId, false);
  matrixBrightnessController(currMtxBrightness);
  matrix.clearDisplay(matrixId);
  randomSeed(analogRead(A5));  //initialize random number generator
  currentState = intro;        //switch to intro state after the setup runs
  buzzerController(startupFreqSound, soundDuration);
}
//todo impement demo board switcher, highscore update and how to play
void loop() {
  displayMatrix();
  getJoystickState();
  getJoySwitchState();
  getRedBtnState();
  if ((currInternalMenuState == inGame || currInternalMenuState == endGame) && currentState == inSubmenu) {
    if (isInGame == true && currInternalMenuState == inGame) {
      handleInGame();  //pressing red button again here ends the game
    } else if (isInGame == false && currInternalMenuState == endGame) {
      handleEndGame();  //pressing red button again resets to main  menu
    }
  } else {
    if (isInSubmenu) {
      handleSubmenuNavigation();
      handleSettingsSubmenus();
    } else {
      handleMenuNavigation();
      handleMenu();
    }
  }
  //autoBrightnessController(); - disabled
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
    copyByteMatrix(checkICO, logicalMatrix);
    lcd.print("Welcome to");
    lcd.setCursor(0, 1);
    lcd.print("Lights OUT!");
    introHasAppeared = true;
  }
  if ((millis() - prevMillisForDisplays) >= introShowTime) {
    currentState = startGame;
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

  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    cmdExecutedRedSw = true;
    startGameFn();
  }
}

void handleHighscores() {
  if (currentState != previousState) {
    lcd.print("Highscores:");
    lcd.setCursor(0, 1);
    lcd.print("Press joystick");
    addArrowsToDisplay(enterArrow);
    copyByteMatrix(highscoreICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
    hsIndex = 0;
  }
  if (hsIndex > noOfHighscores) {
    currentState = startGame;
    handleMenu();
  } else {
    if (joySwState == HIGH && joySwCmdExec == false) {
      joySwCmdExec = true;
      lcd.clear();
      lcd.print("Highscore #:");
      lcd.print(hsIndex + offsetIndex);
      addArrowsToDisplay(enterArrow);
      lcd.setCursor(0, 1);
      lcd.print("N:");
      lcd.print(hsArr[hsIndex].name);
      lcd.print(" ");
      lcd.print("S:");
      lcd.print(hsArr[hsIndex].scoreValue);
      hsIndex += 1;
    }
  }
}

void handleSettings() {
  if (currentState != previousState && isInSubmenu == false) {
    lcd.print("Settings:");
    lcd.setCursor(0, 1);
    lcd.print("Press joystick");
    copyByteMatrix(settingsICO, logicalMatrix);
    addArrowsToDisplay(enterArrow);
    addArrowsToDisplay(upDownArr);
  }

  if (joySwState == HIGH && !isInSubmenu) {
    isInSubmenu = true;
    currentState = inSubmenu;
    currentSettingsSubmenu = nameInput;
    handleSettingsSubmenus();
  }
}
void handleAbout() {
  if (currentState != previousState) {
    lcd.print("About the game:");
    lcd.setCursor(0, 1);
    lcd.print("Lights OUT");
    copyByteMatrix(atSymbolICO, logicalMatrix);
    addArrowsToDisplay(enterArrow);
    addArrowsToDisplay(upDownArr);
    menuPage = 0;
  }
  if (menuPage > noOfAboutPages) {
    currentState = tutorial;
    handleMenu();
  }
  if (joySwState == HIGH && joySwCmdExec == false) {
    joySwCmdExec = true;
    switch (menuPage) {
      case aboutPage1:
        lcd.clear();
        lcd.print("Made by:");
        lcd.setCursor(0, 1);
        lcd.print("Alex Mihai");
        addArrowsToDisplay(enterArrow);
        break;
      case aboutPage2:
        lcd.clear();
        lcd.print("GitHub repo:");
        lcd.setCursor(0, 1);
        lcd.print("@AlexMihai1126");
        addArrowsToDisplay(enterArrow);
        break;
      default:
        break;
    }
    menuPage += offsetIndex;
  }
}

void handleTutorial() {
  if (currentState != previousState) {
    lcd.print("How to play:");
    lcd.setCursor(0, 1);
    lcd.print("Press joystick");
    addArrowsToDisplay(enterArrow);
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
    buzzerController(moveSoundHz, moveSoundDuration);
    if (currentState > 0) {
      currentState = static_cast<mainMenuStates>(currentState - 1);
    } else {
      currentState = static_cast<mainMenuStates>(maxAccesibleMenuStates - 1);
    }
  }
}

void navigateMenuDown() {
  if (cmdExecuted == false) {
    buzzerController(moveSoundHz, moveSoundDuration);
    cmdExecuted = true;
    currentState = static_cast<mainMenuStates>((currentState + 1) % maxAccesibleMenuStates);
  }
}

//in game and end game stuff
void handleInGame() {
  unsigned long elapsedTime = millis() - startTime;
  unsigned long elapsedSeconds = elapsedTime / msInSec;
  unsigned long elapsedMinutes = elapsedSeconds / secInMin;
  elapsedSeconds %= secInMin;

  String formattedMinutes = String(elapsedMinutes);
  String formattedSeconds = (elapsedSeconds < 10) ? "0" + String(elapsedSeconds) : String(elapsedSeconds);

  lcd.print("Time: " + formattedMinutes + "m " + formattedSeconds + "s");
  lcd.setCursor(0, 1);
  lcd.print("Moves: " + String(movesCount));
  lcd.setCursor(13, 0);
  lcd.print("X:" + String(mtxIndexX + offsetIndex));
  lcd.setCursor(13, 1);
  lcd.print("Y:" + String(mtxIndexY + offsetIndex));
  lcd.setCursor(0, 0);
  gameLogic();

  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    cmdExecutedRedSw = true;
    currInternalMenuState = endGame;
    isInGame = false;
    isMtxEnabled = true;
    lcd.clear();
  }
}

void startGameFn() {
  isInGame = true;
  startTime = millis();
  currentState = inSubmenu;
  currInternalMenuState = inGame;
  lcd.clear();
  clearLogicalMatrix();
  mtxIndexX = 0;
  mtxIndexY = 0;
  movesCount = 0;
  noOfOnLights = 0;
  if (currentGameType == randomBoard) {
    generateBoard();
  }
  if (currentGameType == demoGame) {
    generateDemoBoard();
  }
}

void handleEndGame() {
  if (endGameLCDPrinted == false) {
    endGameLCDPrinted = true;
    lcd.clear();
    lcd.print("You won!");
    lcd.setCursor(0, 1);
    lcd.print("In " + String(movesCount) + " moves.");
  }

  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    cmdExecutedRedSw = true;
    currentState = startGame;
    currInternalMenuState = inOtherMenu;
    endGameLCDPrinted = false;
  }
}

void checkIndexesOutOfRange(short maxIndex) {
  if (mtxIndexX > maxIndex - offsetIndex) {
    mtxIndexX = 0;
  } else if (mtxIndexX < 0) {
    mtxIndexX = maxIndex - offsetIndex;
  }
  if (mtxIndexY > maxIndex - offsetIndex) {
    mtxIndexY = 0;
  } else if (mtxIndexY < 0) {
    mtxIndexY = maxIndex - offsetIndex;
  }
}

// void printIndexes() {
//   Serial.print("X: ");
//   Serial.println(mtxIndexX);
//   Serial.print("Y: ");
//   Serial.println(mtxIndexY);
// }

void gameJoystickMove(short maxIndexFn) {
  if (joyState == STATIC) {
    cmdExecuted = false;
  } else {
    if (cmdExecuted == false) {
      switch (joyState) {
        case LEFT:
          cmdExecuted = true;
          mtxIndexX--;
          checkIndexesOutOfRange(maxIndexFn);
          //printIndexes();
          break;
        case RIGHT:
          cmdExecuted = true;
          mtxIndexX++;
          checkIndexesOutOfRange(maxIndexFn);
          //printIndexes();
          break;
        case UP:
          cmdExecuted = true;
          mtxIndexY--;
          checkIndexesOutOfRange(maxIndexFn);
          //printIndexes();
          break;
        case DOWN:
          cmdExecuted = true;
          mtxIndexY++;
          checkIndexesOutOfRange(maxIndexFn);
          //printIndexes();
          break;
        default:
          break;
      }
    }
  }
}

void gameLogic() {
  if (currentGameType == freeDraw) {
    gameJoystickMove(maxIndexFree);
    blinkCurrentPixel();
    if (joySwState == HIGH && joySwCmdExec == false) {
      joySwCmdExec = true;
      movesCount++;
      logicalMatrix[mtxIndexY][mtxIndexX] = !logicalMatrix[mtxIndexY][mtxIndexX];
    }
  }

  if (currentGameType == randomBoard) {
    checkForWin();
    gameJoystickMove(maxIndexBoard);
    if (joySwState == HIGH && joySwCmdExec == false) {
      joySwCmdExec = true;
      movesCount++;
      toggleNeighbors(mtxIndexY, mtxIndexX);
    }
  }

  if (currentGameType == demoGame) {
    checkForWin();
    gameJoystickMove(maxIndexBoard);
    if (joySwState == HIGH && joySwCmdExec == false) {
      joySwCmdExec = true;
      movesCount++;
      toggleNeighbors(mtxIndexY, mtxIndexX);
    }
  }
}

void generateBoard() {
  int noOfLights = random(maxIndexBoard * maxIndexBoard * 0.2, maxIndexBoard * maxIndexBoard * 0.8);
  for (int light = 0; light < noOfLights; light++) {
    int i = random(maxIndexBoard);
    int j = random(maxIndexBoard);
    logicalMatrix[j][i] = HIGH;
  }
  for (int i = 0; i < maxIndexBoard; i++) {
    for (int j = 0; j < maxIndexBoard; j++) {
      if (logicalMatrix[j][i] == HIGH) {
        noOfOnLights++;
      }
    }
  }
  //Serial.print("On lights at start: ");
  //Serial.println(noOfOnLights);
}

void generateDemoBoard(){
  logicalMatrix[1][1] = HIGH;
  logicalMatrix[2][0] = HIGH;
  logicalMatrix[2][1] = HIGH;
  logicalMatrix[2][2] = HIGH;
  logicalMatrix[3][1] = HIGH;
  noOfOnLights = 5;
}

void toggleNeighbors(short i, short j) {
  upNeighbor = i - 1;
  downNeighbor = i + 1;
  leftNeighbor = j - 1;
  rightNeighbor = j + 1;
  logicalMatrix[i][j] = !logicalMatrix[i][j];
  if (logicalMatrix[i][j] == HIGH) {
    noOfOnLights++;
  } else {
    noOfOnLights--;
  }
  if (upNeighbor >= 0 && upNeighbor < maxIndexBoard) {
    logicalMatrix[upNeighbor][j] = !logicalMatrix[upNeighbor][j];
    if (logicalMatrix[upNeighbor][j] == HIGH) {
      noOfOnLights++;
    } else {
      noOfOnLights--;
    }
  }

  if (downNeighbor >= 0 && downNeighbor < maxIndexBoard) {
    logicalMatrix[downNeighbor][j] = !logicalMatrix[downNeighbor][j];
    if (logicalMatrix[downNeighbor][j] == HIGH) {
      noOfOnLights++;
    } else {
      noOfOnLights--;
    }
  }

  if (leftNeighbor >= 0 && leftNeighbor < maxIndexBoard) {
    logicalMatrix[i][leftNeighbor] = !logicalMatrix[i][leftNeighbor];
    if (logicalMatrix[i][leftNeighbor] == HIGH) {
      noOfOnLights++;
    } else {
      noOfOnLights--;
    }
  }

  if (rightNeighbor >= 0 && rightNeighbor < maxIndexBoard) {
    logicalMatrix[i][rightNeighbor] = !logicalMatrix[i][rightNeighbor];
    if (logicalMatrix[i][rightNeighbor] == HIGH) {
      noOfOnLights++;
    } else {
      noOfOnLights--;
    }
  }
  //Serial.print("Current on lights: ");
  //Serial.println(noOfOnLights);
}

void checkForWin(){
  if(noOfOnLights == 0){
    currInternalMenuState = endGame;
    isInGame = false;
    isMtxEnabled = true;
    lcd.clear();
  }
}

void blinkCurrentPixel() {
  byte ledState = logicalMatrix[mtxIndexX][mtxIndexY];
  if (!ledState && millis() - prevMillisBlink >= blinkInterval) {
    prevMillisBlink = millis();
    ledState = !ledState;
    matrix.setLed(matrixId, mtxIndexY, mtxIndexX, ledState);
    //isMtxEnabled = !isMtxEnabled;
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
      handleNameInput();  //TODO
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
    lcd.setCursor(0, 1);
  }
}

void handleMtxBrightCtrl() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("Mtx brightness:");
    copyByteMatrix(lightICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
    lcd.setCursor(0, 1);
    switch (currMtxBrightness) {
      case lowBrightness:
        lcd.print("Low  ");
        lcd.setCursor(0, 1);
        break;
      case midBrightness:
        lcd.print("Mid  ");
        lcd.setCursor(0, 1);
        break;
      case highBrightness:
        lcd.print("High  ");
        lcd.setCursor(0, 1);
        break;
      case ultraBrightness:
        lcd.print("Ultra");
        lcd.setCursor(0, 1);
        break;
      case autoB:
        lcd.print("Auto ");
        lcd.setCursor(0, 1);
        break;
      default:
        break;
    }
  }

  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    currMtxBrightness = static_cast<brightnessLevels>((currMtxBrightness + 1) % noOfBrightnessLevels);
    matrixBrightnessController(currMtxBrightness);
    if (currMtxBrightness == autoB) {
      autoBrightnessMtx = true;
    } else {
      autoBrightnessMtx = false;
    }
    cmdExecutedRedSw = true;
    switch (currMtxBrightness) {
      case lowBrightness:
        lcd.print("Low  ");
        lcd.setCursor(0, 1);
        break;
      case midBrightness:
        lcd.print("Mid  ");
        lcd.setCursor(0, 1);
        break;
      case highBrightness:
        lcd.print("High  ");
        lcd.setCursor(0, 1);
        break;
      case ultraBrightness:
        lcd.print("Ultra");
        lcd.setCursor(0, 1);
        break;
      case autoB:
        lcd.print("Auto ");
        lcd.setCursor(0, 1);
        break;
      default:
        break;
    }
  }
}
void handleLcdBrightCtrl() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("LCD Brightness:");
    copyByteMatrix(lightICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
    lcd.setCursor(0, 1);
    switch (currLcdBrightness) {
      case lowBrightness:
        lcd.print("Low  ");
        lcd.setCursor(0, 1);
        break;
      case midBrightness:
        lcd.print("Mid  ");
        lcd.setCursor(0, 1);
        break;
      case highBrightness:
        lcd.print("High  ");
        lcd.setCursor(0, 1);
        break;
      case ultraBrightness:
        lcd.print("Ultra");
        lcd.setCursor(0, 1);
        break;
      case autoB:
        lcd.print("Auto ");
        lcd.setCursor(0, 1);
        break;
      default:
        break;
    }
  }
  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    currLcdBrightness = static_cast<brightnessLevels>((currLcdBrightness + 1) % noOfBrightnessLevels);
    if (currLcdBrightness == autoB) {
      autoBrightnessLCD = true;
    } else {
      autoBrightnessLCD = false;
    }
    cmdExecutedRedSw = true;
    displayBrightnessController(currLcdBrightness);
    switch (currLcdBrightness) {
      case lowBrightness:
        lcd.print("Low  ");
        lcd.setCursor(0, 1);
        break;
      case midBrightness:
        lcd.print("Mid  ");
        lcd.setCursor(0, 1);
        break;
      case highBrightness:
        lcd.print("High  ");
        lcd.setCursor(0, 1);
        break;
      case ultraBrightness:
        lcd.print("Ultra");
        lcd.setCursor(0, 1);
        break;
      case autoB:
        lcd.print("Auto ");
        lcd.setCursor(0, 1);
        break;
      default:
        break;
    }
  }
}

void handleSoundCtrl() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("Game sounds:");
    copyByteMatrix(musicICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
    lcd.setCursor(0, 1);
    if (isSoundOn == true) {
      lcd.print("On ");
      lcd.setCursor(0, 1);
    } else {
      lcd.print("Off");
      lcd.setCursor(0, 1);
    }
  }
  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    isSoundOn = !isSoundOn;
    cmdExecutedRedSw = true;
    if (isSoundOn == true) {
      lcd.print("On ");
      lcd.setCursor(0, 1);
    } else if (isSoundOn == false) {
      lcd.print("Off");
      lcd.setCursor(0, 1);
    }
  }
}

void handleGameTypeSelect() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("Game type:");
    copyByteMatrix(smileyICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
    lcd.setCursor(0, 1);
    if (currentGameType == randomBoard) {
      lcd.print("Random board");
      lcd.setCursor(0, 1);
    } else if (currentGameType == freeDraw) {
      lcd.print("Free drawing");
      lcd.setCursor(0, 1);
    } else if (currentGameType == demoGame) {
      lcd.print("Demo mode");
      lcd.setCursor(0, 1);
    }
  }

  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    currentGameType = (currentGameType == freeDraw) ? randomBoard : freeDraw;
    if (currentGameType == randomBoard) {
      lcd.print("Random board");
      lcd.setCursor(0, 1);
    } else if (currentGameType == freeDraw) {
      lcd.print("Free drawing");
      lcd.setCursor(0, 1);
    }
    cmdExecutedRedSw = true;
  }
}

void handleResetHighscores() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("Rst highscores?");
    copyByteMatrix(deleteICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
    lcd.setCursor(0, 1);
    lcd.print("Back:N, Btn:Y");
  }
  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    executeResetHighscores();
    cmdExecutedRedSw = true;
    isInSubmenu = false;
    currentState = settings;
    currentSettingsSubmenu = inMainMenu;
  }
}

void handleSubmenuNavigation() {
  switch (joyState) {
    case UP:
      if (upMovementEnabled) {
        navigateSubmenuUp();
      }
      break;
    case DOWN:
      if (downMovementEnabled) {
        navigateSubmenuDown();
      }
      break;
    case LEFT:
      if (leftMovementEnabled) {
        navigateBackToMainMenu();
      }
      break;
    default:
      break;
  }
}

void navigateSubmenuUp() {
  if (cmdExecuted == false) {
    cmdExecuted = true;
    buzzerController(moveSoundHz, moveSoundDuration);
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
    buzzerController(moveSoundHz, moveSoundDuration);
    currentSettingsSubmenu = static_cast<submenuStates>((currentSettingsSubmenu + 1) % maxAccesibleSubmenuStates);
  }
}

void navigateBackToMainMenu() {
  if (cmdExecuted == false) {
    saveParameters();
    buzzerController(moveSoundHz, moveSoundDuration);
    cmdExecuted = true;
    isInSubmenu = false;
    currentState = settings;
    currentSettingsSubmenu = inMainMenu;
  }
}

//AUXILIARY FUNCTIONS START HERE

// void generateAutoscrollingText(const char text[], short lineToDisplay) {
//   int textLength = strlen(text);
//   int displayLength = 16;
//   for (int i = 0; i < textLength - displayLength + 1; i++) {
//     lcd.setCursor(0, lineToDisplay);
//     lcd.print(text + i);
//     delay(150);
//   }
// } function no longer used but will remain in the code as a proof of concept

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
    case enterArrow:
      lcd.setCursor(14, 1);
      lcd.write((byte)3);
      break;
    default:
      break;
  }
}

void displayMatrix() {
  if (isMtxEnabled == true) {
    for (int row = 0; row < matrixSize; row++) {
      for (int col = 0; col < matrixSize; col++) {
        matrix.setLed(matrixId, row, col, logicalMatrix[row][col]);
      }
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
  int readAddr = soundOnAddr;
  isSoundOn = EEPROM.read(readAddr);
  readAddr += sizeof(brightnessLevels);
  EEPROM.get(readAddr, currLcdBrightness);
  readAddr += sizeof(brightnessLevels);
  EEPROM.get(readAddr, currMtxBrightness);
  readAddr += sizeof(currentGameType);
  EEPROM.get(readAddr, currentGameType);
}

void saveParameters() {
  int writeAddr = soundOnAddr;
  EEPROM.update(writeAddr, isSoundOn);
  writeAddr += sizeof(brightnessLevels);
  EEPROM.put(writeAddr, currLcdBrightness);
  writeAddr += sizeof(brightnessLevels);
  EEPROM.put(writeAddr, currMtxBrightness);
  writeAddr += sizeof(currentGameType);
  EEPROM.put(writeAddr, currentGameType);
  writeAddr += sizeof(gameType);
  EEPROM.put(writeAddr, currentGameType);
}

void loadHighscores() {
  int readAddress = highscoreStartAddr + sizeof(highscore);
  for (int i = 0; i < noOfHighscores; i++) {
    EEPROM.get(readAddress, hsArr[i]);
    readAddress += sizeof(highscore);
  }
}

void saveHighscores() {
  int writeAddress = highscoreStartAddr + sizeof(highscore);
  for (int i = 0; i < noOfHighscores; i++) {
    EEPROM.put(writeAddress, hsArr[i]);
    writeAddress += sizeof(highscore);
  }
}

// void displayLoadedParameters() {
//   Serial.println("Loaded Parameters:");

//   Serial.print("Sound On: ");
//   Serial.println(isSoundOn);

//   Serial.print("Current LCD Brightness: ");
//   Serial.println(static_cast<int>(currLcdBrightness));

//   Serial.print("Current Matrix Brightness: ");
//   Serial.println(static_cast<int>(currMtxBrightness));

//   Serial.print("Current Game Type: ");
//   Serial.println(static_cast<int>(currentGameType));
// }

// void displayLoadedHighscores() {
//   Serial.println("Loaded Highscores:");
//   for (int i = 0; i < noOfHighscores; i++) {
//     Serial.print("Highscore: ");
//     Serial.print("Name: ");
//     Serial.print(hsArr[i].name);
//     Serial.print(", Score: ");
//     Serial.println(hsArr[i].scoreValue);
//   }
// }  //functions used for testing EEPROM R/W


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
        joySwCmdExec = false;
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
        cmdExecutedRedSw = false;
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
    // case autoB:
    //   autoBrightnessLCD=true;
    //   break;
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
    // case autoB:
    //   autoBrightnessMtx = true;
    //   break;
    default:
      break;
  }
}

// void autoBrightnessController() {
//   if (autoBrightnessLCD == true || autoBrightnessMtx == true) {
//     //enabled only if at least one of the controls is set to automatic to save resources
//     short sampledLightVal = 0;
//     if ((millis() - prevMillisRefresh) >= updateRate) {
//       prevMillisRefresh = millis();
//       sampledLightVal = map(analogRead(ambientLightSensor), 0, 1023, 0, 100);
//     }
//     if (autoBrightnessLCD == true) {
//       if (sampledLightVal < minThresholdValueLight) {
//         displayBrightnessController(lowBrightness);
//       }
//       if (sampledLightVal > minThresholdValueLight && sampledLightVal < midThresholdValueLight) {
//         displayBrightnessController(midBrightness);
//       }
//       if (sampledLightVal > midThresholdValueLight && sampledLightVal < highThresholdValueLight) {
//         displayBrightnessController(highBrightness);
//       }
//       if (sampledLightVal > highThresholdValueLight) {
//         displayBrightnessController(ultraBrightness);
//       }
//     }
//     if (autoBrightnessMtx == true) {
//       if (sampledLightVal < minThresholdValueLight) {
//         matrixBrightnessController(lowBrightness);
//       }
//       if (sampledLightVal > minThresholdValueLight && sampledLightVal < midThresholdValueLight) {
//         matrixBrightnessController(midBrightness);
//       }
//       if (sampledLightVal > midThresholdValueLight && sampledLightVal < highThresholdValueLight) {
//         matrixBrightnessController(highBrightness);
//       }
//       if (sampledLightVal > highThresholdValueLight) {
//         matrixBrightnessController(ultraBrightness);
//       }
//     }
//   }
// }

void buzzerController(unsigned int frequency, unsigned long duration) {
  if (isSoundOn == true) {
    tone(buzzerPin, frequency, duration);
  }
}

void executeResetHighscores() {
  for (int i = 0; i < noOfHighscores; i++) {
    strcpy(hsArr[i].name, "");
    hsArr[i].scoreValue = 0;
  }
  buzzerController(resetSoundHz, moveSoundDuration);
  saveHighscores();
}
