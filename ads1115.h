#ifndef ADS1115_H
#define ADS1115_H

// --------------------------------------------
// I2C ADDRESS
// --------------------------------------------

#define ADS_I2C_ADDR_GND (0x48)
#define ADS_I2C_ADDR_VDD (0x49)
#define ADS_I2C_ADDR_SDA (0x4A)
#define ADS_I2C_ADDR_SCL (0x4B)

// --------------------------------------------
// REGISTER ADDRESS
// --------------------------------------------

#define ADS_REG_CONVERSION (0x00)
#define ADS_REG_CONFIG (0x01)
#define ADS_REG_LOTHRESH (0x02)
#define ADS_REG_HITHRESH (0x03)

#endif