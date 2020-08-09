/*
  user_config_override.h - user configuration overrides my_user_config.h for Tasmota

  Copyright (C) 2020  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

// force the compiler to show a warning to confirm that this file is included
#warning **** user_config_override.h: Using Settings from this File ****

/*****************************************************************************************************\
 * USAGE:
 *   To modify the stock configuration without changing the my_user_config.h file:
 *   (1) copy this file to "user_config_override.h" (It will be ignored by Git)
 *   (2) define your own settings below
 *   (3) for platformio:
 *         All done.
 *       for Arduino IDE:
 *         enable define USE_CONFIG_OVERRIDE in my_user_config.h
 ******************************************************************************************************
 * ATTENTION:
 *   - Changes to SECTION1 PARAMETER defines will only override flash settings if you change define CFG_HOLDER.
 *   - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *   - You still need to update my_user_config.h for major define USE_MQTT_TLS.
 *   - All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
\*****************************************************************************************************/

/*
Examples :

// -- Master parameter control --------------------
#undef  CFG_HOLDER
#define CFG_HOLDER        4617                   // [Reset 1] Change this value to load SECTION1 configuration parameters to flash

// -- Setup your own Wifi settings  ---------------
#undef  STA_SSID1
#define STA_SSID1         "YourSSID"             // [Ssid1] Wifi SSID

#undef  STA_PASS1
#define STA_PASS1         "YourWifiPassword"     // [Password1] Wifi password

// -- Setup your own MQTT settings  ---------------
#undef  MQTT_HOST
#define MQTT_HOST         "your-mqtt-server.com" // [MqttHost]

#undef  MQTT_PORT
#define MQTT_PORT         1883                   // [MqttPort] MQTT port (10123 on CloudMQTT)

#undef  MQTT_USER
#define MQTT_USER         "YourMqttUser"         // [MqttUser] Optional user

#undef  MQTT_PASS
#define MQTT_PASS         "YourMqttPass"         // [MqttPassword] Optional password

// You might even pass some parameters from the command line ----------------------------
// Ie:  export PLATFORMIO_BUILD_FLAGS='-DUSE_CONFIG_OVERRIDE -DMY_IP="192.168.1.99" -DMY_GW="192.168.1.1" -DMY_DNS="192.168.1.1"'

#ifdef MY_IP
#undef  WIFI_IP_ADDRESS
#define WIFI_IP_ADDRESS   MY_IP                  // Set to 0.0.0.0 for using DHCP or enter a static IP address
#endif

#ifdef MY_GW
#undef  WIFI_GATEWAY
#define WIFI_GATEWAY      MY_GW                  // if not using DHCP set Gateway IP address
#endif

#ifdef MY_DNS
#undef  WIFI_DNS
#define WIFI_DNS          MY_DNS                 // If not using DHCP set DNS IP address (might be equal to WIFI_GATEWAY)
#endif

*/

#undef MODULE
#define MODULE USER_MODULE

#undef FALLBACK_MODULE
#define FALLBACK_MODULE        USER_MODULE      // [Module2] Select default module on fast reboot where USER_MODULE is user template
#undef USER_TEMPLATE
#define USER_TEMPLATE "{\"NAME\":\"Avatar UK 10A\",\"GPIO\":[0,0,56,0,0,134,0,0,131,17,132,21,0],\"FLAG\":0,\"BASE\":45}" // [Template] Set JSON template

#undef WIFI_CONFIG_TOOL
#define WIFI_CONFIG_TOOL       WIFI_MANAGER        // [WifiConfig] Default tool if wifi fails to connect (default option: 4 - WIFI_RETRY)

//#undef  STA_SSID1
//#define STA_SSID1         "STA GOES HERE"             // [Ssid1] Wifi SSID

//#undef  STA_PASS1
//#define STA_PASS1         "PASSWORD GOES HERE"     // [Password1] Wifi password

// -- Setup your own MQTT settings  ---------------
#undef  MQTT_HOST
#define MQTT_HOST         "broker.stanford-clark.com" // [MqttHost]

#undef  MQTT_PORT
#define MQTT_PORT         1885                   // [MqttPort] MQTT port (10123 on CloudMQTT)

#undef MQTT_FULLTOPIC
#define MQTT_FULLTOPIC         "AK/%prefix%/%topic%/" // [FullTopic] Subscribe and Publish full topic name - Legacy topic

#undef TELE_PERIOD
#define TELE_PERIOD            60               // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)

#undef WEB_PASSWORD
#define WEB_PASSWORD           "decafbad00"                // [WebPassword] Web server Admin mode Password for WEB_USERNAME (empty string = Disable)

// London
#undef LATITUDE
#define LATITUDE               51.509865         // [Latitude] Your location to be used with sunrise and sunset
#undef LONGITUDE
#define LONGITUDE              -0.118092          // [Longitude] Your location to be used with sunrise and sunset

// BST
#undef APP_TIMEZONE
#define APP_TIMEZONE           99                 // [Timezone] +1 hour (Amsterdam) (-13 .. 14 = hours from UTC, 99 = use TIME_DST/TIME_STD)

// UK
#undef TIME_DST_HOUR
#define TIME_DST_HOUR          1                 // Hour (0 to 23)
#undef TIME_DST_OFFSET
#define TIME_DST_OFFSET        +60              // Offset from UTC in minutes (-780 to +780)

#undef TIME_STD_HOUR   
#define TIME_STD_HOUR          2   
#undef TIME_STD_OFFSET
#define TIME_STD_OFFSET        +0               // Offset from UTC in minutes (-780 to +780)

#ifndef USE_SCRIPT
#define USE_SCRIPT  // adds about 17k flash size, variable ram size
#endif
#ifdef USE_RULES
#undef USE_RULES
#endif  
#define USE_SCRIPT_SUB_COMMAND
#define SCRIPT_POWER_SECTION

#endif  // _USER_CONFIG_OVERRIDE_H_
