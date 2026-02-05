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


ads1115_t ads1115_init(i2c_master_dev_handle_t handle, uint16_t addr)
{
	ads1115_t ads;
    ads.i2c_handle = handle;
    ads.address = addr;
    ads.gain = ADS_FSR_2_048V;
    ads.sps = ADS_SPS_128;
    ads.config = ADS_REG_CONFIG_RESET;
    return ads;
}


void ads1115_set_gain(ads1115_t *ads, ads1115_fsr_t gain)
{
    ads->gain = gain;

    ads->config &= ~ADS_REG_CONFIG_PGA_MASK;
    ads->config |= (uint16_t)gain;
}


void ads1115_set_sps(ads1115_t *ads ads1115_sps_t sps)
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


    if (write_register(ads, ADS1115_CONFIG_REGISTER_ADDR, ads->config) != ESP_OK) {
        return 0;
    }

    uint16_t status = 0;
    do {
        read_register(ads, ADS1115_CONFIG_REGISTER_ADDR, &status);
    } while ((status & ADS_REG_CONFIG_OS_MASK) == 0);

    uint16_t raw_value = 0;
    read_register(ads, ADS1115_CONVERSION_REGISTER_ADDR, &raw_value);

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

	return measure_differential(ads, channel);
}


int16_t ads1115_differential_0_1(ads1115_t *ads)
{
    return (int16_t)measure_differential(ads, ADS_REG_CONFIG_MUX_DIFF_0_1);
}


int16_t ads1115_differential_0_3(ads1115_t *ads)
{
    return (int16_t)measure_differential(ads, ADS_REG_CONFIG_MUX_DIFF_0_3);
}


int16_t ads1115_differential_1_3(ads1115_t *ads)
{
    return (int16_t)measure_differential(ads, ADS_REG_CONFIG_MUX_DIFF_1_3);
}


int16_t ads1115_differential_2_3(ads1115_t *ads)
{
    return (int16_t)measure_differential(ads, ADS_REG_CONFIG_MUX_DIFF_2_3);
}