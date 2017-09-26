# ESP32 Sketches

## testSoftAp

Basic script to connect to WiFi provided an unprotected WiFi network name and start the ESP32 in station+softAP mode. For devices that connect to the ESP32, they can then access two endpoints `/H` and `/L` through the ESP32's IP address, which will turn the blue LED on the ESP32 board on or off (H = high, L = low).
