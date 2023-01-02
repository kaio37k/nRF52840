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

    uint8_t dsa[12] = { 0 };

    //writeOneRegister(0x4b, 0x82);     //  SOFT  RESET
    //writeOneRegister(0x4b, 0x01);     //  POWER CONTROL BIT,  ON
    //writeOneRegister(0x4b, 0x00);     //  POWER CONTROL BIT,  OFF
    //writeOneRegister(0x4C, 0x00);     //  SET   OP  MODE
    
    nrf_delay_ms(50);

    bmm150_init(&dev);

    nrf_delay_ms(50);

    bmm150_user_i2c_reg_read(0x4b, dsa, 1, 0x10);

    printf("dsa[0]:\t%x\n", dsa[0]);
    printf("dsa[1]:\t%x\n", dsa[1]);

    nrf_delay_ms(50);
    bmm150_user_i2c_reg_read(0x4c, dsa, 1, 0x10);

    //    <-- GUARANTEED FUNCTIONAL UNTIL HERE -->
    
    
    mySettings.pwr_mode = BMM150_POWERMODE_NORMAL;
    int tempA = bmm150_set_op_mode(&mySettings, &dev);
    printf("set OP:\t%x\n", tempA);

    //    <-- SEEMS FUNCTIONAL UNTIL HERE -->

    //nrf_delay_ms(1000);
    
    mySettings.preset_mode = BMM150_PRESETMODE_REGULAR;
    int tempB = bmm150_set_presetmode(&mySettings, &dev);
    printf("set preset:\t%x\n", tempB);

    bmm150_user_i2c_reg_read(0x51, dsa, 1 , 0x10);
    printf("0x51[0]:\t%x\n", dsa[0]);

    bmm150_user_i2c_reg_read(0x52, dsa, 1 , 0x10);
    printf("0x52[0]:\t%x\n", dsa[0]);

        bmm150_user_i2c_reg_read(0x4C, dsa, 1 , 0x10);
    printf("0x4C[0]:\t%x\n", dsa[0]);

}
