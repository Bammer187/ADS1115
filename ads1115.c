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


ads1115_t ads1115_init(i2c_master_dev_handle_t handle, uint16_t addr)
{
	ads1115_t ads;
    ads.i2c_handle = handle;
    ads.address = addr;
    ads.gain = ADS_PGA_2_048V;
    ads.config = ADS_REG_CONF_RESET;
    return ads;
}


void ads1115_set_gain(ads1115_t *ads, ads1115_fsr_t gain)
{
	ads->gain = gain;
}