#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// nRF24L01 setup
RF24 radio(9, 8); // CE, CSN
const byte address[6] = "00001";

// Relays (low trigger)
const int redRelay = 3;
const int yellowRelay = 4;
const int greenRelay = 5;


char command[10];
unsigned long delayTime = 10000;

void setup() {
  Serial.begin(9600);
  pinMode(redRelay, OUTPUT);
  pinMode(yellowRelay, OUTPUT);
  pinMode(greenRelay, OUTPUT);
  allRelaysOff();

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&command, sizeof(command));
    command[sizeof(command) - 1] = '\0';

    Serial.print("Received: "); Serial.println(command);

    if (strcmp(command, "STOP") == 0) {
      allRelaysOff();
    } else if (strcmp(command, "RESUM") == 0) {
      // Do nothing, resume normal
    } else if (strcmp(command, "CROSS") == 0) {
      waitForDelay();
      digitalWrite(redRelay, LOW);
      delay(delayTime);
      digitalWrite(redRelay, HIGH);
    } else if (strcmp(command, "RED") == 0) {
      waitForDelay();
      digitalWrite(redRelay, LOW);
      delay(delayTime);
      digitalWrite(redRelay, HIGH);
    } else if (strcmp(command, "GREEN") == 0) {
      waitForDelay();
      digitalWrite(greenRelay, LOW);
      delay(delayTime);
      digitalWrite(greenRelay, HIGH);
    } else if (strcmp(command, "YELLOW") == 0) {
      waitForDelay();
      digitalWrite(yellowRelay, LOW);
      delay(delayTime);
      digitalWrite(yellowRelay, HIGH);
    }
  }
}

void waitForDelay() {
  while (!radio.available()); // wait for delayTime
  radio.read(&delayTime, sizeof(delayTime));
  Serial.print("Delay set to: "); Serial.println(delayTime);
}

void allRelaysOff() {
  digitalWrite(redRelay, HIGH);
  digitalWrite(yellowRelay, HIGH);
  digitalWrite(greenRelay, HIGH);
}
