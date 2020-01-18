# STM32_LCD_HD44780_I2C
LCD_HD44780_I2C Library, to use in 16x2, 20x4 lines LCDs  connected to a PCF 8574T I2C expander.

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
