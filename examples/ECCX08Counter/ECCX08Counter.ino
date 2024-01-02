/*
  ECCX08 Counter

  This sketch uses the ECC508 or ECC608 to increment a monotonic 
  counter at each startup

  Circuit:
   - Any board with ECC508 or ECC608 on board

*/

#include <ArduinoECCX08.h>

const int keyId = 5;
long counter = -1;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!ECCX08.begin()) {
    Serial.println("Failed to communicate with ECC508/ECC608!");
    while (1);
  }
  
  if (!ECCX08.incrementCounter(keyId, counter)) {
    Serial.println("Failed to increment counter");
    while (1);
  }
}

void loop() {
  if (!ECCX08.readCounter(keyId, counter)) {
    Serial.println("Failed to read counter");
    while (1);
  }

  Serial.print("Counter value = ");
  Serial.println(counter);

  delay(1000);
}

