#include "../../main/main.h"
#include "../driver_mqtt/mqtt_driver.h"
#include "../driver_ade/EM.h"
#include "../../build/config/sdkconfig.h"

static char sMqttTopic[80];
static char sMqttMessage[80];
static char meterName[40]; /**< Topic prefix/meter identifier */

static EM_Meas testVal;

// Chars for the messages
static char phaseNames[4] = {'A', 'B', 'C', 'N'};
static const char * rmsNames[2] = {"Current", "Voltage"}; 

void task_communication(void *arg)
{
    sprintf(sMqttTopic, "/ziggy/default");
    sprintf(sMqttMessage, "task teste");
    mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);

    sprintf(meterName, "/ziggy/Building%s/meter%d", CONFIG_METER_BUILDING_NAME, CONFIG_METER_NUMBER); /**< Topic prefix/meter identifier */

    for(;;)
    {
        //! JUST FOR SHOWING
        testVal.RMS.sVoltage.A = 320 + 20*(float)(esp_random())/(float)(UINT32_MAX)-10;
        testVal.RMS.sVoltage.B = 420 + 20*(float)(esp_random())/(float)(UINT32_MAX)-10;
        testVal.RMS.sVoltage.C = 220 + 20*(float)(esp_random())/(float)(UINT32_MAX)-10;

        testVal.RMS.sCurrent.A = 10 + 2*(float)(esp_random())/(float)(UINT32_MAX)-1;
        testVal.RMS.sCurrent.B = 20;
        testVal.RMS.sCurrent.C = 30 + 10*(float)(esp_random())/(float)(UINT32_MAX)-5;
        testVal.RMS.sCurrent.N = 1 + 2*(float)(esp_random())/(float)(UINT32_MAX)-1;

        // Sending the RMS values
        sprintf(sMqttTopic, "%s/RMS", meterName);
        for(int i=0;i<7;i++){
            sprintf(sMqttMessage, "%s %c = %f", i < 4 ? rmsNames[0] : rmsNames[1], 
                                                phaseNames[i%4],
                                                i < 4 ? testVal.RMS.aCurrent[i] : testVal.RMS.aVoltage[i%4]);
            mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);
        }

        vTaskDelay(5000/portTICK_RATE_MS); // TODO: Fix the inverval - 5s is for debug!!
    }
}
