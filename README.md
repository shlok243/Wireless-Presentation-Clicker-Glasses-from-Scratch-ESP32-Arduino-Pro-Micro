# Wireless PPT Clicker using ESP32 + Arduino Pro Micro (Bluetooth)

Control your PowerPoint presentations wirelessly using a touch-sensitive ESP32 as the transmitter and an Arduino Pro Micro as the receiver — which acts as a real USB HID keyboard to your PC.

No drivers. No apps. Just plug in the Pro Micro and touch the ESP32.

---
Youtube Video Link  - https://youtu.be/n3SxURrbLq0?si=4geLxFM7iLBF9cKD

## How It Works

```
[You touch ESP32] → [ESP32 sends command over Bluetooth] → [HC-05 receives it via Serial]
                                                                      |
                                                        [Pro Micro reads Serial data]
                                                                      |
                                                  [Pro Micro sends keypress to PC]
```

The ESP32 uses its built-in capacitive touch sensor — no external touch module needed. The GPIO pin itself acts as the sensor. When touched, it sends a command over Bluetooth Classic to an HC-05 module. The Pro Micro reads that command via Serial and sends the appropriate keyboard shortcut to control the presentation.

---

## Hardware Required

- ESP32 Dev Board (any standard 38-pin) — 1
- Arduino Pro Micro (ATmega32U4) — 1
- HC-05 Bluetooth Module — 1
- LED + 220 ohm resistor (optional, for status indicator)
- Jumper wires
- 2x USB cables

Note: Why Pro Micro and not Uno/Nano? The Pro Micro uses the ATmega32U4 which has native USB HID support. This lets it act as a real keyboard using the Keyboard.h library. Uno and Nano do NOT support this.

---

## Pin Connections

### ESP32 (Transmitter)

- GPIO 14 (T4) → Bare wire or conductive pad (Capacitive Touch Input)
- GPIO 2 → LED + resistor → GND (Status LED, lights up on touch)
- 3.3V and GND → Power

Why GPIO 14?
The ESP32 has 10 built-in capacitive touch-sensing pins (T0 to T9). These pins can detect human touch without any external sensor — just a wire or conductive pad connected to the pin is enough. GPIO 14 maps to Touch channel T4, which is conveniently located and works reliably. The touchRead() function reads a capacitance value; when you touch the wire, the value drops significantly (typically below 30).

All capacitive touch pins on ESP32 for reference:

- T0 → GPIO 4
- T1 → GPIO 0
- T2 → GPIO 2
- T3 → GPIO 15
- T4 → GPIO 14  (used in this project)
- T5 → GPIO 12
- T6 → GPIO 13
- T7 → GPIO 27
- T8 → GPIO 33
- T9 → GPIO 32

---

### HC-05 to Arduino Pro Micro (Receiver)

- HC-05 VCC → Pro Micro 5V
- HC-05 GND → Pro Micro GND
- HC-05 TXD → Pro Micro RX / Pin 0 (Serial1)
- HC-05 RXD → Pro Micro TX / Pin 1 (Serial1)

The Pro Micro uses Serial1 (pins 0 and 1) for hardware UART communication with HC-05, and Serial (USB) for debug output to Serial Monitor.

---

## HC-05 Configuration (AT Commands)

Before using, you need to configure the HC-05 as a slave so the ESP32 master can connect to it.

1. Hold the button on HC-05 while powering it on — it enters AT mode (slow blink)
2. Connect HC-05 TX/RX to a USB-Serial adapter, or use the Pro Micro with the AT command sketch (the commented-out section at the top of receiver.ino)
3. Open Serial Monitor at 38400 baud and send these commands:

```
AT                       (check connection, should reply OK)
AT+NAME=ESP_SLAVE        (set HC-05 name)
AT+ROLE=0                (set as Slave — 0 = slave, 1 = master)
AT+PSWD=1234             (set pairing PIN)
AT+UART=38400,0,0        (set baud rate to 38400)
```

The ESP32 is set as Bluetooth master (SerialBT.begin("ESP_MASTER", true)), so it initiates the connection. The HC-05 must be in slave mode.

---

## Software Setup

### ESP32 (Transmitter)

1. Install Arduino IDE from https://www.arduino.cc/en/software
2. Add ESP32 board support — go to File > Preferences > Additional Board URLs and add:
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
3. Install the board via Tools > Board Manager, search "esp32"
4. Select board: ESP32 Dev Module
5. BluetoothSerial library comes bundled with the ESP32 package, no separate install needed
6. Upload transmitter.ino

### Arduino Pro Micro (Receiver)

1. Select board: Arduino Leonardo (same chip, works perfectly)
2. Keyboard.h is built into Arduino IDE, no extra install needed
3. Upload receiver.ino

---

## Calibrating the Touch Threshold

The ESP32's touchRead() returns a value that varies by environment and wire length. Open Serial Monitor at 9600 baud on the ESP32 after uploading and observe the printed values:

- Not touched: typically 40 to 80
- Touched: typically 5 to 20

In transmitter.ino, adjust this line:

```cpp
int touchThreshold = 30;  // Lower = less sensitive, Higher = more sensitive
```

Set it to a value between your "touched" and "not touched" readings.

---
## Project Structure

```
wireless-ppt-clicker/
├── transmitter/
│   └── transmitter.ino        (ESP32 code)
├── receiver/
│   └── receiver.ino           (Arduino Pro Micro code)
└── README.md
```
---
## Supported Commands

Commands are sent as strings from ESP32 over Bluetooth and received by the Pro Micro via HC-05:

- "NEXT" → Right Arrow key (next slide)
- "PREVIOUS" → Left Arrow key (previous slide)
- "FULLSCREEN" → Shift + F5 to enter fullscreen, ESC to exit (toggles)
- "NUL" → Release all keys
- "HIGH" / "LOW" → Currently mapped to LED on/off on ESP32 side (can be remapped to slide commands)

You can easily add more touch pins on the ESP32 (it has 10 total) and map them to more commands. For example, a second touch wire on GPIO 13 (T6) for previous slide.

---
## Possible Upgrades

- Add a second touch pin for Previous Slide control
- Use a small LiPo battery + TP4056 charger for a fully wireless transmitter
- 3D print an enclosure for the ESP32 with conductive finger pads
- Add haptic feedback (small vibration motor) on touch detection
- Expand to control media playback: volume, mute, etc.

---
## Author

Built by Shlok Chaudhari as a hardware project for wireless presentation control using ESP32's native capacitive touch capability.
