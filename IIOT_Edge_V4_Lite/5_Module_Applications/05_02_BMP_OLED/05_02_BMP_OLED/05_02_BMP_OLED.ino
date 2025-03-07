
/**
 * @file 01_DigitalWrite_Example.ino
 * @author Mrityunjai Kumar (mrityunjai@sincgrid.com)
 * @brief
 * @version 0.1
 * @date 2022-09-20
 *
 * Copyright Sincgrid LLP
 *
 * Problem statement:- Toggle led
 *
 */

#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <IIOT_V4_Pinout.h>
#include <Wire.h>

#define SEALEVELPRESSURE_HPA (1050)
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

Adafruit_BMP280 bmp;
float temperature, humidity, pressure, altitude;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
void setup() {
    Serial.begin(115200);
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    delay(100);

    bmp.begin(0x76);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    delay(2000);
}

// long _stage_millis=0;
void loop() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);

    temperature = bmp.readTemperature();
    // humidity = bmp.readHumidity();
    pressure = bmp.readPressure() / 100.0F;
    altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

    display.print("Temp = ");
    display.print(bmp.readTemperature());
    display.println(" *C");

    display.print("Pressure = ");
    display.print(bmp.readPressure() / 100.0F);
    display.println(" hPa");

    display.print("Alt = ");
    display.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
    display.println(" m");

    // display.print("Humidity = ");
    // display.print(bmp.readHumidity());
    // display.println(" %");

    display.println();

    display.display();
    delay(1000);
}