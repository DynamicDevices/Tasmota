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
//#warning **** user_config_override.h: Using Settings from this File ****

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

#undef PROJECT

#ifdef KC_KETTLE
#define PROJECT                "SmartKettle"         // PROJECT is used as the default topic delimiter
#else
#define PROJECT                "SmartPlug"         // PROJECT is used as the default topic delimiter
#endif

// We are currently using a custom template fo rthe Avatar UK 10A Smart Switch
// so make sure that this is the default when we first power up.
#undef MODULE
#undef FALLBACK_MODULE
#define MODULE USER_MODULE
#define FALLBACK_MODULE        USER_MODULE
#undef USER_TEMPLATE

// - button enabled
//#define USER_TEMPLATE "{\"NAME\":\"Avatar UK 10A\",\"GPIO\":[0,0,56,0,0,134,0,0,131,17,132,21,0],\"FLAG\":0,\"BASE\":45}" // [Template] Set JSON template
// - button disabled locally
//#define USER_TEMPLATE "{\"NAME\":\"Avatar UK 10A\",\"GPIO\":[0,0,56,0,0,134,0,0,131,0,132,21,0],\"FLAG\":0,\"BASE\":45}" // [Template] Set JSON template

// - button enabled
//#define USER_TEMPLATE "{\"NAME\":\"Gosund UP111\",\"GPIO\":[0,320,0,32,2720,2656,0,0,2624,576,224,0,0,0],\"FLAG\":0,\"BASE\":18}" // Template Set JSON template
// - button disabled locally
#ifdef KC_KETTLE
#define USER_TEMPLATE "{\"NAME\":\"SmartKettle\",\"GPIO\":[0,320,0,0,1376,2656,0,0,2624,576,224,0,0,0],\"FLAG\":0,\"BASE\":18}" // Template Set JSON template
#else
#define USER_TEMPLATE "{\"NAME\":\"2NICE UP111\",\"GPIO\":[0,320,0,0,2720,2656,0,0,2624,576,224,0,0,0],\"FLAG\":0,\"BASE\":18}" // Template Set JSON template
#endif

// This needs a little thinking. In general we want to be able to get into the WiFi manager
// running as an AP to configure the network. However if there is a network outage (e.g. AP
// turned off) we don't want to drop into our own AP mode until a power cycle as that will
// lead to intermittent failures.
#undef WIFI_CONFIG_TOOL
#define WIFI_CONFIG_TOOL       WIFI_MANAGER

// Set default AP and password here for ease of debugging
#undef  STA_SSID1
#undef  STA_PASS1
#define STA_SSID1         "Provisioning"
#define STA_PASS1         ""

// Set the minimal OTAU firmware image. This is because with our normal image it is too
// big in the device to do an update. So the process is do an OTA online from the mininmal
// image, let it reboot, then do an upload from the local image with the full feature-set
// NB. Eventually we'll put the full custom image online somewhere too
//
// *** Note that the full image is now getting to the point that it's too big even to update
//     to the minimal image. The get out of jail here is to append .bin.gz to the URL as a
//     "get out of jail" card but I am leaving this set to the uncompressed image size to
//     serve as a warning that things are getting too big in the main image if we can't update
//     to this one ***

// EDIT: Moved to using official one now

// TODO: Change this for different devices

// Use our own build for minimal trampoline to full fat firmware
#undef OTA_URL
#ifdef KC_KETTLE
#define OTA_URL "http://dl.kettlecompanion.com/devel/tasmota-kettle-min-tramp.bin.gz"
#else
#define OTA_URL "http://dl.kettlecompanion.com/devel/tasmota-minimal-trampoline.bin.gz"
#endif

// Are we building a minimal build that will auto-install and auto-trampoline to a full build?
//#define USE_TRAMPOLINE

// -- Setup your own MQTT settings  ---------------
#undef  MQTT_HOST
#undef  MQTT_PORT
#undef MQTT_FULLTOPIC

// We have a partial implementation for Watson topic requirements
// Subscriptions are currently not supported and LWT is disabled?
//#define USE_MQTT_WATSON_IOT
#define USE_MQTT_MOSQUITTO // Use our Mosquitto server (which is based on the WIOT changes). Note we can define both and both CA certs will build in

#if !defined(FIRMWARE_MINIMAL) && !defined(FIRMWARE_LITE)

#if 1
#ifndef USE_MQTT_TLS
#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
#define USE_MQTT_TLS_CA_CERT                   // Force full CA validation instead of fingerprints, slower, but simpler to use.  (+2.2k code, +1.9k mem during connection handshake)
                                                  // This includes the LetsEncrypt CA in tasmota_ca.ino for verifying server certificates
//  #define USE_MQTT_TLS_FORCE_EC_CIPHER           // Force Elliptic Curve cipher (higher security) required by some servers (automatically enabled with USE_MQTT_AWS_IOT) (+11.4k code, +0.4k mem)
#endif
#endif

#endif // FIRMWARE_MINIMAL

#ifdef ESP32

// For cellular
#define USE_MQTT_TINYGSM            true          // Beta: Cellular support
//#define MQTT_MODEM_PORT            Serial       // Beta: Arduino serial port to use for MQTT comms.
//#define TINY_GSM_MODEM_BG96                     // Beta: Compatible with Quectel BG600L
#define MQTT_MODEM_PORT             Serial1       // Beta: Arduino serial port to use for MQTT comms.
#define TINY_GSM_MODEM_SIM800                     // Beta: Compatible with SIM800
#define SIM800L_IP5306_VERSION_20200811
//#define DUMP_AT_COMMANDS
#define TINY_GSM_DEBUG Serial

//#define TINY_GSM_TCP_KEEPALIVE_SECS 3600          // Maximum for SIM800 is 7200 but this fails ? (Leave this as default should be 7200)

#define GPRS_APN "internet.cxn"
#define GPRS_USER ""
#define GPRS_PASS ""
//#define GPRS_APN "wap.o2.co.uk"
//#define GPRS_USER "o2web"
//#define GPRS_PASS "password"

// Increase keepalive for cellular
#undef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE              7200
#undef TELE_PERIOD
#define TELE_PERIOD                 7200          // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)

#endif

// DEBUG For KC Cellular unit testing
#undef CSE_UREF
#define CSE_UREF                    188           // Custom HW power monitoring IC resistor value

//#undef APP_POWERON_STATE
//#define APP_POWERON_STATE      POWER_ALL_ALWAYS_ON


// We are forwarding the connection via DNS so don't
// check the actual hostname of the server
//#define DISABLE_SNI_CHECK

#define MQTT_HOST         "mqtt.kettlecompanion.com"
#define MQTT_PORT         8883
#undef MQTT_TLS_ENABLED
#define MQTT_TLS_ENABLED       true             // [SetOption103] Enable TLS mode (requires TLS version)

#ifndef MQTT_CLIENT_ID
#define MQTT_CLIENT_ID    "dummy_client_id"
#endif
#ifndef MQTT_USER
#define MQTT_USER         "dummy_mqtt_user"
#endif
#ifndef MQTT_PASS
#define MQTT_PASS         "dummy_mqtt_password"
#endif

#undef MQTT_TOPIC
#define MQTT_TOPIC             PROJECT "_FactoryDefault"
#undef MQTT_FULLTOPIC
#define MQTT_FULLTOPIC    "%prefix%/%topic%"

#undef MQTT_GRPTOPIC
#ifdef KC_KETTLE
#define MQTT_GRPTOPIC          "kettles"        // [GroupTopic] MQTT Group topic
#else
#define MQTT_GRPTOPIC          "smartplugs"        // [GroupTopic] MQTT Group topic
#endif

// Heartbeat currently set to 60s. Could easily take this up to 5 mins or more
#undef TELE_PERIOD
#define TELE_PERIOD            120               // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)

// For testing with Wemos LED board (i.e. test GlowOrb firmware)
#undef USE_WS2812_CTYPE
#define USE_WS2812_CTYPE     NEO_RGB           // Color type (NEO_RGB, NEO_GRB, NEO_BRG, NEO_RBG, NEO_RGBW, NEO_GRBW)

// Web / Alexa friendly nam
#undef FRIENDLY_NAME
#ifdef KC_KETTLE
#define FRIENDLY_NAME          "KC_SmartKettle"         // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa
#else
#define FRIENDLY_NAME          "KC_SmartPlug"         // [FriendlyName] Friendlyname up to 32 characters used by webpages and Alexa
#endif

// Set the web admin password here
#undef WEB_PASSWORD
#define WEB_PASSWORD ""

// Enable m-DNS
#undef MDNS_ENABLED
#define MDNS_ENABLED           true             // [SetOption55] Use mDNS (false = Disable, true = Enable)

#undef USE_DISCOVERY
#define USE_DISCOVERY                            // Enable mDNS for the following services (+8k code or +23.5k code with core 2_5_x, +0.3k mem)
#undef WEBSERVER_ADVERTISE
#define WEBSERVER_ADVERTISE                    // Provide access to webserver by name <Hostname>.local/
#undef MQTT_HOST_DISCOVERY
//#define MQTT_HOST_DISCOVERY                    // Find MQTT host server (overrides MQTT_HOST if found)

#undef USE_HOME_ASSISTANT                      // HA publications bugger up Watson

// -- Time - Up to three NTP servers in your region
#undef NTP_SERVER1
#undef NTP_SERVER2
#undef NTP_SERVER3
#define NTP_SERVER1            "pool.ntp.org"       // [NtpServer1] Select first NTP server by name or IP address (129.250.35.250)
#define NTP_SERVER2            "uk.pool.ntp.org"    // [NtpServer2] Select second NTP server by name or IP address (5.39.184.5)
#define NTP_SERVER3            "0.uk.pool.ntp.org"  // [NtpServer3] Select third NTP server by name or IP address (93.94.224.67)

// GPS coords for London for rough sunrise/sunset timings
#undef LATITUDE
#define LATITUDE               51.509865         // [Latitude] Your location to be used with sunrise and sunset
#undef LONGITUDE
#define LONGITUDE              -0.118092          // [Longitude] Your location to be used with sunrise and sunset

// Use DST custom timezone settings
#undef APP_TIMEZONE
#define APP_TIMEZONE           99

// UK DST settings
#undef TIME_DST_HOUR
#define TIME_DST_HOUR          1                 // Hour (0 to 23)
#undef TIME_DST_OFFSET
#define TIME_DST_OFFSET        +60              // Offset from UTC in minutes (-780 to +780)

#undef TIME_STD_HOUR
#define TIME_STD_HOUR          2
#undef TIME_STD_OFFSET
#define TIME_STD_OFFSET        +0               // Offset from UTC in minutes (-780 to +780)

#undef USE_PING
#define USE_PING                                 // Enable Ping command (+2k code)

// I am seeing a problem with constant cycling when updating to the full Tasmota image
// Not sure why this is. The RULE1 seems to be left over in the flash from the previous
// lite or minimal upgrade and we loop constantly. So as an interim fix I am trying to
// disable rules in the full version so we don't loop constantly
#undef USE_RULES

// Enable scripting
#define USE_SCRIPT

#ifdef KC_KETTLE

// Starting boil script implementation for model kettle
#define START_SCRIPT_FROM_BOOT
#define USER_BACKLOG "script 1"
#define PRECONFIGURED_SCRIPT ">D\r\nt=0\r\nr=0\r\ng=0\r\nb=0\r\nlevel=255\r\n>B\r\ndone=0\r\nt=0\r\nr=0\r\ng=0\r\nb=0\r\nlevel=255\r\n>F\r\nif t==0 {\r\nprint Start\r\nr=0\r\ng=0\r\nb=0\r\nlevel=255\r\n-> power2 0\r\n-> dimmer 100\r\n-> hsbcolor 0,100,100\r\n-> channel 100,0,0\r\n-> color %r%,%g%,%b%\r\n-> power2 1\r\n}\r\nelse {\r\nif b<254\r\nand r<254 {\r\nb=b+2\r\n} else {\r\nif b==254\r\nand r<254 {\r\nr=r+2\r\n} else {\r\nif b>0 {\r\nb=b-2\r\n}\r\nelse {\r\nt = -1\r\nr = 0\r\nb = 0\r\ng = 255\r\nprint Stop\r\n+> script 0\r\n}\r\n}\r\n}\r\n-> color %r%,%g%,%b%\r\n}\r\nt = t+1\r\n"

#endif

// RULES - Force upgrade to full on connection
#ifdef USE_TRAMPOLINE
  #undef USE_SCRIPT
  #define USE_RULES

// Run this rule then turn it off
#ifdef KC_KETTLE
#define USER_RULE1 "ON Wifi#Connected DO OTAURL http://dl.kettlecompanion.com/devel/tasmota-kettle.bin.gz ENDON ON Wifi#Connected DO BACKLOG UPGRADE 1; RULE1 0 ENDON\r\nRULE1 1"          // Add rule1 data saved at initial firmware load or when command reset is executed
#else
#define USER_RULE1 "ON Wifi#Connected DO OTAURL http://dl.kettlecompanion.com/devel/tasmota.bin.gz ENDON ON Wifi#Connected DO BACKLOG UPGRADE 1; RULE1 0 ENDON\r\nRULE1 1"          // Add rule1 data saved at initial firmware load or when command reset is executed
#endif

#endif

// LOGGING

//#define SYS_LOG_HOST           "192.168.1.55"
//#define SYS_LOG_LEVEL          LOG_LEVEL_DEBUG_MORE

// APP

// Make plug always come on when plugged in
#undef APP_POWERON_STATE
#define APP_POWERON_STATE      POWER_ALL_ON

// PRIVATE
#include "user_config_override_private.h"

#endif  // _USER_CONFIG_OVERRIDE_H_
