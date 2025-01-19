

#include <IIOT_V4_Pinout.h>
#include <SPI.h>
#include <Wire.h>

#define SerialSim Serial1
#define TINY_GSM_MODEM_A7672X
void setup() {
    // BUTTON
    Serial.begin(115200);
    pinMode(PIN_BUTTON_ON_BOARD, INPUT);
    pinMode(PIN_BUZZER, OUTPUT);

  
    pinMode(PIN_GSM_PWR_KEY, OUTPUT);
    digitalWrite(PIN_GSM_PWR_KEY, LOW);
    delay(1000);
    digitalWrite(PIN_GSM_PWR_KEY, HIGH);
    delay(10);
    SerialSim.begin(
        115200,
        SERIAL_8N1,
        PIN_SIM_TX,
        PIN_SIM_RX,
        false);

    for (int _x = 0; _x < 10; _x++) {
        SerialSim.write("AT\r\n");
        if (SerialSim.available()) {
            String x = SerialSim.readStringUntil('\r');
            // Serial.println(x);
            log_w("%s", x);
            if (x.indexOf("OK") > 0) {
                log_w("SIM module Found");
                break;
            }
            delay(200);
        } else {
            delay(2000);
        }
        log_w("sim...");
    }
}

void loop() {
    if (SerialSim.available())
        Serial.write(SerialSim.read());
    if (Serial.available())
        SerialSim.write(Serial.read());
}