#ifndef __ADE7880_H__
#define __ADE7880_H__

#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "../../main/main.h"

#include "EM.h"
#include "ADE_REG.h"

/** ADE Defines */

/** GPIO Defines */ 
#define ADE_PM0_GPIO        25
#define ADE_PM1_GPIO        26
#define ADE_RESET_GPIO      27
#define ADE_GPIO_PIN_SEL    ((1ULL<<ADE_PM0_GPIO) | (1ULL<<ADE_PM1_GPIO) | (1ULL<<ADE_RESET_GPIO))

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

/** PGA Gains, must be 0, 1, 2, 4, 8 or 16 */ 
#define ADE_PGA_IGAIN       0   /**< PGA1 */
#define ADE_PGA_NGAIN       0   /**< PGA2 */
#define ADE_PGA_VGAIN       0   /**< PGA3 */
#define ADE_USE_ROGOWSKI    0
#define ADE_USE_60HZ        1

/** Configuration register values*/
#define ADE_PMAX_VAL        0x19CE5DE /**< From datasheet: instataneous power when inputs are at full scale */
#define ADE_FS_VAL          1024000 /** Frequency at which the energy is accumulated */
#define ADE_WTHR_VAL        0x03 /**< Default value. For this one and the following, see app note AN-1171, page 5 (Rev. A). */
#define ADE_VARTHR_VAL      0x03 /**< Default value */
#define ADE_VATHR_VAL       0x03 /**< Default value */    
#define ADE_VLEVEL_VAL      0x38000 /**< Default value */
#define ADE_VNOM_VAL        0x23C354  /** From eq. 42, VNOM = V/Vfs * 3766572 = 220*sqrt(2)/500 * 3766572 */
#define ADE_CFXDEN_VAL      0x0DB3 /**< Based on AN-1171. Calibrate later. */

/** ADE Readings */
#define ADE_FULLSCALE_REG   5326737 /**< Max numeric value (positive/negative) corresponding to a max voltage at input */
#define ADE_FULLSCALE_VAL   0.5f /**< Max voltage at the ADC input */
#define ADE_VOLTAGE_ATT     (1.0f / 1001.0f) /**< Voltage attenuation */
#define ADE_CURRENT_FULL    49.4975f /**< Peak full scale current. Irms = 35, Ip = 35 * sqrt(2) */
#define ADE_PMAX            27059678 /**< Instantaneous power when inputs at full scale and in phase */

typedef enum ade_powermode_t{
    ADE_PM0 = 0,
    ADE_PM1,
    ADE_PM2,
    ADE_PM3
} ade_powermode_t;

/**
 * @brief   Function to set the ADE powermode, based on datasheet
 * @param   powermode   ADE_PM0, PM1, PM2 or PM3. Check datasheet
 * @return  None
 */
esp_err_t ade_set_powermode(ade_powermode_t powermode);

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
 * @brief   Function to read RMS values
 * @param   rms     EM_RMS. Check EM.h. Structure to hold RMS values
 * @return  None
*/
void ade_read_rms(EM_RMS * rms);

/** 
 * @brief   Function to read power values
 * @param   rms     EM_Power. Check EM.h. Structure to hold power values
 * @return  None
*/
void ade_read_power(EM_Power * power);

/** 
 * @brief   Function to acquire all measurements
 * @param   meas     EM_Meas. Check EM.h. Structure to hold power values
 * @return  None
*/
void ade_acquire(EM_Meas * meas);


/**
 * TODO:
 * Read power
 * Code harmonics
 * Code power quality
 * Code interruptions
 */

#endif /** __ADE7880_H__ */