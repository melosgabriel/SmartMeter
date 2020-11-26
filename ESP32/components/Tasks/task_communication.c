#include "../../main/main.h"
#include "../driver_mqtt/mqtt_driver.h"
#include "../driver_ade/EM.h"
#include "../driver_ade/ADE7880.h"
#include "../../build/config/sdkconfig.h"

/** FreeRTOS */
QueueHandle_t FIFO_Acq_to_Comm;

// MQTT
static char phaseNames[4] = {'A', 'B', 'C', 'N'};
static const char * rmsNames[2] = {"Current", "Voltage"};
static const char * powerNames[3] = {"Apparent", "Active", "PF"};

static char sMqttTopic[80];
static char sMqttMessage[80];
static char meterName[40]; /**< Topic prefix/meter identifier */

// Values
static EM_Meas testVal;

void task_communication(void *arg)
{
    sprintf(sMqttTopic, "/ziggy/default");
    sprintf(sMqttMessage, "task teste");
    mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);

    sprintf(meterName, "/ziggy/Building%s/meter%d", CONFIG_METER_BUILDING_NAME, CONFIG_METER_NUMBER); /**< Topic prefix/meter identifier */

    uint32_t ulNotifiedValue;

    for(;;)
    {
        xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);
        if(ulNotifiedValue == ADE_MEASURE_OK)
        {
            /** Receiving the values */
            xQueueReceive(FIFO_Acq_to_Comm, &testVal, (100/portTICK_PERIOD_MS) );

            #if DEBUG
                printf("RMS Voltage A: %f\n", testVal.RMS.sVoltage.A); 
                printf("RMS Voltage B: %f\n", testVal.RMS.sVoltage.B); 
                printf("RMS Voltage C: %f\n", testVal.RMS.sVoltage.C); 
                printf("RMS Current A: %f\n", testVal.RMS.sCurrent.A); 
                printf("RMS Current B: %f\n", testVal.RMS.sCurrent.B); 
                printf("RMS Current C: %f\n", testVal.RMS.sCurrent.C); 
                printf("RMS Current N: %f\n", testVal.RMS.sCurrent.N);
            #endif

            /** Sending the RMS values */
            sprintf(sMqttTopic, "%s/RMS", meterName);
            for(int i=0;i<7;i++){
                sprintf(sMqttMessage, "%s %c = %f", i < 4 ? rmsNames[0] : rmsNames[1], 
                                                    phaseNames[i%4],
                                                    i < 4 ? testVal.RMS.aCurrent[i] : testVal.RMS.aVoltage[i%4]);
                mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);
            }

            /** Sending Power Values */
            sprintf(sMqttTopic, "%s/Power", meterName);
            for(int i=0;i<9;i++){
            sprintf(sMqttMessage, "%s %c = %f", i < 3 ? powerNames[0] : (i < 6 ? powerNames[1] : powerNames[2]), 
                                                phaseNames[i%3],
                                                i < 3 ? testVal.Power.aTotal[0][i] : 
                                                i < 6 ? testVal.Power.aTotal[1][i%3] : testVal.Power.aPowerFactor[i%3]);
            mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);
            }

        }
    }
}
