#ifndef __PCF8575_H__
#define __PCF8575_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

struct pcf8575 {
    i2c_inst_t* i2c_dev;
    uint8_t I2c_ADD;
    bool IF_OUTPUT[16];
    bool PIN_SET_VALUE[16];
    bool PIN_READ_VALUE[16];
    bool DEBUG;
};

struct pcf8575 PCF8575_init(i2c_inst_t *i2c, uint8_t I2cAdd);
void PCF8575_SET_DEBGU(struct pcf8575 *r_handle, bool DEBUG);
void PCF8575_set_dir(uint8_t __Pin__, struct pcf8575 *r_handle, bool Pin_Out);
void PCF8575_set_dir_all(uint16_t P_OUT, struct pcf8575 *r_handle);
bool PCF8575_get_dir(uint8_t __Pin__, struct pcf8575 *r_handle);
uint16_t PCF8575_get_dir_all(struct pcf8575 *r_handle);
void PCF8575_SEND_VALUE(struct pcf8575 *r_handle);
void PCF8575_set_value(struct pcf8575 *r_handle, uint8_t __Pin__, bool value);
#endif