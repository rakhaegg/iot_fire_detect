#include <Wire.h>
#include <Arduino.h>


char * convertLDRrESULT(float value){

    static char ldr[7];
    dtostrf(value, 6, 2, ldr);
    return ldr;
}
char * readLDR(){
    float resultAnalaogRead = analogRead(A0);
    Serial.println(resultAnalaogRead);

    return convertLDRrESULT(resultAnalaogRead);

}