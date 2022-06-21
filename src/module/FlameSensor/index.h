#include <Arduino.h>
#define Fire_Digital D5

void setupFire(){
    pinMode(Fire_Digital , OUTPUT);
}
void readFire(){
    int digitalFire = digitalRead(5);


    Serial.print("Digital Fire : ");
    Serial.println(digitalFire);
    
}