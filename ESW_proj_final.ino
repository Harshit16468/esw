#include "DHT.h"

#define DHT_PIN 14 // Change this to the GPIO pin you connected your DHT11 sensor to
#define DHT_TYPE DHT11

#define MQ2_PIN 12 // The analog input pin connected to the DOUT pin of the MQ-2 sensor

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(MQ2_PIN, INPUT);
}

void loop() {
  // Read temperature and humidity from DHT11 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read analog value from MQ-2 gas sensor
  int sensorValue = analogRead(MQ2_PIN);
  float voltage = (sensorValue / 4095.0) * 5.0; // Convert ADC reading to voltage (assuming ESP32's 12-bit ADC)

  // Print DHT11 data
  Serial.println("DHT11 Sensor Data:");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Print MQ-2 gas sensor data
  Serial.println("MQ-2 Sensor Data:");
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  Serial.print("Voltage: ");
  Serial.println(voltage, 2); // Print voltage with 2 decimal places

  // You can add code to interpret the sensor data for gas concentration here
  // Refer to the MQ-2 sensor datasheet for calibration and conversion factors

  Serial.println();
  
  delay(2000); // Wait for 2 seconds before taking the next reading
}