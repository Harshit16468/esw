/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/communication-between-two-esp32
 */

#include <WiFi.h>

#define SERVER_PORT 4080

const char* ssid = "me20p";     // CHANGE TO YOUR WIFI SSID
const char* password = "987654321"; // CHANGE TO YOUR WIFI PASSWORD

WiFiServer TCPserver(SERVER_PORT);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print your local IP address:
  Serial.print("ESP32 #2: TCP Server IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("ESP32 #2: -> Please update the serverAddress in ESP32 #1 code");

  // Start listening for a TCP client (from ESP32 #1)
  TCPserver.begin();
}

void loop() {
  WiFiClient client = TCPserver.available();

  if (client) {
    // char* command = client.read();
    // Serial.print("ESP32 #2: - Received command: ");
    // Serial.println(command);
    char buffer[1024]; // Create a buffer to store the incoming data
    int bytesRead = client.readBytesUntil('\n', buffer, sizeof(buffer)); // Read data until a newline character
    if (bytesRead > 0) {
    buffer[bytesRead] = '\0'; // Null-terminate the buffer to make it a valid C-string
    Serial.print("ESP32 #2: - Received command: ");
    Serial.println(buffer);
    char* arg=strtok(buffer,",");
    float temperature=atof(arg);
    arg=strtok(NULL,",");
    float humidity=atof(arg);
    arg=strtok(NULL,",");
    float mq2reading=atof(arg);
    arg=strtok(NULL,",");
    float mq135reading=atof(arg);
    arg=strtok(NULL,",");
    float pressure=atof(arg);
    arg=strtok(NULL,",");
    float alt=atof(arg);
    

     Serial.println("DHT11 Sensor  Data:");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
   Serial.print("MQ-2 Sensor Data:");
   Serial.print("Sensor Value: ");
   Serial.println(mq2reading);
   Serial.print("MQ-135 Sensor Data:");
   Serial.print("Sensor Value: ");
   Serial.println(mq135reading);
    Serial.println("BMP:");
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.print("Altitude: ");
  Serial.println(alt);  
  }
    client.stop();
  }
}
