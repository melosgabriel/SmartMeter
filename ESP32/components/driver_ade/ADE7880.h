#ifndef ADE7880_H
#define ADE7880_H

#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "../../main/main.h"

#include "EM.h"
#include "ADE_REG.h"

/** ADE Defines */
// PGA Gains, must be 0, 1, 2, 4, 8 or 16
#define ADE_PGA_IGAIN       0   /**< PGA1 */
#define ADE_PGA_NGAIN       0   /**< PGA2 */
#define ADE_PGA_VGAIN       0   /**< PGA3 */
#define ADE_USE_ROGOWSKI    0
#define ADE_USE_60HZ        1

#define ADE_PMAX_VAL        0x19CE5DE /**< From datasheet: instataneous power when inputs are at full scale */
#define ADE_FS_VAL          1024000 /** Frequency at which the energy is accumulated */
#define ADE_WTHR_VAL        0x03 /**< Default value. For this one and the following, see app note AN-1171, page 5 (Rev. A). */
#define ADE_VARTHR_VAL      0x03 /**< Default value */
#define ADE_VATHR_VAL       0x03 /**< Default value */    
#define ADE_VLEVEL_VAL      0x38000 /**< Default value */
#define ADE_VNOM_VAL        0x23C354  /** From eq. 42, VNOM = V/Vfs * 3766572 = 220*sqrt(2)/500 * 3766572 */
#define ADE_CFXDEN_VAL      0x0DB3 /**< Based on AN-1171. Calibrate later. */

/** I²C Defines */
#define SLAVE_7B_ADDRESS    0b0111000 /**< ADE7880 7-bit address, as described on the datasheet */
#define I2C_PORT            I2C_NUM_0
#define I2C_SDA_PIN         GPIO_NUM_21
#define I2C_SCL_PIN         GPIO_NUM_22
#define WRITE_BIT           I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT            I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN        0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS       0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL             0x0         /*!< I2C ack value */
#define NACK_VAL            0x1         /*!< I2C nack value */
#define DEBUG               1

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
 * @param   data    Data to write to register. Must be entered MSB (most significant byte) first.
 * @param   size    Size of data in bytes which should correspond to register address
 * @return  esp_err_t
 */
esp_err_t ade_write_reg(ade_reg_t addr, uint8_t* data, size_t size);

/**
 * @brief   Write to a 8-bit register
 *          @note all registers have 16-bit addresses which are defined in an enum in ADE_REG.h
 * @param   addr    Register address
 * @param   data    Data to write to register.
 * @return  esp_err_t
 */
esp_err_t ade_write_reg_8(ade_reg_t addr, uint8_t data);

/**
 * @brief   Write to a 16-bit register
 *          @note all registers have 16-bit addresses which are defined in an enum in ADE_REG.h
 * @param   addr    Register address
 * @param   data    Data to write to register. Must be entered MSB (most significant byte) first.
 * @return  esp_err_t
 */
esp_err_t ade_write_reg_16(ade_reg_t addr, uint16_t data);

/**
 * @brief   Write to a 32-bit register
 *          @note all registers have 16-bit addresses which are defined in an enum in ADE_REG.h
 * @param   addr    Register address
 * @param   data    Data to write to register. Must be entered MSB (most significant byte) first.
 * @param   size    Size of data in bytes which should correspond to register address
 * @return  esp_err_t
 */
esp_err_t ade_write_reg_32(ade_reg_t addr, uint32_t data);

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
 * @brief   Read from 8-bit register
 * @param   addr    Register address
 * @return  int     Value read from register
 */
int ade_read_reg_8(ade_reg_t addr);

/**
 * @brief   Read from 8-bit register
 * @param   addr    Register address
 * @return  int     Value read from register
 */
int ade_read_reg_16(ade_reg_t addr);

/**
 * @brief   Read from 8-bit register
 * @param   addr    Register address
 * @return  int     Value read from register
 */
int ade_read_reg_32(ade_reg_t addr);

/**
 * @brief   Initialize ADE7880. Based on the quick setup as an energy meter section of the datasheet.
 * @return  None
 */
esp_err_t ade_init(void);

/**
 * convert to float -> static?
 * Read RMS
 * Read power
 * Test with DEBUG=1 + arduino
 */

#endif /** __ADE7880_H__ */