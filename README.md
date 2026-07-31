# ESP8266 Off-Grid Local Chat Server 💬

A completely private, localized chat room hosted entirely on an ESP8266 microcontroller. 

This project turns the ESP8266 into a standalone Wi-Fi Access Point and Web Server. Anyone who connects to the network can access the chat interface and communicate in real-time. The system uses asynchronous JavaScript (`fetch()`) to update the chat logs seamlessly without requiring the page to reload, acting like a local walkie-talkie text network that completely bypasses the internet.

<div align="center">
  <table>
    <tr>
      <td>
        <img src="Local_offline_chat_device2.jpeg" alt="Chat UI - Device 1" width="300">
        <br>
        <p align="center"><i>Device 1 (Mobile)</i></p>
      </td>
      <td>
        <img src="Local_offline_chat_device1.jpeg" alt="Chat UI - Device 2" width="300">
        <br>
        <p align="center"><i>Device 2 (Laptop)</i></p>
      </td>
    </tr>
  </table>
</div>

## Features
## Features
* **100% Offline:** Operates entirely off-the-grid; no internet, cell towers, or external routers required.
* **Asynchronous Updates:** Uses JS `fetch()` and `setInterval()` to automatically poll for new messages seamlessly.
* **Auto-Scrolling:** The chat interface automatically scrolls down when new messages arrive.
* **RAM Management:** Automatically trims older messages to prevent the ESP8266 from running out of memory.

## Hardware Required
* ESP8266 Development Board (e.g., NodeMCU 1.0 / ESP-12E)
* Micro-USB Data Cable (for power)

## Software & Libraries Used
* `ESP8266WiFi.h` (To broadcast the Access Point)
* `ESP8266WebServer.h` (To host the local chat dashboard)

## How to Use
1. Flash the code to your ESP8266 using the Arduino IDE.
2. Power the ESP8266 from a laptop, power bank, or wall adapter.
3. Connect your devices (phones, laptops, tablets) to the **ESP_Offline_Chat** Wi-Fi network (Password: `password123`).
4. Open a web browser on your connected devices and navigate to `http://192.168.4.1`.
5. Enter a username, type a message, and start chatting!
