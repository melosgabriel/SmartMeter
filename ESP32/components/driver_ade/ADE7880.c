#include "ADE7880.h"
#include "../../build/config/sdkconfig.h"

esp_err_t i2c_master_init()
{
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

esp_err_t i2c_master_write_slave(uint8_t *data, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SLAVE_7B_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 100 / portTICK_RATE_MS); // I changed to 100 ms
    i2c_cmd_link_delete(cmd);

    return ret;
}

esp_err_t i2c_master_read_slave(uint8_t *data, size_t size)
{
    if(size < 1) 
    {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    i2c_master_write_byte(cmd, SLAVE_7B_ADDRESS << 1 | READ_BIT, ACK_CHECK_EN);
    if(size > 1)
    {
        i2c_master_read(cmd, data, size-1, ACK_VAL);
    }
    i2c_master_read(cmd, data + size - 1, 1, NACK_VAL);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 100 / portTICK_RATE_MS); // I changed to 100 ms
    i2c_cmd_link_delete(cmd);
    
    return ret;
}

esp_err_t ade_write_reg(ade_reg_t addr, uint8_t* data, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SLAVE_7B_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN); /**< write address */

    /** Inverse the order of the bytes and send register address */
    union {
        uint16_t addr16;
        uint8_t addr8[2];
    } addr_u;
    addr_u.addr16 = addr;
    uint8_t temp = addr_u.addr8[0];
    addr_u.addr8[0] = addr_u.addr8[1];
    addr_u.addr8[1] = temp;
    i2c_master_write(cmd, addr_u.addr8, 2, ACK_CHECK_EN); /**< Write register address */
    
    i2c_master_write(cmd, data, size, ACK_CHECK_EN); /** Write data to register */

    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 100 / portTICK_RATE_MS); // I changed to 100 ms
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t ade_read_reg(ade_reg_t addr, uint8_t * data, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SLAVE_7B_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN); /**< write address */

    /** Inverse the order of the bytes and send register address */
    union {
        uint16_t addr16;
        uint8_t addr8[2];
    } addr_u;
    addr_u.addr16 = addr;
    uint8_t temp = addr_u.addr8[0];
    addr_u.addr8[0] = addr_u.addr8[1];
    addr_u.addr8[1] = temp;
    i2c_master_write(cmd, addr_u.addr8, 2, ACK_CHECK_EN);

    /** SECOND PART */
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, SLAVE_7B_ADDRESS << 1 | READ_BIT, ACK_CHECK_EN);
    if(size > 1)
    {
        i2c_master_read(cmd, data, size-1, ACK_VAL);
    }
    i2c_master_read(cmd, data + size - 1, 1, NACK_VAL);

    i2c_master_stop(cmd);
    
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 100 / portTICK_RATE_MS); // I changed to 100 ms
    i2c_cmd_link_delete(cmd);
    return ret;
}