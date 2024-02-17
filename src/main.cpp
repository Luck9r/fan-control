#include <Arduino.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include <OLEDDisplay.h>
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include "DHT.h"
#define DHTPIN 18
#define DHTTYPE DHT11 

void toggleRelay();
void updateDisplay();
void setRelayOn();
void setRelayOff();
SSD1306Wire display(0x3c, SDA, SCL);  
DHT dht(DHTPIN, DHTTYPE, 60);
bool relay;
bool autoMode;

void setup() {
  display.init(); 
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Hello World!");
  display.display();
  relay=true;
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  dht.begin();
  autoMode =true;
}



void loop() {
  if(!digitalRead(10) && !autoMode)  {
    delay(100);
    if(!digitalRead(10)) {
      toggleRelay();
    }
  }
  if(!digitalRead(11))  {
    delay(100);
    if(!digitalRead(11)) {
      autoMode = !autoMode;
    }
  }
  updateDisplay();
  if (autoMode) {
    if (dht.readTemperature() > 25) {
      setRelayOn();
    } else {
      setRelayOff();
    }
  }

}

void toggleRelay() {
  if(relay) {
    digitalWrite(4, LOW);
    relay = false;
  
  } else {
    digitalWrite(4, HIGH);
    relay = true;
  }
}
void setRelayOn() {
  digitalWrite(4, HIGH);
  relay = true;
}
void setRelayOff() {
  digitalWrite(4, LOW);
  relay = false;
}
void updateDisplay() {
  display.clear();
  if (relay) {
    display.drawString(0, 0, "Relay ON! \nHum: " + String(dht.readHumidity())+ "%\nTemp: " + String(dht.readTemperature()) + "C" + "\nAuto: " + String(autoMode));
  } else {
    display.drawString(0, 0, "Relay OFF! \nHum: " + String(dht.readHumidity())+ "%\nTemp: " + String(dht.readTemperature()) + "C" + "\nAuto: " + String(autoMode));
  }
  display.display();

}
