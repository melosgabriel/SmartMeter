#ifndef MQTT_DRIVER_H
#define MQTT_DRIVER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "esp_log.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "mqtt_client.h"

#define MQTT_CONNECTED_BIT  BIT0
#define MQTT_FAIL_BIT       BIT1

/** 
 * @brief   Start MQTT and connect to broker. This will send a message indicating the
 *          building which the meter is located and the meter number.
*/
void mqtt_app_start(void);

/** 
 * @brief   Publish to broker function
 * @param   sMqttTopic      Topic for mqtt
 * @param   sMqttMessage    Payload (or message) to be sent
 * @param   len             Length of message. 0 to autodetermine.
 * @param   qos             Quality of service (0, 1 or 2)
 * @param   retain          Retain boolean
 * @return  none
*/
void mqtt_publish(char * sMqttTopic, char * sMqttMessage, int len, int qos, int retain);

/** 
 * TODO: Add subscribe function. Then, the data received should be sent to a task via the callback handler.
*/

#endif /* MQTT_DRIVER_H */