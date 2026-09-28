# ESP32 Now Playing Display & Ambient LED

A smart, physical "Now Playing" display and ambient lighting system built with an ESP32. This project connects to your Wi-Fi and provides a physical readout of the music currently playing on your PC (specifically from the Brave browser, though it can be adapted), alongside a breathing LED effect that reacts to the playback state.

## 🎯 Motive

The goal of this project was to create a dedicated, ambient desktop companion for music listening. Instead of having to switch windows or check the taskbar to see what song is currently playing, this project offloads that information to a small, aesthetically pleasing OLED display. Furthermore, the breathing LED adds a layer of ambiance to the room, instantly indicating whether music is actively playing or paused.

## ✨ Features

- **Scrolling OLED Display:** Smoothly scrolls the current track name and artist if it exceeds the screen width.
- **Ambient Breathing LED:** A connected LED gently fades in and out while music is playing, and turns off automatically when playback is paused or stopped.
- **Wi-Fi Enabled:** The ESP32 acts as a web server, receiving track updates over your local network.
- **Linux Desktop Integration:** Uses `playerctl` to seamlessly extract media metadata from the Brave browser.

## 🛠️ Hardware Configuration

### Components Required
- **ESP32 Development Board** (NodeMCU-32S or similar)
- **SSD1306 OLED Display** (128x32 resolution, I2C interface)
- **LED** (Any color, for the breathing effect)
- **Resistor** (e.g., 220Ω or 330Ω, for the LED)
- Breadboard and jumper wires

### Wiring Guide

| Component | Pin | ESP32 Pin |
| :--- | :--- | :--- |
| **SSD1306 OLED** | VCC | 3.3V |
| | GND | GND |
| | SDA | SDA (Default I2C) |
| | SCL | SCL (Default I2C) |
| **LED** | Anode (+) | D3 (via Resistor) |
| | Cathode (-) | GND |

*(Note: The LED pin is defined as `D3` in the code, ensure you connect it to the corresponding GPIO on your specific ESP32 board).*

## 💻 Software Prerequisites

**For the ESP32 (Arduino IDE):**
- ESP32 Board support package installed in Arduino IDE.
- `Adafruit_GFX` Library
- `Adafruit_SSD1306` Library

**For the Host PC (Linux):**
- `playerctl` (to fetch media metadata)
- `curl` (to send HTTP requests)
- Brave Browser (or modify the script for another player)

## 🚀 Setup Instructions

### 1. ESP32 Setup
1. Open `player.ino` in the Arduino IDE.
2. Update the Wi-Fi credentials to match your network:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
3. Compile and upload the code to your ESP32.
4. Once the ESP32 connects to Wi-Fi, the OLED screen will display its assigned local IP address. Note this IP down.

### 2. PC Script Setup
1. Open `stream.sh` in a text editor.
2. Replace the `ESP32_IP` variable with the IP address shown on your OLED display:
   ```bash
   ESP32_IP="192.168.X.X"
   ```
3. Make the script executable:
   ```bash
   chmod +x stream.sh
   ```

## 🎵 Usage

1. Power on your ESP32.
2. Start playing music or a video in your Brave browser.
3. Run the bash script in your terminal:
   ```bash
   ./stream.sh
   ```
4. The script will run in the background, listening for track changes and updating the ESP32 automatically!

## 🔧 Customization

- **Change the Media Player:** To use a different browser or player like Spotify, modify the `playerctl` command in `stream.sh`. Change `--player=brave` to `--player=spotify` or remove the flag entirely to track all media players.
- **Adjust Breathing Speed:** You can alter the speed of the LED breathing effect by tweaking `fadeInterval` and `fadeAmount` in `player.ino`.
