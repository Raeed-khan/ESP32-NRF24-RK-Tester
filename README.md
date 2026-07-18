# ESP32 & NRF24L01 Long-Range RF Spectrum Auditor

## Developed By
* **Lead Architect:** Raeed Khan (BBA Student University of Buner)

---

## Project Overview & Purpose
This repository contains a high-efficiency 2.4GHz RF diagnostic and security testing tool built using an **ESP32** and the **NRF24L01+** transceiver module. 

### Project Objectives
Created for **Academic Research and Cybersecurity Auditing**, this project aims to:
1. **Analyze Wireless Resilience:** Test the physical layer (Layer-1) resistance of localized 2.4GHz infrastructure against persistent carrier-wave interference.
2. **Demonstrate Range Optimization:** Implement specific data-rate and hardware tuning to maximize the effective range and signal penetration of the NRF24L01+ module.
3. **FHSS Visualisation:** Demonstrate how dynamic frequency hopping sweeps affect spectrum stability.

---

## Long-Range Optimization Mechanics
Unlike standard implementations that use a 2MBPS data rate (which sacrifices range for speed), this firmware is optimized for **maximum signal distance and coverage**:
- **Data Rate Tuning:** Lowered to `1MBPS` to increase receiver sensitivity, allowing the signal to travel further and penetrate obstacles better.
- **Power Stacking:** Completely deinitializes the internal ESP32 Wi-Fi and Bluetooth stacks to ensure maximum stable current is routed directly to the NRF24L01+ module via the hardware SPI bus.
- **Frequency Stabilization:** Enhanced microsecond delays allow the transceiver loop to lock cleanly into the constant carrier phase on each hopped channel.

---

## Hardware Configuration (VSPI Pinout)

For optimal long-range execution, connect your ESP32 to the NRF24L01+ transceiver using the hardware **VSPI** layout:

| NRF24L01 Module Pin | ESP32 GPIO Pin | Description |
|---------------------|----------------|-------------|
| **VCC**             | 3.3V           | Power Input (Needs solid current) |
| **GND**             | GND            | Common Ground |
| **CE**              | GPIO 22        | Chip Enable |
| **CSN**             | GPIO 21        | SPI Chip Select |
| **SCK**             | GPIO 18        | SPI Clock |
| **MISO**            | GPIO 19        | SPI Master In Slave Out |
| **MOSI**            | GPIO 23        | SPI Master Out Slave In |

*A physical Toggle or Push Switch must be connected to **GPIO 33** to dynamically control the execution modes.*

> ⚠️ **Hardware Range Note:** To achieve true long-range performance, it is highly recommended to use the **NRF24L01+ PA+LNA** version (with an external antenna) along with a dedicated adapter base board or a $10\mu\text{F}$–$100\mu\text{F}$ decoupling capacitor soldered across the module's VCC and GND pins to prevent voltage drops.

---

## Dual Execution Modes

### 1. Mode ONE (Switch State: LOW / Pressed)
* **Behavior:** Rapidly targets channels 0 through 14 with optimized sweep delays.
* **Use Case:** Emulates dense physical layer interference across standard lower 2.4GHz industrial bands.

### 2. Mode TWO (Switch State: HIGH / Released)
* **Behavior:** Continuous upward and downward frequency sweep across channels 2 to 79.
* **Use Case:** Simulates a long-range frequency-hopping network environment to audit structural shielding and signal loss.

---

## How to Install and Run

1. Connect the hardware components according to the **VSPI Pinout** guide.
2. Clone this repository and open `ESP32-NRF24-RK-Tester.ino` using the **Arduino IDE**.
3. Install the required libraries via the Library Manager:
   - `RF24` by TMRh20
   - `ezButton` by ArduinoGetStarted
4. Select your target **ESP32 Dev Module** from the Boards menu and hit upload.
5. Monitor real-time status shifts via the Serial Monitor at **115200 Baud**.

---

## Educational Disclaimer
This utility is strictly intended for legal, authorized penetration testing, academic spectrum analysis, and defensive cybersecurity research. Deploying intentional RF interference against unauthorized or public infrastructure is illegal in many regions under international wireless telecommunication regulations. The developer, Raeed Khan, assumes no responsibility or legal liability for any misuse of this open-source code.
