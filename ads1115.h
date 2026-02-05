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

// --------------------------------------------
// REGISTER CONFIG
// --------------------------------------------

/**
 * @brief ADS1115 Configuration Register (16-bit)
 * The Config Register is used to control the ADS1115 operating mode, 
 * input selection, data rate, full-scale range, and comparator modes.
 * Bit [15]      : OS (Operational Status / Single-shot conversion start)
 * Bits [14:12]  : MUX (Input multiplexer configuration)
 * Bits [11:9]   : PGA (Programmable gain amplifier configuration)
 * Bit [8]       : MODE (Device operating mode: Continuous or Single-shot)
 * Bits [7:5]    : DR (Data rate / Samples per second)
 * Bit [4]       : COMP_MODE (Comparator mode)
 * Bit [3]       : COMP_POL (Comparator polarity)
 * Bit [2]       : COMP_LAT (Latching comparator)
 * Bits [1:0]    : COMP_QUE (Comparator queue and disable)
 */
#define ADS_REG_CONFIG_OS_MASK			(0x8000)
#define ADS_REG_CONFIG_MUX_MASK     	(0x7000)
#define ADS_REG_CONFIG_PGA_MASK     	(0x0E00)
#define ADS_REG_CONFIG_MODE_MASK    	(0x0100)
#define ADS_REG_CONFIG_DR_MASK      	(0x00E0)
#define ADS_REG_CONFIG_COMP_MODE_MASK	(0x0010)
#define ADS_REG_CONFIG_COMP_POL_MASK	(0x0010)
#define ADS_REG_CONFIG_COMP_POL_MASK	(0x0010)
#define ADS_REG_CONFIG_COMP_QUE_MASK	(0x0010)

#endif