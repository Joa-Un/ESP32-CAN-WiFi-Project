#include <WiFi.h>
#include <WiFiServer.h>

#define TCP_PORT 4210

const char* ssid = "ESP32_AP";
const char* password = "12345678";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(TCP_PORT);

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  server.begin();
  Serial.printf("TCP server started on port %d\n", TCP_PORT);
}

void loop() {
  WiFiClient client = server.available();  // Check for incoming client connection

  if (client) {
    Serial.println("Client connected");

    // Send a message to the connected client
    client.println("Hello from ESP32 via TCP!");

    // You can keep client connected or close after sending:
    client.stop();
    Serial.println("Client disconnected");
  }

  delay(1000);
}
