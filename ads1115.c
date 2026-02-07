#include "ads1115.h"

static esp_err_t write_register(ads1115_t *ads, uint8_t reg, uint16_t data)
{
    if (ads == NULL || ads->i2c_handle == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t buffer[3];
    buffer[0] = reg;
    buffer[1] = (uint8_t)(data >> 8);   // MSB
    buffer[2] = (uint8_t)(data & 0xFF); // LSB

    return i2c_master_transmit(ads->i2c_handle, buffer, sizeof(buffer), -1);
}


static esp_err_t read_register(ads1115_t *ads, uint8_t reg, uint16_t *out)
{
    if (ads == NULL || ads->i2c_handle == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t buffer[2];

    esp_err_t ret =  i2c_master_transmit_receive(ads->i2c_handle, &reg, 1, buffer, 2, -1);

    if (ret == ESP_OK) {
        *out = (uint16_t)((buffer[0] << 8) | buffer[1]);
    }

    return ret;
}


/**
 * @brief Initializes the hardware and syncs the initial configuration.
 *
 * @param ads  Pointer to the device structure.
 * @return     ESP_OK on success, ESP_FAIL if device is not responding.
 */
static esp_err_t ads1115_begin(ads1115_t *ads)
{
    ads->config = ADS_REG_CONFIG_MODE_SINGLE | ADS_REG_CONFIG_PGA_2_048V | 
                  ADS_REG_CONFIG_DR_128SPS | ADS_REG_CONFIG_COMP_QUE_DIS;
    
    return write_register(ads, ADS_REG_CONFIG_ADDRESS, ads->config);
}


esp_err_t ads1115_init(ads1115_t *ads, i2c_master_dev_handle_t handle)
{
    ads->i2c_handle = handle;
    ads->config = ADS_REG_CONFIG_RESET;
    return ads1115_begin(ads);
}


void ads1115_set_gain(ads1115_t *ads, ads1115_fsr_t gain)
{
    ads->gain = gain;

    ads->config &= ~ADS_REG_CONFIG_PGA_MASK;
    ads->config |= (uint16_t)gain;
}


void ads1115_set_sps(ads1115_t *ads, ads1115_sps_t sps)
{
    ads->sps = sps;
    
    ads->config &= ~ADS_REG_CONFIG_DR_MASK;
    ads->config |= (uint16_t)sps;
}


/**
 * @brief Internal helper to perform a measurement with a specific MUX configuration.
 */
static uint16_t measure_differential(ads1115_t *ads, uint16_t mux_setting)
{
    if (ads == NULL) return 0;

    ads->config &= ~ADS_REG_CONFIG_MUX_MASK;
    ads->config |= mux_setting;
    ads->config |= ADS_REG_CONFIG_OS_START;


    if (write_register(ads, ADS_REG_CONFIG_ADDRESS, ads->config) != ESP_OK) {
        return 0;
    }

    uint16_t status = 0;
    do {
        read_register(ads, ADS_REG_CONFIG_ADDRESS, &status);
    } while ((status & ADS_REG_CONFIG_OS_MASK) == 0);

    uint16_t raw_value = 0;
    read_register(ads, ADS_REG_CONVERSION_ADDRESS, &raw_value);

    return raw_value;
}


uint16_t ads1115_get_raw(ads1115_t *ads, uint8_t channel)
{
    uint16_t mux_setting;
    switch(channel) {
        case 0: mux_setting = ADS_REG_CONFIG_MUX_0_GND; break;
        case 1: mux_setting = ADS_REG_CONFIG_MUX_1_GND; break;
        case 2: mux_setting = ADS_REG_CONFIG_MUX_2_GND; break;
        case 3: mux_setting = ADS_REG_CONFIG_MUX_3_GND; break;
        default: return 0;
    }

    return measure_differential(ads, mux_setting);
}


int16_t ads1115_differential_0_1(ads1115_t *ads)
{
    return (int16_t)measure_differential(ads, ADS_REG_CONFIG_MUX_0_1);
}


int16_t ads1115_differential_0_3(ads1115_t *ads)
{
    return (int16_t)measure_differential(ads, ADS_REG_CONFIG_MUX_0_3);
}


int16_t ads1115_differential_1_3(ads1115_t *ads)
{
    return (int16_t)measure_differential(ads, ADS_REG_CONFIG_MUX_1_3);
}


int16_t ads1115_differential_2_3(ads1115_t *ads)
{
    return (int16_t)measure_differential(ads, ADS_REG_CONFIG_MUX_2_3);
}


float ads1115_raw_to_voltage(ads1115_t *ads, int16_t raw)
{
    float fsr = 0.0f;
    switch (ads->gain) {
        case ADS_FSR_6_144V: fsr = 6.144f; break;
        case ADS_FSR_4_096V: fsr = 4.096f; break;
        case ADS_FSR_2_048V: fsr = 2.048f; break;
        case ADS_FSR_1_024V: fsr = 1.024f; break;
        case ADS_FSR_0_512V: fsr = 0.512f; break;
        case ADS_FSR_0_256V: fsr = 0.256f; break;
        default: fsr = 2.048f;
    }
    return (float)raw * (fsr / 32768.0f);
}


void ads1115_enable_rdy_pin(ads1115_t *ads)
{
    write_register(ads, ADS_REG_HITHRESH_ADRESS, 0x8000);
    write_register(ads, ADS_REG_LOTHRESH_ADRESS, 0x0000);
    
    ads->config &= ~ADS_REG_CONFIG_COMP_QUE_MASK;
    ads->config |= ADS_REG_CONFIG_COMP_QUE_1CONV;
    write_register(ads, ADS_REG_CONFIG_ADDRESS, ads->config);
}
