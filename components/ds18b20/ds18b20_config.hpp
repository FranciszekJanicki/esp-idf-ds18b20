#ifndef DS18B20_CONFIG_HPP
#define DS18B20_CONFIG_HPP

#include "ow_device.hpp"
#include <cstdint>

namespace DS18B20 {

    using OWDevice = ESP32_Utility::OWDevice;

    enum struct DevAddress : std::uint64_t {
        ADDRESS1 = 0x6a01191ee2ef3428,
        ADDRESS2 = 0x701191f0a97c228,
        ADDRESS3 = 0x8e01191b0c925e28,
    };

    enum struct CMD : std::uint16_t {
        READ_ROM = 0x33,
        MATCH_ROM = 0x55,
        SKIP_ROM = 0xCC,
        SEARCH_ROM = 0xF0,
        ALARM_SERACH = 0xEC,
        WRITE_SCRATCHPAD = 0x4E,
        READ_SCRATCHPAD = 0xBE,
        COPY_SCRATCHPAD = 0x48,
        CONVERT_T = 0x44,
        RECALL_E2 = 0xB8,
        READ_POW_SUP = 0xB4,
    };

}; // namespace DS18B20

#endif // DS18B20_CONFIG_HPP