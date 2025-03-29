# Smart Plug-Based Real-Time Energy Monitoring System

## Introduction
Energy consumption is a critical concern in modern households, offices, and industrial environments. Traditional energy monitoring methods provide an overall view but lack detailed insights into individual appliance consumption. This project introduces a smart plug-based real-time energy monitoring system that offers accurate, appliance-level energy tracking, real-time insights, and remote control capabilities.

## Features
- **Real-time Energy Monitoring**: Tracks power consumption of individual appliances.
- **Accurate Data Collection & Storage**: Maintains historical data for analysis and optimization.
- **User-Friendly Display & Indicators**: Provides real-time feedback via LCD and LED indicators.
- **Remote Accessibility**: Enables remote monitoring and control of connected appliances.
- **Safety Enhancements**: Detects and prevents excessive power consumption with overload protection.
- **Microcontroller-Based Processing**: Ensures efficient data handling and decision-making.

## System Model
### Block Diagram
- **Current Measuring Sensor**: Uses ACS712 to monitor AC current in real-time.
- **DC Power Supply**: Ensures stable operation of all components.
- **Indicator System**: Provides visual feedback on system status.
- **Microcontroller (ATMEGA328P)**: Acts as the central processing unit, handling data collection and control logic.

### Flowchart Logic
1. System initializes and displays a welcome message.
2. Continuously measures current, voltage, and power consumption.
3. Displays real-time values on the LCD.
4. Checks if the current exceeds 4A.
   - If **Yes**, activates a red LED indicating an overload condition.
   - If **No**, keeps the green LED on to signal normal operation.
5. Loops back to update and monitor energy usage continuously.

## Equations Used
- **Power Calculation**:  
  \[ P = V \times I \]
- **Energy Calculation**:  
  \[ E = P \times \Delta T \]

## Components Used
- **ACS712 Current Sensor**
- **ATMEGA328P Microcontroller**
- **16x2 LCD Display**
- **LED Indicators (Green & Red)**
- **Relay Module (for switching control)**
- **Power Supply Unit**

## Installation & Usage
1. **Hardware Setup**:
   - Connect ACS712 to measure the current.
   - Integrate the microcontroller to process the readings.
   - Attach the LCD display and LED indicators for real-time monitoring.
   - Ensure a stable power supply for operation.
2. **Software Setup**:
   - Upload the Arduino code to the ATMEGA328P.
   - Monitor real-time data via the LCD display.
   - Use LED indicators to check for overload conditions.

## Future Enhancements
- **Integration with IoT Platforms**: Enable cloud-based monitoring and control.
- **Mobile App Connectivity**: Provide a user-friendly mobile application for remote access.
- **Machine Learning for Energy Optimization**: Implement predictive analytics to suggest energy-saving measures.