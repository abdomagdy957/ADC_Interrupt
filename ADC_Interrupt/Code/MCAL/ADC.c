#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"
#include "ADC_Interface.h"

static u8 ADC_BusyFlag;
extern volatile u32 c2;



static void(*ADC_Fptr)(void)=NULLPTR;
//u8 volatile channelCounter = 0;

void ADC_Init(ADC_VoltRef_type vref,ADC_Prescaler_type scaler)
{
	/* vref*/
	switch(vref)
	{
		case VREF_AREF:
		CLEAR_BIT(ADMUX,REFS0);
		CLEAR_BIT(ADMUX,REFS1);
		break;
		case VREF_AVCC:
		SET_BIT(ADMUX,REFS0);
		CLEAR_BIT(ADMUX,REFS1);
		break;
		case VREF_256V:
		SET_BIT(ADMUX,REFS0);
		SET_BIT(ADMUX,REFS1);
		break;
	}
	/* prescallar*/
	
	ADCSRA&=0XF8;
	scaler&=0X07;
	ADCSRA=ADCSRA|scaler;
	/* enable ADC*/
	SET_BIT(ADCSRA,ADEN);
	//SET_BIT(ADCSRA,ADATE);
}

u16 ADC_Read(ADC_Channel_type CH)
{
	

	/* set channel mux*/
	ADMUX&=0XE0;//11100000
	ADMUX|=CH;
	
	/*start conversion*/
	SET_BIT(ADCSRA,ADSC);
	/* w8 ADC finish busy wait( blocking )**/
	while(READ_BIT(ADCSRA,ADSC));
	
	/*get read*/
	//u16 adc=ADCL;
	//adc=adc|(ADCH<<8);
	/* IF adle==1*/
	//u16 adc=ADCL>>6;
	//adc=adc|ADCH<<2;
	
	return ADC;
}

u16 ADC_GetRead(void)
{
	
	
	/*get read*/
	//u16 adc=ADCL;
	//adc=adc|(ADCH<<8);
	/* IF adle==1*/
	//u16 adc=ADCL>>6;
	//adc=adc|ADCH<<2;
	/* w8 ADC finish busy wait( blocking )**/
	while(READ_BIT(ADCSRA,ADSC));
	return ADC;
}

u8 ADC_GetReadPerodic(u16*pdata)
{
	/* perodic ckeck  (polling)*/
	
	if (!READ_BIT(ADCSRA,ADSC))
	{
		*pdata=ADC;
		ADC_BusyFlag=0;
		return 1;
	}
	return 0;
	
}

u16 ADC_GetReadNoblock(void)
{
	return ADC;
}


u16 ADC_VoltRead(ADC_Channel_type CH)
{
	u16 volt;
	u16 adc;
	adc=ADC_GetReadNoblock();
	
	volt=(adc*(u32)5000)/1023;
	return volt;
	
}

void ADC_InterruptEnable(void)
{
	SET_BIT(ADCSRA,ADIE);
	//SET_BIT(ADCSRA,ADATE);
}

void ADC_InterruptDisable(void)
{
	CLEAR_BIT(ADCSRA,ADIE);
	CLEAR_BIT(ADCSRA,ADATE);
}

void ADC_SetCallBack(void (*LocalFptr)(void))
{
	ADC_Fptr = LocalFptr;
}

u16 ADC_Read_Asynchronous(ADC_Channel_type CH)
{
	ADC_InterruptEnable();
	ADC_StartConversion(CH);
}
void ADC_StartConversion(ADC_Channel_type CH)
{
	if (ADC_BusyFlag==0)
	{
		SET_BIT(ADCSRA,ADIE);
		//SET_BIT(ADCSRA,ADATE);	//enable auto trigger "optional" set if we need a synchronousRead
		/* set channel mux*/
		ADMUX&=0XE0;//11100000
		ADMUX|=CH;
		
		/*start conversion*/
		SET_BIT(ADCSRA,ADSC);
		ADC_BusyFlag=1;
	}
	
}
void ADC_TailChaining(ADC_Channel_type CH)
{
	ADC_StartConversion(CH_0);
}
ISR(ADC_vect)
{
	ADC_BusyFlag = 0;
	ADC_Fptr();
}