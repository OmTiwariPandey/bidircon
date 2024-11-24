#include <Wire.h>                       // For I2C communication
#include <Adafruit_GFX.h>               // Graphics library
#include <Adafruit_SSD1306.h>           // OLED library
#include <WiFi.h>
#include <HTTPClient.h>

#define SCREEN_WIDTH 128                // OLED width
#define SCREEN_HEIGHT 64                // OLED height
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "YOUR_SSID";          // Replace with your WiFi SSID
const char* password = "YOUR_PASSWORD"; // Replace with your WiFi Password

const char* googleScriptURL = "YOUR_WEB_APP_URL"; // Replace with your Google Apps Script Web App URL

const int buttonPin = 19; // D19 for the button
const int ledPin = 2;     // D2 for the onboard LED

int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP); // Button connected between D19 and GND
  pinMode(ledPin, OUTPUT);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Use SSD1306_SWITCHCAPVCC for I2C
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Stop execution if initialization fails
  }

  // Clear the display and set text size and color
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Display a startup message
  display.setCursor(0, 0);
  display.println("WiFi Connecting...");
  display.display();

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  // Show success message on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected!");
  display.display();
  delay(1000);
}

void loop() {
  // Debounce the button
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        Serial.println("Button pressed, sending signal");
        sendWriteRequest();
      }
    }
  }
  lastButtonState = reading;

  // Read the value from Google Sheet
  String value = sendReadRequest();

  // Display the value on the OLED
  displayValue(value);

  if (value == "1") {
    // Blink the LED
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);

    // After blinking, reset the value to 0
    sendResetRequest();
  }

  delay(1000); // Wait before next loop
}

void sendWriteRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(googleScriptURL) + "?action=write&value=1";
    http.begin(url);
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS); // Force redirect following
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Write Response: " + payload);
    } else {
      Serial.printf("Error on HTTP request: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}

String sendReadRequest() {
  String payload = "";
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(googleScriptURL) + "?action=read";
    Serial.println("Sending read request to Google Sheet...");
    http.begin(url);
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS); // Force redirect following
    int httpCode = http.GET();

    if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Read Response: " + payload);
    } else {
      Serial.printf("Error on HTTP request: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("WiFi not connected, cannot send read request");
  }
  return payload;
}

void sendResetRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(googleScriptURL) + "?action=write&value=0";
    http.begin(url);
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS); // Force redirect following
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Reset Response: " + payload);
    } else {
      Serial.printf("Error on HTTP request: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}

// Function to display value on OLED
void displayValue(String value) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Google Sheets Value:");
  display.println(value);
  display.display();
}
