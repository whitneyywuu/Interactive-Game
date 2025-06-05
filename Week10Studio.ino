#include <Adafruit_CircuitPlayground.h>

// Game Settings
const float LEVEL_DELAYS[] = {1.5, 1.0, 0.5};  // Delay times for each level
const int POINTS_TO_LEVEL_UP = 5;               // Points needed to advance level
const int MAX_MISTAKES = 3;                     // Mistakes allowed before game over
const int SHAKE_THRESHOLD = 10;                 // Threshold for shake detection

// Game Variables
bool gameActive = false;
int currentLevel = 0;
int score = 0;
int mistakes = 0;
unsigned long lastLightTime = 0;
String currentColor = "";
bool waitingForInput = false;

// Colors
const uint32_t RED = CircuitPlayground.strip.Color(255, 0, 0);
const uint32_t BLUE = CircuitPlayground.strip.Color(0, 0, 255);
const uint32_t YELLOW = CircuitPlayground.strip.Color(255, 255, 0);
const uint32_t GREEN = CircuitPlayground.strip.Color(0, 255, 0);
const uint32_t CYAN = CircuitPlayground.strip.Color(0, 255, 255);
const uint32_t OFF = CircuitPlayground.strip.Color(0, 0, 0);

void showAllPixels(uint32_t color) {
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, color);
  }
}

void clearPixels() {
  showAllPixels(OFF);
}

void playSound(bool success) {
  if (success) {
    CircuitPlayground.playTone(880, 200);
  } else {
    CircuitPlayground.playTone(220, 500);
  }
}

void startGame() {
  gameActive = true;
  currentLevel = 0;
  score = 0;
  mistakes = 0;
  showAllPixels(GREEN);
  CircuitPlayground.playTone(784, 300);
  delay(500);
  clearPixels();
  startRound();
}

void startRound() {
  // Randomly select a color
  int colorChoice = random(3);
  switch (colorChoice) {
    case 0: currentColor = "red"; showAllPixels(RED); break;
    case 1: currentColor = "blue"; showAllPixels(BLUE); break;
    case 2: currentColor = "yellow"; showAllPixels(YELLOW); break;
  }
  
  lastLightTime = millis();
  waitingForInput = true;
}

void endGame() {
  gameActive = false;
  clearPixels();
  delay(500);
  
  // Display score with pixels (up to 10)
  int pixelsToLight = min(score, 10);
  for (int i = 0; i < pixelsToLight; i++) {
    CircuitPlayground.setPixelColor(i, CYAN);
    delay(200);
  }
  
  // Play game over sound
  CircuitPlayground.playTone(220, 500);
  delay(200);
  CircuitPlayground.playTone(196, 500);
  
  // Keep score displayed until restart
  while (!CircuitPlayground.leftButton()) {
    delay(50);
  }
  clearPixels();
}

void checkInput() {
  if (!waitingForInput || !gameActive) return;
  
  float reactionTime = (millis() - lastLightTime) / 1000.0;
  float timeLimit = LEVEL_DELAYS[currentLevel];
  
  // Check for correct input
  bool correct = false;
  if (currentColor == "red" && CircuitPlayground.leftButton()) {
    correct = true;
  } 
  else if (currentColor == "blue" && CircuitPlayground.rightButton()) {
    correct = true;
  } 
  else if (currentColor == "yellow" && abs(CircuitPlayground.motionZ()) > SHAKE_THRESHOLD) {
    correct = true;
  }
  
  // If no input yet, return
  if (!correct && !(reactionTime > timeLimit)) return;
  
  // Handle result
  if (correct && reactionTime <= timeLimit) {
    score++;
    playSound(true);
    
    // Check for level up (only when we have enough points AND haven't reached max level)
    if (score >= (currentLevel + 1) * POINTS_TO_LEVEL_UP && currentLevel < 2) {
      currentLevel++;
      CircuitPlayground.playTone(988, 200);
      delay(200);
      CircuitPlayground.playTone(1318, 300);
      delay(300);
    }
  } 
  else {
    mistakes++;
    playSound(false);
    if (mistakes >= MAX_MISTAKES) {
      endGame();
      return;
    }
  }
  
  waitingForInput = false;
  clearPixels();
  delay(500);  // Brief pause between rounds
  startRound();
}

void checkTimeout() {
  if (!waitingForInput || !gameActive) return;
  
  if ((millis() - lastLightTime) / 1000.0 > LEVEL_DELAYS[currentLevel]) {
    mistakes++;
    playSound(false);
    waitingForInput = false;
    clearPixels();
    
    if (mistakes >= MAX_MISTAKES) {
      endGame();
    } 
    else {
      delay(500);
      startRound();
    }
  }
}

void setup() {
  CircuitPlayground.begin();
  randomSeed(analogRead(0));  // Seed random number generator
}

void loop() {
  if (!gameActive) {
    if (CircuitPlayground.leftButton()) {
      startGame();
    }
  } 
  else {
    checkTimeout();
    checkInput();
  }
  
  delay(50);  // Small delay to prevent overwhelming the board
}