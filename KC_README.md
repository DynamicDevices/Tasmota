19/03/2023  v12.4.0/v2.0.0.0    - Found issues with our TLS configuration as we had a trust anchor for USE_MOSQUITTO for BearSSL
                                  which directly trusted broker-new.kettlecompanion.com. What we actually need is to trust the CA key
                                  which has been used to signed the certificate for the named server. This is changed now to use our
                                  own CA certificate which is [here](https://github.com/DynamicDevices/kc-devops/blob/master/keys/ca.key)

                                  This means older releases will only connect to WIoTP or broker-new. Whereas from now we should
                                  be able to connect to any named server with a key signed by our CA.

15/03/2023  v12.4.0.x/v1.1.5.0  - Merged in master from upstream Tasmota which is currently v12.4.0.0 + some bits
                                - Tested against our Mosquitto KC broker and made various changes to move away from WIOTP implementation
                                - So login and topics and such have changed
                                - Also removed dodgy 'io broker' fix publishing POWER to cmnd topic on connection 
                                - Seems to be working so tagging for test

15/03/2023  v11.1.0.0/v1.1.4.0   - Disable Watson MQTT support

18/04/2022  v11.1.0.0/v1.1.3.0   - Fix issue with OTA URL always being "devel" even when running "prod" firmware
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
