#include "DHT.h"

#define DHT_PIN 14 // Change this to the GPIO pin you connected your DHT11 sensor to
#define DHT_TYPE DHT11

#define MQ2_PIN 2 // The analog input pin connected to the DOUT pin of the MQ-2 sensor
#define MQ135_PIN 4 // The analog input pin connected to the MQ-135 sensor

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(MQ2_PIN, INPUT);
  pinMode(MQ135_PIN, INPUT);
}

void loop() {
  // Read temperature and humidity from DHT11 sensor
  float temperatureDHT = dht.readTemperature();
  float humidityDHT = dht.readHumidity();

  // Read analog value from MQ-2 gas sensor
  int sensorValueMQ2 = analogRead(MQ2_PIN);
  float voltageMQ2 = (sensorValueMQ2 / 4095.0) * 5.0; // Convert ADC reading to voltage

  // Read analog value from MQ-135 gas sensor
  int sensorValueMQ135 = analogRead(MQ135_PIN);
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
  
  delay(2000); // Wait for 2 seconds before taking the next reading
}
