#include "StdTypes.h"
#include "DIO.h"
#include "LCD_Interface.h"
#include "LCD_Cfg.h"
#include "Utils.h"



#define  F_CPU	8000000
#include <util/delay.h>


#if LCD_MODE == _8_BIT

static void WriteInst(u8 ins)
{
	/**this code in this fun repeated more than 1 time in lcd init >> so i make it a function to less code size and for readability**/
	
	DIO_WritePin(RS,LOW);	/*low: write instruction not data*/
	
	DIO_WritePort(LCD_PORT,ins);
	/*enable trigger to execute 1 instruction and wait 1ms*/
	DIO_WritePin(En,HIGH);
	_delay_ms(1);
	DIO_WritePin(En,LOW);
	_delay_ms(1);
	/****end of trigger***/
}
static void WriteData(u8 data)
{
	/**this code in this fun repeated more than 1 time in lcd init >> so i make it a function to less code size and for readability**/
	
	DIO_WritePin(RS,HIGH);	/*high: write data not data*/
	
	DIO_WritePort(LCD_PORT,data);
	/*enable trigger to execute 1 instruction and wait 1ms*/
	DIO_WritePin(En,HIGH);
	_delay_ms(1);
	DIO_WritePin(En,LOW);
	_delay_ms(1);
	/****end of trigger***/
}
void LCD_Init(void)
{
	_delay_ms(50);
	WriteInst(0x38);	/*0X38 TO Initialize 8 bit  mode*/
	WriteInst(0x0c);	//0x0e for cursor ,,, 0x0f for cursor and blink
	WriteInst(0x01);	/*clear screen*/ 
	_delay_ms(1);
	WriteInst(0x06);	/*entry mode reg .. enable (increase address) and disable shift*/
}

#else

static void WriteInst(u8 ins)
{
	/**this code in this fun repeated more than 1 time in lcd init >> so i make it a function to less code size and for readability**/
	
	DIO_WritePin(RS,LOW);	/*low: write instruction not data*/
	/*write the first half of data*/
	DIO_WritePin(D7,READ_BIT(ins,7));	/* write high or low in d7*/
	DIO_WritePin(D6,READ_BIT(ins,6));	/* write high or low in d6*/
	DIO_WritePin(D5,READ_BIT(ins,5));	/* write high or low in d5*/
	DIO_WritePin(D4,READ_BIT(ins,4));	/* write high or low in d4*/
	
	/*enable trigger to execute 1 instruction and wait 1ms*/
	DIO_WritePin(En,HIGH);
	_delay_ms(1);
	DIO_WritePin(En,LOW);
	_delay_ms(1);
	/****end of trigger***/
	
	/*write the second half of data*/
	DIO_WritePin(D7,READ_BIT(ins,3));	/* write high or low in d7*/
	DIO_WritePin(D6,READ_BIT(ins,2));	/* write high or low in d6*/
	DIO_WritePin(D5,READ_BIT(ins,1));	/* write high or low in d5*/
	DIO_WritePin(D4,READ_BIT(ins,0));	/* write high or low in d4*/
	
	/*enable trigger to execute second instruction and wait 1ms*/
	DIO_WritePin(En,HIGH);
	_delay_ms(1);
	DIO_WritePin(En,LOW);
	_delay_ms(1);
	/****end of trigger***/
}
static void WriteData(u8 data)
{
	/**this code in this fun repeated more than 1 time in lcd init >> so i make it a function to less code size and for readability**/
	
	DIO_WritePin(RS,HIGH);	/*high: write data not data*/
	
	/*write the first half of data*/
	DIO_WritePin(D7,READ_BIT(data,7));	/* write high or low in d7*/
	DIO_WritePin(D6,READ_BIT(data,6));	/* write high or low in d6*/
	DIO_WritePin(D5,READ_BIT(data,5));	/* write high or low in d5*/
	DIO_WritePin(D4,READ_BIT(data,4));	/* write high or low in d4*/
		
	/*enable trigger to execute 1 instruction and wait 1ms*/
	DIO_WritePin(En,HIGH);
	_delay_ms(1);
	DIO_WritePin(En,LOW);
	_delay_ms(1);
	/****end of trigger***/
		
	/*write the second half of data*/
	DIO_WritePin(D7,READ_BIT(data,3));	/* write high or low in d7*/
	DIO_WritePin(D6,READ_BIT(data,2));	/* write high or low in d6*/
	DIO_WritePin(D5,READ_BIT(data,1));	/* write high or low in d5*/
	DIO_WritePin(D4,READ_BIT(data,0));	/* write high or low in d4*/
		
	/*enable trigger to execute second instruction and wait 1ms*/
	DIO_WritePin(En,HIGH);
	_delay_ms(1);
	DIO_WritePin(En,LOW);
	_delay_ms(1);
	/****end of trigger***/
}
void LCD_Init(void)
{
	_delay_ms(50);
	WriteInst(0x02);	/*send 2 in 4 bit mode*/
	WriteInst(0x28);	/*0X28 TO Initialize 4 bit  mode*/
	WriteInst(0x0c);	//0x0e for cursor ,,, 0x0f for cursor and blink
	WriteInst(0x01);	/*clear screen*/
	_delay_ms(1);
	WriteInst(0x06);	/*entry mode reg .. enable (increase address) and disable shift*/
}

#endif

void LCD_GoTo(u8 line, u8 cell)
{
	if(line == 0)
	{
		WriteInst(0x80+cell);		/*0x80 is the address of the first line*/
	}
	else if(line == 1)
	{
		WriteInst(0xc0+cell);		/*0xc0 is the address of the second line*/
	}
	else if(line == 2)
	{
		WriteInst(0x94+cell);		/*0x94 is the address of the third line*/
	}
	else if(line == 3)
	{
		WriteInst(0xd4+cell);		/*0xd4 is the address of the forth line*/
	}
}
void LCD_WriteChar(u8 ch)
{
	WriteData(ch);
}
void LCD_WriteString(u8 *str)
{
	u8 i=0;
	while(str[i] != '\0')
	{
		LCD_WriteChar(str[i]);
		i++;
	}
}
void LCD_WriteString_GoTo(u8 line, u8 cell, u8* str)
{
	LCD_GoTo(line,cell);
	LCD_WriteString(str);
}
void swap(u8 *x, u8 *y)
{
	u8 t = *x; *x = *y; *y = t;
}
// Function to reverse `buffer[i…j]`
u8* reverse(u8 *buffer, u8 i, u8 j)
{
	while (i < j) {
		swap(&buffer[i++], &buffer[j--]);
	}
	
	return buffer;
}
u8* itoa(s32 value, u8* buffer, u8 base)
{
	// invalid input
	if (base < 2 || base > 32) {
		return buffer;
	}
	
	// consider the absolute value of the number
	s32 n = value;
	if(n<0)
	{
		n=n*-1;
	}
	u8 i = 0;
	while (n)
	{
		int r = n % base;
		
		if (r >= 10) {
			buffer[i++] = 65 + (r - 10);
		}
		else {
			buffer[i++] = 48 + r;
		}
		
		n = n / base;
	}
	
	// if the number is 0
	if (i == 0) {
		buffer[i++] = '0';
	}
	
	// If the base is 10 and the value is negative, the resulting string
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10) {
		buffer[i++] = '-';
	}
	
	buffer[i] = '\0'; // null terminate string
	
	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}
void LCD_WriteNumber(s32 num,u8 base)
{
	u8 buff[16]; /* String to hold the ascii result */
	itoa(num,buff,base); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	LCD_WriteString(buff);
}

void LCD_WriteNumber_2(s32 num)
{
	s8 str[16];
	u8 i=0;
	s8 j=0;
	if(num == 0)
	{
		LCD_WriteChar('0');
		return;
	}
	if(num<0)
	{
		LCD_WriteChar('-');
		num = num*(-1);
	}
	while(num)
	{
		str[i] = num%10 + '0';
		i++;
		num = num / 10;
	}
	for(j=i-1; j>=0;j--)
	{
		LCD_WriteChar(str[j]);
	}
}
void LCD_Write_4Digit(u16 num)
{
	LCD_WriteChar( ((num%10000)/1000) +'0');
	LCD_WriteChar( ((num%1000)/100) +'0');
	LCD_WriteChar( ((num%100)/10) +'0');
	LCD_WriteChar( ((num%10)/1) +'0');
}
void LCD_WriteBinary(u8 num)
{
	s8 i;
	for(i=7;i>=0;i--)
	{
		LCD_WriteChar(READ_BIT(num,i) ? '1' : '0');
	}
}

void LCD_WriteHex(u8 num)
{
	u8 h_num = num>>4;
	u8 l_num = num & 0x0f;
	if(h_num < 10)
	{
		LCD_WriteChar(h_num+'0');
	}
	else
	{
		LCD_WriteChar((h_num-10)+'A');
	}
	if(l_num < 10)
	{
		LCD_WriteChar(l_num+'0');
	}
	else
	{
		LCD_WriteChar((l_num-10)+'A');
	}
}
void LCD_Clear(void)
{
	WriteInst(0x01);
	_delay_ms(1);
}
void LCD_Clear_Position(u8 line, u8 cell, u8 noCells)
{
	LCD_GoTo(line,cell);
	for(u8 i=0; i<noCells; i++)
	{
		LCD_WriteChar(' ');
	}
}

void LCD_CustomChar(u8 address, u8 *pattern)
{
	 WriteInst(0x40+address*8);		//*8 cuz we need bits not bytes
	 for(u8 i=0; i<8;i++)
	 {
		 WriteData(pattern[i]);
	 }
	 WriteInst(0x80);	//exit CGRAM and return to address 0 again 
}