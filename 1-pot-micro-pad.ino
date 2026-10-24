#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// OLED display width and height, in pixels
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Servo myServo;

const int potPin = A0;   // Potentiometer connected to A0
const int servoPin = 9;  // Servo control pin

int lastPotValue = 0;
int lastAngle = -1;

void setup() {
  // 1. Boot Delay to stabilize power and prevent Nano restart loops from servo inrush current
  delay(1500);

  // Initialize Serial for debugging (optional)
  Serial.begin(9600);

  // Initialize OLED display with I2C address 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true)
      ;  // Don't proceed, loop forever if display fails
  }

  // Clear buffer and show splash/boot screen
  display.clearDisplay();
  drawBootScreen();

  // Attach the servo after the boot delay
  myServo.attach(servoPin);
}

void loop() {
  // Read potentiometer value (0 to 1023)
  int potValue = analogRead(potPin);

  // Map the potentiometer value to servo angle (0 to 180 degrees) using full ADC range
  int servoAngle = map(potValue, 0, 1023, 0, 180);

  // Calculate percentage using full ADC range
  int percentage = map(potValue, 0, 1023, 0, 100);

  // Move the servo
  myServo.write(servoAngle);

  // Update OLED only if values change to reduce flickering
  if (abs(potValue - lastPotValue) > 2) {
    lastPotValue = potValue;
    lastAngle = servoAngle;

    display.clearDisplay();

    // Draw Cyber/Tech UI Border
    drawUIBorder();

    // Title / Header
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(16, 8);
    display.print(F("SERVO COMMAND UI"));

    // Divider line
    display.drawLine(10, 18, 118, 18, SSD1306_WHITE);

    // Main Angle Display (Large Font)
    display.setTextSize(2);
    display.setCursor(18, 26);
    display.print(servoAngle);
    display.setTextSize(1);
    display.print(F(" DEG"));

    // Sub-info: Percentage & Raw ADC
    display.setCursor(18, 46);
    display.print(F("PWR:"));
    display.print(percentage);
    display.print(F("%  ADC:"));
    display.print(potValue);

    display.display();
  }

  // Small delay for smooth ADC reading stability
  delay(15);
}

void drawUIBorder() {
  // Outer double border for tech aesthetic
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.drawRect(2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4, SSD1306_WHITE);

  // Corner accent marks
  display.drawPixel(1, 1, SSD1306_BLACK);
  display.drawPixel(SCREEN_WIDTH - 2, 1, SSD1306_BLACK);
  display.drawPixel(1, SCREEN_HEIGHT - 2, SSD1306_BLACK);
  display.drawPixel(SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2, SSD1306_BLACK);
}

void drawBootScreen() {
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(22, 20);
  display.print(F("SYSTEM BOOT..."));
  display.setCursor(18, 36);
  display.print(F("STABILIZING PWR"));
  display.display();
  delay(1000);
}