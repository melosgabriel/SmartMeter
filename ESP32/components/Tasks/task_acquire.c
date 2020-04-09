#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "../../build/config/sdkconfig.h"
#include "../../main/main.h"
#include "../driver_ade/ADE7880.h"

void task_acquire(void * arg)
{
    uint8_t i= 1;
    int j;
    ESP_ERROR_CHECK(i2c_master_init());
    uint8_t msg[4] = {0, 1, 255, 7};

    for(;;)
    {   
        j = ade_write_reg(ADE_OILVL, (uint8_t *)msg, 4);
        if(j)
        {
            printf("I2C failed: %#08x\n", j);
        } 
        else i++;
        vTaskDelay(5000 / portTICK_RATE_MS);
    }

}