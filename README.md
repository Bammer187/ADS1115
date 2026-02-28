# ADS1115 Library for ESP-IDF (v5.x)
This is a lightweight, non-blocking C library for the ADS1115 16-bit ADC, designed specifically for the **ESP32** using the modern `i2c_master` driver.

## Table of Contents
  - [Features](#features)
  - [Installation](#installation)
  - [License](#license)

## Features

- Modern ESP-IDF: Uses the new i2c_master driver (v5.0+).
- Single-Shot Mode: Power-efficient measurements.
- Differential Measurements: Support for measuring voltage differences (e.g., across shunts).
- Configurable Gain & SPS: Easy-to-use enums for Full Scale Range and Data Rate.
- Polling Synchronization: Ensures data is ready before reading (checks OS bit).

## Installation

### Manual Integration

Copy the source files (`ads1115.c`, `ads1115.h`, `CMakeLists.txt`) into your project's components or main directory. Ensure the header file is accessible within your include path

### Add as submodule
`git submodule add https://github.com/Bammer187/ADS1115.git components/ads1115`

Don't forget to update your `CMakeLists.txt`!

### Example CMakeLists.txt
idf_component_register(
  SRCS main.c
  INCLUDE_DIRS "."
  REQUIRES
    esp_driver_i2c
    ads1115
)

## License

[MIT License](LICENSE)
