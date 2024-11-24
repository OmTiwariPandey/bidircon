# BiconOled: ESP32 Google Sheets Transponder

**BiconOled** is an IoT project that integrates an ESP32 microcontroller with Google Sheets. It allows real-time communication between a button press and a Google Sheet, enabling dynamic data updates and actions such as blinking an LED based on sheet values.

This project is an excellent introduction to combining hardware, cloud services, and simple scripts for practical IoT applications.

---

## Features

- **Cloud Integration**: Seamlessly interact with Google Sheets using Google Apps Script.
- **Real-Time Trigger**: Press a button to send data to a Google Sheet and act on cloud-based responses.
- **Visual Feedback**: Displays the Google Sheet response on an OLED screen and blinks an onboard LED.
- **Debouncing Logic**: Prevents false button triggers with built-in debounce handling.
- **WiFi Connectivity**: Easily connects to your home or office WiFi network.

---

## Getting Started

### Components Required

- **ESP32 Development Board**
- **Push Button**
- **Breadboard and Jumper Wires**
- **OLED Display** (128x64, I2C)
- **Laptop with Arduino IDE**

---

## Installation and Setup

### 1. Clone the Repository

First, clone the repository to your local machine:

```bash
git clone https://github.com/OmTiwariPandey/bidircon.git
cd bidircon
```

Ensure the `BiconOled.ino` file is located in the root of the repository.

---

### 2. Set Up Google Apps Script

#### a. Create a Google Sheet

1. Go to [Google Sheets](https://sheets.google.com) and create a new spreadsheet.
2. Name the spreadsheet (e.g., `ESP32_Trigger`).
3. Make sure the first sheet is named `Sheet1`.
4. In cell `A1`, enter the initial value `0`.

#### b. Open Google Apps Script

1. In the Google Sheet, click on `Extensions > Apps Script`.
2. Delete the default code and replace it with the following:

   ```javascript
   var SHEET_ID = "YOUR_GOOGLE_SHEET_ID"; // Replace with your Google Sheet ID
   var SHEET_NAME = "Sheet1";             // Replace with your Sheet Name if different

   function doGet(e) {
     var action = e.parameter.action;
     if (action == "read") {
       return readValue();
     } else if (action == "write") {
       var value = e.parameter.value;
       return writeValue(value);
     } else {
       return ContentService.createTextOutput("Invalid action")
         .setMimeType(ContentService.MimeType.TEXT);
     }
   }

   function readValue() {
     var ss = SpreadsheetApp.openById(SHEET_ID);
     var sheet = ss.getSheetByName(SHEET_NAME);
     var value = sheet.getRange("A1").getValue();
     return ContentService.createTextOutput(value.toString())
       .setMimeType(ContentService.MimeType.TEXT);
   }

   function writeValue(value) {
     var ss = SpreadsheetApp.openById(SHEET_ID);
     var sheet = ss.getSheetByName(SHEET_NAME);
     sheet.getRange("A1").setValue(value);
     return ContentService.createTextOutput("Success")
       .setMimeType(ContentService.MimeType.TEXT);
   }
   ```

#### c. Configure the Script

1. Replace `YOUR_GOOGLE_SHEET_ID` with the ID from your Google Sheet URL:
   ```
   https://docs.google.com/spreadsheets/d/<YOUR_SHEET_ID>/edit
   ```

#### d. Deploy as a Web App

1. Click on `Deploy > New Deployment`.
2. Choose **Web App** as the deployment type.
3. Set the access to **"Anyone with the link"**.
4. Copy the **Web App URL** from the deployment screen. This will be used in the ESP32 code.

---

### 3. Configure the Arduino Code

#### a. Install ESP32 Board Package

1. Open the Arduino IDE and go to `File > Preferences`.
2. Add the following URL to **Additional Board Manager URLs**:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```

3. Go to `Tools > Board > Boards Manager`, search for **ESP32**, and install it.

#### b. Install Required Libraries

1. Open `Tools > Manage Libraries`.
2. Search and install:
   - **Adafruit GFX**
   - **Adafruit SSD1306**

#### c. Edit and Upload `BiconOled.ino`

1. Open the `BiconOled.ino` file in the Arduino IDE.
2. Replace the placeholders in the code:
   - `YOUR_WIFI_SSID` → Your WiFi SSID.
   - `YOUR_WIFI_PASSWORD` → Your WiFi password.
   - `YOUR_WEB_APP_URL` → The Web App URL from your Google Apps Script.
3. Select your ESP32 board and the appropriate COM port from the `Tools` menu.
4. Upload the code to your ESP32.

---

### 4. Wiring Diagram

| ESP32 Pin | Component  |
|-----------|------------|
| `3.3V`    | OLED `VCC` |
| `GND`     | OLED `GND` |
| `GPIO21`  | OLED `SDA` |
| `GPIO22`  | OLED `SCL` |
| `GPIO19`  | Button One Side |
| `GND`     | Button Other Side |
| `GPIO2`   | Onboard LED |

---

## Usage

1. **Button Press**:
   - Press and hold the button for 5 to 10 seconds to trigger a "write" action to the Google Sheet (`A1` updated to `1`).
2. **Google Sheet Response**:
   - The ESP32 periodically checks the sheet for updates.
   - If the sheet value is `1`, the LED blinks, and the value resets to `0`.
3. **OLED Display**:
   - Shows the current value fetched from the Google Sheet.

---

## Troubleshooting

- **WiFi Not Connecting**:
  - Double-check your WiFi SSID and password.
  - Ensure the ESP32 is within range of your router.
- **Invalid Web App Response**:
  - Verify the Web App URL.
  - Ensure the Apps Script deployment is set to "Anyone with the link."
- **Incorrect Google Sheet ID**:
  - Re-check the ID from your Google Sheet URL.

---

## License

This project is licensed under the [MIT License](LICENSE).

---

Feel free to contribute or raise issues via [GitHub](https://github.com/OmTiwariPandey/bidircon).
