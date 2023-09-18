/*
  ECCX08 AES-GCM

  This sketch uses the ECC608 to compute
  the AES_128_GCM encryption for some data

  This sketch assumes TempKey has been set
  (ECDH sketch will do this)
  and derives a deviceID from the public key
  corresponding to the key in slot 0.

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

  byte ad[20] = {0x14};
  uint64_t adLength = (sizeof(ad));
  Serial.print("AD:  ");
  printHex(ad, adLength);
  byte pt[40] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28};
  uint64_t ptLength = (sizeof(pt));
  Serial.print("PT:  ");
  printHex(pt, ptLength);

  byte IV[12];
  byte ct[40];
  byte tag[16];
  if (!ECCX08.AESEncrypt(IV, ad, pt, ct, tag, adLength, ptLength)){
    Serial.println("Failed to encrypt.");
  } else {
    Serial.print("IV:  ");
    printHex(IV, 12);
    Serial.print("CT:  ");
    printHex(ct, ptLength);
    Serial.print("tag: ");
    printHex(tag, 16);
  }

  byte ptDecrypted[40];
  if (!ECCX08.AESDecrypt(IV, ad, ptDecrypted, ct, tag, adLength, ptLength)){
    Serial.println("Failed to decrypt.");
  } else {
    Serial.print("PT:  ");
    printHex(ptDecrypted, ptLength);
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
