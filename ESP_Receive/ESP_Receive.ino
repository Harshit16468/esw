#include <HTTPClient.h>

/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/communication-between-two-esp32
 */

#include <WiFi.h>
#include <ThingSpeak.h>
#include <PubSubClient.h>

#define SERVER_PORT 4080
HTTPClient http;
const char* ssid = "me20p";     // CHANGE TO YOUR WIFI SSID
const char* password = "987654321"; // CHANGE TO YOUR WIFI PASSWORD
int ChannelID = 2287708;
char* APIKey = "4WTZWUBW20DBD1JT";
const char* server = "mqtt3.thingspeak.com";
char mqttUserName[] = "DzkQOw0wMRUeBREaMQE7Hg0";
char mqttPass[] = "A3aGJ7uZlQgw4JzZunWvwN/0";
const char* clientID = "DzkQOw0wMRUeBREaMQE7Hg0";

const char* serverAddress = "http://127.0.0.1:8000/post_endpoint";
WiFiClient client;
PubSubClient mqttClient(client);
WiFiServer TCPserver(SERVER_PORT);

void mqttPublish(long pubChannelID, char* pubWriteAPIKey, float dataValue1, float dataValue2, float dataValue3, float dataValue4,float data5,float data6) {
  String topicString = "channels/" + String(pubChannelID) + "/publish";
  String dataString = "field1=" + String(dataValue1, 2);
  dataString+= "&field2=";
  dataString+= String(dataValue2, 2);
  dataString+= "&field3=";
  dataString+= String(dataValue3, 2);
  dataString+= "&field4=";
  dataString+= String(dataValue4, 2);
  dataString+= "&field5=";
  dataString+= String(data5, 2);
  dataString+= "&field6=";
  dataString+= String(data6, 2);
  Serial.println(topicString);
  Serial.println(dataString);
  mqttClient.publish(topicString.c_str(), dataString.c_str());
  Serial.println(pubChannelID);
}

void httpPublish(float val1, float val2, float val3, float val4, float val5, float val6){
    http.begin("127.0.0.1:5000/post_data/1");
    int httpResponseCode = http.POST(String(val1));  // convert float to string
     Serial.println("HTTP response code: " + String(httpResponseCode));
    Serial.println("HTTP response body: " + http.getString());
    http.end();
     http.begin("127.0.0.1:5000/post_data/2");
    httpResponseCode = http.POST(String(val2));  // convert float to string
    Serial.print(httpResponseCode);
    http.end();
     http.begin("127.0.0.1:5000/post_data/3");
    httpResponseCode = http.POST(String(val3));  // convert float to string
    Serial.print(httpResponseCode);
    http.end();
     http.begin("127.0.0.1:5000/post_data/4");
     httpResponseCode = http.POST(String(val4));  // convert float to string
     Serial.print(httpResponseCode);
    http.end();
     http.begin("127.0.0.1:5000/post_data/5");
     httpResponseCode = http.POST(String(val5));  // convert float to string
     Serial.print(httpResponseCode);
    http.end();
     http.begin("127.0.0.1:5000/post_data/6");
     httpResponseCode = http.POST(String(val6));  // convert float to string
     Serial.println(httpResponseCode);
    http.end();
}

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
  mqttClient.setServer(server, 1883);
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
      if (!mqttClient.connected()) {
      mqttClient.connect(clientID, mqttUserName, mqttPass);
      delay(1000);
      Serial.println("Connecting to Client...");
  }
  mqttClient.loop();
    mqttPublish(ChannelID, APIKey, temperature, humidity, mq2reading, mq135reading,pressure,alt);
    httpPublish(temperature, humidity, mq2reading, mq135reading,pressure,alt);
    client.stop();
  }

  
  }
}
