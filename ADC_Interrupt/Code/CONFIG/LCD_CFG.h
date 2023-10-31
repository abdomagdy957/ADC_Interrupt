

#ifndef LCD_CFG_H_
#define LCD_CFG_H_

#define  RS		PINA3
#define  En		PINA4

#define _8_BIT		1
#define _4_BIT		0

/****************8-BIT OR 4-BIT********************/
#define	LCD_MODE	_4_BIT

/************* 4 bit mode only***************/

#define D4	PINB0
#define D5	PINB1
#define D6	PINB3
#define D7	PINB4
/************* 8 bit mode only***************/
#define	 LCD_PORT		PA



#endif /* LCD_CFG_H_ */