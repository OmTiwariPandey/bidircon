#ESP Transponder: ESP32 Google Sheet Trigger

**ESP Transponder** is an ESP32 project that demonstrates real-time integration between a physical button and Google Sheets. Press a button to instantly send a signal to a Google Sheet and receive a response to perform an action like blinking an LED. It's a fun and practical IoT project that shows how to interface hardware with cloud services.

## Features
- **Real-time Button Sync:** Send data to a Google Sheet with minimal delay when the button is pressed.
- **Cloud Integration:** Read and write data to Google Sheets using Google Apps Script.
- **Visual Feedback:** Blinks an onboard LED based on cloud responses.
- **Debouncing Logic:** Ensures stable input from the button without false triggers.
- **WiFi Connectivity:** Seamlessly connects to your home or office WiFi network.

## Project Overview
This project uses the following components:
- **ESP32 Microcontroller**: The brain of the project, managing WiFi connectivity and hardware interactions.
- **Google Apps Script Web App**: Acts as the backend to handle read and write requests to a Google Sheet.
- **Button**: Triggers an action to write data to the cloud.
- **Onboard LED**: Provides feedback based on the response from Google Sheets.

### How It Works
1. Press the button to trigger a "write" action to the Google Sheet.
2. The ESP32 periodically checks the Google Sheet for updates.
3. If the sheet's value indicates an action (e.g., `1`), the ESP32 blinks the LED and resets the value in the sheet.

---

## Getting Started

### Prerequisites
1. An **ESP32** microcontroller.
2. A button and optional breadboard for wiring.
3. Access to **Google Apps Script** and a Google Sheet.
4. Install the latest [Arduino IDE](https://www.arduino.cc/en/software) with the ESP32 board package.

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/ButtonSync.git
