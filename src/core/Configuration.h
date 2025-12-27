#ifndef __CONFIG_H__

#define __CONFIG_H__

#include <EEPROM.h>
#include <core/constants.h>

#define EEPROM_EOS                     0xFF
#define CONFIG_CHAR_SIZE               1
#define CONFIG_CHAR50_SIZE             50
#define CONFIG_FLOAT_SIZE              4
#define CONFIG_INT_SIZE                2

#define CONFIG_IPADDRESS_1             0
#define CONFIG_IPADDRESS_2             CONFIG_IPADDRESS_1 + CONFIG_CHAR_SIZE
#define CONFIG_IPADDRESS_3             CONFIG_IPADDRESS_2 + CONFIG_CHAR_SIZE
#define CONFIG_IPADDRESS_4             CONFIG_IPADDRESS_3 + CONFIG_CHAR_SIZE
#define CONFIG_MASK_1                  CONFIG_IPADDRESS_4 + CONFIG_CHAR_SIZE
#define CONFIG_MASK_2                  CONFIG_MASK_1 + CONFIG_CHAR_SIZE
#define CONFIG_MASK_3                  CONFIG_MASK_2 + CONFIG_CHAR_SIZE
#define CONFIG_MASK_4                  CONFIG_MASK_3 + CONFIG_CHAR_SIZE
#define CONFIG_GW_1                    CONFIG_MASK_4 + CONFIG_CHAR_SIZE
#define CONFIG_GW_2                    CONFIG_GW_1 + CONFIG_CHAR_SIZE
#define CONFIG_GW_3                    CONFIG_GW_2 + CONFIG_CHAR_SIZE
#define CONFIG_GW_4                    CONFIG_GW_3 + CONFIG_CHAR_SIZE
#define CONFIG_DHCP                    CONFIG_GW_4 + CONFIG_CHAR_SIZE
#define CONFIG_USERNAME                CONFIG_DHCP + CONFIG_CHAR_SIZE
#define CONFIG_PASSWORD                CONFIG_USERNAME + CONFIG_CHAR50_SIZE
#define CONFIG_HOSTNAME                CONFIG_PASSWORD + CONFIG_CHAR50_SIZE
#define CONFIG_SNMP_COMMUNITY          CONFIG_HOSTNAME + CONFIG_CHAR50_SIZE
#define CONFIG_DNS_1                   CONFIG_SNMP_COMMUNITY + CONFIG_CHAR50_SIZE
#define CONFIG_DNS_2                   CONFIG_DNS_1 + CONFIG_CHAR_SIZE
#define CONFIG_DNS_3                   CONFIG_DNS_2 + CONFIG_CHAR_SIZE
#define CONFIG_DNS_4                   CONFIG_DNS_3 + CONFIG_CHAR_SIZE
#define CONFIG_CAL_CH1_Y1              CONFIG_DNS_4 + CONFIG_CHAR_SIZE
#define CONFIG_CAL_CH1_Y2              CONFIG_CAL_CH1_Y1 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH2_Y1              CONFIG_CAL_CH1_Y2 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH2_Y2              CONFIG_CAL_CH2_Y1 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH3_Y1              CONFIG_CAL_CH2_Y2 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH3_Y2              CONFIG_CAL_CH3_Y1 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH4_Y1              CONFIG_CAL_CH3_Y2 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH4_Y2              CONFIG_CAL_CH4_Y1 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH1_X1              CONFIG_CAL_CH4_Y2 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH1_X2              CONFIG_CAL_CH1_X1 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH2_X1              CONFIG_CAL_CH1_X2 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH2_X2              CONFIG_CAL_CH2_X1 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH3_X1              CONFIG_CAL_CH2_X2 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH3_X2              CONFIG_CAL_CH3_X1 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH4_X1              CONFIG_CAL_CH3_X2 + CONFIG_FLOAT_SIZE
#define CONFIG_CAL_CH4_X2              CONFIG_CAL_CH4_X1 + CONFIG_FLOAT_SIZE
#define CONFIG_CH1_FACTOR              CONFIG_CAL_CH4_X2 + CONFIG_FLOAT_SIZE
#define CONFIG_CH2_FACTOR              CONFIG_CH1_FACTOR + CONFIG_FLOAT_SIZE
#define CONFIG_CH3_FACTOR              CONFIG_CH2_FACTOR + CONFIG_FLOAT_SIZE
#define CONFIG_CH4_FACTOR              CONFIG_CH3_FACTOR + CONFIG_FLOAT_SIZE
#define CONFIG_CH1_MAX_THRESHOLD       CONFIG_CH4_FACTOR + CONFIG_FLOAT_SIZE
#define CONFIG_CH1_MIN_THRESHOLD       CONFIG_CH1_MAX_THRESHOLD + CONFIG_FLOAT_SIZE
#define CONFIG_CH2_MAX_THRESHOLD       CONFIG_CH1_MIN_THRESHOLD + CONFIG_FLOAT_SIZE
#define CONFIG_CH2_MIN_THRESHOLD       CONFIG_CH2_MAX_THRESHOLD + CONFIG_FLOAT_SIZE
#define CONFIG_CH3_MAX_THRESHOLD       CONFIG_CH2_MIN_THRESHOLD + CONFIG_FLOAT_SIZE
#define CONFIG_CH3_MIN_THRESHOLD       CONFIG_CH3_MAX_THRESHOLD + CONFIG_FLOAT_SIZE
#define CONFIG_CH4_MAX_THRESHOLD       CONFIG_CH3_MIN_THRESHOLD + CONFIG_FLOAT_SIZE
#define CONFIG_CH4_MIN_THRESHOLD       CONFIG_CH4_MAX_THRESHOLD + CONFIG_FLOAT_SIZE
#define CONFIG_TRAP_1                  CONFIG_CH4_MIN_THRESHOLD + CONFIG_FLOAT_SIZE
#define CONFIG_TRAP_2                  CONFIG_TRAP_1 + CONFIG_CHAR_SIZE
#define CONFIG_TRAP_3                  CONFIG_TRAP_2 + CONFIG_CHAR_SIZE
#define CONFIG_TRAP_4                  CONFIG_TRAP_3 + CONFIG_CHAR_SIZE

#define CONFIG_IPADDRESS_1_DEFAULT     192
#define CONFIG_IPADDRESS_2_DEFAULT     168
#define CONFIG_IPADDRESS_3_DEFAULT     0
#define CONFIG_IPADDRESS_4_DEFAULT     100
#define CONFIG_MASK_1_DEFAULT          255
#define CONFIG_MASK_2_DEFAULT          255
#define CONFIG_MASK_3_DEFAULT          255
#define CONFIG_MASK_4_DEFAULT          0
#define CONFIG_GW_1_DEFAULT            192
#define CONFIG_GW_2_DEFAULT            168
#define CONFIG_GW_3_DEFAULT            0
#define CONFIG_GW_4_DEFAULT            1
#define CONFIG_DHCP_DEFAULT            0
#define CONFIG_DNS_1_DEFAULT           8
#define CONFIG_DNS_2_DEFAULT           8
#define CONFIG_DNS_3_DEFAULT           8
#define CONFIG_DNS_4_DEFAULT           8
#define CONFIG_USERNAME_DEFAULT        "admin"
#define CONFIG_PASSWORD_DEFAULT        "admin"
#define CONFIG_HOSTNAME_DEFAULT        "monitor"
#define CONFIG_SNMP_COMmUNITY_DEFAULT  "snmp"
#define CONFIG_CURRENT_FACTOR_DEFAULT  24.56
#define CONFIG_VOLTAGE_FACTOR_DEFAULT  24.56


union parse_float_byte{
   float f;
   char b[4];
};

union parse_int_byte{
   uint16_t i;
   char b[2];
};

class Configuration
{
   private:
   int read_array_eeprom(int address, char *array, int length, char eos = EEPROM_EOS)
   {
      for(int i=0;i<length;i++)
      {
         char b = EEPROM.read(address + i);
         if( (eos == b) || (b == EEPROM_EOS) )
            return i;
         array[i] = b;
      }
      return length;
   }

   void write_array_eeprom(int address, char *array, int length)
   {
      for(int i=0;i<length;i++)
      {
         EEPROM.write(address + i, array[i]);
      }
      EEPROM.commit();
   }

   String read_string_eeprom(int address, int length = 512)
   {
      char readed[length];
      int num = read_array_eeprom(address, readed, length, 0);
      String output = "";
      for(int i=0;i<num;i++)
         output += readed[i];

      return output;
   }

   void write_string_eeprom(int address, char *array, int length)
   {
      array[length] = 0;
      write_array_eeprom(address, array, length+1);
   }

   String read_string_parameter(int address, int size, String toset = "", String default_value = "")
   {
      String s = "";
      char array[size];

      if(toset != "")
      {
         if(toset.length() >= size)
            toset = toset.substring(0, size-1);
         toset.toCharArray(array, size);
         write_string_eeprom(address, array, toset.length());
      }
      s = read_string_eeprom(address, size);
      s = s == "" ? default_value : s;

      return s;
   }

   float read_float_parameter(int address, float toset = -1, float default_value = -1)
   {
      if(toset >= 0)
         write_float_parameter(address, toset);
      union parse_float_byte p2;
      int readed = read_array_eeprom(address, p2.b, 4);
      if(readed <= 0)
         return default_value;

      return p2.f;
   }

   int read_int_parameter(int address, int default_value)
   {
      union parse_int_byte p2;
      int readed = read_array_eeprom(address, p2.b, 2);
//      Serial.print("Int readed ");
//      Serial.print(v[1], HEX);
//      Serial.print(" ");
//      Serial.print(v[0], HEX);
//      Serial.print(" received:");
//      Serial.println(readed);
      if(readed <= 0)
         return default_value;
      return p2.i;
   }

   uint8_t read_byte_parameter(int address, uint8_t default_value)
   {
      uint8_t b = EEPROM.read(address);
      return b == EEPROM_EOS ? default_value : b;
   }

   void write_float_parameter(int address, float value)
   {
      union parse_float_byte parse;
      parse.f = value;
      write_array_eeprom(address, parse.b, 4);
   }

   void write_int_parameter(int address, int value)
   {
      union parse_int_byte parse;
      parse.i = value;
      write_array_eeprom(address, parse.b, 2);
   }

   void write_byte_parameter(int address, uint8_t value)
   {
      EEPROM.write(address, value);
      EEPROM.commit();
   }

   int get

   public:

   void GetIpConfig(uint8_t selector, uint8_t *octets){
      uint8_t default_values[4];
      //default_values = selector == IP_ADDRESS_ENUM ? [CONFIG_IPADDRESS_1_DEFAULT,CONFIG_IPADDRESS_2_DEFAULT,CONFIG_IPADDRESS_3_DEFAULT,CONFIG_IPADDRESS_4_DEFAULT] : default_values;
      //default_values = selector == IP_MASK_ENUM ? [CONFIG_MASK_1_DEFAULT,CONFIG_MASK_2_DEFAULT,CONFIG_MASK_3_DEFAULT,CONFIG_MASK_4_DEFAULT] : default_values;
      //default_values = selector == IP_GW_ENUM ? [CONFIG_GW_1_DEFAULT,CONFIG_GW_2_DEFAULT,CONFIG_GW_3_DEFAULT,CONFIG_GW_4_DEFAULT] : default_values;
      //default_values = selector == IP_DNS_ENUM ? [CONFIG_DNS_1_DEFAULT,CONFIG_DNS_2_DEFAULT,CONFIG_DNS_3_DEFAULT,CONFIG_DNS_4_DEFAULT] : default_values;

      int base_address = -1;
      base_address = selector == IP_ADDRESS_ENUM ? CONFIG_IPADDRESS_1 : base_address;
      base_address = selector == IP_MASK_ENUM ? CONFIG_MASK_1 : base_address;
      base_address = selector == IP_GW_ENUM ? CONFIG_GW_1 : base_address;
      base_address = selector == IP_DNS_ENUM ? CONFIG_DNS_1 : base_address;
      base_address = selector == IP_TRAP_ENUM ? CONFIG_TRAP_1 : base_address;
      if(base_address < 0)
         return;

      for(int i=0;i<4;i++)
      {
         *octets = read_byte_parameter(base_address + i*CONFIG_CHAR_SIZE, 0);
         octets++;
      }
   }

   uint8_t GetIpAddress(int octet)
   {
      if(octet == 1)
         return read_byte_parameter(CONFIG_IPADDRESS_1, CONFIG_IPADDRESS_1_DEFAULT);
      if(octet == 2)
         return read_byte_parameter(CONFIG_IPADDRESS_2, CONFIG_IPADDRESS_2_DEFAULT);
      if(octet == 3)
         return read_byte_parameter(CONFIG_IPADDRESS_3, CONFIG_IPADDRESS_3_DEFAULT);
      if(octet == 4)
         return read_byte_parameter(CONFIG_IPADDRESS_4, CONFIG_IPADDRESS_4_DEFAULT);

      return 0;
   }

   void ResetIpConfig(){
      uint8_t octets[4];
      octets[0] = CONFIG_IPADDRESS_1_DEFAULT;
      octets[1] = CONFIG_IPADDRESS_2_DEFAULT;
      octets[2] = CONFIG_IPADDRESS_3_DEFAULT;
      octets[3] = CONFIG_IPADDRESS_4_DEFAULT;
      SetIpConfig(IP_ADDRESS_ENUM, octets);

      octets[0] = CONFIG_MASK_1_DEFAULT;
      octets[1] = CONFIG_MASK_2_DEFAULT;
      octets[2] = CONFIG_MASK_3_DEFAULT;
      octets[3] = CONFIG_MASK_4_DEFAULT;
      SetIpConfig(IP_MASK_ENUM, octets);

      octets[0] = CONFIG_GW_1_DEFAULT;
      octets[1] = CONFIG_GW_2_DEFAULT;
      octets[2] = CONFIG_GW_3_DEFAULT;
      octets[3] = CONFIG_GW_4_DEFAULT;
      SetIpConfig(IP_GW_ENUM, octets);

      octets[0] = CONFIG_DNS_1_DEFAULT;
      octets[1] = CONFIG_DNS_2_DEFAULT;
      octets[2] = CONFIG_DNS_3_DEFAULT;
      octets[3] = CONFIG_DNS_4_DEFAULT;
      SetIpConfig(IP_DNS_ENUM, octets);

      octets[0] = 0;
      octets[1] = 0;
      octets[2] = 0;
      octets[3] = 0;
      SetIpConfig(IP_TRAP_ENUM, octets);

      SetDCHP(false);
   }

   void SetToFactory(){
      PASSWORD(CONFIG_PASSWORD_DEFAULT);
      USERNAME(CONFIG_USERNAME_DEFAULT);
      HOSTNAME(CONFIG_HOSTNAME_DEFAULT);
      SNMP_COMMUNITY(CONFIG_SNMP_COMmUNITY_DEFAULT);

      ResetIpConfig();
   }

   void SetIpConfig(uint8_t selector, uint8_t *octets){
      int base_address = -1;
      base_address = selector == IP_ADDRESS_ENUM ? (CONFIG_IPADDRESS_1) : base_address;
      base_address = selector == IP_MASK_ENUM ? (CONFIG_MASK_1) : base_address;
      base_address = selector == IP_GW_ENUM ? (CONFIG_GW_1) : base_address;
      base_address = selector == IP_DNS_ENUM ? (CONFIG_DNS_1) : base_address;
      base_address = selector == IP_TRAP_ENUM ? (CONFIG_TRAP_1) : base_address;

      if(base_address < 0)
         return;

      for(int i=0;i<4;i++)
      {
         int address = base_address + i*CONFIG_CHAR_SIZE;

         Serial.print("SetIpConfig. Selector: ");
         Serial.print(selector);
         Serial.print(" -> address: ");
         Serial.print(address);
         Serial.print(" value: ");
         Serial.println(*octets);
         write_byte_parameter(address, *octets);
         octets++;
      }
      Serial.println("");
   }

   uint8_t SetIpAddress(int octet, uint8_t value)
   {
      if(octet == 1){
         write_byte_parameter(CONFIG_IPADDRESS_1, value);
      }
      if(octet == 2){
         write_byte_parameter(CONFIG_IPADDRESS_2, value);
      }
      if(octet == 3)
      {
         write_byte_parameter(CONFIG_IPADDRESS_3, value);
      }
      if(octet == 4){
         write_byte_parameter(CONFIG_IPADDRESS_4, value);
      }

      return GetIpAddress(octet);
   }

   uint8_t GetGWAddress(int octet)
   {
      if(octet == 1)
         return read_byte_parameter(CONFIG_GW_1, CONFIG_GW_1_DEFAULT);
      if(octet == 2)
         return read_byte_parameter(CONFIG_GW_2, CONFIG_GW_2_DEFAULT);
      if(octet == 3)
         return read_byte_parameter(CONFIG_GW_3, CONFIG_GW_3_DEFAULT);
      if(octet == 4)
         return read_byte_parameter(CONFIG_GW_4, CONFIG_GW_4_DEFAULT);

      return 0;
   }

   uint8_t SetGWAddress(int octet, uint8_t value)
   {
      if(octet == 1)
         write_byte_parameter(CONFIG_GW_1, value);
      if(octet == 2)
         write_byte_parameter(CONFIG_GW_2, value);
      if(octet == 3)
         write_byte_parameter(CONFIG_GW_3, value);
      if(octet == 4)
         write_byte_parameter(CONFIG_GW_4, value);

      return GetGWAddress(octet);
   }

   uint8_t GetIpMask(int octet)
   {
      if(octet == 1)
         return read_byte_parameter(CONFIG_MASK_1, CONFIG_MASK_1_DEFAULT);
      if(octet == 2)
         return read_byte_parameter(CONFIG_MASK_2, CONFIG_MASK_2_DEFAULT);
      if(octet == 3)
         return read_byte_parameter(CONFIG_MASK_3, CONFIG_MASK_3_DEFAULT);
      if(octet == 4)
         return read_byte_parameter(CONFIG_MASK_4, CONFIG_MASK_4_DEFAULT);

      return 0;
   }

   uint8_t GetIpDns(int octet)
   {
      if(octet == 1)
         return read_byte_parameter(CONFIG_DNS_1, CONFIG_DNS_1_DEFAULT);
      if(octet == 2)
         return read_byte_parameter(CONFIG_DNS_2, CONFIG_DNS_2_DEFAULT);
      if(octet == 3)
         return read_byte_parameter(CONFIG_DNS_3, CONFIG_DNS_3_DEFAULT);
      if(octet == 4)
         return read_byte_parameter(CONFIG_DNS_4, CONFIG_DNS_4_DEFAULT);

      return 0;
   }

   uint8_t SetIpMask(int octet, uint8_t value)
   {
      if(octet == 1)
         write_byte_parameter(CONFIG_MASK_1, value);
      if(octet == 2)
         write_byte_parameter(CONFIG_MASK_2, value);
      if(octet == 3)
         write_byte_parameter(CONFIG_MASK_3, value);
      if(octet == 4)
         write_byte_parameter(CONFIG_MASK_4, value);

      return GetIpMask(octet);
   }

   float ADC_FACTOR(int channel, float factor = -1){
      if(channel == 3 || channel == 4)
         return CONFIG_VOLTAGE_FACTOR_DEFAULT;
         
      int address = 0;
      address = channel == 1 ? CONFIG_CH1_FACTOR : address;
      address = channel == 2 ? CONFIG_CH2_FACTOR : address;
      address = channel == 3 ? CONFIG_CH3_FACTOR : address;
      address = channel == 4 ? CONFIG_CH4_FACTOR : address;

      if(factor > 0){
         Serial.print("Debug - Configuration.ADC_FACTOR. Address = ");
         Serial.println(address);
      }

      if(address == 0)
         return 0;

      float defaultValue = channel == 1 || channel == 2 ? CONFIG_CURRENT_FACTOR_DEFAULT : 1;
      defaultValue = channel == 3 || channel == 4 ? CONFIG_VOLTAGE_FACTOR_DEFAULT : defaultValue;

      if(factor > 0){
         Serial.print("Debug - Configuration.ADC_FACTOR. Default Value = ");
         Serial.println(defaultValue);
         Serial.print("Debug - Configuration.ADC_FACTOR. Factor = ");
         Serial.println(factor);
         write_float_parameter(address, factor);
      }

      return read_float_parameter(address, factor, defaultValue);
   }

   String PASSWORD(String password = "")
   {
      return read_string_parameter(CONFIG_PASSWORD, CONFIG_CHAR50_SIZE, password, CONFIG_PASSWORD_DEFAULT);
   }

   String USERNAME(String username = "")
   {
      return read_string_parameter(CONFIG_USERNAME, CONFIG_CHAR50_SIZE, username, CONFIG_USERNAME_DEFAULT);
   }

   String HOSTNAME(String name = "")
   {
      return read_string_parameter(CONFIG_HOSTNAME, CONFIG_CHAR50_SIZE, name, CONFIG_HOSTNAME_DEFAULT);
   }

   String SNMP_COMMUNITY(String community = "")
   {
      return read_string_parameter(CONFIG_SNMP_COMMUNITY, CONFIG_CHAR50_SIZE, community, CONFIG_SNMP_COMmUNITY_DEFAULT);
   }

   bool GetDHCP()
   {
      byte dhcp = read_byte_parameter(CONFIG_DHCP, CONFIG_DHCP_DEFAULT);
      return dhcp == 1 ? true : false;
   }

   bool SetDCHP(bool dhcp)
   {
      write_byte_parameter(CONFIG_DHCP, dhcp ? 1 : 0);
      return GetDHCP();
   }

   float GetCalYPoint(uint8_t channel, uint8_t point){
      int address = -1;
      address = channel == 1 && point == 1 ? CONFIG_CAL_CH1_Y1 : address;
      address = channel == 2 && point == 1 ? CONFIG_CAL_CH1_Y1 : address;
      address = channel == 3 && point == 1 ? CONFIG_CAL_CH2_Y1 : address;
      address = channel == 4 && point == 1 ? CONFIG_CAL_CH3_Y1 : address;
      address = channel == 1 && point == 2 ? CONFIG_CAL_CH1_Y2 : address;
      address = channel == 2 && point == 2 ? CONFIG_CAL_CH1_Y2 : address;
      address = channel == 3 && point == 2 ? CONFIG_CAL_CH2_Y2 : address;
      address = channel == 4 && point == 2 ? CONFIG_CAL_CH3_Y2 : address;

      return address < 0 ? 1 : read_float_parameter(address, -1, 0);
   }

   int GetCalXPoint(uint8_t channel, uint8_t point){
      int address = -1;
      address = channel == 1 && point == 1 ? CONFIG_CAL_CH1_X1 : address;
      address = channel == 2 && point == 1 ? CONFIG_CAL_CH1_X1 : address;
      address = channel == 3 && point == 1 ? CONFIG_CAL_CH2_X1 : address;
      address = channel == 4 && point == 1 ? CONFIG_CAL_CH3_X1 : address;
      address = channel == 1 && point == 2 ? CONFIG_CAL_CH1_X2 : address;
      address = channel == 2 && point == 2 ? CONFIG_CAL_CH1_X2 : address;
      address = channel == 3 && point == 2 ? CONFIG_CAL_CH2_X2 : address;
      address = channel == 4 && point == 2 ? CONFIG_CAL_CH3_X2 : address;

      return address < 0 ? 1 : (int)read_float_parameter(address, -1, 0);
   }

   float THRESHOLD_MAX(int channel, float threshold = -1){
      int address = 0;
      address = channel == 1 ? CONFIG_CH1_MAX_THRESHOLD : address;
      address = channel == 2 ? CONFIG_CH2_MAX_THRESHOLD : address;
      address = channel == 3 ? CONFIG_CH3_MAX_THRESHOLD : address;
      address = channel == 4 ? CONFIG_CH4_MAX_THRESHOLD : address;

      if(address == 0)
         return 0;

      return read_float_parameter(address, threshold, 1000);
   }

   float THRESHOLD_MIN(int channel, float threshold = -1){
      int address = 0;
      address = channel == 1 ? CONFIG_CH1_MIN_THRESHOLD : address;
      address = channel == 2 ? CONFIG_CH2_MIN_THRESHOLD : address;
      address = channel == 3 ? CONFIG_CH3_MIN_THRESHOLD : address;
      address = channel == 4 ? CONFIG_CH4_MIN_THRESHOLD : address;

      if(address == 0)
         return 0;

      return read_float_parameter(address, threshold, 1000);
   }

// Variables de sistema
};

#endif