# Receiver script for ESP32 TCP communication.
# This script connects to an ESP32 device over TCP and listens for incoming data.
# Sends the received data as ROS2 String messages to a topic named 'esp32_data'.

import socket # Importing socket for TCP communication
import threading # Importing threading for handling TCP connections in a separate thread
import rclpy # Importing ROS2 Python client library

from rclpy.node import Node # Importing Node class to create a ROS2 node
from std_msgs.msg import String  # Importing String message type from standard messages"

ESP32_IP = "192.168.1.1"  # ESP32 AP IP address
TCP_PORT = 4210 # Shared port for TCP communication

class TcpReceiverNode(Node): # Defines a custom class inheriting from Node-class
  def __init__(self): # Constructor for the TcpReceiverNode class
    super().__init__('tcp_receiver_node')
    self.publisher_ = self.create_publisher(String, 'esp32_data', 10)
    self.get_logger().info("TCP Receiver Node started.")

    # Start the TCP listener in a separate thread
    self.tcp_thread = threading.Thread(target=self.tcp_listener, daemon=True)
    self.tcp_thread.start()

  def tcp_listener(self):
    while rclpy.ok():
      try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
          sock.settimeout(10) # Set a timeout for the connection
          print(f"Connecting to {ESP32_IP}:{TCP_PORT}")
          sock.connect((ESP32_IP, TCP_PORT)) # Connect to the ESP32

          while rclpy.ok():
            data = sock.recv(2048)
            if data:
              decoded = data.decode() # Decode the received bytes to a string
              self.get_logger().info(f"Received: {decoded}") # Log the received data into terminal
              msg = String() # Creates an instance of the String message type
              msg.data = decoded # Creates a String message with the received data
              self.publisher_.publish(msg) # Publish the message to the topic
      except (socket.timeout, ConnectionRefusedError, OSError) as e: # Handle timeout, connection refused, or other socket errors
        self.get_logger().warn(f"Connection error: {e}")
        self.get_logger().info("Retrying in 5 seconds...")
        self.sleep_or_wait(5)

  def sleep_or_wait(self, seconds):
    for _ in range(seconds * 10):
      if not rclpy.ok():
        break
      self.get_clock().sleep_for(rclpy.duration.Duration(seconds=0.1))

def main(args=None):
    rclpy.init(args=args)
    node = TcpReceiverNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()
if __name__ == "__main__":
  main()
