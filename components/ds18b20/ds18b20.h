#ifndef DS18B20_DS18B20_H
#define DS18B20_DS18B20_H

#include "ds18b20_config.h"

typedef struct {
    ds18b20_config_t config;
    ds18b20_interface_t interface;
} ds18b20_t;

ds18b20_err_t
ds18b20_initialize(ds18b20_t* ds18b20, ds18b20_config_t const* config, ds18b20_interface_t const* interface);
ds18b20_err_t ds18b20_deinitialize(ds18b20_t* ds18b20);

ds18b20_err_t ds18b20_get_tem_data_raw(ds18b20_t const* ds18b20, int16_t* raw);
ds18b20_err_t ds18b20_get_temp_data_scaled(ds18b20_t const* ds18b20, float* scaled);

ds18b20_err_t ds18b20_transmit_slave_address(ds18b20_t const* ds18b20);
ds18b20_err_t ds18b20_wait_convert(ds18b20_t const* ds18b20);

ds18b20_err_t ds18b20_send_read_rom_cmd(ds18b20_t const* ds18b20);
ds18b20_err_t ds18b20_send_match_rom_cmd(ds18b20_t const* ds18b20);
ds18b20_err_t ds18b20_send_skip_rom_cmd(ds18b20_t const* ds18b20);
ds18b20_err_t ds18b20_send_search_rom_cmd(ds18b20_t const* ds18b20);
ds18b20_err_t ds18b20_send_alarm_search_cmd(ds18b20_t const* ds18b20);
ds18b20_err_t ds18b20_send_write_scratchpad_cmd(ds18b20_t const* ds18b20);
ds18b20_err_t ds18b20_send_read_scratchpad_cmd(ds18b20_t const* ds18b20);
ds18b20_err_t ds18b20_send_copy_scratchpad_cmd(ds18b20_t const* ds18b20);
ds18b20_err_t ds18b20_send_convert_t_cmd(ds18b20_t const* ds18b20);
ds18b20_err_t ds18b20_send_recall_e2_cmd(ds18b20_t const* ds18b20);
ds18b20_err_t ds18b20_send_read_pow_sup_cmd(ds18b20_t const* ds18b20);

#endif // DS18B20_DS18B20_H
