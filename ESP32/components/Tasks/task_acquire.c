#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "../../build/config/sdkconfig.h"
#include "../../main/main.h"
#include "../driver_ade/ADE7880.h"

void task_acquire(void * arg)
{
    int j;
    int reg_val = 0;
    ESP_ERROR_CHECK(i2c_master_init());
    EM_RMS rms;
    //ade_init();

    for(;;)
    {   
        ade_read_rms(&rms);
        reg_val = ade_read_reg_32(ADE_CIRMS);
        printf("Value read: %d\n", reg_val); 
        printf("RMS Voltage: %f\n", rms.sVoltage.A); 
        vTaskDelay(3000 / portTICK_RATE_MS);
    }

}