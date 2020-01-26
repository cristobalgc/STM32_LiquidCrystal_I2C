/******************************************************************************/
/*   All rights reserved. Distribution or duplication without previous        */
/*   written agreement of the owner prohibited.                               */
/*                                                                            */
/******************************************************************************/

/** \file lcd_Hd44780_I2c.c
 *
 * \brief Source code file for lcd_Hd44780_I2c
 *
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> Author:   </td> <td>  CGC   </td></tr>
 * <tr> <td> Date:     </td> <td> 25 oct. 2019           </td></tr>
 * </table>
 * \n
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> COMPONENT: </td> <td> lcd_Hd44780_I2c   </td></tr>
 * <tr> <td> TARGET:    </td> <td> STM32F103C8T6    </td></tr>
 * </table>
 */

/******************************************************************************/
/*                Include common and project definition header                */
/******************************************************************************/

/******************************************************************************/
/*                      Include headers of the component                      */
/******************************************************************************/
#include <lcd_Hd44780I2C.h>
#include<stdio.h>
#include<stdarg.h>
#include<string.h>
/******************************************************************************/
/*                            Include other headers                           */
/******************************************************************************/

/******************************************************************************/
/*                   Definition of local symbolic constants                   */
/******************************************************************************/

#define LCD_DELAY2 (2u)//2ms
#define LCD_DELAY1 (1u)//1ms
#define LCD_DELAY50 (50u)//50ms
#define LCD_DELAY500 (500u)//500ms
#define LCD_DELAY5 (5u)//5ms
#define LCD_IICTIMEOUT (100u)
#define LCD_4BYTESSIZE (4u)
#define LCD_2BYTESSIZE (2u)
#define LCD_1BYTESIZE (1u)
/******************************************************************************/
/*                  Definition of local function like macros                  */
/******************************************************************************/

/******************************************************************************/
/*          Definition of local types (typedef, enum, struct, union)          */
/******************************************************************************/

/******************************************************************************/
/*                       Definition of local variables                        */
/******************************************************************************/

/******************************************************************************/
/*                     Definition of local constant data                      */
/******************************************************************************/

/******************************************************************************/
/*                      Definition of exported variables                      */
/******************************************************************************/

/******************************************************************************/
/*                    Definition of exported constant data                    */
/******************************************************************************/

/******************************************************************************/
/*                  Declaration of local function prototypes                  */
/******************************************************************************/

static char* lcd_convert(unsigned int, int);       //Convert integer number into octal, hex, etc.
static uint8_t lcd_pcf8574Write(LCD_t *lcd, uint8_t *data);
static uint8_t lcd_pcf8574Send(LCD_t *lcd, char data, uint8_t rsbit);

/******************************************************************************/
/*                       Definition of local functions                        */
/******************************************************************************/
/*
 * @brief Write directly on the iic expander
 * @param[in]: uint8_t *data - The data to write
 */
static uint8_t lcd_pcf8574Write(LCD_t *lcd, uint8_t *data)
{
	uint8_t ret = 1u;
	/* Choose your preferences if you want to use DMA or not in .h file */
#ifdef LCD_I2C_USE_IT_TRANSFER
	if(HAL_I2C_Master_Transmit_IT(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t *)data, LCD_1BYTESIZE) != HAL_OK)
	{
		ret = 0u;
	}
#endif
#ifdef LCD_I2C_USE_BLOCK_TRANSFER
	if(HAL_I2C_Master_Transmit(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t *)data, LCD_1BYTESIZE, LCD_IICTIMEOUT) != HAL_OK)
	{
		ret = 0u;
	}
#endif
#ifdef LCD_I2C_USE_DMA_TRANSFER
	if(HAL_I2C_Master_Transmit_DMA(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t *)data, LCD_1BYTESIZE)!= HAL_OK)
	{
		ret = 0u;
	}
#endif
	return ret;
}

/*
 * @brief Send a command or (write/read) to the LCD in 4 bit mode.
 * @note  To send a command rsbit parameter must be equal to "0".
 * 		  To write in LCD rsbit parameter must be equal to "1".
 *
 * @param[in] LCD_t lcd - Lcd object
 * @param[in] char data - Command or data to send to the LCD.
 * @param[in] uint8_t rsbit - Status of the rsbit to (write/read/send a command) to the LCD.
 *
 *   High nibble   |   Low nibble
 * DB7 DB6 DB5 DB4 | DB3 DB2 DB1 DB0
 *
 * */
static uint8_t lcd_pcf8574Send(LCD_t *lcd, char data, uint8_t rsbit)
{
	uint8_t ret = 1u ;
	uint8_t data_t[4];
	uint8_t lownibble;
	uint8_t highnibble;
	highnibble = (data & 0xf0)>>4u;
	lownibble = (data & 0x0f);
	lcd->Data.Register.B.BCKLGHT = lcd->Data._backlightval;
	lcd->Data.Register.B.RS = rsbit; /* read/write */
	lcd->Data.Register.B.EN = 1u;
	/* Send High nibble values with EN=0 and EN=1*/
	lcd->Data.Register.B.Dat = highnibble;
	data_t[0] = lcd->Data.Register.R;
	lcd->Data.Register.B.EN = 0u;
	data_t[1] = lcd->Data.Register.R;
	lcd->Data.Register.B.EN = 1u;
    /* Send Low nibble values with EN=0 and EN=1*/
	lcd->Data.Register.B.Dat = lownibble;
	data_t[2] = lcd->Data.Register.R;
	lcd->Data.Register.B.EN = 0u;
	data_t[3] = lcd->Data.Register.R;

	/* Choose your preferences if you want to use DMA or not in .h file */
#ifdef LCD_I2C_USE_IT_TRANSFER
	if(HAL_I2C_Master_Transmit_IT(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t *)data_t, LCD_4BYTESSIZE) != HAL_OK)
	{
		ret = 0u;
	}
#endif
#ifdef LCD_I2C_USE_BLOCK_TRANSFER
	if(HAL_I2C_Master_Transmit(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t *)data_t, LCD_4BYTESSIZE, LCD_IICTIMEOUT) != HAL_OK)
	{
		ret = 0u;
	}
#endif
#ifdef LCD_I2C_USE_DMA_TRANSFER
	if(HAL_I2C_Master_Transmit_DMA(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t *)data_t, LCD_4BYTESSIZE)!=HAL_OK)
	{
		ret = 0u;
	}
#endif
	while (HAL_I2C_GetState(lcd->Config.hi2c) != HAL_I2C_STATE_READY)
	{
		__NOP();
    }
	return ret;
}

/**
 * @brief  Convert integer numbers in to a string to be represented in the LCD.
 * @note
 * @param[in]  num    	The number to convert
 * @param[in]  base   	The base to convert the number.
 * 					use 10 as base parameter to convert the number in to a decimal value,
 * 					16 to convert it in hexadecimal value and 8 to convert the number in octal.
 * @return  none
 */
static char *lcd_convert(unsigned int num, int base)
{
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = Representation[num%base];
        num /= base;
    }while(num != 0);

    return(ptr);
}

/******************************************************************************/
/*                      Definition of exported functions                      */
/******************************************************************************/

/********** high level commands, for the user! */
/*
 * @brief Initilalize the variables for the LCD.
 *
 * @param[in]: LCD_t *lcd - The lcd to set the config parameters
 * @param[in]: LCD_cfg_t *config - The configuration of the LCD
 */
uint8_t LCD_init(LCD_t * lcd, const LCD_cfg_t *config)
{
	uint8_t i;
	uint8_t ret = 1u;
	uint8_t initcmdbuffer[LCD_2BYTESSIZE] = {0u};
	memcpy(&lcd->Config, config, sizeof(LCD_cfg_t));
	lcd->Data.Register.R = 0u;
	lcd->Data._backlightval = 0u;
	lcd->Data._displaycontrol = 0u;
	lcd->Data._displaymode = 0u;
	lcd->Data._invertBigFont = 0u;

	lcd->Data._displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (lcd->Config._rows > 1u)
	{
		lcd->Data._displayfunction = LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((lcd->Config._dotSize != 0u) && (lcd->Config._rows == 1u))
	{
		lcd->Data._displayfunction |= LCD_5x10DOTS;
	}
	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	HAL_Delay(LCD_DELAY50);

	// Now we pull both RS and R/W low to begin commands
	lcd_pcf8574Write(lcd, &lcd->Data._backlightval);	// reset expanderand turn backlight off (Bit 8 =1)

	HAL_Delay(LCD_DELAY500);

  	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	initcmdbuffer[0] = LCD_BIT_E | (0x03 << 4);//0x30 = (0x03 <<4)
	initcmdbuffer[1] = (0x03 << 4);

	for(i = 0u; i<3; i++) //sending 3 times: select 4-bit mode
	{
#ifdef LCD_I2C_USE_IT_TRANSFER
		if (HAL_I2C_Master_Transmit_IT(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t*)initcmdbuffer, LCD_2BYTESSIZE)!= HAL_OK)
		{
			ret = 0u;
			break;
		}
#endif
#ifdef LCD_I2C_USE_BLOCK_TRANSFER
		if(HAL_I2C_Master_Transmit(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t*)initcmdbuffer, LCD_2BYTESSIZE, LCD_IICTIMEOUT) != HAL_OK)
		{
			ret = 0u;
			break;
		}
#endif
#ifdef LCD_I2C_USE_DMA_TRANSFER
		if (HAL_I2C_Master_Transmit_DMA(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t*)initcmdbuffer, LCD_2BYTESSIZE)!= HAL_OK)
		{
			ret = 0u;
			break;
		}
#endif
        while (HAL_I2C_GetState(lcd->Config.hi2c) != HAL_I2C_STATE_READY)
        {
        	__NOP();
        }
		if (i == 2)
		{
			// For the last cycle delay is less then 1 ms (100us by datasheet)
			HAL_Delay(LCD_DELAY1);
		}
		else
		{
			// For first 2 cycles delay is less then 5ms ( 4.1ms by datasheet)
			HAL_Delay(LCD_DELAY5);
		}
	}

    /* finally, set to 4-bit interface */
	initcmdbuffer[0] = LCD_BIT_E | (LCD_MODE_4BITS << 4);
	initcmdbuffer[1] = (LCD_MODE_4BITS << 4);
#ifdef LCD_I2C_USE_IT_TRANSFER
	if (HAL_I2C_Master_Transmit_IT(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t*)initcmdbuffer, LCD_2BYTESSIZE) != HAL_OK)
	{
		ret = 0u;
	}
#endif
#ifdef LCD_I2C_USE_BLOCK_TRANSFER
	if(HAL_I2C_Master_Transmit(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t*)initcmdbuffer, LCD_2BYTESSIZE, LCD_IICTIMEOUT) != HAL_OK)
	{
		ret = 0u;
	}
#endif
#ifdef LCD_I2C_USE_DMA_TRANSFER
	if (HAL_I2C_Master_Transmit_DMA(lcd->Config.hi2c, lcd->Config._Addr, (uint8_t*)initcmdbuffer, LCD_2BYTESSIZE) != HAL_OK)
	{
		ret = 0u;
	}
#endif
    while (HAL_I2C_GetState(lcd->Config.hi2c) != HAL_I2C_STATE_READY)
    {
    	__NOP();
    }

	// set # lines, font size, etc.
	ret = lcd_pcf8574Send(lcd, LCD_FUNCTIONSET | lcd->Data._displayfunction, WRITECMD);

	// turn the display on with no cursor or blinking default
	lcd->Data._displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	LCD_display(lcd);

	// clear it off
	LCD_clear(lcd);

	// Initialize to default text direction (for roman languages)
	lcd->Data._displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	ret = lcd_pcf8574Send(lcd, LCD_ENTRYMODESET | lcd->Data._displaymode, WRITECMD);

	LCD_home(lcd);

	return ret;
}

/*
 * @brief Print the a string in the LCD.
 *
 * @param[in]: LCD_t *lcd - The lcd object to use.
 * @param[in]: char* data - The data string to be printed on the LCD.
 */
void LCD_printStr(LCD_t *lcd, char* data)
{
	while(*data)
	{
		lcd_pcf8574Send(lcd, *data++, WRITEDATA);
	}
}

void LCD_write(LCD_t *lcd, uint8_t data)
{
		lcd_pcf8574Send(lcd, data, WRITEDATA);
}

/*
 * @brief Print function to be used like printf.
 *
 * @param[in]: LCD_t *lcd - The lcd object to use.
 * @param[in]: char* format - The data string to be printed on the LCD.
 */
void LCD_print(LCD_t * lcd, char* format,...)
{
	char *traverse;
	int32_t i;
	char *s;
	//Module 1: Initializing Myprintf's arguments
	va_list arg;
	va_start(arg, format);

	for(traverse = format; *traverse != '\0'; traverse++)
	{

		while( *traverse != '%' && *traverse != '\0')
		{
			lcd_pcf8574Send(lcd, *traverse, WRITEDATA);
			traverse++;
		}

			traverse++;

			//Module 2: Fetching and executing arguments
			switch(*traverse)
			{
			case 'c' :
				i = va_arg(arg,int);		//Fetch char argument
				lcd_pcf8574Send(lcd,i,WRITEDATA);
				break;

			case 'd' :
				i = va_arg(arg,int);         //Fetch Decimal/Integer argument
				if(i<0)
				{
					i = -i;
					lcd_pcf8574Send(lcd, 45, WRITEDATA);	// 45 is "-" symbol
				}
				LCD_printStr(lcd,lcd_convert(i,10));
				break;

			case 'o':
				i = va_arg(arg,unsigned int); //Fetch Octal representation
				LCD_printStr(lcd,lcd_convert(i,8));
				break;

			case 's':
				s = va_arg(arg,char *);       //Fetch string
				LCD_printStr(lcd,s);
				break;

			case 'x':
				i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
				LCD_printStr(lcd,lcd_convert(i,16));
				break;
			default:
				break;
			}
		}

	//Module 3: Closing argument list to necessary clean-up
	va_end(arg);
}

void LCD_clear(LCD_t *lcd)
{
	lcd_pcf8574Send(lcd, LCD_CLEARDISPLAY, WRITECMD);// clear display, set cursor position to zero
	HAL_Delay(LCD_DELAY2);  // this command takes a long time!
}

void LCD_home(LCD_t *lcd)
{
	lcd_pcf8574Send(lcd, LCD_RETURNHOME, WRITECMD);  // set cursor position to zero
	HAL_Delay(LCD_DELAY2);  // this command takes a long time!
}

void LCD_setCursor(LCD_t *lcd, uint8_t col, uint8_t row)
{
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > lcd->Config._rows )
	{
		row = lcd->Config._rows - 1u;    // we count rows starting w/0
	}
	lcd_pcf8574Send(lcd, LCD_SETDDRAMADDR | (col + row_offsets[row]), WRITECMD);
}

// Turn the display on/off (quickly)
void LCD_noDisplay(LCD_t *lcd)
{
	lcd->Data._displaymode &= ~LCD_DISPLAYON;
	lcd_pcf8574Send(lcd,LCD_DISPLAYCONTROL | lcd->Data._displaymode, WRITECMD);
}
void LCD_display(LCD_t *lcd)
{
	lcd->Data._displaymode |= LCD_DISPLAYON;
	lcd_pcf8574Send(lcd,LCD_DISPLAYCONTROL | lcd->Data._displaymode, WRITECMD);
}

// Turns the underline cursor on/off
void LCD_noCursor(LCD_t *lcd)
{
	lcd->Data._displaymode &= ~LCD_CURSORON;
	lcd_pcf8574Send(lcd,LCD_DISPLAYCONTROL | lcd->Data._displaymode, WRITECMD);
}
void LCD_cursor(LCD_t *lcd)
{
	lcd->Data._displaymode |= LCD_CURSORON;
	lcd_pcf8574Send(lcd, LCD_DISPLAYCONTROL | lcd->Data._displaymode, WRITECMD);
}

// Turn on and off the blinking cursor
void LCD_noBlink(LCD_t *lcd)
{
	lcd->Data._displaymode &= ~LCD_BLINKON;
	lcd_pcf8574Send(lcd, LCD_DISPLAYCONTROL | lcd->Data._displaymode, WRITECMD);
}
void LCD_blink(LCD_t *lcd)
{
	lcd->Data._displaymode |= LCD_BLINKON;
	lcd_pcf8574Send(lcd, LCD_DISPLAYCONTROL | lcd->Data._displaymode, WRITECMD);
}

// These commands scroll the display without changing the RAM
void LCD_scrollDisplayLeft(LCD_t *lcd)
{
	lcd_pcf8574Send(lcd,LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, WRITECMD);
}
void LCD_scrollDisplayRight(LCD_t *lcd)
{
	lcd_pcf8574Send(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, WRITECMD);
}

// This is for text that flows Left to Right
void LCD_leftToRight(LCD_t *lcd)
{
	lcd->Data._displaymode |= LCD_ENTRYLEFT;
	lcd_pcf8574Send(lcd, LCD_ENTRYMODESET | lcd->Data._displaymode, WRITECMD);
}

// This is for text that flows Right to Left
void LCD_rightToLeft(LCD_t *lcd)
{
	lcd->Data._displaymode &= ~LCD_ENTRYLEFT;
	lcd_pcf8574Send(lcd, LCD_ENTRYMODESET | lcd->Data._displaymode, WRITECMD);
}

// This will 'right justify' text from the cursor
void LCD_autoscroll(LCD_t *lcd)
{
	lcd->Data._displaymode |= LCD_ENTRYSHIFTINCREMENT;
	lcd_pcf8574Send(lcd, LCD_ENTRYMODESET | lcd->Data._displaymode, WRITECMD);
}

// This will 'left justify' text from the cursor
void LCD_noAutoscroll(LCD_t *lcd)
{
	lcd->Data._displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	lcd_pcf8574Send(lcd, LCD_ENTRYMODESET | lcd->Data._displaymode, WRITECMD);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters///(maybe it does not work)
void LCD_createChar(LCD_t *lcd, uint8_t location, uint8_t charmap[])
{
	int i;
	location &= 0x07u; // we only have 8 locations 0-7
	lcd_pcf8574Send(lcd, LCD_SETCGRAMADDR | (location << 3u), WRITECMD);
	for (i = 0u; i<8u; i++)
	{
		lcd_pcf8574Send(lcd, charmap[i], WRITEDATA);
	}
}

/*
 * @brief Set the Backlight
 * @param[in]: LCD_t *lcd - The lcd to set the backlight
 * @param[in]: uint8_t backlight - 1 turn ON- 0 turn OFF
 * @return 1 if OK - 0 if not ok
 */
uint8_t LCD_setBacklight(LCD_t *lcd, uint8_t backlight)
{
	uint8_t ret;

	if(backlight <= 1u)
	{
		lcd->Data._backlightval = backlight;
		lcd->Data.Register.B.BCKLGHT = lcd->Data._backlightval;
		ret = lcd_pcf8574Write(lcd, &lcd->Data.Register.R);
	}
	else
	{
		ret = 0u;
	}

	return ret;
}
