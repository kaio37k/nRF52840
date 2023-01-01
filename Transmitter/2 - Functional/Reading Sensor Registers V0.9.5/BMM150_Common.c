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
#include "nrf_delay.h"

/* TWI instance ID. */
#define TWI_INSTANCE_ID     0

/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

//void twi_init (void)
//{
//    ret_code_t err_code;

//    const nrf_drv_twi_config_t twi_lm75b_config = {
//       .scl                = NRF_GPIO_PIN_MAP(0, 27),
//       .sda                = NRF_GPIO_PIN_MAP(0, 26),
//       .frequency          = NRF_DRV_TWI_FREQ_100K,
//       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
//       .clear_bus_init     = false
//    };

//    err_code = nrf_drv_twi_init(&m_twi, &twi_lm75b_config, NULL, NULL);
//    //printf("Error Code init\t%d\n", err_code);

//    nrf_drv_twi_enable(&m_twi);
//}

/******************************************************************************/
/*!                Static variable definition                                 */

/*! Variable that holds the I2C device address or SPI chip selection */
static uint8_t dev_addr;

/******************************************************************************/
/*!                User interface functions                                   */

uint8_t readAllRegisters(){
  
  uint8_t allRegistersAddressArray[6] = {0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F};

  uint8_t allRegisterDataArray[6];

  int sizeOfARDA = sizeof(allRegisterDataArray) / sizeof(allRegisterDataArray[0]);

  nrf_drv_twi_tx(&m_twi, 0x10, allRegistersAddressArray, 1, false);
  nrf_drv_twi_rx(&m_twi, 0x10, allRegisterDataArray, 1);

  for(int i = 0; i < sizeOfARDA; i++){
    printf("readAllRegister %x:\t%x\n", allRegistersAddressArray[i], allRegisterDataArray[i]);
  }

  return 0;
}

void writeOneRegister(uint8_t writeThisRegister, uint8_t writeThisValue){
  
  uint8_t tempArray[2] = {writeThisRegister, writeThisValue};

  nrf_drv_twi_tx(&m_twi, 0x10, &tempArray, 2, false);
}

void readOneRegister(uint8_t readThisRegister){

  uint8_t register_address[] = {readThisRegister};
  
  uint8_t register_value[1];

  nrf_drv_twi_tx(&m_twi, 0x10, &register_address, 1, true);
  nrf_drv_twi_rx(&m_twi, 0x10, register_value, 1);

  printf("readOneRegister reg_addr:\t%x\n", readThisRegister);
  printf("readOneRegister reg_data:\t%x\n", register_value[0]);
}

/*!
 * @brief Function for initialization of I2C bus.
 */
int8_t bmm150_user_i2c_init(void)
{
    const nrf_drv_twi_config_t twi_lm75b_config = {
       .scl                = 27,//NRF_GPIO_PIN_MAP(0, 27),
       .sda                = 26,//NRF_GPIO_PIN_MAP(0, 26),
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    nrf_drv_twi_init(&m_twi, &twi_lm75b_config, NULL, NULL);
    //printf("Error Code init\t%d\n", err_code);

    nrf_drv_twi_enable(&m_twi);

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
    
    nrf_delay_ms(300);
    /* Wait for a period amount of microseconds. */
}

/*!
 * @brief This function is for writing the sensor's registers through I2C bus.
 */
int8_t bmm150_user_i2c_reg_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    ret_code_t error_report;
    uint8_t tx_buf[2];

    tx_buf[0] = reg_addr;
    tx_buf[1] = reg_data[0];

    printf("======= [ I2C REG WRITE START ] =======\n");

    printf("reg_data[0]:\t%x\n", reg_data[0]);

    printf("user_i2c_reg_write tx_buf 0:\t%x\n", tx_buf[0]);
    printf("user_i2c_reg_write tx_buf 1:\t%x\n", tx_buf[1]);

    error_report = nrf_drv_twi_tx(&m_twi, 0x10, tx_buf, length+1, false);

    if(error_report!=0){
      printf("Error Report:\t%x\n", error_report);
    }

    printf("======= [ I2C REG WRITE END ] =======\n");

    /* Write to registers using I2C. Return 0 for a successful execution. */
    return 0;
}

/*!
 * @brief This function is for reading the sensor's registers through I2C bus.
 */

int8_t bmm150_user_i2c_reg_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{

    printf("======= [ I2C REG READ START ] =======\n");

    ret_code_t error_report_read1;
    ret_code_t error_report_read2;

    uint8_t register_addr = reg_addr;
    uint8_t register_value[20];

    error_report_read1 = nrf_drv_twi_tx(&m_twi, 0x10, &reg_addr, 1, false);
    error_report_read2 = nrf_drv_twi_rx(&m_twi, 0x10, reg_data, length); // HAS TO BE ARRAY TYPE

    printf("Size of read array:\t\t%d\n", length);
    printf("Register Address:\t\t%x\n", reg_addr);
    printf("Register Data 0:\t\t%x\n", reg_data[0]);

    //reg_data[0] = register_value[0];
    //reg_data[1] = register_value[1];
    //reg_data[2] = register_value[2];
    //reg_data[3] = register_value[3];
    //reg_data[4] = register_value[4];
    //reg_data[5] = register_value[5];
    //reg_data[6] = register_value[6];
    //reg_data[7] = register_value[7];
    //reg_data[8] = register_value[8];
    //reg_data[9] = register_value[9];
    //reg_data[10] = register_value[10];
    //reg_data[11] = register_value[11];

    if(error_report_read1!=0||error_report_read2!=0){
      printf("Error Report 1:\t%x\n", error_report_read1);
      printf("Error Report 2:\t%x\n", error_report_read2);
    }

    printf("======= [ I2C REG READ END ] =======\n");


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
            int p;
            /* To initialize the user I2C function */
            p = bmm150_user_i2c_init();
            printf("i2c init:\t%x\n", p);

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
