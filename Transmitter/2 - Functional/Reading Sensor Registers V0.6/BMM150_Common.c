/**\
 * Copyright (c) 2020 Bosch Sensortec GmbH. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

/******************************************************************************/
#include <stdio.h>
#include "bmm150_common.h"
#include "bmm150.h"

#include "nrf_drv_twi.h"
#include "nrfx_twi.h"

#include "boards.h"

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

/******************************************************************************/
/*!                Static variable definition                                 */

/*! Variable that holds the I2C device address or SPI chip selection */
static uint8_t dev_addr;

/******************************************************************************/
/*!                User interface functions                                   */

int8_t readAllRegisters(){
  
  uint8_t allRegistersAddressArray[6] = {0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F};

  uint8_t allRegisterDataArray[6];

  int sizeOfARDA = sizeof(allRegisterDataArray) / sizeof(allRegisterDataArray[0]);

  nrf_drv_twi_tx(&m_twi, 0x10, allRegistersAddressArray, 1, false);
  nrf_drv_twi_rx(&m_twi, 0x10, allRegisterDataArray, 1);

  for(int i = 0; i < sizeOfARDA; i++){
    printf("Register %x:\t%x\n", allRegistersAddressArray[i], allRegisterDataArray[i]);
  }

  return 0;
}

void writeRegister(int registerAddress, int registerValue){

  printf("Write Register\n");
  
  uint8_t tempArray[2] = {registerAddress, registerValue};

  nrf_drv_twi_tx(&m_twi, 0x10, &tempArray, 2, false);

  printf("tempArray[0]\t%d\n", tempArray[0]);
  printf("tempArray[1]\t%d\n", tempArray[1]);
}

int8_t readOneRegister(int registerAddressX){

  uint8_t register_addr[1] = {registerAddressX};
  uint8_t register_value[1] = {7};

  nrf_drv_twi_tx(&m_twi, 0x10, &register_addr, 1, true);
  nrf_drv_twi_rx(&m_twi, 0x10, register_value, 1);

  printf("Read reg Value:\t%x\n", register_value[0]);
}

/*!
 * @brief Function for initialization of I2C bus.
 */
int8_t bmm150_user_i2c_init(void)
{

    /* Implement I2C bus initialization according to the target machine. */
    return 0;
}

/*!
 * @brief Function for initialization of SPI bus.
 */
int8_t bmm150_user_spi_init(void)
{

    /* Implement SPI bus initialization according to the target machine. */
    return 0;
}

/*!
 * @brief This function provides the delay for required time (Microseconds) as per the input provided in some of the
 * APIs.
 */
void bmm150_user_delay_us(uint32_t period_us, void *intf_ptr)
{
    
    /* Wait for a period amount of microseconds. */
}

/*!
 * @brief This function is for writing the sensor's registers through I2C bus.
 */
int8_t bmm150_user_i2c_reg_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{

    uint8_t tempArray[2] = {reg_addr, reg_data};

    nrf_drv_twi_tx(&m_twi, 0x10, tempArray, 2, false);

    printf("bmm150_user_i2c_reg_write\n");

    /* Write to registers using I2C. Return 0 for a successful execution. */
    return 0;
}

/*!
 * @brief This function is for reading the sensor's registers through I2C bus.
 */
int8_t bmm150_user_i2c_reg_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{  

    uint8_t register_addr[1] = {reg_addr};
    uint8_t register_value[1] = {7};

    nrf_drv_twi_tx(&m_twi, 0x10, &register_addr, 1, true);
    nrf_drv_twi_rx(&m_twi, 0x10, register_value, 1);

    printf("bmm150_user_i2c_reg_read\n");

    printf("Register Value:\t%x\n", register_value[0]);

    //Read from registers using I2C. Return 0 for a successful execution.
    return 0;
} 

/*!
 * @brief This function is for writing the sensor's registers through SPI bus.
 */
int8_t bmm150_user_spi_reg_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{

    /* Write to registers using SPI. Return 0 for a successful execution. */
    return 0;
}

/*!
 * @brief This function is for reading the sensor's registers through SPI bus.
 */
int8_t bmm150_user_spi_reg_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{

    /* Read from registers using SPI. Return 0 for a successful execution. */
    return 0;
}

/*!
 *  @brief This function is to select the interface between SPI and I2C.
 */
int8_t bmm150_interface_selection(struct bmm150_dev *dev)
{
    int8_t rslt = BMM150_OK;

    if (dev != NULL)
    {
        /* Select the interface for execution
         * For I2C : BMM150_I2C_INTF
         * For SPI : BMM150_SPI_INTF
         */
        dev->intf = BMM150_I2C_INTF;

        /* Bus configuration : I2C */
        if (dev->intf == BMM150_I2C_INTF)
        {
            printf("I2C Interface \n");

            /* To initialize the user I2C function */
            bmm150_user_i2c_init();

            dev_addr = BMM150_DEFAULT_I2C_ADDRESS;
            dev->read = bmm150_user_i2c_reg_read;
            dev->write = bmm150_user_i2c_reg_write;
        }
        /* Bus configuration : SPI */
        else if (dev->intf == BMM150_SPI_INTF)
        {
            printf("SPI Interface \n");

            /* To initialize the user SPI function */
            bmm150_user_spi_init();

            dev_addr = 0;
            dev->read = bmm150_user_spi_reg_read;
            dev->write = bmm150_user_spi_reg_write;
        }

        /* Assign device address to interface pointer */
        dev->intf_ptr = &dev_addr;

        /* Configure delay in microseconds */
        dev->delay_us = bmm150_user_delay_us;
    }
    else
    {
        rslt = BMM150_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This internal API prints the execution status
 */
void bmm150_error_codes_print_result(const char api_name[], int8_t rslt)
{
    if (rslt != BMM150_OK)
    {
        printf("%s\t", api_name);

        switch (rslt)
        {
            case BMM150_E_NULL_PTR:
                printf("Error [%d] : Null pointer error.", rslt);
                printf(
                    "It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case BMM150_E_COM_FAIL:
                printf("Error [%d] : Communication failure error.", rslt);
                printf(
                    "It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case BMM150_E_DEV_NOT_FOUND:
                printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                       rslt);
                break;

            case BMM150_E_INVALID_CONFIG:
                printf("Error [%d] : Invalid sensor configuration.", rslt);
                printf(" It occurs when there is a mismatch in the requested feature with the available one\r\n");
                break;

            default:
                printf("Error [%d] : Unknown error code\r\n", rslt);
                break;
        }
    }
}
