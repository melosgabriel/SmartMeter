#include "../../main/main.h"
#include "../driver_mqtt/mqtt_driver.h"
#include "../driver_ade/electricalMeas.h"

static char sMqttTopic[80];
static char sMqttMessage[80];
static char sMqttMessage2[80];

static electricalMeas testVal = {
        .rms = {.current = {.A = 0, .B = 1, .C = 1.5, .N = 0.2},
                .voltage = {.A = 220, .B = 218, .C = 222}}
    };

void task_communication(void *arg)
{
    sprintf(sMqttTopic, "/ziggy/default");
    sprintf(sMqttMessage, "task teste");

    sprintf(sMqttMessage2, "Voltage B = %f", testVal.rms.voltage.B);

    ESP_LOGI("MQTT_EXAMPLE", RMS_TOPIC);

    for(;;)
    {
        mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);
        mqtt_publish(RMS_TOPIC, sMqttMessage2,0,1,0);
        vTaskDelay(3000/portTICK_RATE_MS); // TODO: Fix the inverval - 3s is for debug!!
    }
}
