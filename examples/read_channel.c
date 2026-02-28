/*
 * SPDX-FileCopyrightText: 2026 Lukas Bammer
 *
 * SPDX-License-Identifier: MIT
 */
/*
    Reads the voltage from channel 0 and prints it.

    | ADS1115 Pin | ESP32 Pin (GPIO) | Function      | Notes                         |
    | ----------- | ---------------- | ------------- | ----------------------------- |
    | VDD         | 3.3V             | Power         | Stable 3.3V Supply            |
    | GND         | GND              | Ground        | Common Ground                 |
    | SCL         | GPIO 22          | I2C Clock     | Needs 4.7k - 10k Pull-up      |
    | SDA         | GPIO 21          | I2C Data      | Needs 4.7k - 10k Pull-up      |
    | ADDR        | GND              | I2C Address   | Set to GND for Address 0x48   |
*/

#include "freertos/FreeRTOS.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "ads1115.h"

static const char *TAG = "ADS1115_TEST";

#define SCL_GPIO GPIO_NUM_22
#define SDA_GPIO GPIO_NUM_21
#define I2C_FREQUENZY 100000
#define ADS_CHANNEL 0
#define DELAY_MS 1000

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

    ads1115_t ads;
    if (ads1115_init(&ads, &bus_handle, ADS_I2C_ADDR_GND, I2C_FREQUENZY) != ESP_OK) {
        ESP_LOGE(TAG, "ADS1115 init failed!");
        return;
    }

    ads1115_set_gain(&ads, ADS_FSR_4_096V);
    ads1115_set_sps(&ads, ADS_SPS_128);

    while (1) {
        uint16_t raw = ads1115_get_raw(&ads, ADS_CHANNEL);

        float voltage = ads1115_raw_to_voltage(&ads, raw);

        ESP_LOGI(TAG, "Channel 0: Raw: %d | Voltage: %.4f V\n", raw, voltage);

        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
}