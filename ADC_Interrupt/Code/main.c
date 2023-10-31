#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"

#include "DIO.h"

#include "LCD_Interface.h"
#include "ADC_Interface.h"
#include "Ex_Interrupt.h"

#define  F_CPU	8000000
#include <util/delay.h>
void ADC_Fun(void);
void Sensor_1(void);
void Sensor_2(void);
void Sensor_3(void);

#define IN_2	PINB2
#define IN_1	PIND3
#define IN_0	PIND2

u8 volatile channelCounter=0;
volatile u16  sensorReadArr[3]={0};

int main(void)
{
	
	EXI_SetCallBack(EX_INT0,Sensor_1);
	EXI_SetCallBack(EX_INT1,Sensor_2);
	EXI_SetCallBack(EX_INT2,Sensor_3);
	DIO_Init();
	LCD_Init();
	sei();
	
	ADC_Init(VREF_AVCC,ADC_SCALER_64);
	ADC_SetCallBack(ADC_Fun);

	EXI_Init();
	
	
	EXI_Enable(EX_INT0);
	EXI_Enable(EX_INT1);
	EXI_Enable(EX_INT2);

	ADC_TailChaining(CH_0);

 	ADC_InterruptEnable();
    while(1) 
    {	
		DIO_WritePin(IN_0,LOW);
		DIO_WritePin(IN_1,LOW);
		DIO_WritePin(IN_2,LOW);
		
		if(sensorReadArr[0] > 400)
		{
			DIO_WritePin(IN_0,HIGH);
		}
		else
		{
			LCD_Clear_Position(1,0,10);
			LCD_Clear_Position(0,0,4);
			LCD_GoTo(0,0);
			LCD_WriteNumber_2(sensorReadArr[0]);
			DIO_WritePin(IN_0,LOW);
		}
		
		if(sensorReadArr[1] > 400)
		{
			DIO_WritePin(IN_1,HIGH);
		}
		else
		{
			LCD_Clear_Position(2,0,10);
			LCD_Clear_Position(0,8,4);
			LCD_GoTo(0,8);
			LCD_WriteNumber_2(sensorReadArr[1]);
			DIO_WritePin(IN_1,LOW);
		}
		
		if(sensorReadArr[2] > 400)
		{
			DIO_WritePin(IN_2,HIGH);
		}
		else
		{
			LCD_Clear_Position(3,0,10);
			LCD_Clear_Position(0,15,4);
			LCD_GoTo(0,15);
			LCD_WriteNumber_2(sensorReadArr[2]);
			DIO_WritePin(IN_2,LOW);
		}
		ADC_StartConversion(channelCounter);
		SET_BIT(ADCSRA,ADEN);
		_delay_ms(100);
	}
}
void ADC_Fun(void)
{
	CLEAR_BIT(ADCSRA,ADEN);
	if(channelCounter == 0)
	{
		sensorReadArr[channelCounter] = ADC_GetReadNoblock();
		channelCounter=1;
	}
	else if(channelCounter == 1)
	{
		sensorReadArr[channelCounter] = ADC_GetReadNoblock();
		channelCounter=2;
	}
	else if(channelCounter == 2)
	{
		sensorReadArr[channelCounter] = ADC_GetReadNoblock();
		channelCounter=0;
	}
}

void Sensor_1(void)
{
	LCD_Clear_Position(0,0,4);
	LCD_WriteString_GoTo(1,0,"Sensor 1.");
	_delay_ms(250);
	DIO_WritePin(IN_0,LOW);
}
void Sensor_2(void)
{
	LCD_Clear_Position(0,8,4);
	LCD_WriteString_GoTo(2,0,"Sensor 2.");
	_delay_ms(250);
	DIO_WritePin(IN_1,LOW);
}
void Sensor_3(void)
{
	LCD_Clear_Position(0,15,4);
	LCD_WriteString_GoTo(3,0,"Sensor 3.");
	_delay_ms(250);
	DIO_WritePin(IN_2,LOW);
}