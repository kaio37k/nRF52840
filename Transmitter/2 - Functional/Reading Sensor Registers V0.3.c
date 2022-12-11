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

uint8_t registerRX[] = {7};

const uint8_t registerTX[] = {0x4D};

struct bmm150_dev dev;

struct bmm150_mag_data mag_data;

struct bmm150_settings mySettings;

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

void readRegister(int registerAddressX, bool printCodes){
  
    uint8_t registerTX1[] = {registerAddressX}; // Pointer
    uint8_t registerRX1[] = {7};

    ret_code_t tx_code;
    ret_code_t rx_code;

    tx_code = nrf_drv_twi_tx(&m_twi, 0x10, &registerTX1, 1, true);
    rx_code = nrf_drv_twi_rx(&m_twi, 0x10, registerRX1, 1);
    printf("Register (%x", registerAddressX);
    printf("):\t%x\n", registerRX1[0]);

    
    if(printCodes){
      printf("TX Code:\t%d\n", tx_code);
      printf("RX Code:\t%d\n", rx_code);
    }
}

void writeRegister(int registerAddressY, int setValue, bool printCodes){
  
  uint8_t registerTX2[] = {registerAddressY, setValue};

  ret_code_t tx_code;

  tx_code = nrf_drv_twi_tx(&m_twi, 0x10, &registerTX2, 2, false);
  
  if(printCodes){
      printf("TX Code:\t%d\n", tx_code);
  }

}

void readSensor(){
  
  uint8_t registerPointer[] = {0x43};
  int16_t sensorData[] = {0};

  ret_code_t tx_code;
  ret_code_t rx_code;

  tx_code = nrf_drv_twi_tx(&m_twi, 0x10, &registerPointer, 1, true);
  rx_code = nrf_drv_twi_rx(&m_twi, 0x10, sensorData, 1);

  printf("Sensor Data:\t%d\n", sensorData[0]);
}

void writeSensor(){
  
  //uint8_t registerPointer[] = {0x4E, 0x87};
  
  //ret_code_t tx_code_1;
  //ret_code_t tx_code_2;
  
  //tx_code_1 = nrf_drv_twi_tx(&m_twi, 0x10, &registerPointer, 2, false);
  
  //printf("TX Code 1:\t%d\n", tx_code_1);
  //printf("TX Code 2:\t%d\n", tx_code_2);

}

static int8_t set_config(struct bmm150_dev *dev)
{
    /* Status of api are returned to this variable. */
    int8_t rslt;

    struct bmm150_settings settings;

    /* Set powermode as normal mode */
    settings.pwr_mode = BMM150_POWERMODE_NORMAL;
    rslt = bmm150_set_op_mode(&settings, dev);
    bmm150_error_codes_print_result("bmm150_set_op_mode", rslt);

    if (rslt == BMM150_OK)
    {
        /* Setting the preset mode as Low power mode
         * i.e. data rate = 10Hz, XY-rep = 1, Z-rep = 2
         */
        settings.preset_mode = BMM150_PRESETMODE_LOWPOWER;
        rslt = bmm150_set_presetmode(&settings, dev);
        bmm150_error_codes_print_result("bmm150_set_presetmode", rslt);

        if (rslt == BMM150_OK)
        {
            /* Map the data interrupt pin */
            settings.int_settings.drdy_pin_en = 0x01;
            rslt = bmm150_set_sensor_settings(BMM150_SEL_DRDY_PIN_EN, &settings, dev);
            bmm150_error_codes_print_result("bmm150_set_sensor_settings", rslt);
        }
    }

    return rslt;
}

static int8_t get_data(struct bmm150_dev *dev)
{
    /* Status of api are returned to this variable. */
    int8_t rslt;

    int8_t idx;

    struct bmm150_mag_data mag_data;

    /* Reading the mag data */
    while (1)
    {
        /* Get the interrupt status */
        rslt = bmm150_get_interrupt_status(dev);

        if (dev->int_status & BMM150_INT_ASSERTED_DRDY)
        {
            printf("Data interrupt occurred\n");

            for (idx = 0; idx < 50; idx++)
            {
                /* Read mag data */
                rslt = bmm150_read_mag_data(&mag_data, dev);
                bmm150_error_codes_print_result("bmm150_read_mag_data", rslt);

                /* Unit for magnetometer data is microtesla(uT) */
                printf("MAG DATA[%d]  X : %d uT   Y : %d uT   Z : %d uT\n", idx, mag_data.x, mag_data.y, mag_data.z);
            }
        }

        break;
    }

    return rslt;
}

void checkAllRegisters(){
  readRegister(0x52, false);
  readRegister(0x51, false);
  readRegister(0x50, false);
  readRegister(0x4f, false);
  readRegister(0x4e, false);
  readRegister(0x4d, false);
  readRegister(0x4c, false);
  readRegister(0x4b, false);
  readRegister(0x4A, false);
  readRegister(0x49, false);
  readRegister(0x48, false);
  readRegister(0x47, false);
  readRegister(0x46, false);
  readRegister(0x45, false);
  readRegister(0x44, false);
  readRegister(0x43, false);
  readRegister(0x42, false);
  readRegister(0x41, false);
  readRegister(0x40, false);
}

int main(void)
{
    
    dev.read = readSensor;
    dev.write = writeSensor;
    dev.delay_us = 3;

    twi_init();

    readRegister(0x4C, false);

    writeRegister(0x4E, 0x07, false);

    printf("bmm150 intf sel:\t%d\n", bmm150_interface_selection(&dev));

    printf("bmm150 init:\t\t%d\n", bmm150_init(&dev));
    printf("bmm150 set_config:\t%d\n", set_config(&dev));
    printf("bmm150 get_data:\t%d\n", get_data(&dev));

    //mySettings.data_rate = BMM150_DATA_RATE_10HZ;
    mySettings.preset_mode = BMM150_PRESETMODE_HIGHACCURACY;
    mySettings.pwr_mode = BMM150_POWERMODE_NORMAL;
    //mySettings.xyz_axes_control = BMM150_XYZ_CHANNEL_ENABLE;

    
    bmm150_set_presetmode(&mySettings, &dev);
    bmm150_set_op_mode(&mySettings, &dev);

    nrf_delay_ms(500);

    printf("read_mag:\t%d\n", bmm150_read_mag_data(&mag_data, &dev));

    printf("Mag X:\t%d\n", mag_data.x);
    printf("Mag Y:\t%d\n", mag_data.y);
    printf("Mag Z:\t%d\n", mag_data.z);

    printf("-------------\n");
    readRegister(0x43, false);
    printf("-------------\n");
    
    checkAllRegisters();

}
