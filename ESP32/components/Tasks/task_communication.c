#include "../../main/main.h"
#include "../driver_mqtt/mqtt_driver.h"
#include "../driver_ade/electricalMeas.h"

static char sMqttTopic[80];
static char sMqttMessage[80];

static char meterName[40]; /**< Topic prefix/meter identifier */

static electricalMeas testVal = {
        .rms = {.current = {.A = 0, .B = 1, .C = 1.5, .N = 0.2},
                .voltage = {.A = 220, .B = 218, .C = 222}}
    };

void task_communication(void *arg)
{
    sprintf(sMqttTopic, "/ziggy/default");
    sprintf(sMqttMessage, "task teste");
    mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);

    sprintf(meterName, "/ziggy/Building%s/meter%d", CONFIG_METER_BUILDING_NAME, CONFIG_METER_NUMBER); /**< Topic prefix/meter identifier */

    ESP_LOGI("MQTT_EXAMPLE", RMS_TOPIC);

    for(;;)
    {
        //! JUST FOR SHOWING
        testVal.rms.voltage.A = 220 + 20*(float)(esp_random())/(float)(UINT32_MAX)-10;
        testVal.rms.voltage.B = 220 + 20*(float)(esp_random())/(float)(UINT32_MAX)-10;
        testVal.rms.voltage.C = 220 + 20*(float)(esp_random())/(float)(UINT32_MAX)-10;

        testVal.rms.current.A = 10 + 2*(float)(esp_random())/(float)(UINT32_MAX)-1;
        testVal.rms.current.B = 20;
        testVal.rms.current.C = 30 + 10*(float)(esp_random())/(float)(UINT32_MAX)-5;
        testVal.rms.current.N = 1 + 2*(float)(esp_random())/(float)(UINT32_MAX)-1;

        // Sending the RMS values
        sprintf(sMqttTopic, "%s/RMS", meterName);
        sprintf(sMqttMessage, "Current A = %f", testVal.rms.current.A);
        mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);

        sprintf(sMqttTopic, "%s/RMS", meterName);
        sprintf(sMqttMessage, "Current B = %f", testVal.rms.current.B);
        mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);

        sprintf(sMqttTopic, "%s/RMS", meterName);
        sprintf(sMqttMessage, "Current C = %f", testVal.rms.current.C);
        mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);

        sprintf(sMqttTopic, "%s/RMS", meterName);
        sprintf(sMqttMessage, "Current N = %f", testVal.rms.current.N);
        mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);

        sprintf(sMqttTopic, "%s/RMS", meterName);
        sprintf(sMqttMessage, "Voltage A = %f", testVal.rms.voltage.A);
        mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);

        sprintf(sMqttTopic, "%s/RMS", meterName);
        sprintf(sMqttMessage, "Voltage B = %f", testVal.rms.voltage.B);
        mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);

        sprintf(sMqttTopic, "%s/RMS", meterName);
        sprintf(sMqttMessage, "Voltage C = %f", testVal.rms.voltage.C);
        mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);

        

        // mqtt_publish(meterName, sMqttMessage2,0,1,0);
        vTaskDelay(5000/portTICK_RATE_MS); // TODO: Fix the inverval - 5s is for debug!!
    }
}
