/* 
  Embedded software for ziggy
  created by: Gabriel de Melo Souza
*/
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "../components/driver_mqtt/mqtt_driver.h"
#include "../components/driver_wifi/wifi_driver.h"


// TODO: Properly define this in a component
typedef struct measurements
{
    struct rms
    {
        float voltage;
        float current;
    } rms;
    float harmonics;
} measurements;

static char sMqttTopic[80];
static char sMqttMessage[80];

static void task_comm(void *arg)
{
    sprintf(sMqttTopic, "/ziggy/teste");
    sprintf(sMqttMessage, "task teste");

    for(;;)
    {
        mqtt_publish(sMqttTopic,sMqttMessage,0,1,0);
        vTaskDelay(3000/portTICK_RATE_MS); // TODO: Fix the inverval - 3s is for debug!!
    }
}

static void test_function(void * args)
{
    while(1)
    {
        printf("so testando uma coisa...\n");
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

void app_main()
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize Wi-Fi
    wifi_init_sta();
    // Initialize MQTT
    mqtt_app_start();
    // mqtt_publish("/ziggy/teste", "FUNCIONA PUVAVINHO 1 todos", 0, 1, 0);

    //start communication task
    xTaskCreate(task_comm, "task_comm", 2048, NULL, 10, NULL);
    xTaskCreate(test_function, "teste", 2048, NULL, 10, NULL);
    // vTaskStartScheduler();
    while(1){
        printf("\n\nTesting2...\n\n");
        vTaskDelay(5000 / portTICK_RATE_MS);
    }
}
