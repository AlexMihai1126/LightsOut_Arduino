//KNOWN BUGS: LEDs DO NOT BLINK WHEN SELECTED!

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
#define blinkInterval 250
#define maxCharsName 4
#define charStrToIndexes 2
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
#define noOfTutorialPages 11
#define aboutPage1 0
#define aboutPage2 1
#define moveSoundHz 1000
#define moveSoundDuration 50
#define resetSoundHz 2500
#define btnPressSoundHz 500
#define msInSec 1000
#define secInMin 60
#define minPercentOfLitPixels 0.2
#define maxPercentOfLitPixels 0.8
#define maxIndexFree 8
#define maxIndexBoard 5
#define tutorialPage1 0
#define tutorialPage2 1
#define tutorialPage3 2
#define tutorialPage4 3
#define tutorialPage5 4
#define tutorialPage6 5
#define tutorialPage7 6
#define tutorialPage8 7
#define tutorialPage9 8
#define tutorialPage10 9
#define tutorialPage11 10
#define gameTypes 3
#define noOfSecForFormat 10
#define debounceTime 100
#define introShowTime 2500
#define minThreshold 480
#define maxThreshold 550
#define defaultName "AAA"

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

byte heartICO[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 1, 0 },
  { 0, 1, 1, 1, 0, 1, 1, 1 },
  { 0, 1, 1, 1, 1, 1, 1, 1 },
  { 0, 1, 1, 1, 1, 1, 1, 1 },
  { 0, 0, 1, 1, 1, 1, 1, 0 },
  { 0, 0, 0, 1, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 0, 0 }
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
  inMainMenu,
  inNameEditor
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
  short scoreValue = 9999;  //we start with a high value as my game the highscore is the smallest number of moves
};

bool isSoundOn = true;  //defaults
//bool autoBrightnessLCD = true;
//bool autoBrightnessMtx = true;

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
bool endComesFromWin = false;
bool isInNameEditor = false;
bool nameEditorInitialized = false;

byte joySwReading = LOW;
byte joySwState = LOW;
byte joySwStateLastReading = LOW;

byte redSwReading = LOW;
byte redSwState = LOW;
byte redSwStateLastReading = LOW;

short xValue = 0;
short yValue = 0;
short upNeighbor = 0;
short downNeighbor = 0;
short leftNeighbor = 0;
short rightNeighbor = 0;


unsigned long startIntroTime = 0;
unsigned long lastDebounceTimeJoySw = 0;
unsigned long lastDebounceTimeRedSw = 0;
//unsigned long prevMillisRefresh = 0;
unsigned long prevMillisForDisplays = 0;
unsigned long startTime = 0;
unsigned long prevMillisBlink = 0;

highscore hsArr[noOfHighscores];
highscore currPlayer;

short hsIndex = 0;
short menuPage = 0;
short mtxIndexX = 0;
short mtxIndexY = 0;
short movesCount = 0;
short noOfOnLights = 0;
short currSymbol = 0;
short charIndex = 0;

const char symbols[noOfSymbols] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

LiquidCrystal lcd(displayRS, displayEN, displayD4, displayD5, displayD6, displayD7);
LedControl matrix = LedControl(matrixDIN, matrixCLK, matrixCS, noOfMatrix);
void setup() {
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
  strcpy(currPlayer.name, defaultName);
}

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
    if (isInSubmenu || currentSettingsSubmenu == inNameEditor) {
      if (isInNameEditor && currentSettingsSubmenu == inNameEditor) {
        nameEditor();
      } else {
        handleSubmenuNavigation();
        handleSettingsSubmenus();
      }

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
    copyByteMatrix(heartICO, logicalMatrix);
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
    menuPage++;
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
    menuPage = 0;
  }
  if (menuPage > noOfTutorialPages) {
    currentState = startGame;
    handleMenu();
  }
  if (joySwState == HIGH && joySwCmdExec == false) {
    joySwCmdExec = true;
    switch (menuPage) {
      case tutorialPage1:
        lcd.clear();
        lcd.print("RANDOM BOARD:");
        lcd.setCursor(0, 1);
        lcd.print("Your goal is");
        addArrowsToDisplay(enterArrow);
        break;
      case tutorialPage2:
        lcd.clear();
        lcd.print("to turn off all");
        lcd.setCursor(0, 1);
        lcd.print("LEDs on matrix");
        addArrowsToDisplay(enterArrow);
        break;
      case tutorialPage3:
        lcd.clear();
        lcd.print("in the smallest");
        lcd.setCursor(0, 1);
        lcd.print("no. of moves");
        addArrowsToDisplay(enterArrow);
        break;
      case tutorialPage4:
        lcd.clear();
        lcd.print("You move with");
        lcd.setCursor(0, 1);
        lcd.print("the joystick");
        addArrowsToDisplay(enterArrow);
        break;
      case tutorialPage5:
        lcd.clear();
        lcd.print("On pressing it");
        lcd.setCursor(0, 1);
        lcd.print("You toggle the");
        addArrowsToDisplay(enterArrow);
        break;
      case tutorialPage6:
        lcd.clear();
        lcd.print("current pixel");
        lcd.setCursor(0, 1);
        lcd.print("and its");
        addArrowsToDisplay(enterArrow);
        break;
      case tutorialPage7:
        lcd.clear();
        lcd.print("neighbors.");
        lcd.setCursor(0, 1);
        lcd.print("FREE DRAW:");
        addArrowsToDisplay(enterArrow);
        break;
      case tutorialPage8:
        lcd.clear();
        lcd.print("The matrix is a");
        lcd.setCursor(0, 1);
        lcd.print("drawing canvas.");
        addArrowsToDisplay(enterArrow);
        break;
      case tutorialPage9:
        lcd.clear();
        lcd.print("Use the stick");
        lcd.setCursor(0, 1);
        lcd.print("to move around.");
        addArrowsToDisplay(enterArrow);
        break;
      case tutorialPage10:
        lcd.clear();
        lcd.print("Clicking it will");
        lcd.setCursor(0, 1);
        lcd.print("Toggle the");
        addArrowsToDisplay(enterArrow);
        break;
      case tutorialPage11:
        lcd.clear();
        lcd.print("current pixel.");
        lcd.setCursor(0, 1);
        lcd.print("Have fun!");
        addArrowsToDisplay(enterArrow);
        break;
      default:
        break;
    }
    menuPage++;
  }
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

void nameEditor() {
  if (nameEditorInitialized == false) {
    nameEditorInitialized = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Name editor:");
    lcd.setCursor(0, 1);
    lcd.print(currPlayer.name);
    lcd.setCursor(0, 1);
    lcd.cursor();
    upMovementEnabled = false;
    downMovementEnabled = false;
    leftMovementEnabled = false;
    currSymbol = 0;
    charIndex = 0;
  }
  lcd.setCursor(charIndex, 1);

  if (joyState == RIGHT && cmdExecuted == false) {
    cmdExecuted = true;
    charIndex++;
    if (charIndex > maxCharsName - charStrToIndexes) {
      charIndex = 0;
    }
    for (int i = 0; i < noOfSymbols; ++i) {
      if (symbols[i] == currPlayer.name[charIndex]) {
        currSymbol = i;
        break;
      }
    }
  }

  if (joyState == UP && cmdExecuted == false) {
    cmdExecuted = true;
    currSymbol++;
    if (currSymbol < 0) {
      currSymbol = noOfSymbols - offsetIndex;
    }
    if (currSymbol > noOfSymbols - offsetIndex) {
      currSymbol = 0;
    }
  }

  if (joyState == DOWN && cmdExecuted == false) {
    cmdExecuted = true;
    currSymbol--;
    if (currSymbol < 0) {
      currSymbol = noOfSymbols - offsetIndex;
    }
    if (currSymbol > noOfSymbols - offsetIndex) {
      currSymbol = 0;
    }
  }

  char newChar = symbols[currSymbol];
  if (currPlayer.name[charIndex] != newChar) {
    currPlayer.name[charIndex] = newChar;
    lcd.print(currPlayer.name[charIndex]);
  }

  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    cmdExecutedRedSw = true;
    lcd.noCursor();
    lcd.clear();
    isInNameEditor = false;
    currentSettingsSubmenu = nameInput;
    upMovementEnabled = true;
    downMovementEnabled = true;
    leftMovementEnabled = true;
    nameEditorInitialized = false;
  }
}

void handleNameInput() {
  if (currentSettingsSubmenu != previousSubmenuState) {
    lcd.print("Input name:");
    copyByteMatrix(atSymbolICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
    lcd.setCursor(0, 1);
    lcd.print(currPlayer.name);
  }
  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    cmdExecutedRedSw = true;
    isInNameEditor = true;
    currentSettingsSubmenu = inNameEditor;
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
    // if (currMtxBrightness == autoB) {
    //   autoBrightnessMtx = true;
    // } else {
    //   autoBrightnessMtx = false;
    // }
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
    // if (currLcdBrightness == autoB) {
    //   autoBrightnessLCD = true;
    // } else {
    //   autoBrightnessLCD = false;
    // }
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

    switch (currentGameType) {
      case randomBoard:
        lcd.print("Random board");
        break;
      case freeDraw:
        lcd.print("Free drawing");
        break;
      case demoGame:
        lcd.print("Demo mode");
        break;
    }
  }

  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    currentGameType = static_cast<gameType>((static_cast<int>(currentGameType) + 1) % gameTypes);

    lcd.clear();  // Clear the display to update with the new game type
    lcd.print("Game type:");
    copyByteMatrix(smileyICO, logicalMatrix);
    addArrowsToDisplay(upDownArr);
    lcd.setCursor(0, 1);

    switch (currentGameType) {
      case randomBoard:
        lcd.print("Random board");
        break;
      case freeDraw:
        lcd.print("Free drawing");
        break;
      case demoGame:
        lcd.print("Demo mode");
        break;
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
      currentSettingsSubmenu = static_cast<submenuStates>(currentSettingsSubmenu - offsetIndex);
    } else {
      currentSettingsSubmenu = static_cast<submenuStates>(maxAccesibleSubmenuStates - offsetIndex);
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

//in game and end game stuff
void handleInGame() {
  unsigned long elapsedTime = millis() - startTime;
  unsigned short elapsedSeconds = elapsedTime / msInSec;
  unsigned short elapsedMinutes = elapsedSeconds / secInMin;
  elapsedSeconds %= secInMin;

  String formattedMinutes = String(elapsedMinutes);
  String formattedSeconds = (elapsedSeconds < noOfSecForFormat) ? "0" + String(elapsedSeconds) : String(elapsedSeconds);

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
    buzzerController(btnPressSoundHz, moveSoundDuration);
    lcd.clear();
  }
}

void startGameFn() {
  isInGame = true;
  endComesFromWin = false;
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
    if (endComesFromWin) {
      lcd.clear();
      lcd.print("You won!");
      lcd.setCursor(0, 1);
      lcd.print("In " + String(currPlayer.scoreValue) + " moves.");
      if (isHighScore()) {
        lcd.clear();
        lcd.print("You beat the hs");
        lcd.setCursor(0, 1);
        lcd.print("With " + String(currPlayer.scoreValue) + " moves.");
      }
    } else {
      lcd.clear();
      lcd.print("Game ended!");
    }
  }

  if (redSwState == HIGH && cmdExecutedRedSw == false) {
    buzzerController(btnPressSoundHz, moveSoundDuration);
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
          buzzerController(moveSoundHz, moveSoundDuration);
          break;
        case RIGHT:
          cmdExecuted = true;
          mtxIndexX++;
          checkIndexesOutOfRange(maxIndexFn);
          buzzerController(moveSoundHz, moveSoundDuration);
          break;
        case UP:
          cmdExecuted = true;
          mtxIndexY--;
          checkIndexesOutOfRange(maxIndexFn);
          buzzerController(moveSoundHz, moveSoundDuration);
          break;
        case DOWN:
          cmdExecuted = true;
          mtxIndexY++;
          checkIndexesOutOfRange(maxIndexFn);
          buzzerController(moveSoundHz, moveSoundDuration);
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
    //blinkCurrentPixel();
    if (joySwState == HIGH && joySwCmdExec == false) {
      joySwCmdExec = true;
      movesCount++;
      logicalMatrix[mtxIndexY][mtxIndexX] = !logicalMatrix[mtxIndexY][mtxIndexX];
      buzzerController(btnPressSoundHz, moveSoundDuration);
    }
  }

  if (currentGameType == randomBoard) {
    checkForWin();
    gameJoystickMove(maxIndexBoard);
    if (joySwState == HIGH && joySwCmdExec == false) {
      joySwCmdExec = true;
      movesCount++;
      toggleNeighbors(mtxIndexY, mtxIndexX);
      buzzerController(btnPressSoundHz, moveSoundDuration);
    }
  }

  if (currentGameType == demoGame) {
    checkForWin();
    gameJoystickMove(maxIndexBoard);
    if (joySwState == HIGH && joySwCmdExec == false) {
      joySwCmdExec = true;
      movesCount++;
      toggleNeighbors(mtxIndexY, mtxIndexX);
      buzzerController(btnPressSoundHz, moveSoundDuration);
    }
  }
}

void generateBoard() {
  int noOfLights = random(maxIndexBoard * maxIndexBoard * minPercentOfLitPixels, maxIndexBoard * maxIndexBoard * maxPercentOfLitPixels);
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
}

void generateDemoBoard() {
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
  buzzerController(btnPressSoundHz, moveSoundDuration);
  //Serial.print("Current on lights: ");
  //Serial.println(noOfOnLights);
}

void checkForWin() {
  if (noOfOnLights == 0) {
    currInternalMenuState = endGame;
    isInGame = false;
    lcd.clear();
    endComesFromWin = true;
    currPlayer.scoreValue = movesCount;
    updateHighScores();
  }
}

bool isHighScore() {
  for (int i = 0; i < noOfHighscores; ++i) {
    if (currPlayer.scoreValue < hsArr[i].scoreValue) {
      return true;
    }
  }
  return false;
}

void updateHighScores() {
  if (isHighScore()) {
    for (int i = noOfHighscores - 1; i > 0; --i) {
      strcpy(hsArr[i].name, hsArr[i - 1].name);
      hsArr[i].scoreValue = hsArr[i - 1].scoreValue;
    }
    strcpy(hsArr[0].name, currPlayer.name);
    hsArr[0].scoreValue = currPlayer.scoreValue;
    saveHighscores();
  }
}

//AUXILIARY FUNCTIONS START HERE

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

// void displayMatrix() {
//   for (int row = 0; row < matrixSize; row++) {
//     for (int col = 0; col < matrixSize; col++) {
//       matrix.setLed(matrixId, row, col, logicalMatrix[row][col]);
//     }
//   }
// }

void displayMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      // if (isInGame) {
      //   // Check if the current pixel matches the blink conditions
      //   if (row == mtxIndexX && col == mtxIndexY) {
      //     bool ledState = logicalMatrix[mtxIndexX][mtxIndexY];
      //       if (millis() - prevMillisBlink >= blinkInterval) {
      //         prevMillisBlink = millis();
      //         ledState = !ledState;
      //         matrix.setLed(matrixId, mtxIndexY, mtxIndexX, ledState);
      //       }
      //   } else {
      //     // Display the matrix as usual
      //     matrix.setLed(matrixId, row, col, logicalMatrix[row][col]);
      //   }
      // } else {
      //   // Display the matrix as usual when not in a game
      //   matrix.setLed(matrixId, row, col, logicalMatrix[row][col]);
      // }
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
    hsArr[i].scoreValue = 9999;
  }
  buzzerController(resetSoundHz, moveSoundDuration);
  saveHighscores();
}
