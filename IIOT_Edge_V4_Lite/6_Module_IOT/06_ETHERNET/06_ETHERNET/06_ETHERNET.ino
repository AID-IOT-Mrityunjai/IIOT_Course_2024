

#include <IIOT_V4_Pinout.h>
#include <SPI.h>
#include <Wire.h>
#pragma once
#include <Arduino.h>

#if !(defined(ESP32))
#error This code is designed for (SP32_S2/3, ESP32_C3 + W5500) to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#define DEBUG_ETHERNET_WEBSERVER_PORT Serial

// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_ 0

#include <WebServer_ESP32_SC_W5500.h>

WiFiServer server(80);

// Enter a MAC address and IP address for your controller below.
#define NUMBER_OF_MAC 20

byte mac[][NUMBER_OF_MAC] =
    {
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x02},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x04},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x06},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x07},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x08},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x09},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0A},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0B},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0C},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0E},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0F},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x10},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x12},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x13},
        {0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x14},
};

// Select the IP address according to your local network
IPAddress myIP(192, 168, 0, 113);
IPAddress myGW(192, 168, 0, 1);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);

int reqCount = 0;  // number of requests received

class EthernetW5500 {
   private:
    /* data */
    long _send_millis = 0;

   public:
    EthernetW5500(/* args */);
    ~EthernetW5500();
    void init();
    void thread();
    IPAddress getIp();
};

EthernetW5500::EthernetW5500(/* args */) {
}

EthernetW5500::~EthernetW5500() {
}

void EthernetW5500::init() {
    Serial.print(F("\nStart WebServer on "));
    Serial.print(ARDUINO_BOARD);
    Serial.print(F(" with "));
    Serial.println(SHIELD_TYPE);
    Serial.println(WEBSERVER_ESP32_SC_W5500_VERSION);

    ET_LOGWARN(F("Default SPI pinout:"));
    ET_LOGWARN1(F("SPI_HOST:"), ETH_SPI_HOST);
    ET_LOGWARN1(F("MOSI:"), MOSI_GPIO);
    ET_LOGWARN1(F("MISO:"), MISO_GPIO);
    ET_LOGWARN1(F("SCK:"), SCK_GPIO);
    ET_LOGWARN1(F("CS:"), CS_GPIO);
    ET_LOGWARN1(F("INT:"), INT_GPIO);
    ET_LOGWARN1(F("SPI Clock (MHz):"), SPI_CLOCK_MHZ);
    ET_LOGWARN(F("========================="));

    ///////////////////////////////////

    // To be called before ETH.begin()
    ESP32_W5500_onEvent();

    // start the ethernet connection and the server:
    // Use DHCP dynamic IP and random mac
    uint16_t index = millis() % NUMBER_OF_MAC;

    // bool begin(int MISO_GPIO, int MOSI_GPIO, int SCLK_GPIO, int CS_GPIO, int INT_GPIO, int SPI_CLOCK_MHZ,
    //            int SPI_HOST, uint8_t *W5500_Mac = W5500_Default_Mac);
    // ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST );
    //  ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST, mac[index] );
    ETH.begin(PIN_ETH_MISO, PIN_ETH_MOSI, PIN_ETH_SCLK, PIN_ETH_CS, PIN_ETH_INT, SPI_CLOCK_MHZ, ETH_SPI_HOST, mac[index]);

    // Static IP, leave without this line to get IP via DHCP
    // bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
    // ETH.config(myIP, myGW, mySN, myDNS);

    ESP32_W5500_waitForConnect();

    ///////////////////////////////////

    // start the web server on port 80
    server.begin();
}
IPAddress EthernetW5500::getIp() {
    return ETH.localIP();
    // return "0";
}
void EthernetW5500::thread() {
    // listen for incoming clients
    WiFiClient client = server.available();

    if (client) {
        Serial.println(F("New client"));
        // an http request ends with a blank line
        bool currentLineIsBlank = true;

        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);

                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if (c == '\n' && currentLineIsBlank) {
                    Serial.println(F("Sending response"));

                    // send a standard http response header
                    // use \r\n instead of many println statements to speedup data send
                    client.print(
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Connection: close\r\n"  // the connection will be closed after completion of the response
                        "Refresh: 20\r\n"        // refresh the page automatically every 20 sec
                        "\r\n");
                    client.print("<!DOCTYPE HTML>\r\n");
                    client.print("<html>\r\n");
                    client.print(String("<h2>Hello World from ") + BOARD_NAME + "!</h2>\r\n");
                    client.print("Requests received: ");
                    client.print(++reqCount);
                    client.print("<br>\r\n");
                    client.print("<br>\r\n");
                    client.print("</html>\r\n");
                    break;
                }

                if (c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }

        // give the web browser time to receive the data
        delay(10);

        // close the connection:
        client.stop();
        Serial.println(F("Client disconnected"));
    }
}
EthernetW5500 eth;
void setup() {
    eth.init();
    Serial.println(ETH.localIP());
    log_w("%s", ETH.localIP().toString());
}

void loop() {
    eth.thread();
}