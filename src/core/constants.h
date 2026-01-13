#ifndef __CONSTANTS_H__

#define __CONSTANTS_H__

#define STATUS_LED               2
#define POWER_LED                5
#define RPM_INPUT                19
#define RPM_TIMEOUT_COUNTER      10
#define RPM_BUFFER_AVG_MAX       50
#define BLINK_LED_TIME           300
#define TIMEOUT_1S               1000
#define TIMEOUT_TX_DATA          800
#define TIMEOUT_10S              20 * TIMEOUT_1S
#define WIFI_TIMEOUT_CONNECT     5000
#define ADC_VREF                 3.3
#define DEFAULT_VREF             1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES            50          //Multisampling
#define NO_ADC_CHANNEL           6
#define NO_PT100_CHANNEL         2
#define BLINK_TIME_MS            300
#define I2C_SDA 21
#define I2C_SCL 22

#define SENSOR_VERSION           "1.0.8"
#define WIFI_SSID_DEFAULT        "WashingTesterWifi"//"WashingTesterConnection"
#define WIFI_PASS_DEFAULT        "WT134679!" //"WT134679"
#define HW_NAME_DEFAULT          "Sensor"
#define HW_SERIAL_DEFAULT        "WT00001"
#define HW_UNIT_DEFAULT          ""
#define BOARD_ADDRESS_DEFAULT    "10.3.141.1"
#define CAL_CURRENT_DEFAULT      20          // Corriente de calibración en mA
#define CAL_FACTOR_DEFAULT       1.0
#define IMPENDANCE               50
#define ZERO_ADJUST_DEFAULT      0
#define CAL_20MA_Y_DEFAULT       20
#define CAL_20MA_X_DEFAULT       1241
#define CAL_4MA_Y_DEFAULT        4
#define CAL_4MA_X_DEFAULT        248
#define NUM_CHANNELS_DEFAULT     4
#define ENABLE_PT100_DEFAULT     0
#define ENABLE_RPM_DEFAULT       0
#define RPM_AVG_DEFAULT          10
#define RPM_TIMEOUT_DEFAULT      10
#define IP_ADDRESS_ENUM          0
#define IP_MASK_ENUM            1
#define IP_GW_ENUM              2
#define IP_DNS_ENUM             3
#define IP_TRAP_ENUM            4

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

#define API_KEY "Bearer 1NxkoZYiT3rW5IMVmeA0CqPenSXLRdFZ"

#endif