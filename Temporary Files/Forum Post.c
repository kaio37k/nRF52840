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

/* TWI instance ID. */
#define TWI_INSTANCE_ID     0

/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

void twi_init (void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t twi_lm75b_config = {
       .scl                = NRF_GPIO_PIN_MAP(0, 27),
       .sda                = NRF_GPIO_PIN_MAP(0, 26),
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_lm75b_config, NULL, NULL);
    //printf("Error Code init\t%d\n", err_code);

    nrf_drv_twi_enable(&m_twi);
}

//=====================================================
//                        MAIN
//=====================================================

int main(void)
{
    struct bmm150_dev dev;
    struct bmm150_settings mySettings;
    struct bmm150_mag_data mag_data;

    int a1, a2, b, c, d, e, f, g, h, i;

    dev.intf = BMM150_I2C_INTF;
    dev.read = bmm150_user_i2c_reg_read;
    dev.write = bmm150_user_i2c_reg_write;
    dev.delay_us = 3;

    printf("Breaker 1\n");

    twi_init();

    uint8_t tempValx[8] = {0};

    bmm150_get_regs(0x4b, tempValx, 1, &dev);

    for(int a = 0; a < sizeof(tempValx); a++){
      printf("getRegs %d\t%d\n", a, tempValx[a]);
    }
    
    printf("Breaker 2\n");

    nrf_delay_ms(2500);


    a1 = bmm150_interface_selection(&dev);
    printf("rslt A1:\t%d\n", a1);

    a2 = bmm150_init(&dev);
    printf("rslt A2:\t%d\n", a2);
    
    mySettings.pwr_mode = BMM150_POWERMODE_NORMAL;
    b = bmm150_set_op_mode(&mySettings, &dev);
    printf("rslt B:\t%d\n", b);

    mySettings.preset_mode = BMM150_PRESETMODE_REGULAR;
    c = bmm150_set_presetmode(&mySettings, &dev);
    printf("rslt C:\t%d\n", c);
    
    mySettings.int_settings.drdy_pin_en = 0x01;
    d = bmm150_set_sensor_settings(BMM150_SEL_DRDY_PIN_EN, &mySettings, &dev);
    printf("rslt D:\t%d\n", d);
    
    e = bmm150_get_interrupt_status(&dev);
    printf("rslt E:\t%d\n", e);

    printf("Dev->int_status:\t%d\n", dev.int_status);
    printf("BMM150_INT_ASSRTED_DRDY:\t%d\n", BMM150_INT_ASSERTED_DRDY);



    //================================
    //    Startup Config Functions
    //================================

    /*
    mySettings.data_rate = BMM150_DATA_RATE_10HZ;
    mySettings.xyz_axes_control = BMM150_XYZ_CHANNEL_ENABLE;
    
    bmm150_set_sensor_settings(BMM150_SEL_DATA_RATE, &mySettings, &dev);
    */
    

    printf("Mag X:\t%d\n", mag_data.x);
    printf("Mag Y:\t%d\n", mag_data.y);
    printf("Mag Z:\t%d\n", mag_data.z);

    uint8_t regValTemp[8] = {0};

    checkAllRegisters();

    while(true){

      uint8_t dsa = bmm150_read_mag_data(&mag_data, &dev);
      printf("bmm150_read_mag_data:\t%d\n", dsa);

      printf("Interrupt Status\t%d\n", dev.int_status);

      nrf_delay_ms(100);
    }

}
