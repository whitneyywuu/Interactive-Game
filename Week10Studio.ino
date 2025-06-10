#include <Adafruit_CircuitPlayground.h>

// Game Settings
const float LEVEL_DELAYS[] = {1.5, 1.0, 0.5};  // Delay times for each level
const int POINTS_TO_LEVEL_UP = 10;            // CHANGED: Level up every 10 points
const int MAX_POINTS = 50;                    // ADDED: Win condition
const int MAX_MISTAKES = 3;
const int SHAKE_THRESHOLD = 18;

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
  int colorChoice = random(3);
  switch (colorChoice) {
    case 0: currentColor = "red"; showAllPixels(RED); break;
    case 1: currentColor = "blue"; showAllPixels(BLUE); break;
    case 2: currentColor = "yellow"; showAllPixels(YELLOW); break;
  }
  lastLightTime = millis();
  waitingForInput = true;
}

void playVictoryTune() {
  CircuitPlayground.playTone(988, 200);
  delay(200);
  CircuitPlayground.playTone(1175, 200);
  delay(200);
  CircuitPlayground.playTone(1319, 400);
}

void endGame() {
  gameActive = false;
  clearPixels();
  delay(500);

  int pixelsToLight = min(score, 10);
  for (int i = 0; i < pixelsToLight; i++) {
    CircuitPlayground.setPixelColor(i, CYAN);
    delay(200);
  }

  Serial.begin(9600);
  Serial.print("Final Score: ");
  Serial.print(score);
  Serial.print(" | Mistakes: ");
  Serial.println(mistakes);

  if (score >= MAX_POINTS) {
    playVictoryTune();  // ADDED: happy sound on win
  } else {
    CircuitPlayground.playTone(220, 500);
    delay(200);
    CircuitPlayground.playTone(196, 500);
  }

  while (!CircuitPlayground.leftButton()) {
    delay(50);
  }
  clearPixels();
}

void checkInput() {
  if (!waitingForInput || !gameActive) return;

  float reactionTime = (millis() - lastLightTime) / 1000.0;
  float timeLimit = LEVEL_DELAYS[currentLevel];

  bool correct = false;
  if (currentColor == "red" && CircuitPlayground.leftButton()) {
    correct = true;
  } else if (currentColor == "blue" && CircuitPlayground.rightButton()) {
    correct = true;
  } else if (currentColor == "yellow" && abs(CircuitPlayground.motionZ()) > SHAKE_THRESHOLD) {
    correct = true;
  }

  if (!correct && !(reactionTime > timeLimit)) return;

  if (correct && reactionTime <= timeLimit) {
    score++;
    playSound(true);

    if (score >= MAX_POINTS) {      // ADDED: End on win
      endGame();
      return;
    }

    if (score >= (currentLevel + 1) * POINTS_TO_LEVEL_UP && currentLevel < 2) {
      currentLevel++;
      CircuitPlayground.playTone(988, 200);
      delay(200);
      CircuitPlayground.playTone(1318, 300);
      delay(300);
    }
  } else {
    mistakes++;
    playSound(false);
    if (mistakes >= MAX_MISTAKES) {
      endGame();
      return;
    }
  }

  waitingForInput = false;
  clearPixels();
  delay(500);
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
    } else {
      delay(500);
      startRound();
    }
  }
}

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  if (!gameActive) {
    if (CircuitPlayground.leftButton()) {
      startGame();
    }
  } else {
    checkTimeout();
    checkInput();
  }

  delay(50);
}
