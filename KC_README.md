
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