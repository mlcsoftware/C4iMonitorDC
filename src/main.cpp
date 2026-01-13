/****************************************************************************************************************************
  AdvancedWebServer.h

  For Ethernet shields using ESP32_ENC (ESP32 + ENC28J60)

  WebServer_ESP32_ENC is a library for the ESP32 with Ethernet ENC28J60 to run WebServer

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/WebServer_ESP32_ENC
  Licensed under GPLv3 license

  Copyright (c) 2015, Majenko Technologies
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  Neither the name of Majenko Technologies nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************************************************************/

 #if !( defined(ESP32) )
 #error This code is designed for (ESP32 + ENC28J60) to run on ESP32 platform! Please check your Tools->Board setting.
#endif

#define SYSTEM_NAME "C4i MONITOR DC"
#define DEVICE_NAME "MONITOR DC"
#define DEBUG_ETHERNET_WEBSERVER_PORT       Serial

// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_       3

#define DEBUG_MODE                          0
#define SESSION_TIMEOUT_SEG                 600

//////////////////////////////////////////////////////////

// Optional values to override default settings
//#define ETH_SPI_HOST        SPI3_HOST
//#define SPI_CLOCK_MHZ       20

// Must connect INT to GPIOxx or not working
#define INT_GPIO            4

#define MISO_GPIO           19
#define MOSI_GPIO           23
#define SCK_GPIO            18
#define CS_GPIO             5

#define ADC_CH1             A0
#define ADC_CH2             A3
#define ADC_CH3             A6
#define ADC_CH4             A7
#define FLASH_INIT_NUMBER   5
#define FLASH_DELAY         300

#define OID_CH1             ".1.3.6.1.64047.1.1.1"
#define OID_CH2             ".1.3.6.1.64047.1.1.2"
#define OID_CH3             ".1.3.6.1.64047.1.2.3"
#define OID_CH4             ".1.3.6.1.64047.1.2.4"
#define OID_VDIFF           ".1.3.6.1.64047.1.2.5"
#define OID_TRAP_MAX_CH1    ".1.3.6.1.64047.2.1.1"
#define OID_TRAP_MIN_CH1    ".1.3.6.1.64047.2.1.0"
#define OID_TRAP_MAX_CH2    ".1.3.6.1.64047.2.2.1"
#define OID_TRAP_MIN_CH2    ".1.3.6.1.64047.2.2.0"
#define OID_TRAP_MAX_CH3    ".1.3.6.1.64047.2.3.1"
#define OID_TRAP_MIN_CH3    ".1.3.6.1.64047.2.3.0"
#define OID_TRAP_MAX_CH4    ".1.3.6.1.64047.2.4.1"
#define OID_TRAP_MIN_CH4    ".1.3.6.1.64047.2.4.0"
#define OID_TRAP_CH1_SAFE   ".1.3.6.1.64047.3.1.0"
#define OID_TRAP_CH2_SAFE   ".1.3.6.1.64047.3.2.0"
#define OID_TRAP_CH3_SAFE   ".1.3.6.1.64047.3.3.0"
#define OID_TRAP_CH4_SAFE   ".1.3.6.1.64047.3.4.0"

#define OID_TICK            ".1.3.6.1.64047.1.9.0"

#define BUFFER_AVG          100
#define CURRENT_MAX         150
#define HISTERESIS_CURRENT  2
#define HISTERESIS_VOLTAGE  2
#define THRESHOLD_CURRENT   1

#define LED                 2

//////////////////////////////////////////////////////////

//#include "core/globals.h"
#include <ArduinoJson.h>
#include <SNMP_Agent.h>
#include <SNMPTrap.h>
#include <Cli.h>

WebServer server(80);
WiFiUDP udp;
SNMPAgent snmp = SNMPAgent("public", "private");

SNMPTrap* trapCH1Max = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);
SNMPTrap* trapCH1Min = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);
SNMPTrap* trapCH2Max = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);
SNMPTrap* trapCH2Min = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);
SNMPTrap* trapCH3Max = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);
SNMPTrap* trapCH3Min = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);
SNMPTrap* trapCH4Max = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);
SNMPTrap* trapCH4Min = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);

SNMPTrap* trapCH1Safe = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);
SNMPTrap* trapCH2Safe = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);
SNMPTrap* trapCH3Safe = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);
SNMPTrap* trapCH4Safe = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);

SNMPTrap* trapVDiffMax = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);
SNMPTrap* trapVDiffMin = new SNMPTrap(glbConfig.SNMP_COMMUNITY().c_str(), SNMP_VERSION_2C);

ValueCallback* currentCH1OID;
ValueCallback* currentCH2OID;
ValueCallback* voltageCH3OID;
ValueCallback* voltageCH4OID;
ValueCallback* voltageDiffOID;

ValueCallback* changingNumberOID;
ValueCallback* settableNumberOID;
TimestampCallback* timestampCallbackOID;
Cli cli;

// Numbers used to response to Get requests
int changingNumber = 1;
int settableNumber = 0;
uint32_t tensOfMillisCounter = 0;
int snmpCurrentCH1 = 0;
int snmpCurrentCH2 = 0;
int snmpVoltageCH3 = 0;
int snmpVoltageCH4 = 0;
int snmpVoltageDiff = 0;
int snmpPower1 = 0;
int snmpPower2 = 0;
bool serverInitiated = false;
bool trapMaxSent[5];
bool trapMinSent[5];
bool trapVDiffMaxSent = false;
bool trapVDiffMinSent = false;
float ch1Buffer[BUFFER_AVG];
float ch2Buffer[BUFFER_AVG];
float ch3Buffer[BUFFER_AVG];
float ch4Buffer[BUFFER_AVG];
int ch1BufferCounter = 0;
int ch2BufferCounter = 0;
int ch3BufferCounter = 0;
int ch4BufferCounter = 0;

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

int reqCount = 0;                // number of requests received

float CalculateAdc(uint16_t x, uint8_t channel);
void SetIPConfig();
void LoadServer();
void WriteLog(String message, bool linebreak);
bool is_authenticated();
void handleLogin();
void handleConfig();
void handleResetPassword();
void handleReboot();
void handleReset();

String HtmlStyle = F("\
      <style>\
          body {\
              margin: 0;\
              font-family: Arial, sans-serif;\
          }\
          header {\
              background-color: #333;\
              color: #fff;\
              padding: 10px;\
          }\
          nav ul {\
              list-style: none;\
              padding: 0;\
              margin: 0;\
          }\
          nav li {\
              display: inline; /* Hace que los ítems del menú estén en una línea horizontal */\
              padding-left: 20px; /* Espacio entre los ítems del menú */\
              padding-right: 20px;\
              margin: auto;\
          }\
          nav a {\
              text-decoration: none;\
              color: #fff;\
              font-size: 16px;\
          }\
          nav a.active{\
              color: yellow;\
          }\
          nav a:hover {\
              text-decoration: underline;\
              cursor: pointer;\
          }\
          .contenedor-center{\
              display: flex;\
              align-items: center;\
              justify-content: center;\
          }\
          .title{\
              margin-left: 20px;\
          }\
          .subtitle{\
              background-color: black;\
              color: white;\
              padding-left: 20px;\
          }\
          .parameter-container{\
              display: flex;\
              padding-left: 100px;\
              align-items: center;\
              margin-top: 5px;\
              margin-bottom: 5px;\
          }\
          .parameter-container > label{\
              width: 250px;\
          }\
          .octet{\
              width: 50px;\
              text-align: center;\
          }\
          .link{\
              text-decoration: underline;\
              color: blue;\
          }\
          input{\
            padding: 2px 10px;margin:2px;font-size:1rem;\
          }\
          form > button{\
              margin-left: 20px;\
          }\
          .text-danger{ color: red;margin-left:3rem }\
          .text-success{ color: green;margin-left:3rem }\
      </style>\
  ");
String HtmlTitle = "<title>MONITOR DC</title>";
String GetHtmlMenu(String menu){
  String HtmlMenu = "\
        <header> \
          <nav><ul>\
            <li><a href=\"/\" class=\"#INICIO#\">INICIO</a></li>\
            <li><a href=\"monitor\" class=\"#MONITOR#\">MONITOREO</a></li>\
            <li><a href=\"configuration\" class=\"#CONFIGURATION#\">CONFIGURACION</a></li>\
            <li><a href=\"/login?DISCONNECT=YES\">DESCONECTARSE</a></li>\
          </ul></nav> \
        </header> \
  ";
  HtmlMenu.replace("#"+ menu +"#", "active");

  return HtmlMenu;
}

void handleRoot()
{
  String content = "\
  <html>\
    <head>\
      " + HtmlTitle + HtmlStyle + "\
    </head>\
    <body>\
      " + GetHtmlMenu("INICIO") + "\
      <div class='contenedor-center' style='height:100vh;'>\
        <div>\
          <p style='font-size:5rem;margin-bottom:0.5rem;text-align:center;'>C4i</p>\
          <h2>Sistema de monitoreo DC</h2>\
        </div>\
      </div>\
    </body>\
  </html>";

  String header;

  if (!is_authenticated())
  {
    server.sendHeader(F("Location"), F("/login"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.send(301);
    return;
  }
  server.send(200, F("text/html"), content);
}

void handleMonitor(){
  
  if (!is_authenticated())
  {
    server.sendHeader(F("Location"), F("/login"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.send(301);
    return;
  }

  String data;
  String html ="\
  <!DOCTYPE html>\
    <html lang=\"en\">\
      <head>\
        <meta http-equiv='refresh' content='5'/>\
        <meta charset=\"UTF-8\">\
        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
        " + HtmlTitle + HtmlStyle + "\
      </head>\
      <body> \
        "+ GetHtmlMenu("MONITOR") +"\
        <h1 class=\"title\">Valores de monitoreo</h1> \
        <h3 class=\"subtitle\">Parametros</h3> \
        <div class=\"parameter-container\"><label>Corriente Canal 1 [A]:</label> <p>{CH1}</p> </div> \
        <div class=\"parameter-container\"> <label>Corriente Canal 2 [A]:</label> <p>{CH2}</p> </div> \
        <div class=\"parameter-container\"> <label>Tensión Canal 3 [V]:</label> <p>{CH3}</p> </div> \
        <div class=\"parameter-container\"> <label>Tensión Canal 4 [V]:</label> <p>{CH4}</p> </div>\
        <hr /><a href=\"monitor\">Refrescar</a>\
      </body> \
    </html>";
  
  // CH1
  data = String(CurrentCH1, 3);
  html.replace("{CH1}", data);
  // CH2
  data = String(CurrentCH2, 3);
  html.replace("{CH2}", data);
  // CH3
  data = String(VoltageCH3, 3);
  html.replace("{CH3}", data);
  // CH4
  data = String(VoltageCH4, 3);
  html.replace("{CH4}", data);
  // CHDIFF
  data = String(VoltageCH4 - VoltageCH3, 3);
  html.replace("{CHDIFF}", data);
  // POWER1
  data = String(VoltageCH3 * CurrentCH1, 3);
  html.replace("{POWER1}", data);
  // POWER2
  data = String(VoltageCH4 * CurrentCH2, 3);
  html.replace("{POWER2}", data);

  server.send(200, F("text/html"), html);
}

void handleNotFound()
{
  String message = F("File Not Found\n\n");

  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, F("text/plain"), message);
}

void handlePostKeepAlive(){

}

void drawGraph()
{
  String out;
  out.reserve(3000);
  char temp[70];

  out += F("<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"310\" height=\"150\">\n");
  out += F("<rect width=\"310\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"2\" stroke=\"rgb(0, 0, 0)\" />\n");
  out += F("<g stroke=\"blue\">\n");
  int y = rand() % 130;

  for (int x = 10; x < 300; x += 10)
  {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"2\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }

  out += F("</g>\n</svg>\n");

  server.send(200, F("image/svg+xml"), out);
}

void setup()
{
  Serial.begin(115200);
  pinMode(A6, INPUT);
  pinMode(LED, OUTPUT);

  // inicializa los flags de traps
  for(int i=0;i<5;i++)
  {
    trapMaxSent[i]=false;
    trapMinSent[i]=false;
  }
  // Inicializa los buffers
  for(int i=0;i<BUFFER_AVG;i++)
  {
    ch1Buffer[i] = 0;
    ch2Buffer[i] = 1;
    ch3Buffer[i] = 2;
    ch4Buffer[i] = 3;
  }

  EEPROM.begin(1000);

  // Flash inicio
  for(int i=0;i<FLASH_INIT_NUMBER;i++){
    digitalWrite(LED, HIGH);
    delay(FLASH_DELAY);
    digitalWrite(LED,LOW);
    delay(FLASH_DELAY);
  }

  while (!Serial && (millis() < 5000));

  /*

  Serial.print(F("\nStart AdvancedWebServer on "));
  Serial.print(F(" with "));
  Serial.println(SHIELD_TYPE);
  Serial.println(WEBSERVER_ESP32_ENC_VERSION);

  ET_LOGWARN(F("Default SPI pinout:"));
  ET_LOGWARN1(F("MOSI:"), MOSI_GPIO);
  ET_LOGWARN1(F("MISO:"), MISO_GPIO);
  ET_LOGWARN1(F("SCK:"),  SCK_GPIO);
  ET_LOGWARN1(F("CS:"),   CS_GPIO);
  ET_LOGWARN1(F("INT:"),  INT_GPIO);
  ET_LOGWARN1(F("SPI Clock (MHz):"), SPI_CLOCK_MHZ);
  ET_LOGWARN(F("========================="));

  */

  ///////////////////////////////////

  // To be called before ETH.begin()
  ESP32_ENC_onEvent();

  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  //bool begin(int MISO_GPIO, int MOSI_GPIO, int SCLK_GPIO, int CS_GPIO, int INT_GPIO, int SPI_CLOCK_MHZ,
  //           int SPI_HOST, uint8_t *W6100_Mac = W6100_Default_Mac);
  ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST );
  //ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST, mac[millis() % NUMBER_OF_MAC] );

  SetIPConfig();

  if(ESP32_ENC_isConnected())
    LoadServer();

  digitalWrite(LED, HIGH);

  cli.begin();
}

void LoadServer(){
    ///////////////////////////////////
    Serial.println("Iniciando servidor");

    server.on(F("/"), handleRoot);
    server.on(F("/monitor"), handleMonitor);
    server.on(F("/login"), handleLogin);
    server.on(F("/configuration"), handleConfig);
    server.on(F("/resetpassword"), handleResetPassword);
    server.on(F("/reboot"), handleReboot);
    server.on(F("/reset"), handleReset);
    server.on(F("/keepalive"), HTTP_POST, handlePostKeepAlive);

    server.onNotFound(handleNotFound);
    //here the list of headers to be recorded
    const char * headerkeys[] = {"User-Agent", "Cookie"} ;
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);

    //ask server to track these headers
    server.collectHeaders(headerkeys, headerkeyssize);
    server.begin();

    Serial.print(F("HTTP EthernetWebServer is @ IP : "));
    Serial.println(ETH.localIP());

    // Habilita snmp
    snmp.setUDP(&udp);
    snmp.begin();

    // add 'callback' for an OID - pointer to an integer
    currentCH1OID = snmp.addIntegerHandler(OID_CH1, &snmpCurrentCH1);
    currentCH2OID = snmp.addIntegerHandler(OID_CH2, &snmpCurrentCH2);
    voltageCH3OID = snmp.addIntegerHandler(OID_CH3, &snmpVoltageCH3);
    voltageCH4OID = snmp.addIntegerHandler(OID_CH4, &snmpVoltageCH4);
  
    // Setup SNMP TRAP
    // The SNMP Trap spec requires an uptime counter to be sent along with the trap.
    timestampCallbackOID = (TimestampCallback*)snmp.addTimestampHandler(OID_TICK, &tensOfMillisCounter);

    trapCH1Max->setUDP(&udp); // give a pointer to our UDP object
    trapCH1Max->setTrapOID(new OIDType(OID_TRAP_MAX_CH1)); // OID of the trap
    trapCH1Max->setSpecificTrap(1); 
    trapCH2Max->setUDP(&udp); // give a pointer to our UDP object
    trapCH2Max->setTrapOID(new OIDType(OID_TRAP_MAX_CH2)); // OID of the trap
    trapCH2Max->setSpecificTrap(1); 
    trapCH3Max->setUDP(&udp); // give a pointer to our UDP object
    trapCH3Max->setTrapOID(new OIDType(OID_TRAP_MAX_CH3)); // OID of the trap
    trapCH3Max->setSpecificTrap(1); 
    trapCH4Max->setUDP(&udp); // give a pointer to our UDP object
    trapCH4Max->setTrapOID(new OIDType(OID_TRAP_MAX_CH4)); // OID of the trap
    trapCH4Max->setSpecificTrap(1); 
    trapCH1Min->setUDP(&udp); // give a pointer to our UDP object
    trapCH1Min->setTrapOID(new OIDType(OID_TRAP_MIN_CH1)); // OID of the trap
    trapCH1Min->setSpecificTrap(1); 
    trapCH2Min->setUDP(&udp); // give a pointer to our UDP object
    trapCH2Min->setTrapOID(new OIDType(OID_TRAP_MIN_CH2)); // OID of the trap
    trapCH2Min->setSpecificTrap(1); 
    trapCH3Min->setUDP(&udp); // give a pointer to our UDP object
    trapCH3Min->setTrapOID(new OIDType(OID_TRAP_MIN_CH3)); // OID of the trap
    trapCH3Min->setSpecificTrap(1); 
    trapCH4Min->setUDP(&udp); // give a pointer to our UDP object
    trapCH4Min->setTrapOID(new OIDType(OID_TRAP_MIN_CH4)); // OID of the trap
    trapCH4Min->setSpecificTrap(1); 

    trapCH1Safe->setUDP(&udp); // give a pointer to our UDP object
    trapCH1Safe->setTrapOID(new OIDType(OID_TRAP_CH1_SAFE)); // OID of the trap
    trapCH1Safe->setSpecificTrap(1); 
    trapCH2Safe->setUDP(&udp); // give a pointer to our UDP object
    trapCH2Safe->setTrapOID(new OIDType(OID_TRAP_CH2_SAFE)); // OID of the trap
    trapCH2Safe->setSpecificTrap(1); 
    trapCH3Safe->setUDP(&udp); // give a pointer to our UDP object
    trapCH3Safe->setTrapOID(new OIDType(OID_TRAP_CH3_SAFE)); // OID of the trap
    trapCH3Safe->setSpecificTrap(1); 
    trapCH4Safe->setUDP(&udp); // give a pointer to our UDP object
    trapCH4Safe->setTrapOID(new OIDType(OID_TRAP_CH4_SAFE)); // OID of the trap

    // Set the uptime counter to use in the trap (required)
    trapCH1Max->setUptimeCallback(timestampCallbackOID);
    trapCH1Min->setUptimeCallback(timestampCallbackOID);
    trapCH2Max->setUptimeCallback(timestampCallbackOID);
    trapCH2Min->setUptimeCallback(timestampCallbackOID);
    trapCH3Max->setUptimeCallback(timestampCallbackOID);
    trapCH3Min->setUptimeCallback(timestampCallbackOID);
    trapCH4Max->setUptimeCallback(timestampCallbackOID);
    trapCH4Min->setUptimeCallback(timestampCallbackOID);
    trapVDiffMax->setUptimeCallback(timestampCallbackOID);
    trapVDiffMin->setUptimeCallback(timestampCallbackOID);

    trapCH1Safe->setUptimeCallback(timestampCallbackOID);
    trapCH2Safe->setUptimeCallback(timestampCallbackOID);
    trapCH3Safe->setUptimeCallback(timestampCallbackOID);
    trapCH4Safe->setUptimeCallback(timestampCallbackOID);
    trapVDiffMax->setUptimeCallback(timestampCallbackOID);
    trapVDiffMin->setUptimeCallback(timestampCallbackOID);

    // Set some previously set OID Callbacks to send these values with the trap (optional)
    trapCH1Max->addOIDPointer(currentCH1OID);
    trapCH1Min->addOIDPointer(currentCH1OID);
    trapCH2Max->addOIDPointer(currentCH2OID);
    trapCH2Min->addOIDPointer(currentCH2OID);
    trapCH3Max->addOIDPointer(voltageCH3OID);
    trapCH3Min->addOIDPointer(voltageCH3OID);
    trapCH4Max->addOIDPointer(voltageCH4OID);
    trapCH4Min->addOIDPointer(voltageCH4OID);

    trapCH1Safe->addOIDPointer(currentCH1OID);
    trapCH2Safe->addOIDPointer(currentCH2OID);
    trapCH3Safe->addOIDPointer(voltageCH3OID);
    trapCH4Safe->addOIDPointer(voltageCH4OID);

    trapCH1Max->setIP(ETH.localIP()); // Set our Source IP
    trapCH1Min->setIP(ETH.localIP()); // Set our Source IP
    trapCH2Max->setIP(ETH.localIP()); // Set our Source IP
    trapCH2Min->setIP(ETH.localIP()); // Set our Source IP
    trapCH3Max->setIP(ETH.localIP()); // Set our Source IP
    trapCH3Min->setIP(ETH.localIP()); // Set our Source IP
    trapCH4Max->setIP(ETH.localIP()); // Set our Source IP
    trapCH4Min->setIP(ETH.localIP()); // Set our Source IP

    trapCH1Safe->setIP(ETH.localIP()); // Set our Source IP
    trapCH2Safe->setIP(ETH.localIP()); // Set our Source IP
    trapCH3Safe->setIP(ETH.localIP()); // Set our Source IP
    trapCH4Safe->setIP(ETH.localIP()); // Set our Source IP

    // Ensure to sortHandlers after adding/removing and OID callbacks - this makes snmpwalk work
    snmp.sortHandlers();
    
    Serial.println("SNMP Iniciado");

    serverInitiated = true;
}

void check_status()
{
  byte octets[4];
  glbConfig.GetIpConfig(IP_TRAP_ENUM, octets);
  //IPAddress destinationIP = IPAddress(octets[0],octets[0],octets[0],octets[0]);
  IPAddress destinationIP = IPAddress(octets[0],octets[1],octets[2],octets[3]);
  float value = 0;
  for(int i=0;i<4;i++){
    value = i == 0 ? CurrentCH1 : value;
    value = i == 1 ? CurrentCH2 : value;
    value = i == 2 ? VoltageCH3 : value;
    value = i == 3 ? VoltageCH4 : value;
    value = i == 4 ? VoltageCH3 - VoltageCH4 : value;
    SNMPTrap *trapMax = 0;
    SNMPTrap *trapMin = 0;
    SNMPTrap *trapSafe = 0;
    trapMax = i == 0 ? trapCH1Max : trapMax;
    trapMin = i == 0 ? trapCH1Min : trapMin;
    trapSafe = i == 0 ? trapCH1Safe : trapSafe;
    trapMax = i == 1 ? trapCH2Max : trapMax;
    trapMin = i == 1 ? trapCH2Min : trapMin;
    trapSafe = i == 1 ? trapCH2Safe : trapSafe;
    trapMax = i == 2 ? trapCH3Max : trapMax;
    trapMin = i == 2 ? trapCH3Min : trapMin;
    trapSafe = i == 2 ? trapCH3Safe : trapSafe;
    trapMax = i == 3 ? trapCH4Max : trapMax;
    trapMin = i == 3 ? trapCH4Min : trapMin;
    trapSafe = i == 3 ? trapCH4Safe : trapSafe;

    float max = glbConfig.THRESHOLD_MAX(i+1);
    float min = glbConfig.THRESHOLD_MIN(i+1);
    if(!trapMaxSent[i] && value > max){
       if(snmp.sendTrapTo(trapMax, destinationIP, true, 2, 5000) != INVALID_SNMP_REQUEST_ID){ 
          Serial.println("");
          Serial.print("Sent SNMP Trap Max CH");
          Serial.println(i+1);
          trapMaxSent[i] = true;
      } else {
          Serial.println("Couldn't send SNMP Trap");
      }
    }
    if(trapMaxSent[i] && value <= (max - HISTERESIS_CURRENT)){
      if(snmp.sendTrapTo(trapSafe, destinationIP, true, 2, 5000) != INVALID_SNMP_REQUEST_ID){ 
        Serial.println("");
        Serial.print("Sent SNMP Trap SAFE CH");
        Serial.println(i+1);
        Serial.println(destinationIP);
        trapMaxSent[i] = false;
      } else {
        Serial.println("Couldn't send SNMP Trap");
      }
    }

    if(!trapMinSent[i] && value < min){
       if(snmp.sendTrapTo(trapMin, destinationIP, true, 2, 5000) != INVALID_SNMP_REQUEST_ID){ 
          Serial.println("");
          Serial.print("Sent SNMP Trap Min CH");
          Serial.println(i+1);
          Serial.println(destinationIP);
          trapMinSent[i] = true;
      } else {
          Serial.println("Couldn't send SNMP Trap");
      }
    }
    if(trapMinSent[i] && value >= (min + HISTERESIS_CURRENT)){
      if(snmp.sendTrapTo(trapSafe, destinationIP, true, 2, 5000) != INVALID_SNMP_REQUEST_ID){ 
        Serial.println("");
        Serial.print("Sent SNMP Trap SAFE CH");
        Serial.println(i+1);
        Serial.println(destinationIP);
        trapMinSent[i] = false;
      } else {
        Serial.println("Couldn't send SNMP Trap");
      }

    }
  }
}

void loop()
{
  
  // Lee la linea de comando
  cli.Read();

  CurrentCH1 = CalculateAdc(analogRead(ADC_CH1), 1);
  CurrentCH2 = CalculateAdc(analogRead(ADC_CH2), 2);
  VoltageCH3 = CalculateAdc(analogRead(ADC_CH3), 3);
  VoltageCH4 = CalculateAdc(analogRead(ADC_CH4), 4);

  snmpCurrentCH1 = (int)(CurrentCH1 * 100);
  snmpCurrentCH2 = (int)(CurrentCH2 * 100);
  snmpVoltageCH3 = (int)(VoltageCH3 * 100);
  snmpVoltageCH4 = (int)(VoltageCH4 * 100);

  if(!serverInitiated && ESP32_ENC_isConnected())
    LoadServer();

  if(serverInitiated)
  {
    server.handleClient();
    check_status();

    snmp.loop();
    // changingNumber++;
    // if(changingNumber > 200){
    //   IPAddress destinationIP = IPAddress(10,1,2,97);
    //   if(snmp.sendTrapTo(settableNumberTrap, destinationIP, true, 2, 5000) != INVALID_SNMP_REQUEST_ID){ 
    //       Serial.println("Sent SNMP Trap");
    //   } else {
    //       Serial.println("Couldn't send SNMP Trap");
    //   }
    //   changingNumber = 0;
    // }
  }
  delay(10);
}

void SetIPConfig(){
  uint8_t octets[4];

  // Se fija si tiene que configurar DHCP
  if(!glbConfig.GetDHCP()){
    // Dirección IP
    glbConfig.GetIpConfig(IP_ADDRESS_ENUM, octets);
    IPAddress ip(octets);

    octets[0] = glbConfig.GetIpMask(1);
    octets[1] = glbConfig.GetIpMask(2);
    octets[2] = glbConfig.GetIpMask(3);
    octets[3] = glbConfig.GetIpMask(4);

    // Mascara
    //glbConfig.GetIpConfig(IP_MASK_ENUM, octets);
    IPAddress mask(octets);
    // Gateway
    glbConfig.GetIpConfig(IP_GW_ENUM, octets);
    IPAddress gw(octets);
    // DNS
    glbConfig.GetIpConfig(IP_DNS_ENUM, octets);
    IPAddress dns(octets);
    // Configura los parametros IP
    ETH.config(ip, gw, mask, dns);
    // Muestra log
    Serial.print("Setting static ip addres IP:");
    Serial.print(ip);
    Serial.print(" Mask:");
    Serial.print(mask);
    Serial.print(" Gw:");
    Serial.print(gw);
    Serial.print(" DNS:");
    Serial.println(dns);
 }
}

float GetAverage(int channel, float value){
  float *ptr = 0;
  int *counter = 0;

  ptr = channel == 1 ? ch1Buffer : ptr;
  ptr = channel == 2 ? ch2Buffer : ptr;
  ptr = channel == 3 ? ch3Buffer : ptr;
  ptr = channel == 4 ? ch4Buffer : ptr;

  counter = channel == 1 ? &ch1BufferCounter : counter;
  counter = channel == 2 ? &ch2BufferCounter : counter;
  counter = channel == 3 ? &ch3BufferCounter : counter;
  counter = channel == 4 ? &ch4BufferCounter : counter;

  if(ptr == 0)
    return 0;

  float *origin = ptr;
  ptr += *counter;
  *ptr = value;
  (*counter)++;
  *counter = *counter >= BUFFER_AVG ? 0 : *counter;
  float output = 0;
  for(int i=0;i<BUFFER_AVG;i++)
    output += *(origin + i);

  output /= BUFFER_AVG;

  return output;
}

float CalculateAdc(uint16_t x, uint8_t channel)
{
  if(channel < 1 || channel > 4)
    return 0;

  float f = GetAverage(channel, x);

  // if(channel == 3 || channel == 4)
  //   return glbConfig.ADC_FACTOR(channel) * f * 3.3f / 4096;

  // Obtiene puntos de calibración
  int x1 = glbConfig.GetCalXPoint(channel, 1);
  int x2 = glbConfig.GetCalXPoint(channel, 2);
  float y1 = glbConfig.GetCalYPoint(channel, 1);
  float y2 = glbConfig.GetCalYPoint(channel, 2);
  // Verifica si esta en el primer tramo
  if(x < x1){
    x2 = x1;
    y2 = y1;
    x1 = 0;
    y1 = 0;
  }
  // Calcula deltas
  int dx = (x2 - x1) != 0 ? x2 - x1 : 1;
  float dy = y2 - y1;
  // Calcula pendiente
  float m = dy / (float)dx;
  // Calcula valor
  float corrected = m * (f - x1) + y1;

  if(channel == 1 || channel == 2){
    //float current = (f - 3030) * 0.1176;
    corrected = corrected > CURRENT_MAX || corrected < -CURRENT_MAX ? 0 : corrected;
    corrected = corrected > THRESHOLD_CURRENT || corrected < -THRESHOLD_CURRENT ? corrected : 0;
    return corrected; //(x - 2048) * 3.3 / 4096; //glbConfig.ADC_FACTOR(channel) * x * 3.3f / 4096;
  }

  return corrected;
}

void WriteLog(String message, bool linebreak){
  if(DEBUG_MODE && linebreak)
    Serial.println(message);
  if(DEBUG_MODE && !linebreak)
    Serial.print(message);
}

//Check if header is present and correct
bool is_authenticated()
{
  WriteLog(F("Enter is_authenticated"), true);

  for(int i=0;i<server.headers();i++)
    WriteLog(server.headerName(i), true);

  if (server.hasHeader(F("Cookie")))
  {
    WriteLog(F("Found cookie: "), true);
    String cookie = server.header(F("Cookie"));
    WriteLog(cookie, true);

    if (cookie.indexOf(F("ESPSESSIONID=1")) != -1)
    {
      WriteLog(F("Authentication Successful"), true);
      return true;
    }
  }

  WriteLog(F("Authentication Failed"), true);
  return false;
}

//login page, also called for disconnect
void handleLogin()
{
  String msg = "";

  if (server.hasHeader(F("Cookie")))
  {
    WriteLog(F("Found cookie: "), false);
    String cookie = server.header(F("Cookie"));
    WriteLog(cookie, true);
  }

  if (server.hasArg(F("DISCONNECT")))
  {
    int sessionTimeout = SESSION_TIMEOUT_SEG;
    String cookie = String("ESPSESSIONID=0;Max-Age="+String(sessionTimeout)+";");
    WriteLog(F("Disconnection"), true);
    server.sendHeader(F("Location"), F("/login"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.sendHeader(F("Set-Cookie"), cookie);
    server.send(301);
    return;
  }

  if (server.hasArg(F("USERNAME")) && server.hasArg(F("PASSWORD")))
  {
    if (server.arg(F("USERNAME")) == glbConfig.USERNAME() &&  server.arg(F("PASSWORD")) == glbConfig.PASSWORD())
    {
      int sessionTimeout = SESSION_TIMEOUT_SEG;
      String cookie = String("ESPSESSIONID=1;Max-Age="+String(sessionTimeout)+";");
      server.sendHeader(F("Location"), F("/"));
      server.sendHeader(F("Cache-Control"), F("no-cache"));
      server.sendHeader(F("Set-Cookie"), cookie);
      server.send(301);
      WriteLog(F("Log in Successful"), true);
      return;
    }
    else{
      msg = F("Credenciales incorrectas");
      WriteLog(F("Log in Failed"), true);
    }
  }

  String content = "\
  <html>\
    <head>\
      <meta charset=\"UTF-8\">\
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
      <title>MONITOR DC - Configuración</title>\
      <style>\
          body {\
              margin: 0;\
              font-family: Arial, sans-serif;\
              display: flex; \
              justify-content: center; /* Centrado horizontal */ \
              align-items: center;    /* Centrado vertical */ \
              height: 100vh;          /* Asegúrate de que el padre tenga altura para centrar verticalmente */ \
          }\
          .contenedor-center{\
              display: flex;\
              align-items: center;\
              justify-content: center;\
          }\
          .container-right{\
            display: flex;\
              align-items: center;\
              justify-content: right;\
          }\
          .title{\
              margin-left: 20px;\
          }\
          .subtitle{\
              background-color: black;\
              color: white;\
              padding-left: 20px;\
          }\
          form > button{\
              margin-left: 20px;\
          }\
          table{\
            margin: 10px 0px;\
          }\
          tr{\
            margin: 2px 0px;\
          }\
          td{\
            padding: 0px 10px;\
          }\
          input{\
            padding: 2px 10px;margin:2px;font-size:1rem;\
          }\
          .text-danger{\
            color: red;\
          }\
      </style>\
    </head>\
    <body>\
      <div>\
        <p style='font-size:5rem;margin-bottom:0.5rem;text-align:center;'>C4i</p>\
        <h2 style='margin-left:10px;'>Sistema de monitoreo DC</h2>\
        <form action='/login' method='POST'>\
          <p style='margin:0px 0px 0px 12px;'>Ingrese al sistema</p>\
          <table>\
            <tbody>\
              <tr>\
                <td>Usuario:</td>\
                <td><input type='text' name='USERNAME' placeholder='nombre de usuario'></td>\
              </tr>\
              <tr>\
                <td>Contraseña</td>\
                <td><input type='password' name='PASSWORD' placeholder='contraseña'></td>\
              </tr>\
            </tbody>\
          </table>\
          <div class='contenedor-center'>\
            <input type='submit' name='SUBMIT' value='Ingresar' style='padding: 2px 5px;'></form>\
          </div>\
          <p class='text-danger'>#MESSAGE#</p>\
        </form>\
      </div>\
    </body>\
  </html>";

  content.replace("#MESSAGE#", msg);

  server.send(200, F("text/html"), content);
}

void handleConfig(){
  String message = "";
  bool isError = false;
  uint8_t octets[4];

  if (!is_authenticated())
  {
    server.sendHeader(F("Location"), F("/login"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.send(301);
    return;
  }

  // Verifica si entro por POST
  if(server.method() == HTTP_POST){
    if(server.hasArg("txtHostName"))
      glbConfig.HOSTNAME(server.arg("txtHostName"));

      
    octets[0] = server.hasArg("txtip1") ? (uint8_t)server.arg("txtip1").toInt() : CONFIG_IPADDRESS_1_DEFAULT;
    octets[1] = server.hasArg("txtip2") ? (uint8_t)server.arg("txtip2").toInt() : CONFIG_IPADDRESS_2_DEFAULT;
    octets[2] = server.hasArg("txtip3") ? (uint8_t)server.arg("txtip3").toInt() : CONFIG_IPADDRESS_3_DEFAULT;
    octets[3] = server.hasArg("txtip4") ? (uint8_t)server.arg("txtip4").toInt() : CONFIG_IPADDRESS_4_DEFAULT;
    glbConfig.SetIpConfig(IP_ADDRESS_ENUM, octets);


    octets[0] = server.hasArg("txtmask1") ? (uint8_t)server.arg("txtmask1").toInt() : CONFIG_MASK_1_DEFAULT;
    octets[1] = server.hasArg("txtmask2") ? (uint8_t)server.arg("txtmask2").toInt() : CONFIG_MASK_2_DEFAULT;
    octets[2] = server.hasArg("txtmask3") ? (uint8_t)server.arg("txtmask3").toInt() : CONFIG_MASK_3_DEFAULT;
    octets[3] = server.hasArg("txtmask4") ? (uint8_t)server.arg("txtmask4").toInt() : CONFIG_MASK_4_DEFAULT;
    glbConfig.SetIpConfig(IP_MASK_ENUM, octets);


    octets[0] = server.hasArg("txtgw1") ? (uint8_t)server.arg("txtgw1").toInt() : CONFIG_GW_1_DEFAULT;
    octets[1] = server.hasArg("txtgw2") ? (uint8_t)server.arg("txtgw2").toInt() : CONFIG_GW_2_DEFAULT;
    octets[2] = server.hasArg("txtgw3") ? (uint8_t)server.arg("txtgw3").toInt() : CONFIG_GW_3_DEFAULT;
    octets[3] = server.hasArg("txtgw4") ? (uint8_t)server.arg("txtgw4").toInt() : CONFIG_GW_4_DEFAULT;
    glbConfig.SetIpConfig(IP_GW_ENUM, octets);
      
    glbConfig.SetDCHP(server.hasArg("dhcp") && server.arg("dhcp") == "1");

    octets[0] = server.hasArg("txtdns1") ? (uint8_t)server.arg("txtdns1").toInt() : CONFIG_DNS_1_DEFAULT;
    octets[1] = server.hasArg("txtdns2") ? (uint8_t)server.arg("txtdns2").toInt() : CONFIG_DNS_2_DEFAULT;
    octets[2] = server.hasArg("txtdns3") ? (uint8_t)server.arg("txtdns3").toInt() : CONFIG_DNS_3_DEFAULT;
    octets[3] = server.hasArg("txtdns4") ? (uint8_t)server.arg("txtdns4").toInt() : CONFIG_DNS_4_DEFAULT;
    glbConfig.SetIpConfig(IP_DNS_ENUM, octets);

    octets[0] = server.hasArg("txttrap1") ? (uint8_t)server.arg("txttrap1").toInt() : 0;
    octets[1] = server.hasArg("txttrap2") ? (uint8_t)server.arg("txttrap2").toInt() : 0;
    octets[2] = server.hasArg("txttrap3") ? (uint8_t)server.arg("txttrap3").toInt() : 0;
    octets[3] = server.hasArg("txttrap4") ? (uint8_t)server.arg("txttrap4").toInt() : 0;
    glbConfig.SetIpConfig(IP_TRAP_ENUM, octets);

    if(server.hasArg("snmp-community"))
      glbConfig.SNMP_COMMUNITY(server.arg("snmp-community"));

    if(server.hasArg("alarmI1Max"))
      glbConfig.THRESHOLD_MAX(1, server.arg("alarmI1Max").toFloat());
    if(server.hasArg("alarmI2Max"))
      glbConfig.THRESHOLD_MAX(2, server.arg("alarmI2Max").toFloat());
    if(server.hasArg("alarmV3Max"))
      glbConfig.THRESHOLD_MAX(3, server.arg("alarmV3Max").toFloat());
    if(server.hasArg("alarmV4Max"))
      glbConfig.THRESHOLD_MAX(4, server.arg("alarmV4Max").toFloat());
    if(server.hasArg("alarmI1Min"))
      glbConfig.THRESHOLD_MIN(1, server.arg("alarmI1Min").toFloat());
    if(server.hasArg("alarmI2Min"))
      glbConfig.THRESHOLD_MIN(2, server.arg("alarmI2Min").toFloat());
    if(server.hasArg("alarmV3Min"))
      glbConfig.THRESHOLD_MIN(3, server.arg("alarmV3Min").toFloat());
    if(server.hasArg("alarmV4Min"))
      glbConfig.THRESHOLD_MIN(4, server.arg("alarmV4Min").toFloat());

    message = "La configuración ha sido guardada correctamente";
  }

  if(message != "" && !isError)
    message = "<p class='text-success'>" + message + "</p>";
  if(message != "" && isError)
    message = "<p class='text-danger'>" + message + "</p>";

  String content = "\
  <!DOCTYPE html>\
  <html lang=\"en\">\
    <head>\
      <meta charset=\"UTF-8\">\
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
      " + HtmlTitle + HtmlStyle + "\
  </head>\
  <body>\
      " + GetHtmlMenu("CONFIGURATION") + "\
      <h1 class=\"title\">Configuración</h1>\
      <form method=\"post\" action=\"configuration\">\
          "+ message +"\
          <!-- CONFIGURA NOMBRE DEL HOST -->\
          <h3 class=\"subtitle\">General</h3>\
          <div class=\"parameter-container\">\
              <label>Nombre del equipo:</label>\
              <input name=\"txtHostName\" value=\"#NAME#\" />\
          </div>\
          <div class=\"parameter-container\">\
              <label>Constraseña:</label>\
              <a href=\"resetpassword\" class=\"link\">Reiniciar contraseña</a>\
          </div>\
          <!-- CONFIGURACION IP -->\
          <h3 class=\"subtitle\">Configuración de red</h3>\
          <div class=\"parameter-container\">\
              <label>Habilitar DHCP:</label>\
              <input type=\"checkbox\" name=\"dhcp\" value=\"1\" #DHCP# onchange=\"ChangeDhcp(this)\"/>\
          </div>\
          <div class=\"parameter-container\">\
              <label>Dirección IPV4:</label>&nbsp;\
              <input name=\"txtip1\" class=\"octet staticip\" value=\"#txtip1#\" #DISABLED# /> . \
              <input name=\"txtip2\" class=\"octet staticip\" value=\"#txtip2#\" #DISABLED# /> . \
              <input name=\"txtip3\" class=\"octet staticip\" value=\"#txtip3#\" #DISABLED# /> . \
              <input name=\"txtip4\" class=\"octet staticip\" value=\"#txtip4#\" #DISABLED# />\
          </div>\
          <div class=\"parameter-container\">\
              <label>Mascara de subred:</label>&nbsp;\
              <input name=\"txtmask1\" class=\"octet staticip\" value=\"#txtmask1#\" #DISABLED# /> . \
              <input name=\"txtmask2\" class=\"octet staticip\" value=\"#txtmask2#\" #DISABLED# /> . \
              <input name=\"txtmask3\" class=\"octet staticip\" value=\"#txtmask3#\" #DISABLED# /> . \
              <input name=\"txtmask4\" class=\"octet staticip\" value=\"#txtmask4#\" #DISABLED# />\
          </div>\
          <div class=\"parameter-container\">\
              <label>Puerta de enlace:</label>&nbsp;\
              <input name=\"txtgw1\" class=\"octet staticip\" value=\"#txtgw1#\" #DISABLED# /> . \
              <input name=\"txtgw2\" class=\"octet staticip\" value=\"#txtgw2#\" #DISABLED# /> . \
              <input name=\"txtgw3\" class=\"octet staticip\" value=\"#txtgw3#\" #DISABLED# /> . \
              <input name=\"txtgw4\" class=\"octet staticip\" value=\"#txtgw4#\" #DISABLED# />\
          </div>\
          <div class=\"parameter-container\">\
              <label>Servidor DNS:</label>&nbsp;\
              <input name=\"txtdns1\" class=\"octet staticip\" value=\"#txtdns1#\" #DISABLED# /> . \
              <input name=\"txtdns2\" class=\"octet staticip\" value=\"#txtdns2#\" #DISABLED# /> . \
              <input name=\"txtdns3\" class=\"octet staticip\" value=\"#txtdns3#\" #DISABLED# /> . \
              <input name=\"txtdns4\" class=\"octet staticip\" value=\"#txtdns4#\" #DISABLED# />\
          </div>\
          <!-- SNMP -->\
          <h3 class=\"subtitle\">Configuración SNMP</h3>\
          <div class=\"parameter-container\">\
              <label>Comunidad SNMP:</label>\
              <input name=\"snmp-community\" value=\"#community#\" />\
          </div>\
          <div class=\"parameter-container\">\
              <label>Servidor SNMP:</label>&nbsp;\
              <input name=\"txttrap1\" class=\"octet\" value=\"#txttrap1#\" /> . \
              <input name=\"txttrap2\" class=\"octet\" value=\"#txttrap2#\" /> . \
              <input name=\"txttrap3\" class=\"octet\" value=\"#txttrap3#\" /> . \
              <input name=\"txttrap4\" class=\"octet\" value=\"#txttrap4#\" />\
          </div>\
          <!-- ALARMAS -->\
          <h3 class=\"subtitle\">Alarmas</h3>\
          <div class=\"parameter-container\">\
              <label>Corriente Máxima Canal1 [A]:</label>\
              <input name=\"alarmI1Max\" value=\"#alarmI1Max#\" />\
          </div>\
          <div class=\"parameter-container\">\
              <label>Corriente Mínima Canal1 [A]:</label>\
              <input name=\"alarmI1Min\" value=\"#alarmI1Min#\" />\
          </div>\
          <div class=\"parameter-container\">\
              <label>Corriente Máxima Canal2 [A]:</label>\
              <input name=\"alarmI2Max\" value=\"#alarmI2Max#\" />\
          </div>\
          <div class=\"parameter-container\">\
              <label>Corriente Mínima Canal2 [A]:</label>\
              <input name=\"alarmI2Min\" value=\"#alarmI2Min#\"/>\
          </div>\
          <div class=\"parameter-container\">\
              <label>Tensión Máxima Canal3 [V]:</label>\
              <input name=\"alarmV3Max\" value=\"#alarmV3Max#\" />\
          </div>\
          <div class=\"parameter-container\">\
              <label>Tensión Mínima Canal3 [V]:</label>\
              <input name=\"alarmV3Min\" value=\"#alarmV3Min#\" />\
          </div>\
          <div class=\"parameter-container\">\
              <label>Tensión Máxima Canal4 [V]:</label>\
              <input name=\"alarmV4Max\" value=\"#alarmV4Max#\" />\
          </div>\
          <div class=\"parameter-container\">\
              <label>Tensión Mínima Canal4 [V]:</label>\
              <input name=\"alarmV4Min\" value=\"#alarmV4Min#\" />\
          </div>\
          <!-- ACCIONES -->\
          <h3 class=\"subtitle\">Acciones</h3>\
          <div class=\"parameter-container\">\
            <button type='button' onclick='RebootDevice()'>Reiniciar dispositivo</button>\
          </div>\
          <div class=\"parameter-container\">\
            <button type='button' onclick='ResetDevice()'>Configurar valores de fabrica</button>\
          </div>\
          <br />\
          <br />\
          <button style=\"margin-bottom:50px;padding:5px 10px;\">Guardar configuración</button>\
      </form>\
      <script>\
      function RebootDevice(){\
        if(confirm('¿Esta seguro que quiere reiniciar el dispositivo?'))\
          window.location = \'reboot\';\
      }\
      function ResetDevice(){\
        if(confirm('Si reinicia a los valores de fabrica se perderán todos los parametros configurados.¿Esta seguro que quiere volver a los valores de fabrica?'))\
          window.location = \'reset\';\
      }\
      function ChangeDhcp(cbox){\
        const c = cbox.checked;\
        const elements = document.querySelectorAll('.staticip');\
        elements.forEach(element => {\
            element.disabled = c;\
        });\
      }\
      </script>\
  </body>\
  </html>";

  String data;
  byte octects[4];

  bool useDhcp = glbConfig.GetDHCP();
  content.replace("#DHCP#", useDhcp ? "checked" : "");
  content.replace("#DISABLED#", useDhcp ? "disabled='disabled'" : "");

  data = glbConfig.HOSTNAME();
  content.replace("#NAME#", data);

  data = String(glbConfig.GetIpAddress(1));
  content.replace("#txtip1#", data);
  data = String(glbConfig.GetIpAddress(2));
  content.replace("#txtip2#", data);
  data = String(glbConfig.GetIpAddress(3));
  content.replace("#txtip3#", data);
  data = String(glbConfig.GetIpAddress(4));
  content.replace("#txtip4#", data);

  data = String(glbConfig.GetIpMask(1));
  content.replace("#txtmask1#", data);
  data = String(glbConfig.GetIpMask(2));
  content.replace("#txtmask2#", data);
  data = String(glbConfig.GetIpMask(3));
  content.replace("#txtmask3#", data);
  data = String(glbConfig.GetIpMask(4));
  content.replace("#txtmask4#", data);

  data = String(glbConfig.GetGWAddress(1));
  content.replace("#txtgw1#", data);
  data = String(glbConfig.GetGWAddress(2));
  content.replace("#txtgw2#", data);
  data = String(glbConfig.GetGWAddress(3));
  content.replace("#txtgw3#", data);
  data = String(glbConfig.GetGWAddress(4));
  content.replace("#txtgw4#", data);

  glbConfig.GetIpConfig(IP_DNS_ENUM, octects);

  data = String(octects[0]);
  content.replace("#txtdns1#", data);
  data = String(octects[1]);
  content.replace("#txtdns2#", data);
  data = String(octects[2]);
  content.replace("#txtdns3#", data);
  data = String(octects[3]);
  content.replace("#txtdns4#", data);

  data = glbConfig.SNMP_COMMUNITY();
  content.replace("#community#", data);
  data = glbConfig.THRESHOLD_MAX(1);
  content.replace("#alarmI1Max#", data);
  data = glbConfig.THRESHOLD_MAX(2);
  content.replace("#alarmI2Max#", data);
  data = glbConfig.THRESHOLD_MAX(3);
  content.replace("#alarmV3Max#", data);
  data = glbConfig.THRESHOLD_MAX(4);
  content.replace("#alarmV4Max#", data);
  data = glbConfig.THRESHOLD_MIN(1);
  content.replace("#alarmI1Min#", data);
  data = glbConfig.THRESHOLD_MIN(2);
  content.replace("#alarmI2Min#", data);
  data = glbConfig.THRESHOLD_MIN(3);
  content.replace("#alarmV3Min#", data);
  data = glbConfig.THRESHOLD_MIN(4);
  content.replace("#alarmV4Min#", data);

  glbConfig.GetIpConfig(IP_TRAP_ENUM, octects);

  data = String(octects[0]);
  content.replace("#txttrap1#", data);
  data = String(octects[1]);
  content.replace("#txttrap2#", data);
  data = String(octects[2]);
  content.replace("#txttrap3#", data);
  data = String(octects[3]);
  content.replace("#txttrap4#", data);

  server.send(200, F("text/html"), content);
}

void handleResetPassword(){
  String message = "";
  bool isError = false;
  
  if (!is_authenticated())
  {
    server.sendHeader(F("Location"), F("/login"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.send(301);
    return;
  }

  if(server.method() == HTTP_POST){
    String username = server.hasArg("USERNAME") ? server.arg("USERNAME") : "";
    String oldpassword = server.hasArg("PASSWORD") ? server.arg("PASSWORD") : "";
    String newpassword = server.hasArg("NEWPASSWORD") ? server.arg("NEWPASSWORD") : "";
    String confirmation = server.hasArg("CONFIRMATION") ? server.arg("CONFIRMATION") : "";

    if(confirmation != newpassword)
      message = "Las contraseñas no coinciden";
    if(newpassword == "")
      message = "Debe ingresar una nueva contraseña";
    if(username != glbConfig.USERNAME() || oldpassword != glbConfig.PASSWORD())
      message = "Credenciales incorrectas";

    // Guarda contraseña
    if(message == ""){
      glbConfig.PASSWORD(newpassword);
    }

    isError = message != "";
    message = message != "" ? "<p class='text-danger'>" + message + "</p>" : 
      "<p class='text-success'>La contraseña ha sido correctamente actualizada";
  }

  String content ="\
  <!DOCTYPE html>\
    <html lang=\"en\">\
      <head>\
        <meta charset=\"UTF-8\">\
        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" >\
        " + HtmlTitle + HtmlStyle + "\
      </head>\
      <body> \
        "+ GetHtmlMenu("CONFIGURATION") +"\
        <h1 class=\"title\">Configuracion de seguridad</h1> \
        <h3 class=\"subtitle\">Cambiar contraseña</h3> \
        <form action='/resetpassword' method='POST' style='margin-left:50px;'>\
          <table>\
            <tbody>\
              <tr>\
                <td>Usuario:</td>\
                <td><input type='text' name='USERNAME' placeholder='nombre de usuario'></td>\
              </tr>\
              <tr>\
                <td>Contraseña actual</td>\
                <td><input type='password' name='PASSWORD' placeholder='contraseña actual'></td>\
              </tr>\
              <tr>\
                <td>Nueva ontraseña</td>\
                <td><input type='password' name='NEWPASSWORD' placeholder='nueva contraseña'></td>\
              </tr>\
              <tr>\
                <td>Contraseña</td>\
                <td><input type='password' name='CONFIRMATION' placeholder='confirmación nueva contraseña'></td>\
              </tr>\
            </tbody>\
          </table>\
          <div class='container-right'>\
            <input type='submit' name='SUBMIT' value='Ingresar' style='padding: 2px 5px;'></form>\
          </div>\
          #MESSAGE#\
        </form>\
      </body> \
    </html>";

    content.replace("#MESSAGE#", message);

  server.send(200, F("text/html"), content);
}

void handleReboot()
{
  if (!is_authenticated())
  {
    server.sendHeader(F("Location"), F("/login"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.send(301);
    return;
  }

  String content = "\
  <html>\
    <head>\
      " + HtmlTitle + HtmlStyle + "\
        <meta http-equiv='refresh' content='10'/>\
    </head>\
    <body>\
      <div class='contenedor-center' style='height:100vh;'>\
        <h2>REINICIANDO DISPOSITIVO</h2>\
      </div>\
    </body>\
  </html>";

  int sessionTimeout = SESSION_TIMEOUT_SEG;
  String cookie = String("ESPSESSIONID=0;Max-Age="+String(sessionTimeout)+";");
  WriteLog(F("Disconnection"), true);
  server.sendHeader(F("Cache-Control"), F("no-cache"));
  server.sendHeader(F("Set-Cookie"), cookie);
  server.send(200, F("text/html"), content);
  ESP.restart();
}

void handleReset(){
  if (!is_authenticated())
  {
    server.sendHeader(F("Location"), F("/login"));
    server.sendHeader(F("Cache-Control"), F("no-cache"));
    server.send(301);
    return;
  }

  String content = "\
  <html>\
    <head>\
      " + HtmlTitle + HtmlStyle + "\
    </head>\
    <body>\
      <div class='contenedor-center' style='height:100vh;'>\
        <div>\
          <h2>LOS VALORES DEL DISPOSITIVO HAN SIDO REINICIADOS A FABRICA</h2>\
          <div style='display:flex;justify-content:center;margin:10;'>\
            <button type='button' onclick='reboot()'>Reiniciar dispositivo</button>\
          </div>\
        </div>\
      </div>\
      <script>\
        function reboot(){\
          window.location = \'reboot\';\
        }\
      </script>\
    </body>\
  </html>";
  server.send(200, F("text/html"), content);

  glbConfig.SetToFactory();
}

