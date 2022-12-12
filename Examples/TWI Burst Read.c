    uint8_t pointerArray[] = {0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49}; // These are the pointer values, they are the addresses to read in the burst
    uint16_t registerValue[8];                                                 // This is where the register values are stored. Should be equal to the number...
                                                                               // ...of pointers in the pointer array.
    for(int i = 0; i < sizeof(XYZRegisters); i++){
      nrf_drv_twi_tx(&m_twi, 0x10, &XYZRegisters, 1, true);
      nrf_drv_twi_rx(&m_twi, 0x10, registerValue, 8);

      printf("-----------------\n");
      printf("register:\t%x\n", XYZRegisters[i]);
      printf("registerValue:\t%d\n", registerValue[i]);
