#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "../../build/config/sdkconfig.h"
#include "../../main/main.h"
#include "../driver_ade/ADE7880.h"

QueueHandle_t FIFO_Acq_to_Comm;
TaskHandle_t xTaskCommHandle;

void task_acquire(void * arg)
{
    int reg_val = 0;
    ESP_ERROR_CHECK(i2c_master_init());
    EM_RMS rms;
    //ade_init();

    for(;;)
    {   
        ade_read_rms(&rms);
        reg_val = ade_read_reg_32(ADE_AFIRMSOS);
        printf("Value read: %d\n", reg_val); 

        // Send values to the communication task
        xQueueSend( FIFO_Acq_to_Comm, &rms, 10 / portTICK_RATE_MS ); 
        xTaskNotify(xTaskCommHandle, ADE_MEASURE_OK, eSetBits); // Notify the other task
        vTaskDelay(3000 / portTICK_RATE_MS);
    }

}