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
    printf("registerRX: \t%x\n", registerRX1[0]);

    
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

int main(void)
{

    twi_init();

    readRegister(0x4F, false);
    readRegister(0x4E, false);
    readRegister(0x4D, false);
    readRegister(0x4C, false);
    readRegister(0x4B, false);

    nrf_delay_ms(500);
    

}
