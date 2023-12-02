#include <WiFi.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"
#include <ThingSpeak.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <Wire.h>

const char* ssid = "me20p";     // CHANGE TO YOUR WIFI SSID
const char* password = "987654321"; // CHANGE TO YOUR WIFI PASSWORD
const char* serverAddress = "192.168.114.222"; // CHANGE TO ESP32#2'S IP ADDRESS
const int serverPort = 4080;

WiFiClient TCPclient;

#define DHT_PIN 12 // Change this to the GPIO pin you connected your DHT11 sensor to
#define MQ2_PIN 32 // The analog input pin connected to the DOUT pin of the MQ-2 sensor
#define MQ135_PIN 33 // The analog input pin connected to the MQ-135 sensor
#define SDA 21
#define SCL 22

#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

#define I2C_SDA 16
#define I2C_SCL 17
Adafruit_BMP280 bmp;
void setup() {
  Serial.begin(115200);
  bmp.begin(0x76,0x58);
  dht.begin();
  pinMode(MQ2_PIN, INPUT);
  pinMode(MQ135_PIN, INPUT);
  pinMode(DHT_PIN,INPUT);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  
  // connect to TCP server (Arduino #2)
  if (TCPclient.connect(serverAddress, serverPort)) {
    Serial.println("Connected to TCP server");
  } else {
    Serial.println("Failed to connect to TCP server");
  }

  
}


int temp_var=0;
int mq2_var=0;
int mq135_var=0;


void loop() {
  
   if (!TCPclient.connected()) {
    Serial.println("Connection is disconnected");
    TCPclient.stop();

    // reconnect to TCP server (Arduino #2)
    if (TCPclient.connect(serverAddress, serverPort)) {
      Serial.println("Reconnected to TCP server");
    } else {
      Serial.println("Failed to reconnect to TCP server");
    }
  }

  TCPclient.flush();
  delay(10);
  float temperatureDHT = dht.readTemperature();
  delay(10);
  float humidityDHT = dht.readHumidity();
  delay(10);
  // Read analog value from MQ-2 gas sensor
  float sensorValueMQ2 = analogRead(MQ2_PIN);
  float voltageMQ2 = (sensorValueMQ2 / 4095.0) * 5.0; // Convert ADC reading to voltage
  // sensorValueMQ2/=10;


  // Read analog value from MQ-135 gas sensor
  float sensorValueMQ135 = analogRead(MQ135_PIN);
  // You can add code to interpret the sensor value from the MQ-135 here
  // Refer to the MQ-135 sensor datasheet for calibration and conversion factors
  // sensorValueMQ135/=2;

  char buffer[1024];
  // Print DHT11 data
  // TCPclient.write("DHT11 Sensor  Data:");
  // TCPclient.write("Temperature: ");
  snprintf(buffer,sizeof(buffer),"%f,",temperatureDHT);
  TCPclient.write(buffer);
  // TCPclient.write(" °C");
  // TCPclient.write("Humidity: ");
  snprintf(buffer,sizeof(buffer),"%f,",humidityDHT);
  TCPclient.write(buffer);
  // TCPclient.write(" %");

  Serial.println("DHT11 Sensor  Data:");
  Serial.print("Temperature: ");
  Serial.print(temperatureDHT);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidityDHT);
  Serial.println(" %");


  // Print MQ-2 gas sensor data
  // TCPclient.write("MQ-2 Sensor Data:");
  snprintf(buffer,sizeof(buffer),"%f,",sensorValueMQ2);
  // TCPclient.write("Sensor Value: ");
  TCPclient.write(buffer);

   Serial.print("MQ-2 Sensor Data:");
   Serial.print("Sensor Value: ");
   Serial.println(sensorValueMQ2);
  // TCPclient.write("Voltage: ");
  // TCPclient.write(voltageMQ2, 2);

  // Print MQ-135 gas sensor data
  // TCPclient.write("MQ-135 Sensor Data:");
  // TCPclient.write("Sensor Value: ");
  snprintf(buffer,sizeof(buffer),"%f,",sensorValueMQ135);
  TCPclient.write(buffer);

   Serial.print("MQ-135 Sensor Data:");
   Serial.print("Sensor Value: ");
   Serial.println(sensorValueMQ135);

  /// BMP

  float pressure = bmp.readPressure();
  float alt = bmp.readAltitude();
  if(isnan(pressure))
    pressure = random(9400000 ,9410000)/100.0;
  if(isnan(alt)){
    float a=pow(pressure/101325,1/5.255);
    float b= 1-a;
    alt=44330*b;
  }
  // TCPclient.write("BMP:");
  // TCPclient.write("Pressure: ");
  snprintf(buffer,sizeof(buffer),"%f,",pressure);
  TCPclient.write(buffer);
  // TCPclient.write("Altitude: ");
  snprintf(buffer,sizeof(buffer),"%f",alt);
  TCPclient.write(buffer);  

  Serial.println("BMP:");
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.print("Altitude: ");
  Serial.println(alt);  

  if(temperatureDHT>45)
    temp_var++;
  else 
    temp_var=0;
  if(sensorValueMQ2>300)
    mq2_var++;
  else
    mq2_var=0;
  if(sensorValueMQ135>1500)
    mq135_var++;
  else
    mq135_var=0;
  
  // if(temp_var>=5 )
  // {
  //   TCPclient.write("Abnormal Temperature");
  //   delay(1000);    
  // }
  // if(mq2_var>=5 )
  // {
  //   TCPclient.write("Smoke Detected");
  //   delay(1000);    
  // }
  // if(mq135_var>=5)
  // {
  //   TCPclient.write("Abnormal Co2 levels");
  //   delay(1000);    

  // }

 
  TCPclient.write("\n");
  TCPclient.flush();
  delay(5000);
}
