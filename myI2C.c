#include "myI2C.h"

int readOnce() {
	uint8_t byte;
	uint8_t byte2;
	uint8_t byte3;
		
	TWSR = (0<<TWPS1) | (0<<TWPS0);
	TWBR = 0xFF;
		
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // start condition
	while(!(TWCR & 1<<TWINT)); // Wait for TWINT flag set. This indicates that the START condition has been transmitted
	_delay_ms(1);
	if((TWSR & 0xF8)!=0x08)
	{
		return 1;
	}
		
	TWDR = SLA_W;
	TWCR = (1<<TWINT) |(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	_delay_ms(1);
		
	if((TWSR & 0xF8)!=0x18) {
		return 1;
	}
		
	TWDR = 0b00100011;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	_delay_ms(1);
		
	if ((TWSR & 0xF8)!= 0x18) {
		return 1;
	}
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// stop condition
		
	_delay_ms(30);
		
	//read

	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // start condition
	while(!(TWCR & 1<<TWINT)); // Wait for TWINT flag set. This indicates that the START condition has been transmitted
	_delay_ms(1);
	if((TWSR & 0xF8)!=0x08)
	{
		return 1;
	}
		
	TWDR = SLA_R;
	TWCR = (1<<TWINT) |(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	_delay_ms(1);
		
	if ((TWSR & 0xF8)!= 0x18) {
		return 1;
	}
		
	byte = TWDR;
	TWCR = (0<<TWSTA)|(0<<TWSTO)|(1<<TWINT)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	_delay_ms(1);
		
	byte2 = TWDR;
	TWCR = (0<<TWSTA)|(0<<TWSTO)|(1<<TWINT)|(0<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	_delay_ms(1);
	byte3 = TWDR;
		
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// stop condition
	int level = byte;
	level <<= 8;
	level |= byte2;
	level <<= 8;
	level |= byte3;
	return level;
}
