#include "ADE7880.h"
#include "../../build/config/sdkconfig.h"
#include <string.h> /**< For memset */

/** POWER MODE */

esp_err_t ade_set_powermode(ade_powermode_t powermode)
{
    esp_err_t ret;
    switch (powermode)
    {
    case ADE_PM0:
        ret = gpio_set_level(ADE_PM0_GPIO, 1);
        ret = ret | gpio_set_level(ADE_PM1_GPIO, 0);
        break;
    case ADE_PM1:
        ret = gpio_set_level(ADE_PM0_GPIO, 0);
        ret = ret | gpio_set_level(ADE_PM1_GPIO, 0);
        break;
    case ADE_PM2:
        ret = gpio_set_level(ADE_PM0_GPIO, 0);
        ret = ret | gpio_set_level(ADE_PM1_GPIO, 1);
        break;
    case ADE_PM3:
        ret = gpio_set_level(ADE_PM0_GPIO, 1);
        ret = ret | gpio_set_level(ADE_PM1_GPIO, 1);
        break;

    default: /** Set PM0 */
        ret = ESP_FAIL;
        break;
    }
    return ret;
}

/** BASIC COMMUNICATION FUNCTIONS FOR I²C */

esp_err_t i2c_master_init()
{
    i2c_config_t hi2c = {.mode = I2C_MODE_MASTER,
                         .sda_io_num = I2C_SDA_PIN,
                         .sda_pullup_en = GPIO_PULLUP_DISABLE,
                         .scl_io_num = I2C_SCL_PIN,
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

/** 
 * @brief   Function to convert an integer of any size to an uint8_t array, starting from MSB
 * @param   num     Number to convert
 * @param   size    Size in bytes
 * @param   ret     Pointer to the array
 * @return  None
*/
static void uintxto8(int num, int size, uint8_t * ret){
    for(int i=0;i<size;i++){
        ret[i] = num >> (size - i - 1)*8;
    }
}

esp_err_t ade_write_reg(ade_reg_t addr, uint8_t* data, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SLAVE_7B_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN); /**< write address */

    /** Inverse the order of the bytes and send register address */
    uint8_t addrReg8[2];
    uintxto8(addr, 2, addrReg8);
    i2c_master_write(cmd, addrReg8, 2, ACK_CHECK_EN); /**< Write register address */
    
    i2c_master_write(cmd, data, size, ACK_CHECK_EN); /** Write data to register */

    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 100 / portTICK_RATE_MS); // I changed to 100 ms
    i2c_cmd_link_delete(cmd);
    vTaskDelay(1/portTICK_RATE_MS);
    return ret;
}

esp_err_t ade_write_reg_8(ade_reg_t addr, uint8_t data)
{
    int num_bytes = 1;
    uint8_t data8[num_bytes];
    uintxto8(data, num_bytes, data8);
    esp_err_t ret = ade_write_reg(addr, data8, num_bytes);
    return ret;
}

esp_err_t ade_write_reg_16(ade_reg_t addr, uint16_t data)
{
    int num_bytes = 2;
    uint8_t data8[num_bytes];
    uintxto8(data, num_bytes, data8);
    esp_err_t ret = ade_write_reg(addr, data8, num_bytes);
    return ret;
}

esp_err_t ade_write_reg_32(ade_reg_t addr, uint32_t data)
{
    int num_bytes = 4;
    uint8_t data8[num_bytes];
    uintxto8(data, num_bytes, data8);
    esp_err_t ret = ade_write_reg(addr, data8, num_bytes);
    return ret;
}

static int uint8toint(uint8_t * num, size_t size)
{
    int ret = 0;
    for(int i = 0; i < size; i++)
    {
        ret = ret | num[i] << (size - i - 1)*8;       
    }
    return ret;
}

esp_err_t ade_read_reg(ade_reg_t addr, uint8_t * data, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SLAVE_7B_ADDRESS << 1) | WRITE_BIT, ACK_CHECK_EN); /**< write address */

    /** Inverse the order of the bytes and send register address */
    uint8_t addrReg8[2];
    uintxto8(addr, 2, addrReg8);
    i2c_master_write(cmd, addrReg8, 2, ACK_CHECK_EN);

    #if DEBUG /** Give arduino some time and a stop condition */
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_PORT, cmd, 100 / portTICK_RATE_MS);
        vTaskDelay(100 /portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);
        cmd = i2c_cmd_link_create();
    #endif

    /** SECOND PART */
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SLAVE_7B_ADDRESS << 1) | READ_BIT, ACK_CHECK_EN);
    if(size > 1)
    {
        i2c_master_read(cmd, data, size-1, ACK_VAL);
    }
    i2c_master_read(cmd, data + size - 1, 1, NACK_VAL);

    i2c_master_stop(cmd);
    
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 100 / portTICK_RATE_MS); // I changed to 100 ms
    i2c_cmd_link_delete(cmd);
    vTaskDelay(1/portTICK_RATE_MS);
    return ret;
}

int ade_read_reg_8(ade_reg_t addr)
{
    int num_bytes = 1;
    uint8_t data[num_bytes];
    memset(data, 0, num_bytes);
    ade_read_reg(addr, data, num_bytes);
    int ret = uint8toint(data, num_bytes);
    return ret;
}

int ade_read_reg_16(ade_reg_t addr)
{
    int num_bytes = 2;
    uint8_t data[num_bytes];
    memset(data, 0, num_bytes);
    ade_read_reg(addr, data, num_bytes);
    int ret = uint8toint(data, num_bytes);
    return ret;
}

int ade_read_reg_32(ade_reg_t addr)
{
    int num_bytes = 4;
    uint8_t data[num_bytes];
    memset(data, 0, num_bytes);
    ade_read_reg(addr, data, num_bytes);
    int ret = uint8toint(data, num_bytes);
    return ret;
}

/** ADE7880 CONFIGURATION FUNCTIONS */

/** 
 * @brief   Quickly calculate the log base 2 of a number for the PGA gains
 * */ 
static uint32_t quick_log2(uint32_t num){
    uint32_t log = 0;
    while(num >>= 1) ++log;
    return log;
}

esp_err_t ade_init(void)
{
    // Configure ADE GPIOs and set powermode
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = ADE_GPIO_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    ade_set_powermode(ADE_PM0);

    // Choose I2C as the main interface and lock
    ade_write_reg_8(ADE_CONFIG2, 0x02);

    // Set gains
    uint16_t gain;
    gain = (quick_log2(ADE_PGA_VGAIN) << 6) | (quick_log2(ADE_PGA_NGAIN) << 3) | (quick_log2(ADE_PGA_IGAIN) << 0);
    ade_write_reg_16(ADE_Gain, gain);

    // Set CONFIG register
    #if ADE_USE_ROGOWSKI
        ade_write_reg_32(ADE_DICOEFF, 0xFF8000);
        ade_write_reg(ADE_CONFIG, 0x0001);
    #endif

    // Set COMPMODE register
    #if ADE_USE_60HZ
        ade_write_reg_16(ADE_COMPMODE, 0x41FF); /**< Default value + bit 14 set to 1*/ 
    #endif

    // Set other data memory ram registers

    // Set WTHR, VARTHR, VATHR, VLEVEL and VNOM
    ade_write_reg_8(ADE_WTHR, ADE_WTHR_VAL);
    ade_write_reg_8(ADE_VARTHR, ADE_VARTHR_VAL);
    ade_write_reg_8(ADE_VATHR, ADE_VATHR_VAL);
    ade_write_reg_32(ADE_VLEVEL, ADE_VLEVEL_VAL);
    ade_write_reg_32(ADE_VNOM, ADE_VNOM_VAL);

    // Initialize CF1DEN, CF2DEN and CF3DEN
    ade_write_reg_16(ADE_CF1DEN, ADE_CFXDEN_VAL);
    ade_write_reg_16(ADE_CF2DEN, ADE_CFXDEN_VAL);
    ade_write_reg_16(ADE_CF3DEN, ADE_CFXDEN_VAL);
    
    // Enable RAM protection
    ade_write_reg_8(0xE7FE, 0xAD);
    ade_write_reg_8(0xE7E3, 0x80);

    // Start the DSP    
    ade_write_reg_16(ADE_RUN, 0x0001);

    // Read xWATTHR, xVAHR, xFWATTHR, xFVARHR to erase their content
    ade_read_reg_32(ADE_AWATTHR);
    ade_read_reg_32(ADE_BWATTHR);
    ade_read_reg_32(ADE_CWATTHR);
    ade_read_reg_32(ADE_AVAHR);
    ade_read_reg_32(ADE_BVAHR);
    ade_read_reg_32(ADE_CVAHR);
    ade_read_reg_32(ADE_AFWATTHR);
    ade_read_reg_32(ADE_BFWATTHR);
    ade_read_reg_32(ADE_CFWATTHR);
    ade_read_reg_32(ADE_AFVARHR);
    ade_read_reg_32(ADE_BFVARHR);
    ade_read_reg_32(ADE_CFVARHR);
    
    // Enable CF1 and CF2 converter outputs + default values
    ade_write_reg_16(ADE_CFMODE, 0x08A0);

    return 0;
}

/**
 * @brief   Function to extend the sign of a number. Useful when reading signed numbers padded with zeros
 * @param   val     Old number, padded with zeros
 * @param   numbits Number of bits on the old number. A 24-bit signed integer has numbits = 24.
 * @return  int     The sign-extended number.
 */
static int signextension(int val, int numbits)
{
    int newval;
    if( val & (1 << (numbits-1) ) ) newval = (0xFFFFFFFF << numbits) | val;
    else newval = val;
    return newval;
}

/**
 * @brief   Function to read RMS voltage
 * @param   addr    Address of register to read
 * @return  float   Voltage read from register
 */
static float read_rms_v(ade_reg_t addr)
{
    int reg_val = ade_read_reg_32(addr);
    reg_val = signextension(reg_val, 24);
    float ret = reg_val * (ADE_FULLSCALE_VAL / ADE_FULLSCALE_REG) / ADE_VOLTAGE_ATT;
    return ret;
}

/**
 * @brief   Function to read RMS current
 * @param   addr    Address of register to read
 * @return  float   Current read from register
 */
static float read_rms_i(ade_reg_t addr)
{
    int reg_val = ade_read_reg_32(addr);
    reg_val = signextension(reg_val, 24);
    float ret = reg_val * (ADE_FULLSCALE_VAL / ADE_FULLSCALE_REG) * ADE_CURRENT_FULL;
    return ret;
}

void ade_read_rms(EM_RMS * rms)
{
    rms->sVoltage.A = read_rms_v(ADE_AVRMS);
    rms->sVoltage.B = read_rms_v(ADE_BVRMS);
    rms->sVoltage.C = read_rms_v(ADE_CVRMS);

    rms->sCurrent.A = read_rms_i(ADE_AIRMS);
    rms->sCurrent.B = read_rms_i(ADE_BIRMS);
    rms->sCurrent.C = read_rms_i(ADE_CIRMS);
    rms->sCurrent.N = read_rms_i(ADE_NIRMS);
}

void ade_read_power(EM_Power * power)
{
    
}