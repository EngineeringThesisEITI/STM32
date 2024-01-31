/* Private includes */

#include "ADS1191.h"

/* Private external variables */

extern SPI_HandleTypeDef hspi1;

/* Private variables */

/* 	value of COUNT must be equal to the number of elements in array	  */

/*					 COUNT     CONFIG1	   	  CONFIG2	      LOFF	   		CH1SET	   	   CH2SET	     RLD_SENS		*/

uint8_t device1[] = { 11,	 0b00000110,	0b10100000,    0b00010000,    0b00000000,    0b10000000,    0b00000000,		\

/*							  LOFF_SENS   	 LOFF_STAT	   	  MISC1			 MISC2			GPIO						*/

							 0b00000000,    0b00011100,    0b00000010, 	  0b00000000,    0b00000000 };

uint8_t device2[] = { 11, 0x06, 0xA0, 0x10, 0x50, 0x80, 0x00, 0x00, 0x1C, 0x02, 0x00, 0x00 };

uint8_t* commands[] = { device1, device2 };

uint8_t registerValues[2][12];	  // Buffer for data read from configuration registers

// Private function declarations

float ConvertToFloat (uint16_t data)
{
	if 		    (data == 0x7FFF)   return (float)   V_REF;
	else if   (data == 0x8000)   return (float)  -V_REF;
	else if   (data  < 0x7FFF)   return (float)  (V_RES * data);
	else  						           return (float)  (V_RES * data) - (2 * V_REF);
}

void PowerUpSequence(void)
{
	SET_LOW(RESET|CS1|CS2|START);
	HAL_Delay(1000);
	SET_HIGH(RESET);
	HAL_Delay(1000);
	GPIOA->BSRR = CLKSEL;
}

void Initialize(uint16_t device)
{
	uint8_t n;

	switch (device){

		case DEVICE1:
			n = 0;
			Select(DEVICE1);
			break;

		case DEVICE2:
			n = 1;
			Select(DEVICE2);
			break;
	}

	Send_Byte(RESET_DEVICE);					        // Transmit RESET pulse
	HAL_Delay(1000);
	Send_Byte(STOP_CONTINUOUS);					      // Stop Continuous data transfer

	Send_WREG_Command(CONFIG1, commands[n]);	// Send config commands starting from CONFIG1 register
	Send_RREG_Command(registerValues[n]);		  // Save values of config registers
}

void Start_Continuous(void)
{
	  Select(NONE);
	  SET_HIGH(START);

	  Select(DEVICE1);
	  Send_Byte(START_CONTINUOUS);

	  Select(DEVICE2);
	  Send_Byte(START_CONTINUOUS);

	  Select(NONE);
}

void Send_RREG_Command(uint8_t* buffer)
{
	Send_Byte(REGISTER_READ);	  // Write register from address 0x0
	Send_Byte(0x0C);			      // Number of registers to read = 12

	for (uint8_t i = 0; i <= 12; i++)
		buffer[i] = Send_Byte(SEND_DUMMY);
}

void Send_WREG_Command(uint8_t address, uint8_t *data)
{
	Send_Byte(REGISTER_WRITE + address);		// Write register from address
	Send_Byte(data[0] - 1);						      // Number of registers to write

	for (uint8_t i = 1; i <= data[0]; i++)
		Send_Byte(data[i]);						        // Send data to write
}

uint8_t Send_Byte(uint8_t byte)
{
	uint8_t rxBuffer;		    // Variable to store data received from ADS1191
	uint8_t command = byte;

	HAL_SPI_TransmitReceive(&hspi1, &command, &rxBuffer, 1, HAL_MAX_DELAY);
	HAL_Delay(1);

	return rxBuffer;
}

inline void SET_HIGH(uint16_t pin)
{
	GPIOB->BSRR = pin;
}

inline void SET_LOW(uint16_t pin)
{
	GPIOB->BSRR = (uint32_t)pin << 16u;
}

inline void Select(uint16_t pin)
{
	GPIOB->BSRR =  (uint32_t) (~pin&(CS1|CS2)) + (pin << 16u);
}
