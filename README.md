# ADS1115 Library for ESP-IDF (v5.x)
This is a lightweight, non-blocking C library for the ADS1115 16-bit ADC, designed specifically for the **ESP32** using the modern `i2c_master` driver.

## Table of Contents
  - [Features](#features)
  - [Installation](#installation)
  - [Example](#example)
  - [License](#license)

## Features

- Modern ESP-IDF: Uses the new i2c_master driver (v5.0+).
- Single-Shot Mode: Power-efficient measurements.
- Differential Measurements: Support for measuring voltage differences (e.g., across shunts).
- Configurable Gain & SPS: Easy-to-use enums for Full Scale Range and Data Rate.
- Polling Synchronization: Ensures data is ready before reading (checks OS bit).

## Installation

### Manual Integration

Copy the source files (`ads1115.c` and `ads1115.h`) into your project's components or main directory. Ensure the header file is accessible within your include path

### Add as submodule
`git submodule add https://github.com/Bammer187/ADS1115.git components/ads1115`

Don't forget to update your `CMakeLists.txt`!

## Example
Simple example where the data from channel 0 is read and printed.

### Pin Mapping

| ADS1115 Pin | ESP32 Pin (GPIO) | Function      | Notes                         |
| :---------- | :--------------- | :------------ | :---------------------------- |
| **VDD** | 3.3V             | Power         | Stable 3.3V Supply            |
| **GND** | GND              | Ground        | Common Ground                 |
| **SCL** | GPIO 22          | I2C Clock     | Needs 4.7k - 10k Pull-up      |
| **SDA** | GPIO 21          | I2C Data      | Needs 4.7k - 10k Pull-up      |
| **ADDR** | GND              | I2C Address   | Set to GND for Address 0x48   |
```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "ads1115.h"

static const char *TAG = "ADS1115_TEST";

#define SCL_GPIO GPIO_NUM_22
#define SDA_GPIO GPIO_NUM_21

void app_main(void)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = SDA_GPIO,
        .scl_io_num = SCL_GPIO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
    };

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = ADS_I2C_ADDR_GND,
        .scl_speed_hz = 100000,
    };

    i2c_master_dev_handle_t ads_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &ads_handle));

    ads1115_t ads;
    if (ads1115_init(&ads, ads_handle) != ESP_OK) {
        ESP_LOGE(TAG, "ADS1115 init failed!");
        return;
    }

    ads1115_set_gain(&ads, ADS_FSR_4_096V);
    ads1115_set_sps(&ads, ADS_SPS_128);

    while (1) {
        uint16_t raw = ads1115_get_raw(&ads, 0);

        float voltage = ads1115_raw_to_voltage(&ads, raw);

        ESP_LOGI(TAG, "Channel 0: Raw: %d | Voltage: %.4f V\n", raw, voltage);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

## License

[MIT License](LICENSE)
