/*
This code sets up an ESP32 as a WiFi access point and a TCP server that listens for incoming connections.
It receives CAN messages from the CAN bus and sends them to connected clients over TCP.
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

/*
@brief This function configures the ESP32 to act as a WiFi access point.
*/
void configureWifi() {
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin(); // Start the TCP server
  Serial.printf("TCP server started on port %d\n", TCP_PORT);
}

/*
@brief Configures and starts the TWAI (CAN) driver on the ESP32.
Set up general, timing, and filter configurations and ensurs the driver is started.
The system halts if initialization fails.
*/
void configureCan() {
  // Configure TWAI controller. TWAI is ESPRessifs own Two-Wire Automotive Interface, which is CAN bus compatible.
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS(); // Set the bitrat to 500 kbps
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL(); // Accept all messages, no filtering

  // Install TWAI driver into the ESP32
  if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
    Serial.println("Failed to install TWAI driver");
    while (true); // Stop the program by looping forever if the driver install fails
  }

  // Start the TWAI driver
  if (twai_start() != ESP_OK) {
    Serial.println("Failed to start TWAI driver");
    while (true); // Stop the program by looping forever if the driver install fails
  }

  Serial.println("TWAI receiver started");
}

/*
@brief Send data to the connected client.
Check if the client is connected, and if not, it attempts to accept a new connection.
@param: canData - The data to be sent to the client. Received from the CAN bus
*/
void sendData(char* canDataStr) {
  if (!client || !client.connected()) { // Check if client object is invalid or disconnected
    client = server.available(); // Check if there is a new client trying to connect. Return a client object if a connection is available, otherwise returns an invalid client object.
    if (client) {
      Serial.println("Client connected");
    }
  }
  if (client && client.connected()) {
    client.printf(canDataStr);
    Serial.println("Sent CAN data over Wi-Fi");
  } 
}

/*
@brief Receive CAN data and send it to the connected client.
This function waits for a CAN message, and if one is received, it formats the data into a string and sends it to the client
*/
void receiveCanData() {
  twai_message_t rx_msg; // Structure that's defined in the CAN Sender code
  // Try to receive the message within 100ms timeout window
  if (twai_receive(&rx_msg, pdMS_TO_TICKS(100)) == ESP_OK) {
    Serial.print("Received ID: 0x");
    Serial.print(rx_msg.identifier, HEX);
    Serial.print(" Data: ");
    for (int i = 0; i < rx_msg.data_length_code; i++) {
      Serial.printf("%02X ", rx_msg.data[i]);
    }
    Serial.println();

    // Transform the received CAN data into a string for sending over TCP
    char canDataStr[3 * rx_msg.data_length_code + 1]; // Enough space for "XX XX XX XX\0"
    int pos = 0;
    for (int i = 0; i < rx_msg.data_length_code; i++) {
      pos += snprintf(&canDataStr[pos], sizeof(canDataStr) - pos, "%02X ", rx_msg.data[i]);
    }
    canDataStr[pos - 1] = '\0'; // Remove last space and terminate string

    sendData(canDataStr);
  }
}

/*
@brief Initialize the system: set up Wi-Fi and CAN.
*/
void setup() {
  Serial.begin(115200);
  configureWifi(); // Configure the TCP server
  configureCan(); // Configure the CAN interface
}

/*
@brief Main loop of the program.
*/
void loop() {
  receiveCanData(); // Receive CAN data
  delay(10);
}
