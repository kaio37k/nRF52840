uint8_t registerRX[] = {7}; // GLOBAL - Set to 7 (not 0) so it is easy to pick up if it's not being changed

const uint8_t registerTX[] = {0x4D}; // Pointer Byte

ret_code_t tx_code = nrf_drv_twi_tx(&m_twi, 0x10, &registerTX, 1, true); // Setting Pointer

ret_code_t rx_code = nrf_drv_twi_rx(&m_twi, 0x10, registerRX, 1);

printf("TX Code:\t%d\n", tx_code);
printf("RX Code:\t%d\n", rx_code);

printf(registerRX[0]);
