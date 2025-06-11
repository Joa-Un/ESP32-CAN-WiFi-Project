/* 
AP code for an ESP32 sending data to a station PC


*/


#include <WiFi.h>
#include <WiFiServer.h>

#define TCP_PORT 4210 // Port for TCP server

const char* ssid = "ESP32_AP"; // SSID for the access point
const char* password = "12345678";  // Password for the access point

// Static IP configuration
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(TCP_PORT); // Create a TCP server on the specified port
WiFiClient client;

unsigned long lastSendTime = 0;

void setup() {
  Serial.begin(115200);

  // Initialize WiFi in Access Point mode
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  server.begin(); // Start the TCP server
  Serial.printf("TCP server started on port %d\n", TCP_PORT);
}

void loop() {
  // Accept new client if none are connected
  if (!client || !client.connected()) { // Check if there is no client connected
    client = server.available(); // Check for new client connections
    if (client) {
      Serial.println("Client connected");
    }
  }
 // Send data if client is connected
  if (client && client.connected()) {
    unsigned long now = millis();
    if (now - lastSendTime > 300) { // Interval between data batches
      lastSendTime = now; 

      int fakeSensorValue = 5; // Sent data
      client.printf("Sensor value: %d\n", fakeSensorValue); // Sends a string + data
      Serial.println("Sent sensor value");
    }
  }

  delay(10);
}

