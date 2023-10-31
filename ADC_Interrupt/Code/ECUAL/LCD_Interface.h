

#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Clear_Position(u8 line, u8 cell, u8 noCells);

/*line 0:1 , cell 0:15 */
void LCD_GoTo(u8 line, u8 cell);

void LCD_WriteString(u8 *str);
void LCD_WriteString_GoTo(u8 line, u8 cell, u8 *str);	
void LCD_WriteChar(u8 ch);

void swap(u8 *x, u8 *y);
u8* reverse(u8 *buffer, u8 i, u8 j);
u8* itoa(s32 value, u8* buffer, u8 base);

/*** this function	if	(base == 10) >> displays decimal
	****				(base == 2) >> displays binary
	****				(base == 16)>> displays hex		***/ 
void LCD_WriteNumber(s32 num,u8 base);


void LCD_WriteNumber_2(s32 num);

void LCD_Write_4Digit(u16 num);
void LCD_WriteBinary(u8 num);
void LCD_WriteHex(u8 num);

void LCD_CustomChar(u8 address, u8 *pattern);




#endif /* LCD_INTERFACE_H_ */