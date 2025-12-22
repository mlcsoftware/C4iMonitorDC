#ifndef __CLI_H__

#define __CLI_H__

#define KEYCODE_BS                      0x08
#include <HardwareSerial.h>
#include <core/globals.h>
#include <core/constants.h>

class Cli
{
   private:

   String sReceived = "";

   int split(char separator, String data, String *output, int max)
   {
      String parameter = "";
      int counter = 0;

      if(data.indexOf(separator) < 0)
      {
         *output = data;
         return 1;
      }

      bool escape = false;

      for(int i=0;i<data.length();i++)
      {
         escape = data.charAt(i) == '"' ? !escape : escape;
         if( (!escape) && (data.charAt(i) == separator) )
         {
            *output = parameter;
            counter++;
            output++;
            parameter = "";
         }
         else
            if(data.charAt(i) != '"')
               parameter += data.charAt(i);
      }

      if(parameter != "")
      {
         *output = parameter;
         counter++;
      }

      return counter;  
   }

   void process_cmd(String *cmd_received, int length)
   {
      if(cmd_received[0] == "debug")
         cmd_debug(cmd_received, length);
      if(cmd_received[0] == "name")
         cmd_name(cmd_received, length);
      if(cmd_received[0] == "reset")
         cmd_reset(cmd_received, length);
      if(cmd_received[0] == "reboot")
         cmd_reboot(cmd_received, length);
      if(cmd_received[0] == "show")
         cmd_show(cmd_received, length);
      if(cmd_received[0] == "read")
         cmd_read(cmd_received, length);
      if(cmd_received[0] == "factor")
         cmd_factor(cmd_received, length);
   }

   void receive_char()
   {
      String parameters[3];

      char received = Serial.read();

      if( (received != '\r') && (received != '\n') )
         Serial.print(received);

      if(received == KEYCODE_BS)
      {
         if(sReceived.length() > 0){
            Serial.print(" ");
            Serial.print(received);
            sReceived.remove(sReceived.length() - 1);
         }
         return;
      }
      if( (received != '\r') && (received != '\n') && (received != KEYCODE_BS))
         sReceived += received;

      if(received == '\r')
      {
         sReceived.trim();
         int counter = split(' ', sReceived, parameters, 3);
         Serial.println("");
         process_cmd(parameters, counter);
         if(sReceived != "")
            Serial.println();
         sReceived = "";
         show_prompt();
      }

   }

   void cmd_debug(String *parameter, int length)
   {
      show_help("debug");
   }

   void cmd_name(String *parameter, int length)
   {
      if(length <=1)
      {
         show_help("name");
         return;
      }
      String cmd = parameter[1];
      if(cmd == "get")
      {
         Serial.print("Name: ");
         Serial.println(glbConfig.HOSTNAME());
         return;
      }
      if(cmd == "set")
      {
         if(length < 3)
         {
            show_help("name");
            return;
         }
         Serial.print("Name: ");
         Serial.println(glbConfig.HOSTNAME(parameter[2]));
         return;
      }
      show_help("name");
   }

   void cmd_reset(String *parameter, int length)
   {
      // Solicita confirmación
      Serial.println();
      Serial.println("##########################################################");
      Serial.println("#  Restablecer a los valores de fabrica                  #");
      Serial.println("##########################################################");
      Serial.println();
      Serial.println();
      Serial.print("¿Está seguro que quiere restablecer? [s/N]:");
      // Primero vacia el buffer de la comunicación serie
      while(Serial.available())
         Serial.read();
      // Espera hasta que se presione una tecla
      while(!Serial.available());
      char response = Serial.read();
      Serial.println(response);
      // Si se cancela sale
      if( (response != 's') && (response != 'S') )
         return;
      // Reinicia a valores de fabrica
      reset_factory_default();
      Serial.println("Todos los parametros han sido reiniciado a sus valores de fabrica");
      return;
   }

   void cmd_read(String *parameter, int length){
      if(length <=1)
      {
         show_help("read");
         return;
      }
      String channel = parameter[1];
      if(channel == "ch1"){
         Serial.print("Canal I1 [A]:");
         Serial.println(CurrentCH1);
      }
      if(channel == "ch2"){
         Serial.print("Canal I2 [A]:");
         Serial.println(CurrentCH2);
      }
      if(channel == "ch3"){
         Serial.print("Canal V3 [V]:");
         Serial.println(VoltageCH3);
      }
      if(channel == "ch4"){
         Serial.print("Canal V4 [V]:");
         Serial.println(VoltageCH4);
      }
   }

   void cmd_factor(String *parameter, int length){
      if(length <=1)
      {
         return;
      }
      String channel = parameter[1];
      if(channel == "ch1"){
         float factor = -1;
         if(length >= 3){
            factor = parameter[2].toFloat();
            Serial.print("Nuevo valor de Factor: ");
            Serial.println(factor);
         }
         Serial.print("Factor CH1: ");
         Serial.println(glbConfig.ADC_FACTOR(1, factor));
      }
      if(channel == "ch2"){
         float factor = -1;
         if(length >= 3){
            factor = parameter[2].toFloat();
            Serial.print("Nuevo valor de Factor: ");
            Serial.println(factor);
         }
         Serial.print("Factor CH2: ");
         Serial.println(glbConfig.ADC_FACTOR(2, factor));
      }
      if(channel == "ch3"){
         float factor = -1;
         if(length >= 3){
            factor = parameter[2].toFloat();
            Serial.print("Nuevo valor de Factor: ");
            Serial.println(factor);
         }
         Serial.print("Factor CH3: ");
         Serial.println(glbConfig.ADC_FACTOR(3, factor));
      }
      if(channel == "ch4"){
         float factor = -1;
         if(length >= 3){
            factor = parameter[2].toFloat();
            Serial.print("Nuevo valor de Factor: ");
            Serial.println(factor);
         }
         Serial.print("Factor CH4: ");
         Serial.println(glbConfig.ADC_FACTOR(4, factor));
      }
   }

   void reset_factory_default()
   {
      glbConfig.SetToFactory();
   }

   void cmd_reboot(String *parameter, int length)
   {
      // Solicita confirmación
      Serial.println();
      Serial.println("##########################################################");
      Serial.println("#                                                        #");
      Serial.println("#               REINICIANDO DISPOSITIVO                  #");
      Serial.println("#                                                        #");
      Serial.println("##########################################################");
      Serial.println();
      Serial.println();
      Serial.print("¿Está seguro que quiere reinicar? [s/N]:");
      // Primero vacia el buffer de la comunicación serie
      while(Serial.available())
         Serial.read();
      // Espera hasta que se presione una tecla
      while(!Serial.available());
      char response = Serial.read();
      // Si se cancela sale
      if( (response != 's') && (response != 'S') )
         return;
      // Reinicia a valores de fabrica
      Serial.println("Reiniciando...");
      ESP.restart();
      return;
   }

   void cmd_show(String *parameter, int length)
   {
      Serial.println("Monitor DC V/I");
      Serial.println("C4i GP-IoT-48VDC2-100ADC2");
      Serial.print("Versión ");
      Serial.println(SENSOR_VERSION);
      Serial.println();
      Serial.println("Configuration");
      Serial.println("----------------------------");

      // rpm
      Serial.print("Comunidad SNMP:");
      Serial.println(glbConfig.SNMP_COMMUNITY());
      Serial.print("Dirección IP: ");
      Serial.println(ETH.localIP());
      Serial.print("Mascara de subred:");
      Serial.println(ETH.subnetMask());
      Serial.print("Puerta de enlace:");
      Serial.println(ETH.gatewayIP());
      Serial.print("DNS:");
      Serial.println(ETH.dnsIP());
      Serial.println();
   }

   void show_prompt()
   {
      Serial.print(glbConfig.HOSTNAME());
      Serial.print(">");
   }

   void show_help(String cmd)
   {
      if(cmd == "name")
      {
         Serial.println("Name, configura o lee el nombre del sensor");
         Serial.println("Modo de uso: name get|set [nombre del sensor]");
         Serial.println("get: Devuelve el nombre del sensor");
         Serial.println("set: Configura el nombre del sensor");
      }
      if(cmd == "read"){
         Serial.println("Read, lee el valor de la entradas de tensión y corriente");
         Serial.println("Modo de uso: read ch1|ch2|ch3|ch4");
         Serial.println("canal: Nombre del canal de donde leer el valor.");
      }
   }


   public:
   void begin()
   {
      show_prompt();
   }

   void Read()
   {
      if(Serial.available())
      {
         receive_char();
      }
   }

   void Reset()
   {
      reset_factory_default();
   }
};


#endif