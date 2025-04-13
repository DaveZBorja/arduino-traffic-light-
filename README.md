# 🚦 Traffic Light Pedestrian Crossing System with NRF24L01 & IR Remote

This Arduino-based project simulates a traffic light system integrated with a pedestrian crossing mechanism. It includes wireless synchronization via **NRF24L01**, adjustable timing using an **IR remote**, and control through physical **push buttons**. Designed for educational, prototype, or automation projects.

## 📦 Features

- ✅ Fully synchronized transmitter & receiver using NRF24L01
- 🕹️ Manual control with physical buttons (CROSS, STOP, RESET)
- 📡 Remote delay adjustment via IR remote control
- 🔄 EEPROM storage for delay persistence
- 🔧 Active-LOW relay control for Red, Yellow, Green traffic lights
- 🛑 Emergency STOP and Resume functionality

---

## 🔌 Hardware Requirements

- 2x **Arduino Uno/Nano**
- 2x **NRF24L01 Modules**
- 1x **IR Receiver Module (e.g. VS1838B)**
- 1x **IR Remote (NEC protocol)**
- 3x **Relays (5V)**
- 3x **LEDs or actual signal lights** (Red, Yellow, Green)
- 3x **Push Buttons** (Cross, Stop, Reset)
- **10µF Capacitor** (recommended across NRF24L01 VCC & GND)

---

## 📍 Pin Configuration

### Transmitter Arduino

| Pin | Connected To         |
|-----|----------------------|
| D2  | IR Receiver Signal   |
| D3  | Stop Button          |
| D4  | Cross Button         |
| D5  | Red Relay            |
| D6  | Yellow Relay         |
| D7  | Green Relay          |
| D8  | NRF24L01 CSN         |
| D9  | NRF24L01 CE          |
| D10 | Reset Stop Button    |

---

## 🎮 IR Remote Delay Control

| Remote Button | Sets Delay Time  |
|---------------|------------------|
| `1`           | 0.5 seconds      |
| `2`           | 10 seconds       |
| `3`           | 15 seconds       |
| `4`           | 30 seconds       |
| `5`           | 35 seconds       |
| `0`           | 60 seconds       |

EEPROM saves this delay automatically.

---

## 🚶 Functional Behavior

- **Green Light ON by default** for traffic flow.
- **CROSS Button pressed**:
  - Green → Yellow (2s) → Red for set time.
  - After delay, system resumes to Green.
- **STOP Button pressed**:
  - All lights OFF.
  - System paused. "STOP" signal sent to receiver.
- **RESET Button pressed**:
  - Resumes normal operation.
  - Sends "RESUM" to receiver.

---

## 🔁 Wireless Commands Sent

Transmitter sends the following strings and data via NRF24L01:
- `"RED"` / `"YELLOW"` / `"GREEN"`
- `"CROSS"` – pedestrian request
- `"STOP"` – emergency stop
- `"RESUM"` – resume system
- `delayTime` – custom pedestrian delay

---

## 🧠 EEPROM Behavior

- Stores the last chosen `delayTime` for the Red phase.
- Automatically loaded on boot.
- Prevents reset after power failure.

---

## 🛠️ Setup & Usage

1. Upload transmitter and receiver code to respective Arduino boards.
2. Wire relays, buttons, IR module, and NRF modules as specified.
3. Power on both systems.
4. Use the **IR remote** to set crossing delay.
5. Press **Cross** to simulate a pedestrian crossing.
6. Press **Stop** to pause; **Reset** to resume.

---

