#ifndef DS18B20_DS18B20_CONFIG_H
#define DS18B20_DS18B20_CONFIG_H

#include <stddef.h>
#include <stdint.h>

#define DS18B20_LSB_SCALE (1.0F / 16.0F)

typedef enum {
    DS18B20_ERR_OK = 0,
    DS18B20_ERR_FAIL = 1 << 0,
    DS18B20_ERR_NULL = 1 << 1,
} ds18b20_err_t;

typedef enum {
    DS18B20_CMD_READ_ROM = 0x33,
    DS18B20_CMD_MATCH_ROM = 0x55,
    DS18B20_CMD_SKIP_ROM = 0xCC,
    DS18B20_CMD_SEARCH_ROM = 0xF0,
    DS18B20_CMD_ALARM_SERACH = 0xEC,
    DS18B20_CMD_WRITE_SCRATCHPAD = 0x4E,
    DS18B20_CMD_READ_SCRATCHPAD = 0xBE,
    DS18B20_CMD_COPY_SCRATCHPAD = 0x48,
    DS18B20_CMD_CONVERT_T = 0x44,
    DS18B20_CMD_RECALL_E2 = 0xB8,
    DS18B20_CMD_READ_POW_SUP = 0xB4,
} ds18b20_cmd_t;

typedef struct {
    uint64_t address;
} ds18b20_config_t;

typedef struct {
    void* ow_user;
    ds18b20_err_t (*ow_init)(void*);
    ds18b20_err_t (*ow_deinit)(void*);
    ds18b20_err_t (*ow_reset)(void*);
    ds18b20_err_t (*ow_transmit)(void*, uint8_t const*, size_t);
    ds18b20_err_t (*ow_receive)(void*, uint8_t*, size_t);
} ds18b20_interface_t;

#endif // DS18B20_DS18B20_CONFIG_H