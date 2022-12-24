//===========================
//         Main.c
//===========================

uint8_t putReadDataHere = 8; // Number is equal to 8 as to make it obvious if it is NOT being set correctly

bmm150_user_i2c_reg_read(0x4b, putReadDataHere, 1, 0x10);

printf("putReadDataHere:\t%x\n", putReadDataHere);

//===========================
//         Common.c
//===========================

int8_t bmm150_user_i2c_reg_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    ret_code_t error_report_read1;
    ret_code_t error_report_read2;

    uint8_t register_addr[] = {reg_addr};
    uint8_t register_data[] = {reg_data};

    error_report_read1 = nrf_drv_twi_tx(&m_twi, 0x10, &register_addr, 1, true);
    error_report_read2 = nrf_drv_twi_rx(&m_twi, 0x10, register_data, 1);

    reg_data = register_data[0];

    printf("i2c_reg_read reg_addr:\t\t%x\n", register_addr[0]);
    printf("i2c_reg_read reg_data:\t\t%x\n", register_data[0]);

    printf("Error Report 1:\t%x\n", error_report_read1);
    printf("Error Report 2:\t%x\n", error_report_read2);

    printf("----------------------------\n");
    printf("*reg_data:\t%x\n", reg_data);
    printf("----------------------------\n");


    //Read from registers using I2C. Return 0 for a successful execution.
    return 0;
} 
