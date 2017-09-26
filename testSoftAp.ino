#include <WiFi.h>

const char *wifiSSID = "WIFI_NAME_HERE";
const char *espSSID = "ESP_WIFI_NAME_HERE";
const char *espPass = "ESP_PASS_HERE";

//create the server
WiFiServer server(80);

void setup() {
  //baud rate
  Serial.begin(115200);

  //blue LED on esp32 board
  pinMode(5, OUTPUT);
  
  delay(1000);
  Serial.println();
  Serial.println("Configuring access point...");
  //configure the server
  WiFi.softAP(espSSID, espPass);

  //connect to wifi
  Serial.println("Trying to connect to WiFi: ");
  int status = WiFi.begin(wifiSSID);
  while (status != WL_CONNECTED) { 
    Serial.print(".");
    delay(2000);
  }
  Serial.println();
  Serial.println("Connected to WiFi.");

  //print the esp32 ip address to serial monitor
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  //begin listening
  server.begin();
  Serial.println("Server started.");
  Serial.println("Waiting for clients...");
}

void loop() {
  //get the client
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            //write the HTTP response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            //create two buttons, one for turning LED on / one for off
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(5, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(5, LOW);                // GET /L turns the LED off
        }
      }
    }
  }

  client.stop();
}


