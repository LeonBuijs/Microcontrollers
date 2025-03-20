/*
 * bigavr_lcd.h
 *
 * Created: 28-2-2025 14:32:43
 *  Author: jaspe
 */ 


#define LCD_E 	3
#define LCD_RS	2

void lcd_strobe_lcd_e(void);
void init_4bits_mode(void);
void lcd_write_string(char *str);
void lcd_write_data(unsigned char byte);
void lcd_write_cmd(unsigned char byte);
void lcd_clear(void);
