/* 
AP code for an ESP32 sending data to a station PC
*/


#include <WiFi.h>
#include <WiFiServer.h>
#include "driver/twai.h"
#include <HardwareSerial.h>

#define TX_GPIO_NUM GPIO_NUM_21
#define RX_GPIO_NUM GPIO_NUM_22

#define TCP_PORT 4210 // Shared port for TCP communication

const char* ssid = "ESP32_AP"; // SSID for the access point
const char* password = "12345678";  // Password for the access point

// Static IP configuration
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(TCP_PORT); // Create a TCP server on the specified port
WiFiClient client; // Client to handle incoming connections

unsigned long lastSendTime = 0; // Variable used in timing logic

void configureCan() {
  //Configures TWAI controller. TWAI is ESPRessifs own Two-Wire Automotive Interface, which is CAN bus compatible.
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NORMAL); // General settings, like pins and
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS(); // Set the bitrat to 500 kbps
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL(); // Accept all messages, no filtering

  // Installs TWAI driver into the ESP32
  if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
    Serial.println("Failed to install TWAI driver");
    while (true); //Stops the program by looping forever if the driver install fails
  }

  // Starts the TWAI driver
  if (twai_start() != ESP_OK) {
    Serial.println("Failed to start TWAI driver");
    while (true); //Stops the program by looping forever if the driver install fails
  }

  Serial.println("TWAI receiver started");
}

void configureTcp() {
  // Start the TCP server
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  server.begin(); // Start the TCP server
  Serial.printf("TCP server started on port %d\n", TCP_PORT);
}




void setup() {
  Serial.begin(115200);

  configureTcp(); // Configure the TCP server
  configureCan(); // Configure the CAN interface
}

void loop() {
  // Accept new client if none are connected
  if (!client || !client.connected()) { // Check if client object is invalid or disconnected
    client = server.available(); // Check if there is a new client trying to connect. Return a client object if a connection is available, otherwise returns an invalid client object.
    if (client) {
      Serial.println("Client connected");
    }
  }
 // Timing logic for data sending
  if (client && client.connected()) {
    unsigned long now = millis();
    if (now - lastSendTime > 300) { // Interval between data batches
      lastSendTime = now; 

      int fakeSensorValue = 5;
      client.printf("Sensor value: %d\n", fakeSensorValue); // Send a string + data
      Serial.println("Sent sensor value");
    }
  }
  delay(10);
}

