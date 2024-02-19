# Fan Control
This is a simple fan control program for the ESP32 (Arduino compatibility mode). It uses a DHT11 temperature sensor to control the fan state. The fan state is controlled by a relay.

The current state of the device is displayed on an OLED display. The OLED display is connected to the ESP32 using I2C.

The device can be controlled manually with two buttons, controlling the automatic mode and the fan state.