#include <M5Unified.h>
#include <M5GFX.h>

// Stopwatch state variables
unsigned long startTime = 0; 
unsigned long elapsedTime = 0; 
bool isRunning = false; 

M5Canvas canvas(&M5.Display); 

void updateDisplay() {
  //Display elapsed time
  canvas.setCursor(0, M5.Display.height()/2);
  canvas.clear();
  canvas.printf("Time: %d.%03d s", elapsedTime / 1000, elapsedTime % 1000);
  canvas.pushSprite(0,0); 
}

// Function to handle stopwatch start/pause
void toggleStopwatch() {
  if (isRunning) {
    // Pause the stopwatch
    elapsedTime = millis() - startTime; //capture the exact moment
    isRunning = false;
  } else {
    // Start or resume the stopwatch
    // resuming requires that we take into consideration the currently elapsed time
    // we can do this by sutracting the currently elapsed time in this moment
    // this works because we essentially faking that the time started "elapsed time" moments ago. 
    // this would work if we needed to know the exact start time when we initially began the counter 
    startTime = millis() - elapsedTime; 
    isRunning = true;
  }
}

// Function to clear and stop the stopwatch
void resetStopwatch() {
  isRunning = false;
  elapsedTime = 0;
}


void setup() {
  // Initialize M5Unified
  M5.begin();
  canvas.setTextSize(2);

  // Set display rotation and initial content
  M5.Display.setRotation(1); // Landscape orientation
  canvas.createSprite(M5.Display.width(),M5.Display.height()); //we only want to create this once, after the rotation, so it's correct
}

void loop() {
  M5.update();

  // Button handling 
  if (M5.BtnA.wasPressed()) {
    toggleStopwatch();
  }
  if (M5.BtnB.wasPressed()) {
    resetStopwatch();
  }

  // Update elapsed time if stopwatch is running
  if (isRunning) {
    elapsedTime = millis() - startTime;
  }
  updateDisplay();
}