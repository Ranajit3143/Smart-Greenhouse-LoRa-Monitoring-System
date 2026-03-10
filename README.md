
# Smart Greenhouse LoRa Monitoring System

This project implements a smart greenhouse monitoring system using ESP32-S3 and LoRa communication.

## Features

- Temperature and humidity monitoring
- CO2 monitoring
- Light intensity measurement
- Soil moisture monitoring
- Water pH and EC monitoring
- Water level detection
- Water flow measurement
- Long range LoRa communication

## Hardware Used

ESP32-S3
SX1278 LoRa module
DHT22 sensor
BH1750 light sensor
MH-Z19 CO2 sensor
Capacitive soil moisture sensor
E201-C pH sensor
Gravity EC sensor
YF-S201 water flow sensor

## System Architecture

Sensors → ESP32-S3 → LoRa → ESP32-S3 Gateway → Serial Monitor / Cloud

## Communication

LoRa frequency: 433 MHz
