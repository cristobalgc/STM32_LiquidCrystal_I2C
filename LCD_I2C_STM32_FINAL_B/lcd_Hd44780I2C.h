/*
 * lcd_Hd44780_I2C.h
 *
 *  Created on: 25 oct. 2019
 *      Author: CGC
 */

#ifndef LCD_HD44780I2C_H_
#define LCD_HD44780I2C_H_

#include "stm32f1xx_hal.h"

//options
//#define LCD_I2C_USE_DMA_TRANSFER
#define LCD_I2C_USE_IT_TRANSFER
//#define LCD_I2C_USE_BLOCK_TRANSFER

// Macros to manage LCD pins
#define LCD_REGBITSET(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define LCD_REGBITCLEAR(byte,nbit) ((byte) &= ~(1<<(nbit)))
#define LCD_REGBITSWAP(byte,nbit)  ((byte) ^=  (1<<(nbit)))
#define LCD_REGBITCHECK(byte,nbit) ((byte) &   (1<<(nbit)))

// commands
#define LCD_CLEARDISPLAY ((uint8_t)0x01U)
#define LCD_RETURNHOME ((uint8_t)0x02U)
#define LCD_ENTRYMODESET ((uint8_t)0x04U)
#define LCD_DISPLAYCONTROL ((uint8_t)0x08U)
#define LCD_CURSORSHIFT ((uint8_t)0x10U)
#define LCD_FUNCTIONSET ((uint8_t)0x20U)
#define LCD_SETCGRAMADDR ((uint8_t)0x40U)
#define LCD_SETDDRAMADDR ((uint8_t)0x80U)

// flags for display entry mode
#define LCD_ENTRYRIGHT ((uint8_t)0x00U)
#define LCD_ENTRYLEFT ((uint8_t)0x02U)
#define LCD_ENTRYSHIFTINCREMENT ((uint8_t)0x01U)
#define LCD_ENTRYSHIFTDECREMENT ((uint8_t)0x00U)

// flags for display on/off control
#define LCD_DISPLAYON ((uint8_t)0x04U)
#define LCD_DISPLAYOFF ((uint8_t)0x00U)
#define LCD_CURSORON ((uint8_t)0x02U)
#define LCD_CURSOROFF ((uint8_t)0x00U)
#define LCD_BLINKON ((uint8_t)0x01U)
#define LCD_BLINKOFF ((uint8_t)0x00U)

// flags for display/cursor shift
#define LCD_DISPLAYMOVE ((uint8_t)0x08U)
#define LCD_CURSORMOVE ((uint8_t)0x00U)
#define LCD_MOVERIGHT ((uint8_t)0x04U)
#define LCD_MOVELEFT ((uint8_t)0x00U)

// flags for function set
#define LCD_8BITMODE ((uint8_t)0x10U)
#define LCD_4BITMODE ((uint8_t)0x00U)
#define LCD_1LINE ((uint8_t)0x00U)
#define LCD_2LINE ((uint8_t)0x08U)
#define LCD_4LINE    LCD_2LINE
#define LCD_5x10DOTS ((uint8_t)0x04U)
#define LCD_5x8DOTS ((uint8_t)0x00U)

// Bit positions in lcd control register
#define LCD_PIN_RS ((uint8_t)0U)
#define LCD_PIN_RW ((uint8_t)1U)
#define LCD_PIN_EN ((uint8_t)2U)
#define LCD_PIN_BACKLIGHT ((uint8_t)3U)
#define LCD_PIN_P4 ((uint8_t)4U)
#define LCD_PIN_P5 ((uint8_t)5U)
#define LCD_PIN_P6 ((uint8_t)6U)
#define LCD_PIN_P7 ((uint8_t)7U)

// flags for backlight control
#define LCD_BACKLIGHT ((uint8_t)0x01U)
#define LCD_NOBACKLIGHT ((uint8_t)0x00U)

// Flags for interface type
#define LCD_INTERFACE ((uint8_t)0x01U)
#define LCD_BACKLIGHT_REG ((uint8_t)0x08U)

#define LCD_BIT_E  ((uint8_t)0x04U)  // Enable bit
#define LCD_BIT_RW  ((uint8_t)0x02U) // Read/Write bit

#define WRITEDATA ((uint8_t)0x01U) // Register select bit = 1
#define WRITECMD ((uint8_t)0x00U) // Register select bit = 0

#define LCD_MODE_4BITS  ((uint8_t)0x02U)

typedef enum lcd_error_e
{
	LCD_NOK,
	LCD_OK
}lcd_error_t;

typedef struct LCD_cfg_s
{
	I2C_HandleTypeDef  *hi2c;  // I2C Struct
	uint8_t _cols;
	uint8_t _rows;
	uint8_t _Addr;
	uint8_t _dotSize;
}LCD_cfg_t;

typedef struct LCD_data_s
{
	uint8_t _Register;
	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _backlightval;
	uint8_t _invertBigFont;
}LCD_data_t;

typedef  struct LCD_s
{
	LCD_cfg_t Config;
	LCD_data_t Data;
}LCD_t;

extern lcd_error_t LCD_init(LCD_t * lcd, const LCD_cfg_t *config);
extern void LCD_clear(LCD_t *lcd);
extern void LCD_home(LCD_t *lcd);
extern void LCD_setCursor(LCD_t *lcd, uint8_t col, uint8_t row);
extern void LCD_noDisplay(LCD_t *lcd);
extern void LCD_display(LCD_t *lcd);
extern void LCD_noCursor(LCD_t *lcd);
extern void LCD_cursor(LCD_t *lcd);
extern void LCD_noBlink(LCD_t *lcd);
extern void LCD_blink(LCD_t *lcd);
extern void LCD_scrollDisplayLeft(LCD_t *lcd);
extern void LCD_scrollDisplayRight(LCD_t *lcd);
extern void LCD_leftToRight(LCD_t *lcd);
extern void LCD_rightToLeft(LCD_t *lcd);
extern void LCD_autoscroll(LCD_t *lcd);
extern void LCD_noAutoscroll(LCD_t *lcd);
extern lcd_error_t LCD_setBacklight(LCD_t *lcd, uint8_t backlight);
extern void LCD_createChar(LCD_t *lcd, uint8_t location, const char *charmap);
extern void LCD_printStr(LCD_t *lcd, char* data);
extern void LCD_print(LCD_t * lcd, char* format,...);
extern void LCD_write(LCD_t *lcd, uint8_t data);

#endif /* LCD_HD44780I2C_H_ */
