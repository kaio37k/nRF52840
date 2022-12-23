#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrfx_twi.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "bmm150.h"
#include "bmm150_common.h"

//=====================================================
//                        MAIN
//=====================================================

int main(void)
{
    struct bmm150_settings mySettings;
    struct bmm150_raw_mag_data raw_mag;
    struct bmm150_mag_data mag_data;
    struct bmm150_dev dev;

    dev.intf = BMM150_I2C_INTF;
    //dev.intf_ptr = BMM150_DEFAULT_I2C_ADDRESS;
    dev.delay_us = bmm150_user_delay_us;
    dev.read = bmm150_user_i2c_reg_read;
    dev.write = bmm150_user_i2c_reg_write;
    dev.pwr_cntrl_bit = 1;

    bmm150_interface_selection(&dev);

    uint8_t numberOne[1] = {0x01};

    //bmm150_user_i2c_reg_write(0x4b, numberOne, 0x82, &dev);

    uint8_t asd[1] = {9};

    //bmm150_user_i2c_reg_write(0x4b, 0x01, 1, 0x10);
    //printf("---------------------------\n");

    bmm150_user_i2c_reg_read(0x4C, asd, 1, 0x10);

    printf("---------------------------\n");

    //bmm150_init(&dev);

    //writeOneRegister(0x4b, 0x01);

    //mySettings.pwr_mode = BMM150_POWERMODE_NORMAL;
    //bmm150_set_op_mode(&mySettings, &dev);

    //mySettings.preset_mode = BMM150_PRESETMODE_REGULAR;
    //bmm150_set_presetmode(&mySettings, &dev);

    //writeOneRegister(0x4b, 0x82);  //  SOFT  RESET
    //writeOneRegister(0x4b, 0x01);    //  POWER CONTROL BIT,  ON
    //writeOneRegister(0x4C, 0x00);    //  SET   OP  MODE

    //readOneRegister(0x4E);
    //readOneRegister(0x4D);
    //readOneRegister(0x4C);
    //readOneRegister(0x4B);
    //readOneRegister(0x40);

}
