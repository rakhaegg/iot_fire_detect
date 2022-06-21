#include <Arduino.h>
#include <Wire.h>
#include <module/DHT/index.h>
#include <module/LDR/index.h>
//#include <module/Connection/index.h>
#include <module/BMP280/index.h>
#include <module/GPS/index.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
// #include <module/MultiPlexer/index.h>
// #include <module/FlameSensor/index.h>
const char *ssid = "AFI";
const char *password = "085234168201";
const char *mqtt_server = "192.168.1.4";

WiFiClient espClient;
PubSubClient client(espClient);

int Rload = 10000;
double ppm = 100;
float a = 100.2808508;
float b = -1.541315509;
float ro = 35649.36;
float minPPM = 0;
float maxPPM = 0;

// Mux in "SIG" pin
#define SIG_pin A0
float val = 0;
#define flameSensor D6
#define buzzer D7

void setup_wifi()
{
  /*
  ! SETUP WIFI
  */
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
/*
  ! SETUP MQTT
*/
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("esp/halo", "hello world");
      // client.publish("event", "hello world");

      // // ... and resubscribe
      // client.subscribe("codersid/nodemcu/v1");
      // client.subscribe("event");
      // client.subscribe("temperature");
      client.subscribe("esp/halo");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1')
  {
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  }
  if ((char)payload[0] == '0')
  {
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  }
}

void setup()
{

  Serial.begin(9600);

  minPPM = pow((10000 / a), 1 / b);
  maxPPM = pow((10 / a), 1 / b);

  setup_wifi();

  pinMode(D0, INPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, LOW);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

float smooth()
{
  int i;
  float value = 0;
  float numReadings = 10;

  for (i = 0; i < numReadings; i++)
  {
    // Read light sensor data.
    value = value + analogRead(SIG_pin);

    // 1ms pause adds more stability between reads.
    delay(1);
  }

  // Take an average of all the readings.
  value = value / numReadings;

  // Scale to 8 bits (0 - 255).
  value = value / 4;

  return value;
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  // // Read light sensor data.
  // float value = smooth();
  // Serial.println(value);
  // static char ldr[7];
  // dtostrf(value, 6, 2, ldr);

  int adcRaw = analogRead(SIG_pin);
  double rs = ((1024.0 * Rload) / adcRaw) - Rload;
  float rsro = rs / ro;

  if (rsro < maxPPM && rsro > minPPM)
  {
    float ppm = a * pow((float)rs / (float)ro, b);
    static char mq_publish[7];
    dtostrf(ppm, 6, 2, mq_publish);
    client.publish("node/mq", mq_publish);
    Serial.print("CO : ");
    Serial.println(ppm);
  }

  // readBMP();
  static char temperatureTemp[7];
  dtostrf(readTemperature(), 6, 2, temperatureTemp);
  client.publish("node/temperature", temperatureTemp);

  static char humidity[7];
  dtostrf(readHumidity(), 6, 2, humidity);
  client.publish("node/humidity", humidity);

  int flame = digitalRead(D0);
  Serial.print("Flame : ");
  Serial.println(flame);
  
  static char publish_flame[7];
  dtostrf(flame, 6, 2, publish_flame);

  client.publish("node/flame" , publish_flame); 
  
  int buzzerValue = digitalRead(buzzer);
  Serial.print("Buzzer : ");
  Serial.println(buzzerValue);

  /*
    ! Deteksi Api
  */

  /*
   if(buzzerValue == 1){
     Serial.println("API DETEKSI");
     for (size_t i = 0; i < 10; i++)
     {
       digitalWrite(buzzer , HIGH);
     }
     digitalWrite(buzzer , LOW);
   }
   */
  delay(1000);
}