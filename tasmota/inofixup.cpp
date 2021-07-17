
// We don't seem to be able to provide command line macro definitions to .ino files
// so for now we're fixing up the definitions here

#include <stdio.h>

#define MAX_LEN 32

#define XSTR(x) STR(x)
#define STR(x) #x
#pragma message XSTR(MQTT_PASS)

char _mqttPassword[MAX_LEN+1]; 
char _mqttClientId[MAX_LEN+1];

char *GetDefaultMqttPassword(void)
{
    snprintf(_mqttPassword, 32, MQTT_PASS);
    return _mqttPassword;
}

char *GetDefaultMqttClientId(void)
{
    snprintf(_mqttClientId, 32, MQTT_CLIENT_ID);
    return _mqttClientId;
}