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

    uint8_t dsa[12];

    //bmm150_user_i2c_reg_write(0x4b, 0x00, 1, 0x10);
    //bmm150_user_i2c_reg_read(0x4b, dsa, 1, 0x10);
    //printf("dsa[0]:\t%d\n", dsa[0]);
    
    //printf("---------------------------\n");

    //bmm150_user_i2c_reg_read(0x4b, dsa, 1, 0x10);

    //bmm150_get_regs(0x40, dsa, 1, &dev);

    //printf("---------------------------\n");
    //printf("dsa:\t%x\n", dsa[0]);
    
    //writeOneRegister(0x4b, 0x82);     //  SOFT  RESET
    //writeOneRegister(0x4b, 0x01);     //  POWER CONTROL BIT,  ON
    //writeOneRegister(0x4C, 0x00);     //  SET   OP  MODE
    
    nrf_delay_ms(50);
    
    writeOneRegister(0x4b, 0x01);
    nrf_delay_ms(50);
    writeOneRegister(0x4c, 0x06);
    nrf_delay_ms(56);

    bmm150_user_i2c_reg_read(0x4b, dsa, 3, 0x10);
    nrf_delay_ms(56);

    bmm150_init(&dev);

    nrf_delay_ms(100);

    for(int i = 0; i < 12; i++){
      printf("dsa [%d]:\t%d\n", i, dsa[i]);
    }

    bmm150_user_i2c_reg_write(0x4b, 1, 2, 0x10);
    nrf_delay_ms(56);

    bmm150_user_i2c_reg_read(0x4b, dsa, 1, 0x10);
    nrf_delay_ms(56);

    bmm150_user_i2c_reg_read(0x4b, dsa, 1, 0x10);
    nrf_delay_ms(56);

    for(int i = 0; i < 12; i++){
      printf("dsa [%d]:\t%d\n", i, dsa[i]);
    }
    
    mySettings.pwr_mode = BMM150_POWERMODE_NORMAL;
    int tempA = bmm150_set_op_mode(&mySettings, &dev);
    printf("set OP:\t%x\n", tempA);

    nrf_delay_ms(1000);
    
    mySettings.preset_mode = BMM150_PRESETMODE_REGULAR;
    int tempB = bmm150_set_presetmode(&mySettings, &dev);
    printf("set preset:\t%x\n", tempB);

    //printf("=====================\n");
    //bmm150_user_i2c_reg_read(0x4a, dsa, 10, 0x10);
    
    //bmm150_user_i2c_reg_read(0x4c, asd, 1, 0x10);
    
    //printf("dsa:\t%x\n", dsa[0]);
    //printf("asd:\t%x\n", asd[0]);

}
