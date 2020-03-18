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

void mqtt_app_start(void);
void mqtt_publish(char * sMqttTopic, char * sMqttMessage, int len, int qos, int retain);
//TODO: Add a subscribe function if needed. But first determine how we get the data

#endif /* MQTT_DRIVER_H */