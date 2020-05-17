/******************************************************************************/
/*   All rights reserved. Distribution or duplication without previous        */
/*   written agreement of the owner prohibited.                               */
/*                                                                            */
/******************************************************************************/

/** \file bigFont_lcdI2c.c
 *
 * \brief Source code file for bigFont_lcdI2c.c
 *
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> Author:   </td> <td>  CGC   </td></tr>
 * <tr> <td> Date:     </td> <td> 25 oct. 2019           </td></tr>
 * </table>
 * \n
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> COMPONENT: </td> <td> bigFont_lcdI2c.c   </td></tr>
 * <tr> <td> TARGET:    </td> <td> STM32F103C8T6    </td></tr>
 * </table>
 */

/******************************************************************************/
/*                Include common and project definition header                */
/******************************************************************************/

/******************************************************************************/
/*                      Include headers of the component                      */
/******************************************************************************/
#include <bigFont_lcdI2c.h>
#include <lcd_Hd44780I2C.h>
#include <stdio.h>
#include <string.h>
/******************************************************************************/
/*                            Include other headers                           */
/******************************************************************************/

/******************************************************************************/
/*                   Definition of local symbolic constants                   */
/******************************************************************************/

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
static const char pbf_lcd_ch0[]={64,64,64,64,64,64,64,64,0};// 0, Empty
static const char pbf_lcd_ch1[]={64,64,64,64,64,31,31,31,0};//1, Lower 3 lines 220
static const char pbf_lcd_ch2[]={64,64,64,31,31,64,64,64,0};//2, middle 2 lines
static const char pbf_lcd_ch3[]={64,64,64,31,31,31,31,31,0};//3, lower 5 lines
static const char pbf_lcd_ch4[]={31,31,31,64,64,64,64,64,0};//4, upper 3 lines 223
static const char pbf_lcd_ch5[]={31,31,31,64,64,31,31,31,0};//5, upper 3 lines + lower 3 lines
static const char pbf_lcd_ch6[]={31,31,31,31,31,64,64,0};//6, upper 5 lines
static const char pbf_lcd_ch7[]={31,31,31,31,31,31,31,31,0};//7, all 7 lines 219

static const char * const pbf_ch_item[] = {pbf_lcd_ch0, pbf_lcd_ch1, pbf_lcd_ch2, pbf_lcd_ch3, pbf_lcd_ch4, pbf_lcd_ch5, pbf_lcd_ch6, pbf_lcd_ch7};
/* Todo complete the Symbols and updata the documantation */
static const uint8_t font_35_sym1[]={ 7,0,0,1,0,0, 7,7,0,0,0,0, 2,6,0,0,3,0, 4,6,0,0,3,6, 9,6,4,0,3,0, 0,6,0,0,3,0, 0,6,0,0,3,0, 0,6,0,0,3,0};

static const uint8_t font_35_sym2[]={ 0,1,0,0,4,0, 7,7,0,0,0,0, 2,6,0,0,3,0, 4,6,0,0,3,6, 9,6,4,0,3,0, 0,6,0,0,3,0, 0,6,0,0,3,0, 0,6,0,0,3,0};

static const uint8_t font_35_09[]={1,2,3,7,1,6, 1,3,0,0,7,0, 2,2,3,7,5,5, 2,2,3,5,5,7, 3,0,3,4,4,7, 3,2,2,5,5,7, 3,2,2,7,5,7, 2,2,3,0,0,7, 3,2,3,7,5,7, 3,2,3,5,5,7};

static const uint8_t font_35_AZ[]={3,2,3,7,4,7, 3,2,3,7,5,3, 3,2,2,7,1,1, 3,2,1,7,1,6, 3,2,2,7,5,5, 3,2,2,7,4,4, 3,2,2,7,1,7, 3,0,3,7,4,7, 2,3,2,1,7,1, 2,2,3,3,1,7, 3,0,3,7,4,5, 3,0,0,7,1,1, 3,1,3,7,4,7, 3,2,3,7,0,7, 3,2,3,7,1,7, 3,2,3,7,4,4, 3,2,3,4,4,7, 3,2,3,7,4,3, 3,2,2,5,5,7, 2,3,2,0,7,0, 3,0,3,7,1,7, 3,0,3,6,1,6, 3,0,3,7,6,7, 3,0,3,3,4,3, 3,0,3,4,7,4, 2,2,3,3,5,1};

static const uint8_t font_35_az[]={2,2,1,2,5,7, 3,1,0,7,1,6, 1,2,2,6,1,1, 0,1,3,6,1,7, 1,2,1,6,5,5, 0,1,2,4,7,4, 1,2,1,1,5,6, 3,1,0,7,0,7, 0,2,0,0,7,0, 0,0,2,3,1,6, 3,0,1,7,4,3, 3,0,0,6,1,1, 3,3,1,7,4,7, 3,2,1,7,0,7, 1,2,1,6,1,6, 3,2,1,7,4,0, 1,2,3,0,4,7, 1,2,2,7,0,0, 1,2,2,1,5,2, 1,3,1,0,6,1, 3,0,3,6,1,7, 3,0,3,6,1,6, 3,0,3,6,7,7, 3,0,3,3,4,3, 3,0,3,1,5,6, 2,2,3,3,5,1};

/******************************************************************************/
/*                      Definition of exported variables                      */
/******************************************************************************/

/******************************************************************************/
/*                    Definition of exported constant data                    */
/******************************************************************************/

/******************************************************************************/
/*                  Declaration of local function prototypes                  */
/******************************************************************************/
static void bigfont_printTopChar(LCD_t *lcd, char ch);
static void bigfont_printBotChar(LCD_t *lcd, char ch);
/******************************************************************************/
/*                       Definition of local functions                        */
/******************************************************************************/
/*
 * @brief Write the first line of the big character.
 * @param[in]: LCD_t *lcd - The selected lcd object
 * @param[in]: char ch - The data to write
 */
static void bigfont_printTopChar(LCD_t *lcd, char ch)
{
  if ((ch>='0')&&(ch<='9'))
  {
    for (uint8_t i=0; i<3; i++)
    if (!lcd->Data._invertBigFont) LCD_write(lcd,font_35_09[(ch-'0')*6+i]);
    else LCD_write(lcd,7-font_35_09[(ch-'0')*6+i]);
  }
  else if ((ch>='A')&&(ch<='Z'))
  {
    for (uint8_t i=0; i<3; i++)
    if (!lcd->Data._invertBigFont) LCD_write(lcd,font_35_AZ[(ch-'A')*6+i]);
    else  LCD_write(lcd,7-font_35_AZ[(ch-'A')*6+i]);
  }
  else if ((ch>='a')&&(ch<='z'))
  {
    for (uint8_t i=0; i<3; i++)
    if (!lcd->Data._invertBigFont) LCD_write(lcd,font_35_az[(ch-'a')*6+i]);
    else LCD_write(lcd,7-font_35_az[(ch-'a')*6+i]);
  }
  else if ((ch>='!')&&(ch<='/'))
  {
    for (uint8_t i=0; i<3; i++)
    if (!lcd->Data._invertBigFont) LCD_write(lcd,font_35_sym1[(ch-'!')*6+i]);
    else LCD_write(lcd,7-font_35_sym1[(ch-'!')*6+i]);
  }
  else if ((ch>=':')&&(ch<='@'))
  {
    for (uint8_t i=0; i<3; i++)
    if (!lcd->Data._invertBigFont) LCD_write(lcd,font_35_sym2[(ch-':')*6+i]);
    else LCD_write(lcd,7-font_35_sym2[(ch-':')*6+i]);
  }
  if (!lcd->Data._invertBigFont) LCD_write(lcd,' ');
  else LCD_write(lcd,255);
}

/*
 * @brief Write the second line of the big character.
 * @param[in]: LCD_t *lcd - The selected lcd object
 * @param[in]: char ch - The data to write
 */
static void bigfont_printBotChar(LCD_t *lcd, char ch)
{
	if ((ch>='0')&&(ch<='9'))
	{
		for (uint8_t i=0; i<3; i++)
			if (!lcd->Data._invertBigFont) LCD_write(lcd,font_35_09[(ch-'0')*6+i+3]);
			else LCD_write(lcd,7-font_35_09[(ch-'0')*6+i+3]);
	}
	else if ((ch>='A')&&(ch<='Z'))
	{
		for (uint8_t i=0; i<3; i++)
			if (!lcd->Data._invertBigFont) LCD_write(lcd,font_35_AZ[(ch-'A')*6+i+3]);
			else LCD_write(lcd,7-font_35_AZ[(ch-'A')*6+i+3]);
	}
	else if ((ch>='a')&&(ch<='z'))
	{
		for (uint8_t i=0; i<3; i++)
			if (!lcd->Data._invertBigFont) LCD_write(lcd,font_35_az[(ch-'a')*6+i+3]);
			else LCD_write(lcd,7-font_35_az[(ch-'a')*6+i+3]);
	}
	else if ((ch>='!')&&(ch<='/'))
	{
		for (uint8_t i=0; i<3; i++)
			if (!lcd->Data._invertBigFont) LCD_write(lcd,font_35_sym1[(ch-'!')*6+i+3]);
			else LCD_write(lcd,7-font_35_sym1[(ch-'!')*6+i+3]);
	}
	else if ((ch>=':')&&(ch<='@'))
	{
		for (uint8_t i=0; i<3; i++)
			if (!lcd->Data._invertBigFont) LCD_write(lcd,font_35_sym2[(ch-':')*6+i+3]);
			else LCD_write(lcd,7-font_35_sym2[(ch-':')*6+i+3]);
	}
	if (!lcd->Data._invertBigFont) LCD_write(lcd,' ');
	else LCD_write(lcd,255);
}

/******************************************************************************/
/*                      Definition of exported functions                      */
/******************************************************************************/

/*
 * @brief Print a a big char on the lcd (only one char)
 *
 * */
void BIGFONT_printChar(LCD_t *lcd, char ch, uint8_t loc_x, uint8_t loc_y)
{
	LCD_setCursor(lcd,loc_x,loc_y);
	bigfont_printTopChar(lcd ,ch);
	LCD_setCursor(lcd,loc_x,loc_y+1);
	bigfont_printBotChar(lcd, ch);
}

/*
 * @brief Print a message with big characters on the LCD
 *
 * */
void BIGFONT_printMsg(LCD_t *lcd, char msg[], uint8_t loc_x, uint8_t loc_y)
{
	uint8_t i = 0;
	while(msg[i])
	{
		BIGFONT_printChar(lcd, msg[i],loc_x+i*4, loc_y);
		i++;
	}
}

/*
 * @brief Print a number with big characters on the LCD
 *
 * */
void BIGFONT_printNumber(LCD_t *lcd, int number, uint8_t loc_x, uint8_t loc_y)
{
	char msg[11];
	sprintf(msg,"%d",number);
	BIGFONT_printMsg(lcd ,msg, loc_x, loc_y);
}

//Display strings stored in PROGMEM. Provide the string name stored in PROGMEM to be displayed on the LCD's current cursor position.
void big_msg_lcd(LCD_t *lcd, char* msg_line, uint8_t loc_x, uint8_t loc_y)
{
	char msg_buffer[17];
	strcpy(msg_buffer,msg_line);
	msg_buffer[4]=0;
	BIGFONT_printMsg(lcd, msg_buffer, loc_x, loc_y);
}

/*
 * @brief Clear the LCD
 *
 * */
void BIGFONT_clear(LCD_t *lcd)
{
	if (!lcd->Data._invertBigFont) LCD_clear(lcd);
	else
	{
		LCD_setCursor(lcd, 0, 0);
		for (uint8_t i=0;i<80;i++)
		{
			LCD_write(lcd,255);
		}
	}
}

/*
 * @brief Invert the characters LCD
 *
 * */
void BIGFONT_invertBackground(LCD_t *lcd, uint8_t inv)
{
	lcd->Data._invertBigFont = inv;
}

/*
 * @brief Initialize the LCD to allow printing characters with big font.
 *
 * */
void BIGFONT_init(LCD_t *lcd)
{
	uint8_t i;
	for (i=0;i<8;i++)
	{
		LCD_createChar(lcd, i, pbf_ch_item[i]);
	}
}

/*
 * @brief Display a custom image on the LCD.
 *
 * */
void BIGFONT_printImg(LCD_t *lcd, uint8_t msg[])
{
	LCD_setCursor(lcd, 0, 0);
	for (uint8_t i=0; i<80; i++)
	{
		LCD_write(lcd, msg[i]);
	}
}
