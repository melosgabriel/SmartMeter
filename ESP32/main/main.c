/* 
  Embedded software for ziggy
  created by: Gabriel de Melo Souza
*/

#include <string.h>
#include "main.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "../components/driver_mqtt/mqtt_driver.h"
#include "../components/driver_wifi/wifi_driver.h"
#include "../components/driver_ade/electricalMeas.h"

static void test_function(void * args)
{
    while(1)
    {
        printf("so testando uma coisa...\n");
        vTaskDelay(8000 / portTICK_RATE_MS);
    }
}

void app_main()
{
    //* Initialize Components

    esp_err_t ret = nvs_flash_init();    //Initialize NVS
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta(); // Initialize Wi-Fi
    mqtt_app_start(); // Initialize MQTT

    //* Initialize tasks

    // xTaskCreate(task_communication, "Communication task for MQTT", 2048, NULL, 10, NULL); // MQTT
    xTaskCreate(task_acquire, "Acquisition task for ADE7880", 4096, NULL, 10, NULL); // ADE7880
    xTaskCreate(test_function, "teste", 2048, NULL, 10, NULL);
    
    
    // while(1){
    //     printf("\n\nI shall not be seen...\n\n");
    //     vTaskDelay(5000 / portTICK_RATE_MS);
    // }
}
