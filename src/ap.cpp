#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define CONSOLE_IP "192.168.1.2" // Receiving computer's IP address in the local network
#define CONSOLE_PORT 4210 // Shared port for sending UDP packets

const char* ssid = "ESP32_AP"; // WiFi Access Point SSID
const char* password = "12345678"; // WiFi Access Point password

// Creates a WiFiUDP object, used to send and receive UDP packets over Wi-Fi.
WiFiUDP Udp;
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void setup()
{
  Serial.begin(115200);
  // Creates a WiFi Access Point with the specified SSID and password.
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  Udp.begin(4210); // Starts the UDP server on port 4210
}
void loop() {
  // Sends a UDP packet to the specified console IP and port every second.
  Serial.printf("Sending UDP packet to %s:%d\n", CONSOLE_IP, CONSOLE_PORT);
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.print("Hello from ESP32!");
  Udp.endPacket();

  delay(1000);
}
