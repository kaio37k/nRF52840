#include <Wire.h>

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output

  delay(100);

  Serial.println("Setup Started");
  // Wire.write(0x07);

}

void loop() {
  Wire.beginTransmission(0x10); // START
  Wire.write(0x4C);
  Wire.endTransmission(false);
  
  Wire.requestFrom(0x10, 1);
  byte receivedByte = Wire.read();
  Serial.println(receivedByte);
  
  // delay(500);
}
