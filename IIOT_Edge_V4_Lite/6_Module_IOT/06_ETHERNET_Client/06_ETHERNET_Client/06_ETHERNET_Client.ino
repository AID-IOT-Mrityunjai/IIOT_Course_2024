/****************************************************************************************************************************
  WebClient.ino - Simple Arduino web server sample for Ethernet shield

  For Ethernet shields using ESP32_SC_W5500 (ESP32_S2/3, ESP32_C3 + LwIP W5500)

  WebServer_ESP32_SC_W5500 is a library for the ESP32_S2/3, ESP32_C3 with LwIP Ethernet W5500

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/WebServer_ESP32_SC_W5500
  Licensed under GPLv3 license
 *****************************************************************************************************************************/

/************************************
 * @brief pinouts - IIOT_V4
*************************************/

/**
 * @brief Ethernet connection
 */
#define PIN_ETH_CS 15
#define PIN_ETH_SCLK 10
#define PIN_ETH_MISO 11
#define PIN_ETH_MOSI 12
#define PIN_ETH_INT  13
#define PIN_ETH_RST  14


/*******
 * RS485 pinouts
 *******/

#define PIN_RS485_1_RX 5
#define PIN_RS485_1_TX 6
#define PIN_RS485_1_RTS 7

#define PIN_RS485_2_RX 40
#define PIN_RS485_2_TX 41
#define PIN_RS485_2_RTS 42

#define PIN_WS2812B 38


#define PIN_SD_CARD_CLK  10
#define PIN_SD_CARD_MISO 11
#define PIN_SD_CARD_MOSI 12
#define PIN_SD_CARD_CS 36    


#define PIN_I2C_SDA 1
#define PIN_I2C_SCL 2


/**
 * @brief 4G Module 
 * 
 */

// #define PIN_GSM_STATUS          //NOT CONNECTED
#define PIN_GSM_PWR_KEY 21
#define PIN_SIM_RX      17
#define PIN_SIM_TX      18



// For fast led
#define PIN_LED_WS2812_DATA 38

#define PIN_BUTTON_ON_BOARD 4

#define PIN_BUZZER 16

#if !( defined(ESP32) )
  #error This code is designed for (SP32_S2/3, ESP32_C3 + W5500) to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#define DEBUG_ETHERNET_WEBSERVER_PORT       Serial

// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_       3

//////////////////////////////////////////////////////////

// Optional values to override default settings
// Don't change unless you know what you're doing
//#define ETH_SPI_HOST        SPI3_HOST
//#define SPI_CLOCK_MHZ       25

// Must connect INT to GPIOxx or not working
//#define INT_GPIO            4

//#define MISO_GPIO           13
//#define MOSI_GPIO           11
//#define SCK_GPIO            12
//#define CS_GPIO             10

//////////////////////////////////////////////////////////

#include <WebServer_ESP32_SC_W5500.h>

// Enter a MAC address and IP address for your controller below.
#define NUMBER_OF_MAC      20

byte mac[][NUMBER_OF_MAC] =
{
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x02 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x04 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x06 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x07 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x08 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x09 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0A },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0B },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0C },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0E },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0F },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x10 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x12 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x13 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x14 },
};

// Select the IP address according to your local network
IPAddress myIP(192, 168, 0, 232);
IPAddress myGW(192, 168, 0, 1);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);

char server[] = "arduino.tips";

// Initialize the Ethernet client object
WiFiClient client;

void setup()
{
  Serial.begin(115200);

  while (!Serial && (millis() < 5000));

  delay(500);

  Serial.print(F("\nStart WebClientRepeating on "));
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
  //bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
  // KH, Dynamic not OK yet
  //ETH.config(myIP, myGW, mySN, myDNS);

  ESP32_W5500_waitForConnect();

  ///////////////////////////////////

  Serial.println();
  Serial.println(F("Starting connection to server..."));

  // if you get a connection, report back via serial
  if (client.connect(server, 80))
  {
    Serial.println(F("Connected to server"));
    // Make a HTTP request
    client.println(F("GET /asciilogo.txt HTTP/1.1"));
    client.println(F("Host: arduino.tips"));
    client.println(F("Connection: close"));
    client.println();
  }
}

void printoutData()
{
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available())
  {
    char c = client.read();
    Serial.write(c);
    Serial.flush();
  }
}

void loop()
{
  printoutData();

  // if the server's disconnected, stop the client
  if (!client.connected())
  {
    Serial.println();
    Serial.println(F("Disconnecting from server..."));
    client.stop();

    // do nothing forever
    while (true)
      yield();
  }
}
