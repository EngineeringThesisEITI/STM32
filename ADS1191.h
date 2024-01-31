#ifndef INC_ADS1191_H_
#define INC_ADS1191_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

// PINS

#define    DEVICE1 		CS1
#define    DEVICE2 		CS2
#define	   NONE	   		0x0
#define    CLKSEL  		GPIO_PIN_2 			// GPIOA
#define    DRDY2  		GPIO_PIN_3 			// GPIOA
#define    DRDY1  		GPIO_PIN_4			// GPIOA
#define    CS1  		  GPIO_PIN_0			// GPIOB
#define    START  		GPIO_PIN_1			// GPIOB
#define    RESET  		GPIO_PIN_10			// GPIOB
#define    CS2  		  GPIO_PIN_11   	// GPIOB

// commands

//		Dummy Command						        //
#define	 SEND_DUMMY				  0x00		  //  Send dummy byte, use this while reading data from device

//   	System Commands						      //
#define  WAKEUP_DEVICE   		0x02      //  Wake-up from standby mode. NOP command in normal mode.  0000 0010 (02h)
#define  PUT_IN_STANDBY 		0x04      //  Enter standby mode  0000 0100 (04h)
#define  START_CONVERSION		0x08		  //  Start conversion 0000 1000 (08h)
#define  STOP_CONVERSION  	0x0A      //  Stop conversion 0000 1010 (0Ah)
#define	 RESET_DEVICE			  0x06		  // 	Reset the device 0000 0110 (06h)

//    	Data Read Commands					  //
#define  START_CONTINUOUS   0x10    	//  Enable Read Data Continuous mode. This mode is the default mode at power-up. *  0001 0000 (10h)
#define  STOP_CONTINUOUS   	0x11    	//  Stop Read Data Continuously mode  0001 0001 (11h)
#define  READ_DATA   			  0x12    	//  Read data by command  supports multiple read back.  0001 0010 (12h)

//   	Register Read Commands				  //
#define  REGISTER_READ    	0x20      //  Read n nnnn registers starting at address rrrr  **  001r rrrr (2xh) 000n nnnn
#define  REGISTER_WRITE    	0x40      //  Write n nnnn registers starting at address rrrr **  010r rrrr (4xh) 000n nnnn

//		Registers addresses					    //
#define	 CONFIG1				0x01
#define	 CH1SET					0x04
#define	 CH2SET					0x05

//		Reference voltage 					    //
#define  V_REF					2.42
#define  V_RES   				0.00007385

/*  Private function prototypes  */

/**
  * @brief	Simple timer (initialize timer4 first)
  * @param  Delay: Delay to wait in APB1 / Clock Prescaler
  * @retval None
  */
void Delay(uint32_t);

/**
 * @brief  Function used to start Read Data Continuous for all devices
 * @param  None
 * @retval None
 */
void Start_Continuous(void);

/**
  * @brief User Friendly Device Selecting Function
  * @param  pin: specifies the Device to select
  * @retval None
  */
void Select(uint16_t);

/**
  * @brief User Friendly Pin Setting Function
  * @param  pin: specifies the pin to be set high
  * @retval None
  */
void SET_HIGH(uint16_t);

/**
  * @brief User Friendly Pin Resetting Function
  * @param  pin: specifies the pin to be set low
  * @retval None
  */
void SET_LOW(uint16_t);

/**
  * @brief User Friendly Number Conversion Function
  * 	   Where V_REF is reference voltage, V_RES is
  * 	   voltage resolution of 16 BIT ADC
  * @param  data: 16 bit value red by ADS1191
  * @retval converted value in float
  */
float ConvertToFloat (uint16_t);

/**
  * @brief 	Power Up Sequence for ADS1191
  * @retval None
  */
void PowerUpSequence(void);

/**
  * @brief Device Initialization Function
  * @param device: Specifies device to initialize
  * @retval None
  */
void Initialize(uint16_t device);

/**
  * @brief  Sends RREG command to selected device
  * Value of each register after read is stored in registerValue array
  * @retval None
  */
void Send_RREG_Command(uint8_t*);

/**
  * @brief  Sends WREG command to selected device, the number of registers
  * 		to write must be located in first element of data array: data[0]
  * @param  address: address to first register to write
  * @param	data: pointer to data with bits to save in registers
  * @retval None
  */
void Send_WREG_Command(uint8_t, uint8_t*);

/**
  * @brief  Sends 8bit (one byte) data through SPI interface
  *			Delay after transmission is needed to meet ADS1191
  *			tSDECODE time specification between sending data
  *
  * @param  byte: data to be send through SPI
  * @retval command: data from SPI Rx buffer recently received
  */
uint8_t Send_Byte(uint8_t);

#ifdef __cplusplus
}
#endif

#endif /* INC_ADS1191_H_ */
