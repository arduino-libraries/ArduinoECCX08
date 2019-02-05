/*
  ArduinoECCX08 - Self Signed Cert

  This sketch can be used to generate a self signed certificate
  for a private key generated in an ECC508/ECC608 crypto chip slot.
  The issue and expired date, and signature are stored in another
  slot for reconstrution.

  If the ECC508/ECC608 is not configured and locked it prompts
  the user to configure and lock the chip with a default TLS
  configuration.

  The user can also select the slot number to use for the private key
  and storage.
  A new private key can also be generated in this slot.

  The circuit:
  - Arduino MKR board equipped with ECC508 or ECC608 chip

  This example code is in the public domain.
*/

#include <ArduinoECCX08.h>
#include <utility/ECCX08SelfSignedCert.h>
#include <utility/ECCX08DefaultTLSConfig.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

  String serialNumber = ECCX08.serialNumber();

  Serial.print("ECCX08 Serial Number = ");
  Serial.println(serialNumber);
  Serial.println();

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

  Serial.println("Hi there, in order to generate a new self signed cert for your board, we'll need the following information ...");
  Serial.println();

  String issueYear          = promptAndReadLine("Please enter the issue year of the certificate? (2000 - 2031)", "2019");
  String issueMonth         = promptAndReadLine("Please enter the issue month of the certificate? (1 - 12)", "1");
  String issueDay           = promptAndReadLine("Please enter the issue day of the certificate? (1 - 31)", "1");
  String issueHour          = promptAndReadLine("Please enter the issue hour of the certificate? (0 - 23)", "0");
  String expireYears        = promptAndReadLine("Please enter how many years the certificate is valid for? (1 - 31)", "31");
  String privateKeySlot     = promptAndReadLine("What slot would you like to use for the private key? (0 - 4)", "0");
  String storageSlot        = promptAndReadLine("What slot would you like to use for storage? (8 - 15)", "8");
  String generateNewKey     = promptAndReadLine("Would you like to generate a new private key? (Y/n)", "Y");

  Serial.println();

  generateNewKey.toLowerCase();

  if (!ECCX08SelfSignedCert.beginStorage(privateKeySlot.toInt(), storageSlot.toInt(), generateNewKey.startsWith("y"))) {
    Serial.println("Error starting self signed cert generation!");
    while (1);
  }

  ECCX08SelfSignedCert.setCommonName(ECCX08.serialNumber());
  ECCX08SelfSignedCert.setIssueYear(issueYear.toInt());
  ECCX08SelfSignedCert.setIssueMonth(issueMonth.toInt());
  ECCX08SelfSignedCert.setIssueDay(issueDay.toInt());
  ECCX08SelfSignedCert.setIssueHour(issueHour.toInt());
  ECCX08SelfSignedCert.setExpireYears(expireYears.toInt());

  String cert = ECCX08SelfSignedCert.endStorage();

  if (!cert) {
    Serial.println("Error generating self signed cert!");
    while (1);
  }

  Serial.println("Here's your self signed cert, enjoy!");
  Serial.println();
  Serial.println(cert);
  Serial.println();

  Serial.print("SHA1: ");
  Serial.println(ECCX08SelfSignedCert.sha1());
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
