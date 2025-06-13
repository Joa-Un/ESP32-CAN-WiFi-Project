// Test code for sending CAN messages from one ESP32 to another
// Generates sensor-like CAN messages and send them over the CAN bus.

#include <Arduino.h>
#include "driver/twai.h"
#include <HardwareSerial.h>

#define TX_GPIO_NUM GPIO_NUM_21
#define RX_GPIO_NUM GPIO_NUM_22

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); // Use floating analog pin to seed RNG
  
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  // Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
    Serial.println("Failed to install TWAI driver");
    while (true); // Stop the program by looping forever if the driver install fails
  }

  // Start TWAI driver
  if (twai_start() != ESP_OK) {
    Serial.println("Failed to start TWAI driver");
    while (true); // Stop the program by looping forever if the driver install fails
  }
  Serial.println("TWAI initialized successfully");
}

void loop() {
  // Create the CAN message
  twai_message_t tx_msg; // CAN message structure
  tx_msg.identifier = 0x100; // CAN message ID
  tx_msg.extd = 0;  // Standard frame
  tx_msg.rtr = 0;   // Data frame
  tx_msg.data_length_code = 4; // Byte amount

  // Generate 4 random bytes
  for (int i = 0; i < 4; i++) {
    tx_msg.data[i] = (uint8_t)random(0, 256);
  }

  // Transmit message
  if (twai_transmit(&tx_msg, pdMS_TO_TICKS(1000)) == ESP_OK) {
    Serial.print("CAN data sent: ");
    // Print the message ID and data
    for (int i = 0; i < 4; i++) {
      if (tx_msg.data[i] < 0x10) Serial.print("0"); 
      Serial.print(tx_msg.data[i], HEX);
      Serial.print(" ");
      }
    Serial.println();
  } else {
    Serial.println("Failed to send CAN data");
  }
  delay(1000); // Interval between messages
}