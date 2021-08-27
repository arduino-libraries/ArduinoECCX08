/*
  ECCX08 AES-GCM

  This sketch uses the ECC608 to compute
  the AES_128_GCM encryption for some data

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

  byte devicePubKey[64];
  if (!ECCX08.generatePublicKey(0, devicePubKey)){
    Serial.println("Failed to generate private key.");
  } else {
    Serial.println("Device Public key:");
    printHex(devicePubKey, 64);
  }

  byte IV[12];
  if (!ECCX08.AESGenIV(IV)){
    Serial.println("Failed to initialize IV.");
  } else {
    Serial.print("IV: ");
    printHex(IV, 12);
  }

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
