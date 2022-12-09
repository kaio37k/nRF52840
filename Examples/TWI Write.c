const uint8_t registerTX[] = {0x4B, 0x83}; // GLOBAL

ret_code_t tx_code = nrf_drv_twi_tx(&m_twi, 0x10, &registerTX, 2, false);

printf("TX Code:\t%d\n", tx_code);
