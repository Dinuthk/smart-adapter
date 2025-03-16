#include <Arduino.h> /*  * ACS712 Current Sensor Demo with Power Cycling Display  * Modified from miliohm.com  */
#include "ACS712.h" 
#include <Wire.h>

// Define I2C address for LCD
#define LCD_I2C_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// LCD commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// LCD flags for display control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// LCD flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

// LCD flags for entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// PCF8574 pins connected to LCD
#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

// Define LED pins
const int greenLED = 12;
const int redLED = 13;
const float currentThreshold = 4.0; // Threshold for LED switching (in Amperes)

// Variables for LCD control
uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _backlightval;

ACS712 sensor(ACS712_05B, A0); //ACS712_20A for 20 Amp type
                              //ACS712_30A for 30 Amp type

// Variables to track energy consumption
unsigned long previousMillis = 0;
float energyConsumed = 0;  // in watt-hours
float voltage = 230.0;     // AC voltage (modify if different in your region)

// Display state control
enum DisplayState {POWER, CURRENT, ENERGY};
DisplayState currentState = POWER;
unsigned long lastStateChange = 0;
const unsigned long STATE_DURATION = 3000;  // 3 seconds per state

// LCD I2C helper functions
void expanderWrite(uint8_t data) {
  Wire.beginTransmission(LCD_I2C_ADDR);
  Wire.write(data | _backlightval);
  Wire.endTransmission();
}

void pulseEnable(uint8_t data) {
  expanderWrite(data | (1 << LCD_EN));  // Enable high
  delayMicroseconds(1);                // Enable pulse must be >450ns
  expanderWrite(data & ~(1 << LCD_EN)); // Enable low
  delayMicroseconds(50);               // Commands need > 37us to settle
}

void write4bits(uint8_t value) {
  expanderWrite(value);
  pulseEnable(value);
}

void send(uint8_t value, uint8_t mode) {
  uint8_t highnib = value & 0xf0;
  uint8_t lownib = (value << 4) & 0xf0;
  
  write4bits((highnib) | mode);
  write4bits((lownib) | mode);
}

void lcd_command(uint8_t value) {
  send(value, 0);
}

void lcd_write(uint8_t value) {
  send(value, (1 << LCD_RS));
}

void lcd_begin() {
  Wire.begin();
  _displayfunction = LCD_FUNCTIONSET | 0x08; // 4-bit mode, 2 lines, 5x8 dots
  _displaycontrol = LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  _displaymode = LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  _backlightval = LCD_BACKLIGHT;
  
  // Initialization sequence (datasheet p.46)
  delay(50);
  expanderWrite(0);  // Reset expander
  delay(1000);
  
  // 4-bit mode initialization
  write4bits(0x03 << 4);
  delayMicroseconds(4500);
  write4bits(0x03 << 4);
  delayMicroseconds(4500);
  write4bits(0x03 << 4);
  delayMicroseconds(150);
  write4bits(0x02 << 4);  // Finally, set to 4-bit interface
  
  // Set # lines, font size, etc.
  lcd_command(_displayfunction);
  
  // Turn the display on with no cursor or blinking
  lcd_command(_displaycontrol);
  
  // Clear display
  lcd_command(LCD_CLEARDISPLAY);
  delay(2);
  
  // Set entry mode
  lcd_command(_displaymode);
}

void lcd_clear() {
  lcd_command(LCD_CLEARDISPLAY);
  delay(2);  // This command takes a long time!
}

void lcd_setCursor(uint8_t col, uint8_t row) {
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if (row >= LCD_ROWS) {
    row = LCD_ROWS - 1;  // Set to last row if out of bounds
  }
  lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd_print(const char* str) {
  while (*str) {
    lcd_write(*str++);
  }
}

void lcd_print(float value) {
  char buffer[10];
  dtostrf(value, 4, 2, buffer);
  lcd_print(buffer);
}

void lcd_backlight() {
  _backlightval = LCD_BACKLIGHT;
  expanderWrite(0);
}

void setup() {
  Serial.begin(9600);
  
  // Initialize LED pins as outputs
  DDRB |= (1 << 4) | (1 << 5);  // Set DDRB bits 4 and 5 to OUTPUT
  
  sensor.calibrate();
  
  // Initialize LCD with register-level code
  lcd_begin();
  lcd_backlight();
  lcd_setCursor(0, 0);
  lcd_print("SMART ADAPTER");
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Get current measurement
  float I = sensor.getCurrentAC();
  if (I < 0.09) {
    I = 0;
  }
  
  // Control LEDs based on current measurement
  if (I <= currentThreshold) {
    // Turn on green LED, turn off red LED
    // Arduino Uno pins 12 and 13 are on PORTB (bits 4 and 5)
    PORTB |= (1 << 4);   // Set PORTB bit 4 (pin 12) to HIGH
    PORTB &= ~(1 << 5);  // Set PORTB bit 5 (pin 13) to LOW
  } else {
    // Turn off green LED, turn on red LED
    PORTB &= ~(1 << 4);  // Set PORTB bit 4 (pin 12) to LOW
    PORTB |= (1 << 5);   // Set PORTB bit 5 (pin 13) to HIGH
  }
  
  // Calculate power
  float power = I * voltage;
  
  // Calculate energy consumption (watt-hours)
  if (previousMillis > 0) {
    float timeHours = (currentMillis - previousMillis) / 3600000.0;  // convert ms to hours
    energyConsumed += power * timeHours;
  }
  previousMillis = currentMillis;
  
  // Check if it's time to change display state
  if (currentMillis - lastStateChange >= STATE_DURATION) {
    currentState = static_cast<DisplayState>((currentState + 1) % 3);
    lastStateChange = currentMillis;
    lcd_clear();  // Clear display when changing states
  }
  
  // Display information based on current state
  lcd_setCursor(0, 0);
  switch(currentState) {
    case POWER:
      lcd_print("Power: ");
      lcd_print(power);
      lcd_print(" W");
      break;
      
    case CURRENT:
      lcd_print("Current: ");
      lcd_print(I);
      lcd_print(" A");
      break;
      
    case ENERGY:
      lcd_print("Energy: ");
      lcd_print(energyConsumed / 1000);  // Convert to kWh
      lcd_print(" kWh");
      break;
  }
  
  // Show the state on second line
  lcd_setCursor(0, 1);
  switch(currentState) {
    case POWER:
      lcd_print("Measuring Power");
      break;
    case CURRENT:
      lcd_print("Measuring Current");
      break;
    case ENERGY:
      lcd_print("Power Consumed");
      break;
  }
  
  delay(300);  // Small delay for stability
}