/******************************************************************************/
/*                                                                            */
/*   All rights reserved. Distribution or duplication without previous        */
/*   written agreement of the owner prohibited.                               */
/*                                                                            */
/******************************************************************************/

/** \file bigFont_lcdI2c.h
 *
 * \brief Header file for bigFont_lcdI2c component
 *
 * Header file for bigFont_lcdI2c manager
 *
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> Author:   </td> <td> C.Garcia   </td></tr>
 * <tr> <td> Date:     </td> <td> 04/09/2018             </td></tr>
 * </table>
 * \n
 * <table border="0" cellspacing="0" cellpadding="0">
 * <tr> <td> COMPONENT: </td> <td> BIGFONT    </td></tr>
 * <tr> <td> SCOPE:     </td> <td> Public      </td></tr>
 * <tr> <td> TARGET:    </td> <td> STM32        </td></tr>
 * </table>
 * \note
 *
 * \see
 */

#ifndef BIG_FONT_H_
#define BIG_FONT_H_

/******************************************************************************/
/*                         Project Includes                                   */
/******************************************************************************/
#include "lcd_Hd44780I2C.h"
/******************************************************************************/
/*                 Definition of exported symbolic constants                  */
/******************************************************************************/

/******************************************************************************/
/*                Definition of exported function like macros                 */
/******************************************************************************/

/******************************************************************************/
/*         Definition of exported types (typedef, enum, struct, union)        */
/******************************************************************************/

/******************************************************************************/
/*                    Declaration of exported variables                       */
/******************************************************************************/

/******************************************************************************/
/*                  Declaration of exported constant data                     */
/******************************************************************************/

/******************************************************************************/
/*               Declaration of exported function prototypes                  */
/******************************************************************************/

extern void BIGFONT_printChar(LCD_t *lcd, char ch, uint8_t loc_x, uint8_t loc_y);

extern void BIGFONT_printMsg(LCD_t *lcd, char msg[], uint8_t loc_x, uint8_t loc_y);

extern void BIGFONT_printNumber(LCD_t *lcd, int number, uint8_t loc_x, uint8_t loc_y);

extern void big_msg_lcd(LCD_t *lcd, char* msg_line, uint8_t loc_x, uint8_t loc_y);

extern void BIGFONT_invertBackground(LCD_t *lcd, uint8_t inv);

extern void BIGFONT_clear(LCD_t *lcd);

extern void BIGFONT_init(LCD_t *lcd);

extern void BIGFONT_printImg(LCD_t *lcd, uint8_t msg[]);

#endif /* BIG_FONT_H */
