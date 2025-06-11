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

void setup() {
  Serial.begin(115200);

  // Initialize WiFi in Access Point mode
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  server.begin(); // Start the TCP server
  Serial.printf("TCP server started on port %d\n", TCP_PORT);
}

void loop() {
  WiFiClient client = server.available();  // Check for incoming client connection

  if (client) {
    Serial.println("Client connected");

    client.println("Hello from ESP32 via TCP!"); // Send a message to the connected client

    // Closing the client connection after sending the message
    client.stop();
    Serial.println("Client disconnected");
  }

  delay(1000);
}
