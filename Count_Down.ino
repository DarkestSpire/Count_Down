// Brady Heater
// 8/19/2020
// Count Down

// General Includes
#include "bitmaps_Sprites.h" // Import Assets
#include "bitmaps_Backrounds.h" // Import Assets
#include <Arduboy2.h>
#include <Sprites.h>

// Define current state
#define START 1
#define INSTRUCTIONS 2
#define GAME 3
#define LOSE 4
#define WIN 5

// Game State menus
#define ROLL 1
#define MATH 2
#define ROLLING 3
#define DOUBLE_SIX 4


// Define general variables
unsigned char state = START; // Begin in the start up state
int lastButtonPressed = 0;
int gameStrikes;
int gameValues[10];

// Decare Objects
Arduboy2Base arduboy;

// Set Program
void setup() 
{
  // TEST
  Serial.begin(9600);
  
  // Start Arduboy
  arduboy.begin();
  arduboy.initRandomSeed();

  // LEDs
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  // Wait for user to press any button
  // Start Logo
  digitalWrite(BLUE_LED, LOW);
  arduboy.clear();
  arduboy.drawBitmap(0,0,TITLE_SCREEN,128,64,1);
  arduboy.display();
  arduboy.delayShort(2000);

  // Start Logo Banner and wait for the user
  arduboy.clear();
  arduboy.drawBitmap(0,0,TITLE_SCREEN_BANNER,128,64,1);
  arduboy.display();
  arduboy.delayShort(500);
  while(getUserInput() == 0) {}
  digitalWrite(BLUE_LED, HIGH);
}

// General Code
void loop() 
{
  //Start the Start Screen
  if (state == START) {
    StartScreen();
  }
  else if (state == INSTRUCTIONS) {
    InstructionScreen();
  }
  else if (state == GAME) {
    GameScreen();
  }
  else if (state == WIN) {
    EndScreen();
  }
  else if (state == LOSE) {
    EndScreen();
  }
  
}

// General Game
void GameScreen() 
{

  // Start Values
  int gameState = ROLL;
  int pointerLocation = 1;
  int input;
  int firstDice = -1;
  int secondDice = -1;
  int valueCal;

  // Reset Values
  gameStrikes = 0;
  for (int i=0; i<10; i++) {
    gameValues[i] = 0;
  }
  

  while (state == GAME) {
    // Display The Backround
    arduboy.clear();
    displayGameScreen();
    
  
    if (gameState == ROLL) {
      arduboy.drawBitmap(2,2,POINTER,8,8,1);
      arduboy.drawBitmap(11,3,ROLL_OPT,32,8,1);
      arduboy.display();
      
    }
    else if (gameState == ROLLING) {
      // Roll Animation
      for (int i=0; i<random(3, 10); i++) {
        firstDice = random(1, 7);
        secondDice = random(1, 7);
        arduboy.clear();
        displayGameScreen();
        arduboy.drawBitmap(52,23,getDiceImage(firstDice),16,16,1);
        arduboy.drawBitmap(76,23,getDiceImage(secondDice),16,16,1);
        arduboy.display();
        arduboy.delayShort(200);
      }
    }
    else if (gameState == MATH) {
      // Display Math Options
      arduboy.drawBitmap(11,11,ADD,32,8,1);
      arduboy.drawBitmap(11,19,SUBTRACT,32,8,1);
      arduboy.drawBitmap(11,27,MULTIPLY,32,8,1);
      arduboy.drawBitmap(11,35,DIVIDE,32,8,1);
      // Display Locations
      arduboy.drawBitmap(2,(pointerLocation*8+2),POINTER,8,8,1);
      // Display Dice
      arduboy.drawBitmap(52,23,getDiceImage(firstDice),16,16,1);
      arduboy.drawBitmap(76,23,getDiceImage(secondDice),16,16,1);
      // Display the Total Image
      arduboy.display();
      
    }

    // Double Six! -> Reset Values
    if (firstDice == 6 && secondDice == 6) {
      for (int i=0; i<10; i++) {
        gameValues[i] = 0;
      }
      arduboy.delayShort(1000);
      gameState = DOUBLE_SIX;
      firstDice = -1;
      secondDice = -1;
    }
    

     // Get The user input based on current location
    input = getUserInput();
    if (gameState == ROLL && input == 5) {

      // Change State
      gameState = ROLLING;
    }
    else if (gameState == ROLLING) {
      // Change State
      gameState = MATH;
    }
    else if (gameState == DOUBLE_SIX) {
      // Change State
      gameState = ROLL;
    }
    else if (gameState == MATH && input == 4 && pointerLocation > 1) {
      // Moveing DOWN
      pointerLocation--;
      
    }
    else if (gameState == MATH && input == 3 && pointerLocation < 4) {
      pointerLocation++;
    }
    else if (gameState == MATH && input == 5) {
      // Do the MATH
      // ADD
      if (pointerLocation == 1) {
        valueCal = firstDice + secondDice;
        // Check if Valid
        if (valueCal > 10) {
          setStrike();
        }
        else if (gameValues[valueCal-1] == 1){
          setStrike();
        }
        else {
          setAValue(valueCal);
        }
      }
      // SUBTRACT
      else if (pointerLocation == 2) {
        // Check values
        if (firstDice > secondDice) {
          valueCal = firstDice - secondDice;
          if (gameValues[valueCal-1] == 1) {
            setStrike();
          }
          else {
            setAValue(valueCal);
          }
        }
        else if (firstDice < secondDice){
          valueCal = secondDice - firstDice;
          if (gameValues[valueCal-1] == 1) {
            setStrike();
          }
          else {
            setAValue(valueCal);
          }
        }
        else {
          setStrike();
        }
      }
      // MULIPLY
      else if (pointerLocation == 3) {
        valueCal = firstDice * secondDice;
        if (valueCal > 10) {
          setStrike();
        }
        else if (gameValues[valueCal-1] == 1){
          setStrike();
        }
        else {
          setAValue(valueCal);
        }
      }
      // DIVIDE
      else if (pointerLocation == 4) {
        // Check values
        if (firstDice >= secondDice) {
          if (firstDice % secondDice != 0) {
            setStrike();
          }
          else {
            valueCal = firstDice / secondDice;
            if (gameValues[valueCal-1] == 1){
              setStrike();
            }
            else {
              setAValue(valueCal);
            }
          }
        }
        else {
          if (secondDice % firstDice != 0) {
            setStrike();
          }
          else {
            valueCal = secondDice / firstDice;
            if (gameValues[valueCal-1] == 1){
              setStrike();
            }
            else {
              setAValue(valueCal);
            }
          }
        }
      }
      
      // Change State
      gameState = ROLL;
    }
    
  }

}

// Start Screen After title
void StartScreen()
{
  int pointerLocation = 1;
  int input;

  // Draw the Screen
  while(state == START){
    arduboy.clear();
    arduboy.drawBitmap(0,0,START_SCREEN,128,64,1);
    if (pointerLocation == 1) {
      arduboy.drawBitmap(2,5,POINTER,8,8,1);
    }
    else if (pointerLocation == 2){
      arduboy.drawBitmap(2,13,POINTER,8,8,1);
    }
    else {
      // Error
    }
    arduboy.display();

    // Get The user input based on current location
    input = getUserInput();
    if (input == 3 && pointerLocation == 1) {
      pointerLocation = 2;
    }
    else if (input == 4 && pointerLocation == 2) {
      pointerLocation = 1;
    }
    else if (input == 5 && pointerLocation == 2) {
      state = INSTRUCTIONS;
    }
    else if (input == 5 && pointerLocation == 1) {
      state = GAME;
    }
    
  } //state == start
}

// General Instructions
void InstructionScreen() 
{
  arduboy.clear();
  arduboy.drawBitmap(0,0,INSTRUCTION_SCREEN,128,64,1);
  arduboy.display();
  while(getUserInput() == 0) {}
  state = START;
}

// General User input
int getUserInput()
{
  if (arduboy.pressed(LEFT_BUTTON)) {
    if (lastButtonPressed == 1) {
      return 0;
    }
    else {
      lastButtonPressed = 1;
      return 1;
    }
  }
  else if (arduboy.pressed(RIGHT_BUTTON)) {
    if (lastButtonPressed == 2) {
      return 0;
    }
    else {
      lastButtonPressed = 2;
      return 2;
    }
  }
  else if (arduboy.pressed(DOWN_BUTTON)) {
    if (lastButtonPressed == 3) {
      return 0;
    }
    else {
      lastButtonPressed = 3;
      return 3;
    }
  }
  else if (arduboy.pressed(UP_BUTTON)) {
    if (lastButtonPressed == 4) {
      return 0;
    }
    else {
      lastButtonPressed = 4;
      return 4;
    }
  }
  else if (arduboy.pressed(A_BUTTON)) {
    if (lastButtonPressed == 5) {
      return 0;
    }
    else {
      lastButtonPressed = 5;
      return 5;
    }
  }
  else if (arduboy.pressed(B_BUTTON)) {
    if (lastButtonPressed == 6) {
      return 0;
    }
    else {
      lastButtonPressed = 6;
      return 6;
    }
  }
  else {
    lastButtonPressed = 0;
    return 0;
  }
}


// Get a dice image
unsigned char const* getDiceImage(int number)
{
  switch (number) {
  case 1:
    return DICE_ONE;
    break;
  case 2:
    return DICE_TWO;
    break;
  case 3:
    return DICE_THREE;
    break;
  case 4:
    return DICE_FOUR;
    break;
  case 5:
    return DICE_FIVE;
    break;
  case 6:
    return DICE_SIX;
    break;
  default:
    // ERROR
    break;
  }
}

// Add a strike
void setStrike() {
  gameStrikes++;
  digitalWrite(RED_LED, LOW);
  arduboy.delayShort(300);
  digitalWrite(RED_LED, HIGH);
}

// Add a value
void setAValue(int valueIn) {
  gameValues[valueIn-1] = 1;
  digitalWrite(GREEN_LED, LOW);
  arduboy.delayShort(300);
  digitalWrite(GREEN_LED, HIGH);
}

// Get a dice image
void displayGameScreen()
{
  arduboy.drawBitmap(0,0,GAME_BACKROUND,128,64,1);

  // Display and check gameStrikes
  if (gameStrikes > 2) {
    arduboy.drawBitmap(111,9,STRIKE,8,8,1);
    arduboy.drawBitmap(101,9,STRIKE,8,8,1);
    arduboy.drawBitmap(91,9,STRIKE,8,8,1);
    state = LOSE;
  }
  else if (gameStrikes > 1) {
    arduboy.drawBitmap(101,9,STRIKE,8,8,1);
    arduboy.drawBitmap(91,9,STRIKE,8,8,1);
  }
  else if (gameStrikes > 0) {
    arduboy.drawBitmap(91,9,STRIKE,8,8,1);
  }

  // Dispaly and check Numbers
  int countNum = 0;
  for (int i=0; i<10; i++) {
    if (gameValues[i] == 1) {
      arduboy.drawBitmap((i+1)*10+1,45,NUMBER_COVER,8,16,1);
      countNum++;
    }
  }
  if (gameValues[9] == 1){
    arduboy.drawBitmap(109,45,NUMBER_COVER,8,16,1);
  }
  if (countNum == 10) {
    state = WIN;
  } 
}

// Final Screen
void EndScreen() {
  arduboy.clear();
  if (state == WIN) {
    arduboy.drawBitmap(0,0,WIN_GAME,128,64,1);
  }
  else {
    arduboy.drawBitmap(0,0,LOSE_GAME,128,64,1);
  }
  arduboy.display();
  while(getUserInput() == 0) {}
  state = START;
}






  
