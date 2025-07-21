/*
  ECCX08 Counter

  This sketch uses the ECC508 or ECC608 to increment a monotonic
  counter at each startup.

  Circuit:
   - Any board with ECC508 or ECC608 on board

*/

#include <ArduinoECCX08.h>

// The counter to be incremented. Only zero and one are legal values.
const int counterId = 0;
long counter = -1;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!ECCX08.begin()) {
    Serial.println("Failed to communicate with ECC508/ECC608!");
    while (1);
  }

  if (!ECCX08.incrementCounter(counterId, counter)) {
    Serial.println("Failed to increment counter");
    while (1);
  }
}

void loop() {
  if (!ECCX08.readCounter(counterId, counter)) {
    Serial.println("Failed to read counter");
    while (1);
  }

  Serial.print("Counter value = ");
  Serial.println(counter);

  delay(1000);
}

