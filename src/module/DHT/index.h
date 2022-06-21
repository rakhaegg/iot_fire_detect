#include <DHT.h>
#include <Wire.h>
#define DHTTPYE DHT11

DHT dht(D5, DHTTPYE);


float readTemperature(){
    // Read temperature as Celsius (the default)
    
    float t =0;
    
    t = dht.readTemperature();
     // Check if any reads failed and exit early (to try again).
    if (isnan(t))
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        t = 0;
    }
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.println(" C ");
    return t;
}
float readHumidity(){
   // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h =0;
    h= dht.readHumidity();
    

    if (isnan(h) )
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        h=0;
    }
    Serial.print("Humidity : ");
    Serial.print(h);
    Serial.println(" % ");
    return h;

}
// void readDHT()
// {

 

//     // Read temperature as Fahrenheit (isFahrenheit = true)
//     float f = dht.readTemperature(true);

//     // Check if any reads failed and exit early (to try again).
//     if (isnan(h) || isnan(t) || isnan(f))
//     {
//         Serial.println(F("Failed to read from DHT sensor!"));
//         return;
//     }

//     // Compute heat index in Fahrenheit (the default)
//     float hif = dht.computeHeatIndex(f, h);
//     // Compute heat index in Celsius (isFahreheit = false)
//     float hic = dht.computeHeatIndex(t, h, false);

//     Serial.print(F(" Humidity: "));
//     Serial.print(h);
//     Serial.print(F("%  Temperature: "));
//     Serial.print(t);
//     static char temperatureTemp[7];
//     dtostrf(t, 6, 2, temperatureTemp);
    
//     Serial.print(F("C "));
//     Serial.print(f);
//     Serial.print(F("F  Heat index: "));
//     Serial.print(hic);
//     Serial.print(F("C "));
//     Serial.print(hif);
//     Serial.println(F("F"));

//     // publishResultTemperature(celcius);
// }
