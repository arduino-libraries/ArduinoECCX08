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

  int privateKeySlot = 2;
  int counterpartyKeySlot = 3;

  byte devicePubKey[64];
  if (!ECCX08.generatePrivateKey(privateKeySlot, devicePubKey)){
    Serial.println("Failed to generate private key.");
  } else {
    Serial.println("Device Public key:");
    printHex(devicePubKey, 64);
  }

  byte counterPartyPubKey[64];
  // Yes, this function actually returns a public key.
  if (!ECCX08.generatePrivateKey(counterpartyKeySlot, counterPartyPubKey)){
    Serial.println("Failed to generate public key.");
  } else {
    Serial.println("Counterparty Public key:");
    printHex(counterPartyPubKey, 64);
  }

  byte sharedSecret[32];
  if (!ECCX08.ecdh(privateKeySlot, ECDH_MODE_OUTPUT, counterPartyPubKey, sharedSecret)) {
    Serial.println("The ecdh function failed!");
    while (1);
  }

  Serial.print("Shared secret = ");
  printHex(sharedSecret, 32);

  byte output[1];
  if (!ECCX08.ecdh(privateKeySlot, ECDH_MODE_TEMPKEY, counterPartyPubKey, output)) {
    Serial.println("The ecdh function failed!");
    while (1);
  }

  Serial.print("Return code = ");
  printHex(output, 1);
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
