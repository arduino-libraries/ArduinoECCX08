/*
  ECCX08 Signing

  This sketch uses the ECC508 or ECC608 to sign some data
  using the private key of a key slot, the signature
  is printed to the Serial Monitor. Then the signature is
  verified using the public key of the slot.

  NOTE: the input data must be 32 bytes in length!

  Circuit:
   - MKR board with ECC508 or ECC608 on board

  created 5 November 2019
  by Sandeep Mistry
*/

#include <ArduinoECCX08.h>

const byte input[32] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

const int slot = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("ECCX08 Signing");
  Serial.println();

  if (!ECCX08.begin()) {
    Serial.println("Failed to communicate with ECC508/ECC608!");
    while (1);
  }

  if (!ECCX08.locked()) {
    Serial.println("The ECC508/ECC608 is not locked!");
    while (1);
  }

  // print the input
  Serial.print("Input is:                  ");
  printBufferHex(input, sizeof(input));

  // retrieve the public key
  byte publicKey[64];
  ECCX08.generatePublicKey(slot, publicKey);

  // print the public key
  Serial.print("Public key of slot ");
  Serial.print(slot);
  Serial.print(" is:   ");
  printBufferHex(publicKey, sizeof(publicKey));

  // calculate the signature, input MUST be 32-byte array
  byte signature[64];
  ECCX08.ecSign(slot, input, signature);

  // print the signature
  Serial.print("Signature using slot ");
  Serial.print(slot);
  Serial.print(" is: ");
  printBufferHex(signature, sizeof(signature));

  Serial.println();

  // To make the signature verification fail, uncomment the next line:
  //  signature[0] = 0x00;

  // validate the signature
  if (ECCX08.ecdsaVerify(input, signature, publicKey)) {
    Serial.println("Verified signature successfully :D");
  } else {
    Serial.println("Oh no! Failed to verify signature :(");
  }
}

void loop() {
  // do nothing
}

void printBufferHex(const byte input[], int inputLength) {
  for (int i = 0; i < inputLength; i++) {
    Serial.print(input[i] >> 4, HEX);
    Serial.print(input[i] & 0x0f, HEX);
  }
  Serial.println();
}
