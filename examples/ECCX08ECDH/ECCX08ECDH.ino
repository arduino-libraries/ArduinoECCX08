/*
  ECCX08 ECDH

  This sketch uses the ECC608 to compute the ECDH shared secret
  for a ECCX08 private key and a public key.

*/

#include <ArduinoECCX08.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!ECCX08.begin()) {
    Serial.println("Failed to communicate with ECC508/ECC608!");
    while (1);
  }

  if (!ECCX08.locked()) {
    Serial.println("The ECC508/ECC608 is not locked!");
    while (1);
  }

  byte sharedSecret[64];
  if (!ECCX08.ecdh(2, sharedSecret)) {
    Serial.println("The ecdh function failed!");
    while (1);
  }

  Serial.print("Shared secret = ");
  printHex(sharedSecret, 64);
}

void loop() {
}

void printHex(uint8_t *data, uint8_t length) {
    char tmp[16];
    for (int i = 0; i < length; i++) {
        sprintf(tmp, "%.2X", data[i]);
        Serial.print(tmp);
        Serial.print(" ");
    }
    Serial.println();
}
