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

    int a1, a2, b, c, d, e, f, g, h, i;

    dev.chip_id = BMM150_CHIP_ID;
    dev.intf = BMM150_I2C_INTF;
    dev.delay_us = bmm150_user_delay_us;
    dev.read = bmm150_user_i2c_reg_read;
    dev.write = bmm150_user_i2c_reg_write;
    
    
    //mySettings.data_rate = BMM150_DATA_RATE_10HZ;
    //mySettings.xyz_axes_control = BMM150_XYZ_CHANNEL_ENABLE;

    twi_init();

    a1 = bmm150_interface_selection(&dev);

    //a2 = bmm150_init(&dev);
    
    /*
    mySettings.pwr_mode = BMM150_POWERMODE_NORMAL;
    b = bmm150_set_op_mode(&mySettings, &dev);

    mySettings.preset_mode = BMM150_PRESETMODE_REGULAR;
    c = bmm150_set_presetmode(&mySettings, &dev);
    
    mySettings.int_settings.drdy_pin_en = 0x01;
    d = bmm150_set_sensor_settings(BMM150_SEL_DRDY_PIN_EN, &mySettings, &dev);
    
    e = bmm150_get_interrupt_status(&dev);
    */

    //================================
    //    Startup Config Functions
    //================================

    //bmm150_set_sensor_settings(BMM150_SEL_DATA_RATE, &mySettings, &dev);

      writeRegister(0x4b, 0x01);

      //readOneRegister(0x4b);

      uint8_t tempDataStorage[1];
      bmm150_user_i2c_reg_read(0x4C, tempDataStorage, 1, 0);

    while(false){

      nrf_delay_ms(500);
    }

}
