# STM32_LCD_HD44780_I2C
LCD_HD44780_I2C Library, to use in 16x2, 20x4 lines LCDs  connected to a PCF 8574T I2C expander.

### Referenced links
- This library was based on the datasheet documentation and the following references:
	- https://liudr.wordpress.com/libraries/phi_big_font/
	- https://github.com/johnrickman/LiquidCrystal_I2C

### Steps to use the library
- Connect your LCD using the PCF8574 i2c expander to your mcu, i used the blue pill.
	- https://wiki.liutyi.info/display/ARDUINO/LCD+2004A+i2c+PCF8574T
	- https://wiki.relativty.net/index.php/STM32
- Configure your MCU to run with i2c, maybe you have configured another peripheral, and you need to load another i2c configuration (hi2c1, hi2c2...).If you need to use DMA or any other mcu functionality to use with I2C, you need to modify this library to use another HAL function of the STM32 library. You only need to change the following function by the other one you need.


	HAL_I2C_Master_Transmit (lcd->Config.hi2c, lcd->Config._Addr, (uint8_t *) &lcd->Data.Register, sizeof(LCD_reg_t), LCD_IICTIMEOUT);

- Download the library and include it in your source code project.
- Try to inlude this example in your own main.c file to test it.
- You can configure up to 7 I2C dysplays connected to the same i2c physical port with this library.

### Sample of code

	#include <bigFont_lcdI2c.h>
	#include <lcd_Hd44780I2C.h>

	#define LCD_ROWS		(4U)
	#define LCD_COLS		(20U)
	#define LCD_I2C_ADDRESS	(0x4E)
	
	/* Number to print in the LCD */
	static uint32_t encoderPos = 0; 
	/* Lcd object */
	static LCD_t Lcd_2;
	/*Lcd configuration */
	static const LCD_cfg_t Lcd_Hd44780I2cCfg =
	{
		&hi2c1,   /* Depending on your active I2C configuration */
		LCD_COLS, /* Lcd columns */
		LCD_ROWS, /* Lcd rows */
		LCD_I2C_ADDRESS, /* Lcd address. Depending on your PCF8574T expander board it could change. */
		LCD_5x8DOTS /* Pending to remove, here you can set 0*/
	};

	int main(void)
	{
 		LCD_setBacklightOn(&Lcd_2);/* Turn on the Backlight */
 		LCD_init(&Lcd_2, &Lcd_Hd44780I2cCfg); /* Initialize the LCD to print "normal characters"*/
  		BIGFONT_init(&Lcd_2); /*Initialize Big font characters */
  
 		BIGFONT_printNumber(&Lcd_2,encoderPos,0,0);
  		BIGFONT_printChar(&Lcd_2, '!', 8 ,0);
  		LCD_setCursor(&Lcd_2, 0, 3);
  		LCD_print(&Lcd_2,"Numero: %d",encoderPos); 
	}

### End
