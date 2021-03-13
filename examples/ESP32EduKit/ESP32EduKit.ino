/*
  ECCX08 Random Number running on an M5Stack Core2 ESP32 IoT EduKit

  This sketch uses the ECC508 or ECC608 to generate a random number 
  every second and print it to the Serial monitor

  Circuit:
   - M5Stack Core2 ESP32 IoT EduKit

*/
#include <ArduinoECCX08.h>

void setup() {
  Serial.begin(115200); Serial.println();

  // The M5STack IoT EduKit has the ATECECCX08 at I2C address 0x35 and SDA 21, SCL 22.
  Wire.begin(21,22);
  if (!ECCX08.begin(0x35)) {
    Serial.println("Failed to communicate with ECC508/ECC608!");
    while (1);
  }

  if (!ECCX08.locked()) {
    Serial.println("The ECC508/ECC608 is not locked!");
    while (1);
  }
}

void loop() {
  Serial.print("Random number = ");
  Serial.println(ECCX08.random(65535));

  delay(1000);
}

