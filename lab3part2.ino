#include <Wire.h>
#include <Adafruit_BMP280.h>

#define I2C_SDA 21
#define I2C_SCL 22
TwoWire I2CBMP=TwoWire(0);
Adafruit_BMP280 bmp(&I2CBMP);
void setup() {
  I2CBMP.begin(I2C_SDA,I2C_SCL,100000);
  Serial.begin(115200);
  bmp.begin(0x76,0x58);
  
}

void loop() {
  float avg_temp=0;
  float avg_pres=0;
  float avg_alt=0;
  for(int i=0;i<5;i++){
    avg_temp+=bmp.readTemperature();
    avg_pres+=bmp.readPressure();
    avg_alt+=bmp.readAltitude();
  Serial.println("Temperature:");
  Serial.println(bmp.readTemperature());
  Serial.println("Pressure:");
  Serial.println(bmp.readPressure());
  Serial.println("Altitude:");
  Serial.println(bmp.readAltitude());
  delay(1000);
  }

  Serial.print("\t");
  Serial.print("Average temp:");
  Serial.print(avg_temp/5);
  Serial.print("\t");
  Serial.print("Average Pressure:");
  Serial.print(avg_pres/5);
  Serial.print("\t");
  Serial.print("Average Altitude:");
  Serial.print(avg_alt/5);


}
