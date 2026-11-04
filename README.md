# 🌱 Smart Plant Monitoring System

An IoT-based Smart Plant Monitoring System designed to monitor plant health and automate watering and excess-water drainage. The system uses an ESP32 microcontroller, multiple sensors, relay-controlled water pumps, and the Blynk IoT platform to provide real-time monitoring and control.

## 📌 Project Overview

The Smart Plant Monitoring System helps maintain suitable growing conditions by continuously monitoring soil moisture, temperature, humidity, and water depth.

Based on sensor readings, the system automatically operates two water pumps:
- **Water IN Pump:** Supplies water when soil moisture falls below the defined threshold.
- **Water OUT Pump:** Drains excess water when the water level reaches the defined threshold.

The Blynk IoT dashboard allows users to monitor sensor readings remotely and manually control the water inlet pump.

## ✨ Features

- Real-time soil moisture monitoring.
- Temperature and humidity monitoring using DHT22.
- Water depth monitoring using a water-level sensor.
- Automatic plant watering based on soil moisture.
- Automatic excess-water drainage.
- Two independently controlled pumps using a relay module.
- Manual water pump control through Blynk.
- Remote monitoring through a mobile dashboard.
- Low-temperature and high-temperature status monitoring.
- Low-temperature notification through Blynk.
- Pump protection logic to prevent both pumps from running simultaneously.

## 🛠️ Hardware Components

| Component | Quantity |
|---|---:|
| ESP32 DevKit V1 | 1 |
| Soil Moisture Sensor | 1 |
| DHT22 Temperature and Humidity Sensor | 1 |
| Water Depth Sensor | 1 |
| 5V 2-Channel Relay Module | 1 |
| 5V Submersible DC Water Pump | 2 |
| 5V External Power Supply | 1 |
| Jumper Wires | As required |
| Breadboard | 1 |
| Water Tubes | As required |
| Water Container | 1 |

## 💻 Software and Technologies

- Arduino IDE
- Embedded C++ / Arduino Framework
- ESP32
- Blynk IoT Platform
- Wi-Fi Communication

## 🔌 ESP32 Pin Configuration

| Component | ESP32 GPIO |
|---|---|
| Soil Moisture Sensor (AO) | GPIO 34 |
| DHT22 (OUT) | GPIO 4 |
| Water Depth Sensor (AO) | GPIO 35 |
| Water IN Relay (IN1) | GPIO 26 |
| Water OUT Relay (IN2) | GPIO 27 |

## ⚙️ Working Principle

### 1. Soil Moisture Monitoring
The soil moisture sensor continuously measures the moisture level in the soil and sends the readings to the ESP32.

- Below 50%: Water IN pump starts.
- At or above 80%: Water IN pump stops.

### 2. Temperature and Humidity Monitoring
The DHT22 sensor measures surrounding temperature and humidity. The readings are sent to the Blynk dashboard.

Temperature status:
- Below 20°C: Low-temperature alert.
- 20°C to 35°C: Normal.
- Above 35°C: High-temperature status.

### 3. Water Depth Monitoring
The water depth sensor monitors the water level and sends the calculated percentage to the Blynk dashboard.

- At or above 80%: Water OUT pump starts.
- At or below 60%: Water OUT pump stops.

### 4. Automatic Pump Control
The ESP32 processes sensor readings and controls the relay module.

- The Water IN pump operates according to soil moisture.
- The Water OUT pump operates according to water depth.
- Water OUT operation receives priority.
- Both pumps are prevented from operating simultaneously.

### 5. Blynk IoT Monitoring
The Blynk dashboard provides remote access to sensor information and manual pump control.

| Virtual Pin | Function |
|---|---|
| V1 | Soil Moisture |
| V2 | Temperature |
| V3 | Humidity |
| V4 | Manual Water IN Control |
| V6 | Water Depth |
| V8 | Temperature Status |

## 📂 Project Structure

```text
smart-plant-monitoring-system/
│
├── SmartPlantMonitoring/
│   └── SmartPlantMonitoring.ino
│
├── images/
│   ├── hardware-setup.jpg
│   ├── circuit-connection.jpg
│   └── blynk-dashboard.jpg
│
└── README.md
```

## 🚀 Getting Started

### Prerequisites
- Arduino IDE installed.
- ESP32 board package configured.
- Blynk account and IoT template.
- Required sensors and hardware components.

### Required Libraries

Install the following libraries through Arduino IDE Library Manager:

- Blynk
- DHT sensor library

### Setup Instructions

1. Clone or download this repository.
2. Open `SmartPlantMonitoring.ino` in Arduino IDE.
3. Install the required libraries.
4. Configure your ESP32 board and COM port.
5. Add your Blynk template ID, authentication token, Wi-Fi name, and password.
6. Connect the sensors and relay module according to the pin configuration.
7. Upload the code to the ESP32.
8. Open the Blynk dashboard to monitor the system.

**Important:** Keep your Blynk authentication token and Wi-Fi credentials private. Do not upload real credentials to GitHub.

## 📊 Expected Output

The system provides:
- Live soil moisture percentage.
- Temperature and humidity readings.
- Live water depth percentage.
- Automatic watering based on soil moisture.
- Automatic drainage based on water depth.
- Remote pump control through Blynk.
- Temperature status and notifications.

## 🔮 Future Enhancements

- Mobile application with an improved user interface.
- Historical sensor data visualization.
- Multiple plant monitoring.
- Weather-based irrigation automation.
- AI-based plant health analysis.
- Cloud data storage and analytics.

## 👨‍💻 Project Information

**Project:** Smart Plant Monitoring System  
**Domain:** Internet of Things (IoT)  
**Controller:** ESP32 DevKit V1  
**Platform:** Blynk IoT  

## 📜 License

This project is intended for educational and academic purposes.
