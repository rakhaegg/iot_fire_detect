#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Update these with values suitable for your network.
#define DHTTYPE DHT11
#define sensorLDR A0
int nilaiSensor;

const char *ssid = "JTI-POLINEMA";
const char *password = "jtifast!";
const char *mqtt_server = "192.168.73.243"; 

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

DHT dht(D1, DHTTYPE);
// LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buzzer = D7; 

void setup_wifi()
{

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
      // client.publish("codersid/nodemcu/v1", "hello world");
      // client.publish("event", "hello world");

      // // ... and resubscribe
      // client.subscribe("codersid/nodemcu/v1");
      // client.subscribe("event");
      // client.subscribe("temperature");
      client.subscribe("node1/temperature");
      client.subscribe("node1/humidity");
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

void setup()
{
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

  pinMode(LED_BUILTIN, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  // lcd.init(); // initialize the lcd
  // lcd.backlight();
  // lcd.clear();
  // lcd.home();
}

void loop()
{
  byte error, address;
  int nDevices;
  nDevices = 0;
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000)
  {
    lastMsg = now;
    ++value;
    // snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f))
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Computes temperature values in Celsius
    float hic = dht.computeHeatIndex(t, h, false);
    static char temperatureTemp[7];
    dtostrf(hic, 6, 2, temperatureTemp);

    // Uncomment to compute temperature values in Fahrenheit
    // float hif = dht.computeHeatIndex(f, h);
    // static char temperatureTemp[7];
    // dtostrf(hif, 6, 2, temperatureTemp);

    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);

    // Publishes Temperature and Humidity values
    client.publish("node1/temperature", temperatureTemp);
    client.publish("node1/humidity", humidityTemp);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t Heat index: ");
    Serial.print(hic);
    Serial.println(" *C ");

    nilaiSensor = analogRead(sensorLDR);

    static char ldr[7];
    dtostrf(nilaiSensor, 6, 2, ldr);

    client.publish("node1/ldr", ldr);
    Serial.print("Nilai Sensor : ");
    Serial.println(nilaiSensor);

    if (hic > 29 )
    {

      client.publish("node1/detectionFire", "API Detect");
      tone(buzzer, 1000); // Send 1KHz sound signal...
       delay(1000);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...
      delay(1000); 
    }
    else
    {
      client.publish("node1/nodeDetection", "Tidak Detect");
    }
   
    // // lcd.home();
    // // lcd.print("Nilai Intensitas");
    // String myString = String(nilaiSensor);
    // // lcd.setCursor(0, 1);
    // // scrollText(1, myString, 250, 16);
    // lcd.print(nilaiSensor);
  }
}