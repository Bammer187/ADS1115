#include "ads1115.h"

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