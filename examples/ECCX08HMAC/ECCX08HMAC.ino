/*
    ECCX08 HMAC functionality example

    This sketch uses the ECC608 to generate an hmac on some data.
    Stores key using nonce.

    Tested on the Arduino Nano RP2040.

    created 10 October 2022
    by Raul Leclair
*/

#include <ArduinoECCX08.h>

#define TEMPKEY_SLOT 0xFFFF

byte nonceKey[] = {
    0x10, 0x10, 0x10, 0x10
};

byte data[] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};

int dataLength = sizeof(data);

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!ECCX08.begin()) {
        Serial.println("Failed to initialize ECCX08 board.");
        while (1);
    }

    // Perform nonce
    if (!ECCX08.nonce(nonceKey))
    {
        Serial.println("Failed to perform nonce.");
        while (1);
    }

    // Starting HMAC operation on tempkey slot
    if (!ECCX08.beginHMAC(TEMPKEY_SLOT)) {
        Serial.println("Failed to start HMAC operation.");
        while (1);
    }

    if (!ECCX08.updateHMAC(data, dataLength)) {
        Serial.println("Failed to update HMAC operation.");
        while (1);
    }

    byte resultHMAC[32];
    if (!ECCX08.endHMAC(resultHMAC)) {
        Serial.println("Failed to end HMAC operation");
        while (1);
    }

    Serial.println("HMAC Result: ");
    for (int i = 0; i < sizeof(resultHMAC); i++) {
        char hexChar[2];
        sprintf(hexChar, "%02X", resultHMAC[i]);
        Serial.print(hexChar);
        Serial.print(" ");
    }
}

void loop() {
}
