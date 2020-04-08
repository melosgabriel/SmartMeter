#include "freertos/FreeRTOS.h"
#include "../../main/main.h"
#include "driver/i2c.h"

//TODO: Put this in ADE_driver
#define SLAVE_7B_ADDRESS    0b1110000
#define I2C_PORT            I2C_NUM_0
#define WRITE_BIT           I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT            I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN        0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS       0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL             0x0         /*!< I2C ack value */
#define NACK_VAL            0x1         /*!< I2C nack value */

esp_err_t i2c_master_write_slave(uint8_t *data, size_t size){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SLAVE_7B_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 100 / portTICK_RATE_MS); // I changed to 100 ms
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t i2c_master_read_slave(uint8_t *data, size_t size){
    if(size < 1) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SLAVE_7B_ADDRESS << 1 | READ_BIT, ACK_CHECK_EN);
    if(size > 1){
        i2c_master_read(cmd, data, size-1, ACK_VAL);
    }
    i2c_master_read(cmd, data + size - 1, 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 100 / portTICK_RATE_MS); // I changed to 100 ms
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t i2c_master_init(){
    i2c_config_t hi2c = {.mode = I2C_MODE_MASTER,
                         .sda_io_num = GPIO_NUM_21,
                         .sda_pullup_en = GPIO_PULLUP_DISABLE,
                         .scl_io_num = GPIO_NUM_22,
                         .scl_pullup_en = GPIO_PULLUP_DISABLE,
                         .master.clk_speed = 100000
                         };
    esp_err_t ret = i2c_param_config(I2C_PORT, &hi2c);
    ret = ret | i2c_driver_install(I2C_PORT, hi2c.mode, 0, 0, 0);
    return ret;
}

void task_acquire(void * arg)
{
    uint8_t i= 0;
    int j;
    ESP_ERROR_CHECK(i2c_master_init());
    
    for(;;)
    {   
        j = i2c_master_write_slave(&i, 1);
        if(j)
        {
            printf("I2C failed: %#08x\n", j);
        } 
        else i++;
        vTaskDelay(5000 / portTICK_RATE_MS);
    }

}