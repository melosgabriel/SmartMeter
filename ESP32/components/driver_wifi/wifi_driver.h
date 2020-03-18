#ifndef WIFI_DRIVER_H
#define WIFI_DRIVER_H

// Wi-Fi event group to wait for event
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_event.h"
#include "esp_log.h"

// Wi-Fi Credentials defined in menuconfig
#define ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void wifi_init_sta(void);

extern const char *WIFI_TAG;

#endif /* WIFI_DRIVE_H */