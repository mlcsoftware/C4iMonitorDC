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
      if(length == 0 || cmd_received[0].length() == 0)
         return;

      if(cmd_received[0] == "debug"){
         cmd_debug(cmd_received, length);
         return;
      }
      if(cmd_received[0] == "name"){
         cmd_name(cmd_received, length);
         return;
      }
      if(cmd_received[0] == "reset"){
         cmd_reset(cmd_received, length);
         return;
      }
      if(cmd_received[0] == "reboot"){
         cmd_reboot(cmd_received, length);
         return;
      }
      if(cmd_received[0] == "show"){
         cmd_show(cmd_received, length);
         return;
      }
      if(cmd_received[0] == "read"){
         cmd_read(cmd_received, length);
         return;
      }
      if(cmd_received[0] == "factor"){
         cmd_factor(cmd_received, length);
         return;
      }
      if(cmd_received[0] == "calibrate"){
         cmd_calibrate(cmd_received, length);
         return;
      }
      Serial.println("Comando incorrecto");
   }

   void receive_char()
   {
      String parameters[5];

      char received = Serial.read();

      if( (received != '\r') && (received != '\n') && (received != KEYCODE_BS) )
         Serial.print(received);

      if(received == KEYCODE_BS)
      {
         if(sReceived.length() > 0){
            Serial.print(received);
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
      if(channel == "1"){
         Serial.print("Canal CH1:");
         Serial.println(analogRead(A0));
      }
      if(channel == "2"){
         Serial.print("Canal CH2:");
         Serial.println(analogRead(A3));
      }
      if(channel == "3"){
         Serial.print("Canal CH3:");
         Serial.println(analogRead(A6));
      }
      if(channel == "4"){
         Serial.print("Canal CH4:");
         Serial.println(analogRead(A7));
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
      if(length == 1){
         Serial.println("Monitor DC V/I");
         Serial.println("C4i GP-IoT-48VDC2-100ADC2");
         Serial.print("Versión ");
         Serial.println(SENSOR_VERSION);
         Serial.println();
         Serial.println("Configuration");
         Serial.println("----------------------------");

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
      if(length == 2 && parameter[1] == "calibrate"){
         show_calibrate();
      }
   }

   void show_calibrate(){
      for(int channel=1;channel<5;channel++)
         show_calibrate_channel(channel);
   }

   void show_calibrate_channel(int channel){
      Serial.print("Calibracion canal ");
      Serial.println(channel);
      Serial.println("");
      for(int point=1;point<3;point++){
         Serial.print("x");
         Serial.print(point);
         Serial.print(": ");
         Serial.println(glbConfig.GetCalXPoint(channel, point));
         Serial.print("y");
         Serial.print(point);
         Serial.print(": ");
         Serial.print(glbConfig.GetCalYPoint(channel, point));
         Serial.println(channel == 1 || channel == 2 ? " A" : " V");
         Serial.println("");
      }
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
      if(cmd == "calibrate")
      {
         Serial.println("Calibrate, calibra los canales analogicos");
         Serial.println("Modo de uso: calibrate ch1|ch2|ch3|ch4 punto1 punto2");
         Serial.println("canal: Nombre del canal que se desea calibrar.");
         Serial.println("punto1: Valor de tensión/corriente del punto 1");
         Serial.println("punto2: Valor de tensión/corriente del punto 2");
      }
   }

   void cmd_calibrate(String *parameter, int length)
   {
      if(length < 4){
         show_help("calibrate");
         return;
      }
      int channel = getChannelNumberByName(parameter[1]);
      if(channel < 0)
      {
         show_help("calibrate");
         return;
      }
      // captura los valores de calibracion
      float values[2];
      values[0] = parameter[2].toFloat();
      values[1] = parameter[3].toFloat();

      // Solicita confirmación
      Serial.println();
      Serial.println("##########################################################");
      Serial.println("#                                                        #");
      Serial.println("#               ADVERTENCIA CALIBRACION                  #");
      Serial.println("#                                                        #");
      Serial.println("##########################################################");
      Serial.println();
      Serial.println("Va a cambiar la calibración del dispositivo, es podría dañar el funcionamiento del dispositivo.");
      Serial.print("¿Está seguro que quiere cambiar la calibración? [s/N]:");
      // Primero vacia el buffer de la comunicación serie
      while(Serial.available())
         Serial.read();
      // Espera hasta que se presione una tecla
      while(!Serial.available());
      char response = Serial.read();
      // Si se cancela sale
      if( (response != 's') && (response != 'S') )
         return;
      
      Calibrate(channel, values);

      return;
   }

   void Calibrate(int channel, float *value){
      for(int i=0;i<2;i++){
         Serial.print("Calibración del PUNTO ");
         Serial.println(i+1);
         Serial.println();
         Serial.print("Ajuste el valor de calibración ");
         Serial.print(*value);
         Serial.print(channel == 1 || channel == 2 ? " A" : " V");
         Serial.print(", y presione 'c' para calibrar o 'x' para salir");
         // Primero vacia el buffer de la comunicación serie
         while(Serial.available())
            Serial.read();
         // Espera hasta que se presione una tecla
         while(!Serial.available());
         char response = Serial.read();
         // Se fija si tiene que salir
         if(response == 'x')
         {
            Serial.println("Calibracion cancelada.");
            return;
         }
         // Mensaje calibrando
         Serial.println("Calibrando, por favor espere...");
         // Guarda valor x
         int x = GetCalibrateXAvg(channel);
         if(x < 0)
            return;

         if(!glbConfig.SetCalXPoint(channel, i+1, x) || !glbConfig.SetCalYPoint(channel, i+1, *value)){
            Serial.println("Ha ocurrido un error al intentar calibrar el dispositivo.");
            return;
         }
         Serial.println("Calibración correcta");
         Serial.println("");

         value++;
      }

      Serial.print("El canal ");
      Serial.print(channel);
      Serial.println(" ha sido calibrado correctamente");
   }

   int GetCalibrateXAvg(int channel){
      uint8_t adc = 255;
      adc = channel == 1 ? ADC_CH1 : adc;
      adc = channel == 2 ? ADC_CH2 : adc;
      adc = channel == 3 ? ADC_CH3 : adc;
      adc = channel == 4 ? ADC_CH4 : adc;
      int avg = 0;

      if(adc == 255)
      {
         Serial.println("Error de calibración");
         Serial.print("Numero de canal incorrecto Canal ");
         Serial.println(channel);
         return -1;
      }

      for(int i=0;i<BUFFER_AVG;i++)
         avg += analogRead(adc);

      return BUFFER_AVG > 0 ? (int)(avg / BUFFER_AVG) : 0;
   }

   int getChannelNumberByName(String name){
      if(name == "ch1")
         return 1;
      if(name == "ch2")
         return 2;
      if(name == "ch3")
         return 3;
      if(name == "ch4")
         return 4;

      return -1;
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