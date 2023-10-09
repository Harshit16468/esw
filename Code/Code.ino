#include <Adafruit_BMP280.h>

#include <WiFi.h>

#include "DHT.h"
#include <ThingSpeak.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "me20p";
const char* password = "987654321";
int ChannelID = 2287708;
char* APIKey = "4WTZWUBW20DBD1JT";
const char* server = "mqtt3.thingspeak.com";
char mqttUserName[] = "DzkQOw0wMRUeBREaMQE7Hg0";
char mqttPass[] = "A3aGJ7uZlQgw4JzZunWvwN/0";
const char* clientID = "DzkQOw0wMRUeBREaMQE7Hg0";

WiFiClient client;
PubSubClient mqttClient(client);


void mqttPublish(long pubChannelID, char* pubWriteAPIKey, float dataValue1, float dataValue2, float dataValue3, float dataValue4) {
  String topicString = "channels/" + String(pubChannelID) + "/publish";
  String dataString = "field1=" + String(dataValue1, 2);
  dataString+= "&field2=";
  dataString+= String(dataValue2, 2);
  dataString+= "&field3=";
  dataString+= String(dataValue3, 2);
  dataString+= "&field4=";
  dataString+= String(dataValue4, 2);
  Serial.println(topicString);
  Serial.println(dataString);
  mqttClient.publish(topicString.c_str(), dataString.c_str());
  Serial.println(pubChannelID);
}

#define DHT_PIN 12 // Change this to the GPIO pin you connected your DHT11 sensor to
#define DHT_TYPE DHT11

#define MQ2_PIN 32 // The analog input pin connected to the DOUT pin of the MQ-2 sensor
#define MQ135_PIN 33 // The analog input pin connected to the MQ-135 sensor

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(MQ2_PIN, INPUT);
  pinMode(MQ135_PIN, INPUT);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
  mqttClient.setServer(server, 1883);
}

void loop() {

  if (!mqttClient.connected()) {
    mqttClient.connect(clientID, mqttUserName, mqttPass);
    delay(1000);
    Serial.println("Connecting to Client...");
  }
  mqttClient.loop();
  // Read temperature and humidity from DHT11 sensor
  float temperatureDHT = dht.readTemperature();
  float humidityDHT = dht.readHumidity();

  // Read analog value from MQ-2 gas sensor
  float sensorValueMQ2 = analogRead(MQ2_PIN);
  float voltageMQ2 = (sensorValueMQ2 / 4095.0) * 5.0; // Convert ADC reading to voltage

  // Read analog value from MQ-135 gas sensor
  float sensorValueMQ135 = analogRead(MQ135_PIN);
  // You can add code to interpret the sensor value from the MQ-135 here
  // Refer to the MQ-135 sensor datasheet for calibration and conversion factors

  // Print DHT11 data
  Serial.println("DHT11 Sensor Data:");
  Serial.print("Temperature: ");
  Serial.print(temperatureDHT);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidityDHT);
  Serial.println(" %");

  // Print MQ-2 gas sensor data
  Serial.println("MQ-2 Sensor Data:");
  Serial.print("Sensor Value: ");
  Serial.println(sensorValueMQ2);
  Serial.print("Voltage: ");
  Serial.println(voltageMQ2, 2);

  // Print MQ-135 gas sensor data
  Serial.println("MQ-135 Sensor Data:");
  Serial.print("Sensor Value: ");
  Serial.println(sensorValueMQ135);

  Serial.println();
  mqttPublish(ChannelID, APIKey, temperatureDHT, humidityDHT, sensorValueMQ2, sensorValueMQ135);
  delay(2000); // Wait for 2 seconds before taking the next reading

}
