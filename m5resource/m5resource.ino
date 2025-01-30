#include "M5Unified.h"
void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();

  if(Serial.available() > 2){
    int cpu = Serial.read();
    int mem = Serial.read();
    int bat = Serial.read();
    //M5.Display.clear();
    M5.Display.setCursor(0,0);
    M5.Display.printf("CPU=%d,MEM=%d,BAT=%d",cpu,mem,bat);
  }
  if(M5.BtnA.wasPressed()){
    Serial.write(1);
  }
  if(M5.BtnA.wasReleased()){
    Serial.write(0);
  }
  M5.delay(1);
}
