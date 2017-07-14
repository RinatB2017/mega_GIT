
void I2C_Configuration(void);

void I2C_ByteWrite(I2C_TypeDef* I2Cx, uint16_t WriteAddr, uint8_t val);
uint8_t I2C_ByteRead(I2C_TypeDef* I2Cx, uint16_t ReadAddr);

