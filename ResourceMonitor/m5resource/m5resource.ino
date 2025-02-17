#include "M5Unified.h"
#include "M5GFX.h"

unsigned long last_message = 0; //used to determine when to change the display when "disconnected"
M5Canvas canvas(&M5.Display); 

typedef struct {
  uint16_t cpu;
  uint8_t mem;
  uint8_t battery;
} StatsPacket; //this data is sent by the connected device over serial

void setup() {
  M5.begin();
  Serial.begin(115200);
  //setup the basic M5 display settings and canvas
  M5.Display.setRotation(1); //landscape
  canvas.createSprite(M5.Display.width(),M5.Display.height()); 
  canvas.setTextSize(2); //larger text for readability
  //we also initialize last_message to the current time, to have a more accurate delta when we check again
  last_message = millis();
}
void loop() {
  M5.update();
  
  //this code reads data from the connected device, which only sends StatsPackets
  if(Serial.available() > sizeof(StatsPacket)){
    StatsPacket packet;
    Serial.readBytes((uint8_t*)&packet,sizeof(StatsPacket));
    canvas.clear();
    canvas.setCursor(0,10);
    canvas.printf(" CPU=%d\n MEM=%d\n BAT=%d",packet.cpu, packet.mem, packet.battery);
    last_message = millis();
  }

  //the next code will handle our UI
  //our event codes are 1 (pressed button), 0 (released button) and 255 (end data collection)
  if(M5.BtnA.wasPressed()){
    Serial.write(1); 
  }
  if(M5.BtnA.wasReleased()){
    Serial.write(0); 
  }
  if(M5.BtnB.wasPressed()){
    Serial.write(255);
  }

  //this code is an extra and handles displaying if the PC is disconnected
  unsigned long curr_time = millis();
  if(curr_time-last_message > 2000){
    canvas.clear();
    canvas.setCursor(5,10);
    canvas.printf("No connection");
  }

  canvas.pushSprite(0,0); //display any drawing
}
