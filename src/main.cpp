#include <Arduino.h>
#include <Wire.h>             // Include the Wire library for I2C communication
#include <OLEDDisplay.h>      // Include the OLEDDisplay library
#include "SSD1306Wire.h"      // Include the SSD1306Wire library for SSD1306 OLED display
#include "DHT.h"             // Include the DHT library for DHT temperature and humidity sensor

#define DHTPIN 18            // Define the pin for DHT sensor
#define DHTTYPE DHT11        // Define the type of DHT sensor

// Function declarations
void toggleRelay();
void updateDisplay();
void setRelayOn();
void setRelayOff();

SSD1306Wire display(0x3c, SDA, SCL);  // Create an instance of the SSD1306Wire class for OLED display
DHT dht(DHTPIN, DHTTYPE, 60);          // Create an instance of the DHT class for DHT sensor

bool relay;       // Variable to store relay state (ON/OFF)
bool autoMode;    // Variable to store auto mode state (enabled/disabled)

void setup() {
  display.init();                     // Initialize the OLED display
  display.flipScreenVertically();     // Flip the screen vertically
  display.setFont(ArialMT_Plain_10); // Set font for the display
  display.setTextAlignment(TEXT_ALIGN_LEFT); // Set text alignment
  display.drawString(0, 0, "Hello World!"); // Display a greeting message
  display.display();                 // Display the message
  relay = true;                      // Initialize relay state to ON
  pinMode(10, INPUT_PULLUP);         // Set pin 10 as input with pull-up resistor
  pinMode(11, INPUT_PULLUP);         // Set pin 11 as input with pull-up resistor
  pinMode(4, OUTPUT);                // Set pin 4 as output for controlling relay
  dht.begin();                       // Initialize DHT sensor
  autoMode = true;                   // Initialize auto mode to enabled
}

void loop() {
  // Check if manual toggle button is pressed and auto mode is disabled
  if (!digitalRead(10) && !autoMode)  {
    delay(100);
    if (!digitalRead(10)) {
      toggleRelay();  // Toggle relay state
    }
  }
  // Check if auto/manual mode toggle button is pressed
  if (!digitalRead(11))  {
    delay(100);
    if (!digitalRead(11)) {
      autoMode = !autoMode; // Toggle auto mode
    }
  }
  updateDisplay();  // Update OLED display
  if (autoMode) {
    // If in auto mode, control relay based on temperature
    if (dht.readTemperature() > 25) {
      setRelayOn();  // Turn on relay if temperature is above 25°C
    } else {
      setRelayOff(); // Turn off relay if temperature is below or equal to 25°C
    }
  }
}

// Function to toggle relay state
void toggleRelay() {
  if (relay) {
    digitalWrite(4, LOW); // Turn off relay
    relay = false;
  } else {
    digitalWrite(4, HIGH); // Turn on relay
    relay = true;
  }
}

// Function to turn on relay
void setRelayOn() {
  digitalWrite(4, HIGH); // Turn on relay
  relay = true;
}

// Function to turn off relay
void setRelayOff() {
  digitalWrite(4, LOW); // Turn off relay
  relay = false;
}

// Function to update OLED display
void updateDisplay() {
  display.clear(); // Clear the display
  // Display current relay state, humidity, temperature, and auto mode state
  if (relay) {
    display.drawString(0, 0, "Relay ON! \nHum: " + String(dht.readHumidity()) + "%\nTemp: " + String(dht.readTemperature()) + "C" + "\nAuto: " + String(autoMode));
  } else {
    display.drawString(0, 0, "Relay OFF! \nHum: " + String(dht.readHumidity()) + "%\nTemp: " + String(dht.readTemperature()) + "C" + "\nAuto: " + String(autoMode));
  }
  display.display(); // Display the updated content on the OLED display
}
