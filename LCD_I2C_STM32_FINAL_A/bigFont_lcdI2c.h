
#include <lcd_Hd44780I2C.h>

extern void BIGFONT_printChar(LCD_t *lcd, char ch, uint8_t loc_x, uint8_t loc_y);
extern void BIGFONT_printMsg(LCD_t *lcd, char msg[], uint8_t loc_x, uint8_t loc_y);
extern void BIGFONT_printNumber(LCD_t *lcd, int number, uint8_t loc_x, uint8_t loc_y);
extern void big_msg_lcd(LCD_t *lcd, char* msg_line, uint8_t loc_x, uint8_t loc_y);
extern void BIGFONT_invertBackground(LCD_t *lcd, uint8_t inv);
extern void BIGFONT_clear(LCD_t *lcd);
extern void BIGFONT_init(LCD_t *lcd);
extern void BIGFONT_printImg(LCD_t *lcd, uint8_t msg[]);
