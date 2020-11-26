#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "../../build/config/sdkconfig.h"
#include "../../main/main.h"
#include "../driver_ade/ADE7880.h"

QueueHandle_t FIFO_Acq_to_Comm;
TaskHandle_t xTaskCommHandle;

void task_acquire(void * arg)
{
    #if DEBUG
    uint8_t val[4]= {0,0,0,0};
    #endif
    
    ESP_ERROR_CHECK(i2c_master_init());
    //EM_RMS rms;
    EM_Meas meas;
    ade_init();

    for(;;)
    {   
        ade_acquire(&meas);
        #if DEBUG
        ade_read_reg(ADE_AWATT, val, 4);
        printf("Value read: %d %d %d %d\n", val[0], val[1], val[2], val[3]); 
        #endif 

        // Send values to the communication task
        xQueueSend( FIFO_Acq_to_Comm, &meas, 10 / portTICK_RATE_MS ); 
        xTaskNotify(xTaskCommHandle, ADE_MEASURE_OK, eSetBits); // Notify the other task

        #if DEBUG
        vTaskDelay(3000 / portTICK_RATE_MS); // bigger delay for arduino
        #else
        vTaskDelay(500 / portTICK_RATE_MS);
        #endif
    }

}