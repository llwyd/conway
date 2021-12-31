#ifndef _I2C_H_
#define _I2C_H_

void I2C_Init( void );
void I2C_Write( unsigned char address, unsigned char * data, unsigned char len );
void I2C_TMP102( void );

#endif /* _I2C_H_ */
