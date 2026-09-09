# 🏎️ Smart 4WD Car Controller

[![STM32](https://img.shields.io/badge/MCU-STM32F401CCU6-blue.svg?logo=stmicroelectronics)](https://www.st.com/)
[![ESP32](https://img.shields.io/badge/Gateway-ESP32%20Wi--Fi-red.svg?logo=espressif)](https://www.espressif.com/)
[![Flutter](https://img.shields.io/badge/App-Flutter%203.x-02569B.svg?logo=flutter)](https://flutter.dev/)
[![Language](https://img.shields.io/badge/Firmware-Embedded%20C%20%2F%20C%2B%2B-brightgreen.svg)]()
[![License](https://img.shields.io/badge/License-MIT-green.svg)]()

An end-to-end intelligent 4WD robotic vehicle platform featuring a high-performance **STM32F401 (ARM Cortex-M4)** real-time controller, an **ESP32 Wi-Fi Gateway**, and a futuristic **Flutter Mobile Cockpit Application**.

---

## 📑 Table of Contents
- [Architecture Overview](#-architecture-overview)
- [Hardware & Pinout Mapping](#-hardware--pinout-mapping)
- [Key Features](#-key-features)
- [Subsystems](#-subsystems)
  - [1. STM32 Firmware (`ARM/`)](#1-stm32-firmware-arm)
  - [2. ESP32 Wi-Fi Gateway (`esp_code/`)](#2-esp32-wi-fi-gateway-esp_code)
  - [3. Flutter Mobile Cockpit (`smart_car_controller/`)](#3-flutter-mobile-cockpit-smart_car_controller)
- [Communication Protocols](#-communication-protocols)
  - [REST API (Mobile ↔ ESP32)](#rest-api-mobile--esp32)
  - [UART Protocol (ESP32 ↔ STM32)](#uart-protocol-esp32--stm32)
- [Getting Started & Flashing](#-getting-started--flashing)
  - [STM32 Build & Flash](#stm32-build--flash)
  - [ESP32 Setup](#esp32-setup)
  - [Flutter Mobile App](#flutter-mobile-app)

---

## 🏛️ Architecture Overview

```
 ┌──────────────────────────────────────────────────────────┐
 │               Flutter Mobile Cockpit                     │
 │          Tactical HUD / Radar / D-Pad                    │
 └────────────────────────────┬─────────────────────────────┘
                              │ HTTP REST / Wi-Fi (SoftAP)
                              ▼
 ┌──────────────────────────────────────────────────────────┐
 │                 ESP32 Gateway                            │
 │          SSID: SmartCar-ESP32 (192.168.4.1)              │
 └────────────────────────────┬─────────────────────────────┘
                              │ Full-Duplex UART (115200 Baud)
                              │ TX: GPIO17 -> PA3 | RX: GPIO16 <- PA2
                              ▼
 ┌──────────────────────────────────────────────────────────┐
 │            STM32F401CCU6 (BlackPill)                     │
 │      Real-Time Core / Motors / Sensors / Lights          │
 └───────┬─────────────┬─────────────┬─────────────┬────────┘
         │             │             │             │
         ▼             ▼             ▼             ▼
   L298N H-Bridge  HC-SR04     Headlight &    Buzzer /
   4WD DC Motors   Ultrasonic  Reverse LEDs   Horn (PB5)
```

---

## 🔌 Hardware & Pinout Mapping

### STM32F401CCU6 (BlackPill) Pin Assignment

| Peripheral | STM32 Pin | Direction | Description | Connected To |
| :--- | :--- | :--- | :--- | :--- |
| **Ultrasonic TRIG** | `PA0` | Output | HC-SR04 Trigger Pulse | Ultrasonic Trigger |
| **Ultrasonic ECHO** | `PA1` | Input | HC-SR04 Echo Timing | Ultrasonic Echo |
| **UART2 TX** | `PA2` | Output (AF7) | Telemetry & ACKs (115200 bps) | ESP32 `GPIO16` (RX2) |
| **UART2 RX** | `PA3` | Input (AF7) | Command Reception | ESP32 `GPIO17` (TX2) |
| **Headlight (كشاف)** | `PA4` | Output | Main High-Beam Front Lamp | Headlight LED / Transistor |
| **Reverse Lamp (لمبة حمراء)** | `PA5` | Output | Auto Red Reverse Indicator | Red LED (turns ON when backward) |
| **PWM Left Motors** | `PA6` | Output (AF2) | TIM3 Channel 1 PWM | L298N `ENA` |
| **PWM Right Motors** | `PA7` | Output (AF2) | TIM3 Channel 2 PWM | L298N `ENB` |
| **Left Motor IN1** | `PB0` | Output | Direction Pin L1 | L298N `IN1` |
| **Left Motor IN2** | `PB1` | Output | Direction Pin L2 | L298N `IN2` |
| **Right Motor IN3** | `PB2` | Output | Direction Pin R1 | L298N `IN3` |
| **Right Motor IN4** | `PB3` | Output | Direction Pin R2 | L298N `IN4` |
| **Buzzer / Horn (بازر)** | `PB5` | Output | Acoustic Warning Horn | Piezo / Active Buzzer |
| **Heartbeat LED** | `PC13` | Output | Onboard Blue LED | Blinks every 300ms on active loop |
| **Common Ground** | `GND` | Ground | Shared Reference (Crucial) | ESP32 `GND` & L298N `GND` |

---

## ⚡ Key Features

- 🏎️ **Precision 4WD Dual-H-Bridge Control:** Full torque 100% PWM with differential tank steering (spin-in-place).
- 🛑 **Hold-to-Drive Safety Mechanism:** Periodic 150ms command heartbeat with instant brake release (stops motors within < 20ms of lifting finger).
- 📡 **Real-time Ultrasonic Radar Telemetry:** Non-blocking distance calculation feeding continuous HUD updates.
- 💡 **Automated Reverse Tail Light:** The red tail lamp lights up automatically when reversing and extinguishes immediately upon stopping.
- 🔦 **Cockpit-Controlled Headlight:** High-beam front lamp toggled from the mobile app.
- 📢 **Hold-to-Honk Horn:** Acoustic buzzer support with hold-to-honk and single-tap beep modes.
- 💓 **Heartbeat Diagnostics:** Onboard `PC13` LED pulses rhythmically to provide clear visual confirmation of firmware execution.

---

## 📦 Subsystems

### 1. STM32 Firmware (`ARM/`)
- Written in clean bare-metal C / CMSIS for the STM32F401CCU6 (Cortex-M4).
- **Core modules:**
  - `smart_car_controller.c`: Main application state machine, telemetry loop, and command dispatcher.
  - `USART2_IRQHandler`: Ring-buffer interrupt service routine for zero-drop UART parsing.
  - `TIM3`: Hardware PWM generation at 16 MHz.
  - `MSYSTICK`: Microsecond & millisecond calibrated timebase.

### 2. ESP32 Wi-Fi Gateway (`esp_code/`)
- Standalone Arduino sketch transforming the ESP32 into a soft Access Point (`SmartCar-ESP32`).
- Embedded non-blocking HTTP REST server listening on port 80.
- Handles `/api/command`, `/api/state`, and `/api/health`.
- Full-duplex HardwareSerial UART2 bridge (`Serial2`) running at 115200 baud.

### 3. Flutter Mobile Cockpit (`smart_car_controller/`)
- Cyberpunk dark obsidian aesthetic tailored for real-time robotic telemetry.
- **Components:**
  - **Dynamic Radar HUD:** Semicircular sweep radar with color-coded safety zones (Green: Safe, Amber: Caution, Crimson: Danger < 18cm).
  - **Tactile D-Pad:** Forward, Reverse, Spin-Left, Spin-Right, and Emergency Brake with touch callbacks.
  - **Action Toolbar:** Headlight toggle button with neon beam glow and hold-to-honk acoustic button.

---

## 📡 Communication Protocols

### REST API (Mobile ↔ ESP32)

#### `GET /api/state`
Returns the latest vehicle telemetry.
```json
{
  "distance": 42,
  "mode": "AUTO_SCAN"
}
```

#### `POST /api/command`
Dispatches vehicle actuation commands.
- **Drive:** `{"type": "move", "direction": "forward" | "backward" | "left" | "right" | "stop"}`
- **Headlight:** `{"type": "light", "state": true | false}`
- **Horn:** `{"type": "horn", "state": true | false}`
- **Emergency Stop:** `{"type": "stop"}`

---

### UART Protocol (ESP32 ↔ STM32)

Carried over **USART2** at 115200 baud, 8N1:

| Command Sent to STM32 | Action | STM32 Response |
| :--- | :--- | :--- |
| `CMD MOVE FORWARD` | Drive forward at 100% | `ACK MOVE` |
| `CMD MOVE BACKWARD` | Reverse + Turn ON Red LED | `ACK MOVE` |
| `CMD MOVE LEFT` | Spin in-place counter-clockwise | `ACK MOVE` |
| `CMD MOVE RIGHT` | Spin in-place clockwise | `ACK MOVE` |
| `CMD STOP` | Cut power to all motors | `ACK STOP` |
| `CMD LIGHT ON` | Turn ON Headlight (`PA4` HIGH) | `ACK LIGHT` |
| `CMD LIGHT OFF` | Turn OFF Headlight (`PA4` LOW) | `ACK LIGHT` |
| `CMD HORN ON` | Turn ON Buzzer (`PB5` HIGH) | `ACK HORN` |
| `CMD HORN OFF` | Turn OFF Buzzer (`PB5` LOW) | `ACK HORN` |

**Telemetry Sent from STM32:**
```text
TEL LEFT <dist> RIGHT <dist> DIST <dist> MODE AUTO_SCAN
```

---

## 🚀 Getting Started & Flashing

### STM32 Build & Flash

1. **Build using GNU Arm Toolchain:**
   ```powershell
   cd d:\ARM_ITI\workspace\ARM\Debug
   make all
   ```
2. **Flash using ST-LINK CLI:**
   ```powershell
   & "C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -c -P "d:\ARM_ITI\workspace\ARM\Debug\ARM.hex" -V -Rst
   ```

### ESP32 Setup

1. Open `esp_code/esp_code.ino` in Arduino IDE.
2. Select **ESP32 Dev Module**.
3. Upload to your board.
4. Verify by checking Wi-Fi networks for `SmartCar-ESP32` (Password: `smartcar123`).

### Flutter Mobile App

1. Ensure Flutter SDK is installed and configured:
   ```bash
   cd smart_car_controller
   flutter pub get
   ```
2. Connect your Android or iOS device over USB (with Wi-Fi connected to `SmartCar-ESP32`).
3. Run in debug mode:
   ```bash
   flutter run
   ```

---

## 👨‍💻 Authors & Acknowledgments

- **Abdelrhman AJ.** - Embedded Systems & Mobile Application Engineering.
- Developed as part of the **ARM ITI Track**.
