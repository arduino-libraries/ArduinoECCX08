/*
  ArduinoECCX08 - JWS Public Key

  This sketch can be used to generate a PEM public key for a private key
  generated in an ECC508/ECC608 crypto chip slot.

  If the ECC508/ECC608 is not configured and locked it prompts
  the user to configure and lock the chip with a default TLS
  configuration.

  The user can also select a slot number to use for the private key
  A new private key can also be generated in this slot.

  The circuit:
  - Arduino MKR board equipped with ECC508 or ECC608 chip

  This example code is in the public domain.
*/

#include <ArduinoECCX08.h>
#include <utility/ECCX08JWS.h>
#include <utility/ECCX08DefaultTLSConfig.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

  if (!ECCX08.locked()) {
    String lock = promptAndReadLine("The ECCX08 on your board is not locked, would you like to PERMANENTLY configure and lock it now? (y/N)", "N");
    lock.toLowerCase();

    if (!lock.startsWith("y")) {
      Serial.println("Unfortunately you can't proceed without locking it :(");
      while (1);
    }

    if (!ECCX08.writeConfiguration(ECCX08_DEFAULT_TLS_CONFIG)) {
      Serial.println("Writing ECCX08 configuration failed!");
      while (1);
    }

    if (!ECCX08.lock()) {
      Serial.println("Locking ECCX08 configuration failed!");
      while (1);
    }

    Serial.println("ECCX08 locked successfully");
    Serial.println();
  }

  Serial.println("Hi there, in order to generate a PEM public key for your board, we'll need the following information ...");
  Serial.println();

  String slot               = promptAndReadLine("What slot would you like to use? (0 - 4)", "0");
  String generateNewKey     = promptAndReadLine("Would you like to generate a new private key? (Y/n)", "Y");

  Serial.println();

  generateNewKey.toLowerCase();

  String publicKeyPem = ECCX08JWS.publicKey(slot.toInt(), generateNewKey.startsWith("y"));

  if (!publicKeyPem || publicKeyPem == "") {
    Serial.println("Error generating public key!");
    while (1);
  }

  Serial.println("Here's your public key PEM, enjoy!");
  Serial.println();
  Serial.println(publicKeyPem);
}

void loop() {
  // do nothing
}

String promptAndReadLine(const char* prompt, const char* defaultValue) {
  Serial.print(prompt);
  Serial.print(" [");
  Serial.print(defaultValue);
  Serial.print("]: ");

  String s = readLine();

  if (s.length() == 0) {
    s = defaultValue;
  }

  Serial.println(s);

  return s;
}

String readLine() {
  String line;

  while (1) {
    if (Serial.available()) {
      char c = Serial.read();

      if (c == '\r') {
        // ignore
        continue;
      } else if (c == '\n') {
        break;
      }

      line += c;
    }
  }

  return line;
}
