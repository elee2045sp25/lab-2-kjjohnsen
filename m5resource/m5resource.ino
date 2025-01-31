#include "M5Unified.h"
#include "M5GFX.h"

unsigned long last_message = 0;
M5Canvas canvas(&M5.Display); // a canvas allows you to do all drawing off-screen, helping to avoid artifacts from drawing while displaying

typedef struct {
  uint16_t cpu;
  uint8_t mem;
  uint8_t battery;
} StatsPacket;

void setup() {
  M5.begin();
  Serial.begin(115200);
  M5.Display.setRotation(1);
  canvas.createSprite(M5.Display.width(),M5.Display.height()); //we only want to create this once, after the rotation, so it's correct
  canvas.setTextSize(2);
  last_message = millis();
}
void loop() {
  // put your main code here, to run repeatedly:
  M5.update();

  if(Serial.available() > sizeof(StatsPacket)){
    StatsPacket packet;
    Serial.readBytes((uint8_t*)&packet,sizeof(StatsPacket));
    canvas.clear();
    canvas.setCursor(0,10);
    canvas.printf(" CPU=%d\n MEM=%d\n BAT=%d",packet.cpu,packet.mem,packet.battery);
    last_message = millis();
  }
  if(M5.BtnA.wasPressed()){
    Serial.write(1);
  }
  if(M5.BtnA.wasReleased()){
    Serial.write(0);
  }
  if(M5.BtnB.wasPressed()){
    Serial.write(255);
    
  }

  unsigned long curr_time = millis();
  if(curr_time-last_message > 2000){
    canvas.clear();
    canvas.setCursor(5,10);
    canvas.printf("No connection");
  }
  canvas.pushSprite(0,0); //this assumes the canvas is the same size as the screen
}
