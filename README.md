# System-Monitor-by-ESP8266-and-Libre-Hardware-Monitor
System Monitor by ESP8266 and Libre Hardware Monitor

# Steps to Do
1. First download and run Libre Hardware Monitor (LHM) [here](https://github.com/LibreHardwareMonitor/LibreHardwareMonitor).
2. Run LHM and do not change anything, only activate Remote Web Server.
3. Open and Run "shorten_json.py". (This app is created by Github Copilot to shorten the json output of LHM web server. Because its so huge and ESP8266's memory cant read it all.)
4. Modify your SSID & Password & local IP in ino file.
5. Verify and upload the code (PY_ESP8266_LHM.ino) to your ESP8266.

## Project Demo
[![Watch the video](https://img.youtube.com/vi/OWI3RD5Wnaw/maxresdefault.jpg)](https://www.youtube.com/watch?v=OWI3RD5Wnaw)

# Screenshots of the app

Splash Screen

![background](https://github.com/user-attachments/assets/b027f136-e9e1-4e65-a1c0-368e9272d431)

Main Screen

![main](https://github.com/user-attachments/assets/a113c530-3a6a-41cb-a41a-71d29dfd9321)


# Descriptions 

Splash Screen

![background_des](https://github.com/user-attachments/assets/90b56dc1-d75e-41c5-931e-870bc0f8b804)

Main Screen

![main_des](https://github.com/user-attachments/assets/273fc867-1582-4a75-b6e6-381d65e6a6fa)


# ESP8266 WiFi Connection and Data Display on TFT Screen

This project connects an ESP8266 to a WiFi network, fetches JSON data from a server, and displays the data on a TFT screen. Additionally, it updates and displays the current date and time using an NTP client.

## Features

1. **Include Libraries**:
    - Includes necessary libraries for WiFi, HTTP client, JSON parsing, TFT display, NTP client, and UDP communication.

2. **Define Constants**:
    - Defines constants for WiFi credentials, server URL, and colors.

3. **Initialize NTP Client**:
    - Initializes an NTP client to fetch the current date and time.

4. **Initialize TFT Display and Sprites**:
    - Creates objects for the TFT display and various sprites used to display information.

5. **Setup Function**:
    - Initializes serial communication.
    - Connects to the specified WiFi network.
    - Initializes the TFT display and sets up the screen with a background image.
    - Creates and configures the sprites for the clock, CPU temperature, CPU load, GPU temperature, and GPU load.
    - Waits for a successful WiFi connection.
    - Initializes the NTP client and fetches the current time.

6. **Loop Function**:
    - Checks for a WiFi connection.
    - Sends an HTTP GET request to fetch JSON data from the server.
    - Parses the received JSON data to extract CPU and GPU information.
    - Updates and displays the current date and time if it has changed.
    - Displays the CPU and GPU names and their respective temperatures and loads on the TFT screen.

7. **Date and Time Update**:
    - Fetches the current date and time from the NTP server.
    - Updates the time and date displayed on the screen if they have changed.

8. **Display CPU and GPU Information**:
    - Displays the CPU and GPU names based on the parsed JSON data.
    - Extracts and displays the CPU and GPU temperature and load values on the screen.

## How to Use

1. Clone this repository to your local machine.
2. Open the project in the Arduino IDE.
3. Update the WiFi credentials (`ssid` and `password`) and `serverUrl` with your own information.
4. Upload the code to your ESP8266 board.
5. Open the Serial Monitor to view the status and fetched data.
6. The TFT screen will display the current date, time, and CPU/GPU information from the server.

## License

This project is licensed under the MIT License.

## Important Notes for Users (shorten_json.py)

- **Security**: The application only reads data from the specified Libre Hardware Monitor URL and does not perform any actions that could harm your system.
- **Transparency**: The code is open-source, and you can review it yourself to ensure that it is safe to use.
- **Usage**: Feel free to download and use this application without hesitation. It is designed to be a simple and safe tool for monitoring hardware data.
- **Support**: If you have any questions or concerns, please feel free to open an issue or contact the repository maintainer.

