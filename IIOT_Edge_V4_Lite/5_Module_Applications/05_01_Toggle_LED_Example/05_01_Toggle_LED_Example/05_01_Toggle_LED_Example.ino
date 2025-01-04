
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

#include <Arduino.h>
#include <Ws2812b_led.h>
ws2812b_led led;
void setup() {
    led.init();
}

void loop() {
    // put your main code here, to run repeatedly:
}