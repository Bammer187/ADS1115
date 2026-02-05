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
    ads.config = ADS_REG_CONFIG_RESET;
    return ads;
}


void ads1115_set_gain(ads1115_t *ads, ads1115_fsr_t gain)
{
	ads->gain = gain;
}


uint16_t ads1115_get_raw(ads1115_t *ads, uint8_t channel)
{
	if (channel > 3) return 0;

	ads->config |= ADS_REG_CONFIG_OS_START;

	ads->config &= ~ADS_REG_CONFIG_MUX_MASK;

	switch(channel) {
		case 0: ads->config |= ADS_REG_CONFIG_MUX_0_GND; break;
        case 1: ads->config |= ADS_REG_CONFIG_MUX_1_GND; break;
        case 2: ads->config |= ADS_REG_CONFIG_MUX_2_GND; break;
        case 3: ads->config |= ADS_REG_CONFIG_MUX_3_GND; break;
	}

	ESP_ERROR_CHECK(write_register(ads, ADS_REG_CONFIG, ads->config));

	uint16_t status = 0;
    do {
        read_register(ads, ADS1115_CONFIG_REGISTER_ADDR, &status);
    } while ((status & ADS_REG_CONFIG_OS_MASK) == 0);

	uint16_t raw_value;

	ESP_ERROR_CHECK(read_register(ads, ADS_REG_CONVERSION, &raw_value));

	return raw_value;
}


int16_t ads1115_differential_0_1(ads1115_t *ads)
{
	ads->config |= ADS_REG_CONFIG_OS_START;

	ads->config &= ~ADS_REG_CONFIG_MUX_MASK;

	ads->config |= ADS_REG_CONFIG_MUX_0_1;

	ESP_ERROR_CHECK(write_register(ads, ADS_REG_CONFIG, ads->config));

	uint16_t status = 0;
    do {
        read_register(ads, ADS1115_CONFIG_REGISTER_ADDR, &status);
    } while ((status & ADS_REG_CONFIG_OS_MASK) == 0);

	uint16_t raw_value;

	ESP_ERROR_CHECK(read_register(ads, ADS_REG_CONVERSION, &raw_value));

	return (int16_t)raw_value;
}
