#ifndef ADE7880_H
#define ADE7880_H

#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "../../main/main.h"

#include "EM.h"
#include "ADE_REG.h"

#define SLAVE_7B_ADDRESS    0b0111000
#define I2C_PORT            I2C_NUM_0
#define WRITE_BIT           I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT            I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN        0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS       0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL             0x0         /*!< I2C ack value */
#define NACK_VAL            0x1         /*!< I2C nack value */


/**
 * @brief   Init I²C device as master (21 - SDA; 22 - SCL; 100 kHz; no pull.)
 * @note    I²C port and slave address are #defined
 * @return  esp_err_t
 */
esp_err_t i2c_master_init();

/**
 * @brief   Write to the slave
 * @param   data    Data to be written
 * @param   size    Size (in bytes) of the data.
 * @return  esp_err_t
 */
esp_err_t i2c_master_write_slave(uint8_t *data, size_t size);

/**
 * @brief   Read data from the slave
 * @param   data    Pointer to data to be read
 * @param   size    Size (in bytes) of the data.
 * @return  esp_err_t
 */
esp_err_t i2c_master_read_slave(uint8_t *data, size_t size);

/**
 * @brief   Write to register
 *          @note all registers have 16-bit addresses which are defined in an enum in ADE_REG.h
 * @param   addr    Register address
 * @param   data    Data to write to register
 * @param   size    Size of data in bytes which should correspond to register address
 * @return  esp_err_t
 */
esp_err_t ade_write_reg(ade_reg_t addr, uint8_t* data, size_t size);

/**
 * @brief   Read from register
 *          @note all registers have 16-bit addresses which are defined in an enum in ADE_REG.h
 *          @note To read harmonics calculations set the size to 16
 * @param   addr    Register address
 * @param   data    Data to read from register
 * @param   size    Size of data in bytes which should correspond to register address
 * @return  esp_err_t
 */
esp_err_t ade_read_reg(ade_reg_t addr, uint8_t * data, size_t size);

/**
 * convert to float -> static?
 * initialize
 * 
 */

#endif /** __ADE7880_H__ */