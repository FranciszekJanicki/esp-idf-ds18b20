#include "ds18b20.h"
#include <assert.h>
#include <string.h>

static ds18b20_err_t ds18b20_ow_init(ds18b20_t const* ds18b20)
{
    return ds18b20->interface.ow_init ? ds18b20->interface.ow_init(ds18b20->interface.ow_user) : DS18B20_ERR_NULL;
}

static ds18b20_err_t ds18b20_ow_deinit(ds18b20_t const* ds18b20)
{
    return ds18b20->interface.ow_deinit ? ds18b20->interface.ow_deinit(ds18b20->interface.ow_user) : DS18B20_ERR_NULL;
}

static ds18b20_err_t ds18b20_ow_reset(ds18b20_t const* ds18b20)
{
    return ds18b20->interface.ow_reset ? ds18b20->interface.ow_reset(ds18b20->interface.ow_user) : DS18B20_ERR_NULL;
}

static ds18b20_err_t ds18b20_ow_transmit(ds18b20_t const* ds18b20, uint8_t const* data, size_t data_size)
{
    return ds18b20->interface.ow_transmit ? ds18b20->interface.ow_transmit(ds18b20->interface.ow_user, data, data_size)
                                          : DS18B20_ERR_NULL;
}

static ds18b20_err_t ds18b20_ow_receive(ds18b20_t const* ds18b20, uint8_t* data, size_t data_size)
{
    return ds18b20->interface.ow_receive ? ds18b20->interface.ow_receive(ds18b20->interface.ow_user, data, data_size)
                                         : DS18B20_ERR_NULL;
}

ds18b20_err_t
ds18b20_initialize(ds18b20_t* ds18b20, ds18b20_config_t const* config, ds18b20_interface_t const* interface)
{
    assert(ds18b20);

    memset(ds18b20, 0, sizeof(*ds18b20));
    memcpy(&ds18b20->config, config, sizeof(*config));
    memcpy(&ds18b20->interface, interface, sizeof(*interface));

    return ds18b20_ow_init(ds18b20);
}

ds18b20_err_t ds18b20_deinitialize(ds18b20_t* ds18b20)
{
    assert(ds18b20);

    ds18b20_err_t err = ds18b20_ow_deinit(ds18b20);

    memset(ds18b20, 0, sizeof(*ds18b20));

    return err;
}

ds18b20_err_t ds18b20_get_tem_data_raw(ds18b20_t const* ds18b20, int16_t* raw)
{
    assert(ds18b20 && raw);

    uint8_t data[2] = {};

    ds18b20_err_t err = ds18b20_ow_reset(ds18b20);
    err |= ds18b20_send_skip_rom_cmd(ds18b20);
    err |= ds18b20_send_convert_t_cmd(ds18b20);
    err |= ds18b20_wait_convert(ds18b20);
    err |= ds18b20_ow_reset(ds18b20);
    err |= ds18b20_send_match_rom_cmd(ds18b20);
    err |= ds18b20_transmit_slave_address(ds18b20);
    err |= ds18b20_send_read_scratchpad_cmd(ds18b20);
    err |= ds18b20_ow_receive(ds18b20, data, sizeof(data));

    *raw = (int16_t)((data[0] & 0xFF) | ((data[1] & 0xFF) << 8));

    return err;
}

ds18b20_err_t ds18b20_get_temp_data_scaled(ds18b20_t const* ds18b20, float* scaled)
{
    assert(ds18b20 && scaled);

    int16_t raw = {};

    ds18b20_err_t err = ds18b20_get_tem_data_raw(ds18b20, &raw);

    *scaled = (float)raw * DS18B20_LSB_SCALE;

    return err;
}

ds18b20_err_t ds18b20_transmit_slave_address(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data[8] = {};

    data[0] = ds18b20->config.address & 0xFFU;
    data[1] = (ds18b20->config.address >> 8U) & 0xFFU;
    data[2] = (ds18b20->config.address >> 16U) & 0xFFU;
    data[3] = (ds18b20->config.address >> 24U) & 0xFFU;
    data[4] = (ds18b20->config.address >> 32U) & 0xFFU;
    data[5] = (ds18b20->config.address >> 40U) & 0xFFU;
    data[6] = (ds18b20->config.address >> 48U) & 0xFFU;
    data[7] = (ds18b20->config.address >> 56U) & 0xFFU;

    return ds18b20_ow_transmit(ds18b20, data, sizeof(data));
}

ds18b20_err_t ds18b20_wait_convert(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = {};
    ds18b20_err_t err = {};

    do {
        err |= ds18b20_ow_receive(ds18b20, &data, sizeof(data));
    } while (data == 0U);

    return err;
}

ds18b20_err_t ds18b20_send_read_rom_cmd(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = DS18B20_CMD_READ_ROM;

    return ds18b20_ow_transmit(ds18b20, &data, sizeof(data));
}

ds18b20_err_t ds18b20_send_match_rom_cmd(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = DS18B20_CMD_MATCH_ROM;

    return ds18b20_ow_transmit(ds18b20, &data, sizeof(data));
}

ds18b20_err_t ds18b20_send_skip_rom_cmd(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = DS18B20_CMD_SKIP_ROM;

    return ds18b20_ow_transmit(ds18b20, &data, sizeof(data));
}

ds18b20_err_t ds18b20_send_search_rom_cmd(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = DS18B20_CMD_SEARCH_ROM;

    return ds18b20_ow_transmit(ds18b20, &data, sizeof(data));
}

ds18b20_err_t ds18b20_send_alarm_search_cmd(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = DS18B20_CMD_ALARM_SERACH;

    return ds18b20_ow_transmit(ds18b20, &data, sizeof(data));
}

ds18b20_err_t ds18b20_send_write_scratchpad_cmd(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = DS18B20_CMD_WRITE_SCRATCHPAD;

    return ds18b20_ow_transmit(ds18b20, &data, sizeof(data));
}

ds18b20_err_t ds18b20_send_read_scratchpad_cmd(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = DS18B20_CMD_READ_SCRATCHPAD;

    return ds18b20_ow_transmit(ds18b20, &data, sizeof(data));
}

ds18b20_err_t ds18b20_send_copy_scratchpad_cmd(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = DS18B20_CMD_COPY_SCRATCHPAD;

    return ds18b20_ow_transmit(ds18b20, &data, sizeof(data));
}

ds18b20_err_t ds18b20_send_convert_t_cmd(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = DS18B20_CMD_CONVERT_T;

    return ds18b20_ow_transmit(ds18b20, &data, sizeof(data));
}

ds18b20_err_t ds18b20_send_recall_e2_cmd(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = DS18B20_CMD_RECALL_E2;

    return ds18b20_ow_transmit(ds18b20, &data, sizeof(data));
}

ds18b20_err_t ds18b20_send_read_pow_sup_cmd(ds18b20_t const* ds18b20)
{
    assert(ds18b20);

    uint8_t data = DS18B20_CMD_READ_POW_SUP;

    return ds18b20_ow_transmit(ds18b20, &data, sizeof(data));
}
