#ifndef ADS1115_H
#define ADS1115_H

#include "driver/i2c_master.h"

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

#define ADS_REG_MASK 		(0x03)
#define ADS_REG_CONVERSION	(0x00)
#define ADS_REG_CONFIG 		(0x01)
#define ADS_REG_LOTHRESH 	(0x02)
#define ADS_REG_HITHRESH 	(0x03)

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

// --------------------------------------------
// OS - OPERATIONAL STATUS
// --------------------------------------------

#define ADS_REG_CONFIG_OS_MASK			(0x8000)
#define ADS_REG_CONFIG_OS_START			(0x8000) // Start a single conversion
#define ADS_REG_CONFIG_OS_ACTIVE		(0x0000) // Currently performing a conversion
#define ADS_REG_CONFIG_OS_IDLE			(0x8000) // Not performing a conversion

// --------------------------------------------
// MUX (INPUT MULTIPLEXER CONFIGURATION)
// --------------------------------------------

#define ADS_REG_CONFIG_MUX_MASK     	(0x7000)
#define ADS_REG_CONFIG_MUX_0_1			(0x0000) // Differential P=AIN0, N=AIN1 (default)
#define ADS_REG_CONFIG_MUX_0_3			(0x1000) // Differential P=AIN0, N=AIN3
#define ADS_REG_CONFIG_MUX_1_3			(0x2000) // Differential P=AIN1, N=AIN3
#define ADS_REG_CONFIG_MUX_2_3			(0x3000) // Differential P=AIN2, N=AIN3
#define ADS_REG_CONFIG_MUX_0_GND		(0x4000) // Single-ended AIN0
#define ADS_REG_CONFIG_MUX_1_GND		(0x5000) // Single-ended AIN1
#define ADS_REG_CONFIG_MUX_2_GND		(0x6000) // Single-ended AIN2
#define ADS_REG_CONFIG_MUX_3_GND		(0x7000) // Single-ended AIN3

// --------------------------------------------
// PGA (PROGRAMMABLE GAIN AMPLIFIER)
// --------------------------------------------

#define ADS_REG_CONFIG_PGA_MASK     	(0x0E00)
#define ADS_REG_CONFIG_PGA_6_144V   	(0x0000) // +/-6.144V range
#define ADS_REG_CONFIG_PGA_4_096V   	(0x0200) // +/-4.096V range
#define ADS_REG_CONFIG_PGA_2_048V   	(0x0400) // +/-2.048V range (default)
#define ADS_REG_CONFIG_PGA_1_024V   	(0x0600) // +/-1.024V range
#define ADS_REG_CONFIG_PGA_0_512V   	(0x0800) // +/-0.512V range
#define ADS_REG_CONFIG_PGA_0_256V   	(0x0A00) // +/-0.256V range

// --------------------------------------------
// MODE (DEVICE OPERATING MODE)
// --------------------------------------------

#define ADS_REG_CONFIG_MODE_MASK    	(0x0100)
#define ADS_REG_CONFIG_MODE_CONTINIOUS  (0x0000) // Continuous-conversion mode
#define ADS_REG_CONFIG_MODE_SINGLE  	(0x0100) // Single-shot mode or power-down state (default)

// --------------------------------------------
// DR (DATA RATE / SPS = SAMPLES PER SECOND)
// --------------------------------------------

#define ADS_REG_CONFIG_DR_MASK      	(0x00E0)
#define ADS_REG_CONFIG_DR_8SPS      	(0x0000)
#define ADS_REG_CONFIG_DR_16SPS     	(0x0020)
#define ADS_REG_CONFIG_DR_32SPS     	(0x0040)
#define ADS_REG_CONFIG_DR_64SPS     	(0x0060)
#define ADS_REG_CONFIG_DR_128SPS    	(0x0080) // (default)
#define ADS_REG_CONFIG_DR_250SPS    	(0x00A0)
#define ADS_REG_CONFIG_DR_475SPS    	(0x00C0)
#define ADS_REG_CONFIG_DR_860SPS    	(0x00E0)

// --------------------------------------------
// COMP MODE (COMPARATOR MODE)
// --------------------------------------------

#define ADS_REG_CONFIG_COMP_MODE_MASK	(0x0010)
#define ADS_REG_CONFIG_COMP_MODE_TRAD	(0x0000) // Traditional comparator (default)
#define ADS_REG_CONFIG_COMP_MODE_WINDOW	(0x0010) // Window comparator

// --------------------------------------------
// COMP POL (COMPARATOR POLARITY)
// --------------------------------------------

#define ADS_REG_CONFIG_COMP_POL_MASK	(0x0008)
#define ADS_REG_CONFIG_COMP_POL_LOW		(0x0000) // Active low (default)
#define ADS_REG_CONFIG_COMP_POL_HIGH	(0x0008) // Active high

// --------------------------------------------
// COMP LAT (LATCHING COMPARATOR)
// --------------------------------------------

#define ADS_REG_CONFIG_COMP_LAT_MASK	(0x0004)
#define ADS_REG_CONFIG_COMP_LAT_OFF		(0x0000) // Non-latching (Default)
#define ADS_REG_CONFIG_COMP_LAT_ON		(0x0004) // Latching (stays active until read)

// --------------------------------------------
// COMP QUE (COMPARATOR QUEUE)
// --------------------------------------------

#define ADS_REG_CONFIG_COMP_QUE_MASK	(0x0003)
#define ADS_REG_CONFIG_COMP_QUE_1CONV   (0x0000) // Assert after one conversion
#define ADS_REG_CONFIG_COMP_QUE_2CONV   (0x0001) // Assert after two conversions
#define ADS_REG_CONFIG_COMP_QUE_4CONV   (0x0002) // Assert after four conversions
#define ADS_REG_CONFIG_COMP_QUE_DIS     (0x0003) // Disable comparator and set ALERT/RDY to high-impedance (default)

typedef enum {
    ADS_FSR_6_144V = ADS_REG_CONFIG_PGA_6_144V,
    ADS_FSR_4_096V = ADS_REG_CONFIG_PGA_4_096V,
    ADS_FSR_2_048V = ADS_REG_CONFIG_PGA_2_048V,
    ADS_FSR_1_024V = ADS_REG_CONFIG_PGA_1_024V,
    ADS_FSR_0_512V = ADS_REG_CONFIG_PGA_0_512V,
    ADS_FSR_0_256V = ADS_REG_CONFIG_PGA_0_256V
} ads1115_fsr_t;

typedef struct {
	uint16_t address;
	i2c_master_dev_handle_t i2c_handle;
	ads1115_fsr_t gain;
}ads1115_t;

#endif