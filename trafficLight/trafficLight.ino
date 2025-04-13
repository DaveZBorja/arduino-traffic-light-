#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>
#include <IRremote.h>

RF24 radio(9, 8);  // CE, CSN
const byte address[6] = "00001";

// Relays (LOW trigger)
const int redRelay = 5;
const int yellowRelay = 6;
const int greenRelay = 7;

// Buttons
const int buttonCross = 4;
const int buttonStop = 3;
const int buttonResetStop = 10;

// IR
const int IR_PIN = 2;

unsigned long delayTime = 10000;
bool stopAll = false;
bool isCrossing = false;

void setup() {
  Serial.begin(9600);

  pinMode(redRelay, OUTPUT);
  pinMode(yellowRelay, OUTPUT);
  pinMode(greenRelay, OUTPUT);
  allRelaysOff();

  pinMode(buttonCross, INPUT_PULLUP);
  pinMode(buttonStop, INPUT_PULLUP);
  pinMode(buttonResetStop, INPUT_PULLUP);

  // IR
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR Receiver Initialized");

  // NRF24
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  EEPROM.get(0, delayTime);
  if (delayTime < 1000 || delayTime > 60000) delayTime = 10000;
  Serial.print("Loaded delay: "); Serial.println(delayTime);
}

void loop() {
  handleIRRemote();

  if (digitalRead(buttonStop) == LOW) {
    stopAll = true;
    sendCommand("STOP");
    allRelaysOff();
    delay(300);
    return;
  }

  if (digitalRead(buttonResetStop) == LOW) {
    stopAll = false;
    sendCommand("RESUM");
    allRelaysOff();
    delay(300);
  }

  if (stopAll) return;

  if (digitalRead(buttonCross) == LOW && !isCrossing) {
    isCrossing = true;
    startCrossing();
    isCrossing = false;
  } else {
    // Default state: only green ON
    digitalWrite(redRelay, HIGH);
    digitalWrite(yellowRelay, HIGH);
    digitalWrite(greenRelay, LOW);
    sendCommand("GREEN");
    delay(500);
  }
}

void startCrossing() {
  allRelaysOff();

  // Yellow light phase
  digitalWrite(yellowRelay, LOW);
  sendCommand("YELLOW");
  radio.write(&delayTime, sizeof(delayTime));
  delayWithCheck(2000);  // Yellow for 2 seconds
  digitalWrite(yellowRelay, HIGH);

  // Red light phase
  digitalWrite(redRelay, LOW);
  sendCommand("RED");
  radio.write(&delayTime, sizeof(delayTime));
  delayWithCheck(delayTime);
  digitalWrite(redRelay, HIGH);
}

void sendCommand(const char* cmd) {
  radio.write(cmd, strlen(cmd));
  Serial.print("Sent: "); Serial.println(cmd);
}

void allRelaysOff() {
  digitalWrite(redRelay, HIGH);
  digitalWrite(yellowRelay, HIGH);
  digitalWrite(greenRelay, HIGH);
}

void delayWithCheck(unsigned long duration) {
  unsigned long start = millis();
  while (millis() - start < duration) {
    if (digitalRead(buttonStop) == LOW) {
      stopAll = true;
      sendCommand("STOP");
      allRelaysOff();
      return;
    }
    handleIRRemote();
    delay(10);
  }
}

void handleIRRemote() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.command;
    Serial.print("IR Code: "); Serial.println(code, HEX);

    if (code == 0x45) delayTime = 500;
    else if (code == 0x46) delayTime = 10000;
    else if (code == 0x47) delayTime = 15000;
    else if (code == 0x44) delayTime = 30000;
    else if (code == 0x40) delayTime = 35000;
    else if (code == 0x19) delayTime = 60000;

    EEPROM.put(0, delayTime);
    Serial.print("Saved delay: "); Serial.println(delayTime);
    IrReceiver.resume();
  }
}
