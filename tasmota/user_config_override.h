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

// We are currently using a custom template fo rthe Avatar UK 10A Smart Switch
// so make sure that this is the default when we first power up.
#undef MODULE
#undef FALLBACK_MODULE
#define MODULE USER_MODULE
#define FALLBACK_MODULE        USER_MODULE
#undef USER_TEMPLATE
#define USER_TEMPLATE "{\"NAME\":\"Avatar UK 10A\",\"GPIO\":[0,0,56,0,0,134,0,0,131,17,132,21,0],\"FLAG\":0,\"BASE\":45}" // [Template] Set JSON template

// This needs a little thinking. In general we want to be able to get into the WiFi manager
// running as an AP to configure the network. However if there is a network outage (e.g. AP
// turned off) we don't want to drop into our own AP mode until a power cycle as that will
// lead to intermittent failures.
#undef WIFI_CONFIG_TOOL
#define WIFI_CONFIG_TOOL       WIFI_MANAGER

// Set default AP and password here for ease of debugging
#undef  STA_SSID1
#undef  STA_PASS1
#define STA_SSID1         "AP GOES HERE"
#define STA_PASS1         "PASSWORD GOES HERE"

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
#undef OTA_URL
#define OTA_URL                "http://thehackbox.org/tasmota/release/tasmota-minimal.bin"  // [OtaUrl]

// -- Setup your own MQTT settings  ---------------
#undef  MQTT_HOST
#undef  MQTT_PORT
#undef MQTT_FULLTOPIC

// We have a partial implementation for Watson topic requirements
// Subscriptions are currently not supported and LWT is disabled
#define USE_MQTT_WATSON_IOT

// MQTT TLS works but we are using fingerprint "learning" checking 
// It is unknown if full CA cert checking will work or fit in the space
//
// *** MORE IMPORTANTLY IT IS NOT FULLY UNDERSTOOD WHEN THIS FINGERPRINT MIGHT
//     CHANGE AND WHAT THE IMPACT WOULD BE ON DEVICE OPERATION ***

#ifndef USE_MQTT_TLS 
#define USE_MQTT_TLS                             // Use TLS for MQTT connection (+34.5k code, +7.0k mem and +4.8k additional during connection handshake)
//  #define USE_MQTT_TLS_CA_CERT                   // Force full CA validation instead of fingerprints, slower, but simpler to use.  (+2.2k code, +1.9k mem during connection handshake)
                                                  // This includes the LetsEncrypt CA in tasmota_ca.ino for verifying server certificates
//  #define USE_MQTT_TLS_FORCE_EC_CIPHER           // Force Elliptic Curve cipher (higher security) required by some servers (automatically enabled with USE_MQTT_AWS_IOT) (+11.4k code, +0.4k mem)
#endif

#undef MQTT_USER
#undef MQTT_PASS
#undef MQTT_CLIENT_ID
#undef MQTT_FULLTOPIC

#define MQTT_HOST         "HOST GOES HERE"
#define MQTT_PORT         1883 or 8883 for TLS
#define MQTT_USER         "USER GOES HERE"
#define MQTT_PASS         "PASSWORD GOES HERE"
#define MQTT_CLIENT_ID    "CLIENT ID GOES HERE"
#define MQTT_FULLTOPIC    "iot-2/evt/%prefix%|%topic%"

// Heartbeat currently set to 60s. Could easily take this up to 5 mins or more
#undef TELE_PERIOD
#define TELE_PERIOD            60               // [TelePeriod] Telemetry (0 = disable, 10 - 3600 seconds)

// Set the web admin password here
#undef WEB_PASSWORD
#define WEB_PASSWORD           "PASSWORD GOES HERE"                // [WebPassword] Web server Admin mode Password for WEB_USERNAME (empty string = Disable)

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

#endif  // _USER_CONFIG_OVERRIDE_H_
