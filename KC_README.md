17/11/2024  v9.5.0.3/v1.1.3.1   - Force build

18/04/2022  v9.5.0.3/v1.1.3.0   - Fix issue with OTA URL always being "devel" even when running "prod" firmware
                                - Disable rules support in full Tasmota to try to prevent boot loop lockup

15/04/2022  v9.5.0.3/v1.1.2.0   - fix provisioning AP name from 'Provisoning' to 'Provisioning'
                                - revert to old Watson IoT broker

28/12/2021  v9.5.0.3/v1.1.1.0   - user_config_override: Change template default to '2NICE UP111' #3 
                                - user_config_override: Do not disable SNI check
                                - user_config_override: Change default broker to broker-new.kettlecompa… 
                                - user_config_override: Disable SYSLOG settings 
                                - user_config_override: Make plug always come on enabled when powered #4
                                - settings: Change power high level 1000->500mA #5 
                                - user_config_override: Change smart name to KC_SmartPlug #7 
                                
28/12/2021  v9.5.0.3/v1.1.0.0   - Add support for broker-new.kettlecompanion.com MQTT TLS cert
                                  This is in *addition* to the Watson TLS cert so both should work

25/07/2021  v9.5.0.3/v1.0.6.0   - Fix issue with MQTT configuration being wrong
                                - Don't check TLS server name (SNI) on certificate as we now forward

24/07/2021  v9.5.0.3/v1.0.5.0   - Merge in most recent Tasmota changes

24/07/2021  v9.5.0.2/v1.0.4.0   - Make MQTT_PASS and MQTT_CLIENT_ID "secrets" in the github CI build
                                - Add BUILD_TYPE to fix an issue where the development trampoline updates
                                  to the production image
                                - Change MQTT_HOST to broker.kettlecompanion.com
                                
05/07/2021  v9.5.0.2/v1.0.3.0   - Add in default provisioning MQTT user and password
                                - Change default AP to "Provisoning" with no password

04/07/2021  v9.5.0.2/v1.0.2.0   Merge upstream in

04/07/2021  v1.0.1.0            The Tasmota docs say don't exceed 17dBm for Wifi Tx power and we changed it up to 25dBm
                                so reverting this to 17dBm. Note: the WiFiPower command can be used to change this interactively
                   
03/07/2021  v1.0.0.0            Initial cut with out own KC version
