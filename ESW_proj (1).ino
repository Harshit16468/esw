#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>


#define mq2sensorpin 12
#define mq135sensorpin 13
#define dhtsensorpin 12 

#define I2C_SDA 32
#define I2C_SCL 14
TwoWire I2CBMP=TwoWire(0);
Adafruit_BMP280 bmp(&I2CBMP);

// const char *AP_SSID = "me20p";
// const char *AP_PWD = "987654321";
// WiFiClient wifiClient;
// HTTPClient http;
void setup() {

  Serial.begin(115200);
  I2CBMP.begin(I2C_SDA,I2C_SCL,100000);

  pinMode(mq2sensorpin,INPUT);
  pinMode(mq135sensorpin,INPUT);

  Serial.begin(115200);
  bmp.begin(0x76,0x58);


  // WiFi.begin(AP_SSID, AP_PWD);
  // while (WiFi.status() != WL_CONNECTED) {
  //   Serial.println("Connecting to WiFi...");
  //   delay(1000);
  // }
  // Serial.print("Local IP address: ");
  // Serial.println(WiFi.localIP());
  // put your setup code here, to run once:

}


void loop() {
  float temp;
  float pressure;
  float altitude;
  temp=bmp.readTemperature();
  pressure=bmp.readPressure();
  altitude=bmp.readAltitude();
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.print("Altitude: ");
  Serial.println(altitude);
  delay(1000);

}