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

void startupSequence(bool printCodes){
  uint8_t startupSequenceArrayRegisters1[] = {0x4B, 0x83};
  uint8_t startupSequenceArrayRegisters2[] = {0x4C, 0x00};

  ret_code_t tx_code1, tx_code2;

  tx_code2 = nrf_drv_twi_tx(&m_twi, 0x10, &startupSequenceArrayRegisters2, 2, false);
  tx_code1 = nrf_drv_twi_tx(&m_twi, 0x10, &startupSequenceArrayRegisters1, 2, false);
  
  if(printCodes){
      printf("TX Code:\t%d\n", tx_code2);
      printf("TX Code:\t%d\n", tx_code1);
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

void checkAllRegisters(){

  readRegister(0x52, false);
  readRegister(0x51, false);
  readRegister(0x50, false);
  readRegister(0x4F, false);
  readRegister(0x4E, false);
  readRegister(0x4D, false);
  readRegister(0x4C, false);
  readRegister(0x4B, false);
  readRegister(0x4A, false);
}

void checkXYZRegisters(){

    uint8_t XYZRegisters[] = {0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49};
    uint8_t registerValue[8];

    for(int i = 0; i < sizeof(XYZRegisters); i++){
      nrf_drv_twi_tx(&m_twi, 0x10, &XYZRegisters, 1, true);
      nrf_drv_twi_rx(&m_twi, 0x10, registerValue, 8);

      printf("-----------------\n");
      printf("register:\t%x\n", XYZRegisters[i]);
      printf("registerValue:\t%d\n", registerValue[i]*32);
      //printf("XYZRegisters.length\t%d\n", sizeof(XYZRegisters)); // Line to print the size of the registers array
  }
}

void checkSpecificAxis(char Axis){

    uint8_t XYZRegisters[2];
    uint8_t registerValue[2];

    switch(Axis){

    case 'X':
    XYZRegisters[0] = 0x43;
    XYZRegisters[1] = 0x42;
    nrf_drv_twi_tx(&m_twi, 0x10, &XYZRegisters, 1, true);
    nrf_drv_twi_rx(&m_twi, 0x10, registerValue, 2);
    
    break;

    case 'Y':
    XYZRegisters[0] = 0x45;
    XYZRegisters[1] = 0x44;
    nrf_drv_twi_tx(&m_twi, 0x10, &XYZRegisters, 1, true);
    nrf_drv_twi_rx(&m_twi, 0x10, registerValue, 2);
    
    break;

    case 'Z':
    XYZRegisters[0] = 0x47;
    XYZRegisters[1] = 0x46;
    nrf_drv_twi_tx(&m_twi, 0x10, &XYZRegisters, 1, true);
    nrf_drv_twi_rx(&m_twi, 0x10, registerValue, 2);
    
    break;

  }

      printf("\n%c", Axis);
      printf("\t");
      printf("%d", registerValue[0]*32);
      printf("\t");
      printf("%d", registerValue[1]*32);
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

    checkAllRegisters();
    
    printf("Breaker 2\n");

    //bmm150_soft_reset(&dev);
    printf("Breaker 3\n");

    checkAllRegisters();
    printf("Breaker 4\n");

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
      
      //printf("regValTemp:\t%d\n", regValTemp[0]);
      //printf("regValTemp:\t%d\n", regValTemp[1]);
      //printf("regValTemp:\t%d\n", regValTemp[7]);
      //printf("regValTemp:\t%d\n", regValTemp[8]);

      printf("Mag X:\t%d\n", mag_data.x);
      
      //checkSpecificAxis('Z');

      nrf_delay_ms(100);
    }

}
