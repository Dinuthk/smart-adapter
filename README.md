# Smart Plug-Based Energy Monitoring System

## Description
This project is a smart plug-based real-time energy monitoring system designed to help users track and optimize power consumption efficiently. The system enables plug-level energy monitoring, allowing users to identify inefficient devices and take corrective measures to minimize unnecessary power usage. 

## Features
- **Real-time Energy Monitoring**: Continuously tracks power consumption using sensors.
- **AC Current Measurement**: Utilizes the ACS712 current sensor to measure real-time alternating current (AC) flow accurately.
- **Voltage and Power Calculation**: Captures voltage levels and computes power usage.
- **Microcontroller-Based Processing**: An ATMEGA328P microcontroller processes the sensor data and manages system operations.
- **User-Friendly Display**: A 16x2 LCD shows real-time values of current, power, and energy consumption.
- **LED Indicators**: Provides visual status updates for power on/off and overload warnings.
- **Buzzer Alert**: Activates when an overcurrent condition (>4A) is detected.
- **Historical Data Storage**: Enables tracking of energy consumption over time for analysis.
- **Overload Protection**: Prevents excessive power consumption and ensures device safety.

## System Components
- **ACS712 Current Sensor**: Measures the alternating current (AC) flow.
- **ATMEGA328P Microcontroller**: Processes sensor data and ensures efficient data handling.
- **16x2 LCD Display**: Displays real-time current, power, and energy consumption.
- **LED Indicators**: Shows system status (normal operation or overload condition).
- **Buzzer**: Alerts users when overcurrent is detected (>4A).
- **Stable Power Supply**: Ensures uninterrupted operation of all components.

## System Flow

![image](https://github.com/user-attachments/assets/7425fdcb-eef0-408f-b5f2-fb3968c4ee61)

1. The system starts with an initial welcome message displayed on the LCD.
2. The current sensor measures AC flow in real-time.
3. The microcontroller calculates power and energy usage based on measured values.
4. If the measured current exceeds 4A, the system activates a buzzer and a red LED indicator for overload warning.
5. If the current remains within the safe limit, the green LED remains active.
6. The system continuously updates real-time values and monitors power consumption.

## Power and Energy Calculation
- **Power (P)** = Voltage (V) × Current (I)
- **Energy (E)** = Power (P) × Time (ΔT)

## Installation & Usage
1. Connect the smart plug to a power source and the device to be monitored.
2. Ensure the microcontroller and sensors are properly powered.
3. Monitor energy consumption on the LCD display in real-time.
4. Take action if an overload warning (LED and buzzer alert) is triggered.

## Limitations
- Requires stable internet connectivity for remote monitoring (if applicable).
- Compatibility issues may arise with older appliances.

## Future Enhancements
- Integration with IoT platforms for remote monitoring and control.
- Mobile app support for real-time data visualization.
- AI-based analysis to predict and optimize energy consumption patterns.
