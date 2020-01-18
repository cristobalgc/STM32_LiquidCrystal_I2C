# STM32_LCD_HD44780_I2C
LCD_HD44780_I2C Library, to use in 16x2, 20x4 lines LCDs  connected to a PCF 8574T I2C expander.

### Steps to use the library
- Connect your LCD using the PCF8574 i2c expander to your mcu, i used the blue pill.
	-https://wiki.liutyi.info/display/ARDUINO/LCD+2004A+i2c+PCF8574T
	-https://wiki.relativty.net/index.php/STM32
- Configure your MCU to run with i2c, maybe you have configured another peripheral, and you need to load another i2c configuration (hi2c1, hi2c2...).If you need to use DMA or any other mcu functionality to use with I2C, you need to modify this library to use another HAL function of the STM32 library. You only need to change the following function by the other one you need.


	HAL_I2C_Master_Transmit (lcd->Config.hi2c, lcd->Config._Addr, (uint8_t *) &lcd->Data.Register, sizeof(LCD_reg_t), LCD_IICTIMEOUT);

- Download the library and include it in your source code project.
- Try to inlude this example in your own main.c file to test it.

###Sample of code

	#include <bigFont_lcdI2c.h>
	#include <lcd_Hd44780I2C.h>

	#define LCD_ROWS		(4U)
	#define LCD_COLS		(20U)
	#define LCD_I2C_ADDRESS	(0x4E)

	static uint32_t encoderPos = 0; // a counter for the dial
	static LCD_t Lcd_2;
	static const LCD_cfg_t Lcd_Hd44780I2cCfg =
	{
		&hi2c1,       /* Depending on your configuration */
		LCD_COLS,
		LCD_ROWS,
		LCD_I2C_ADDRESS,
		LCD_5x8DOTS
	};

	int main(void)
	{
 		 LCD_setBacklightOn(&Lcd_2);
 		 LCD_init(&Lcd_2, &Lcd_Hd44780I2cCfg);
  		BIGFONT_init(&Lcd_2);
  
 		 BIGFONT_printNumber(&Lcd_2,encoderPos,0,0);
  		BIGFONT_printChar(&Lcd_2, '!', 8 ,0);
  		LCD_setCursor(&Lcd_2, 0, 3);
  		LCD_print(&Lcd_2,"Numero: %d",encoderPos); 
	}

###End
