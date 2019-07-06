/*
 * Traffic_System.c
 *
 * Created: APRIL-JULY 2015
 *  Author: CODE_00
 */

#define F_CPU 8000000ul
#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

void adcData();//display analog to digital coneverted value
void long_wait_jam();
void short_wait();//interval for light sequence
void normal_wait();//if no jam
void normal_sequence();//vehicle move time
void bootCounter();// loading / booting time
void roadARjam();
void roadBRjam();
void roadCRjam();
void roadDRjam();
void roadALjam();
void roadBLjam();
void roadCLjam();
void roadDLjam();
void signalTest();

int sensorAL;
int sensorBL;
int sensorCL;
int sensorDL;
int sensorAR;
int sensorBR;
int sensorCR;
int sensorDR;
int z;
int i;
void adc_init()
{
	ADCSRA|=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);//prescalar of 128
	ADMUX|=(1<<REFS0)|(1<<ADLAR);
}

uint16_t adc_value(uint16_t ch)
{

	ADMUX&=~0X0F;
	ch&=0x07;
	ADMUX|=ch;
	ADCSRA|=(1<<ADSC);
	while(ADCSRA&_BV(ADSC));
	return (ADCH);

}

int main()
{
	uint16_t pa0,pa1,pa2,pa3,pa4,pa5,pa6,pa7;
	DDRD =0b01111111;  //portC0-portC7 as an output
	DDRC =0b11111111;  //portC0-portC5 as an output
	PORTD = 0x00;
	PORTC = 0x00;
	PORTD |=(1<<PIND6); //System status
	//DDRA=0b00000000;	//ADC Channel Input
	adc_init();
	_delay_ms(50);
	LCDInit(LS_BLINK|LS_ULINE);
	//Erase LCD
	LCDClear();
	LCDWriteStringXY(0,0,"Smart Traffic System");
	LCDWriteStringXY(0,1,"Loading Channel Data");
	LCDWriteStringXY(0,2,"Loading...     %");
	bootCounter();

	while(1)
	{
		//PORTD |=(1<<PIND7);
		sensorCL=adc_value(0);
		sensorCR=adc_value(1);
		sensorBL=adc_value(2);
		sensorBR=adc_value(3);
		sensorAL=adc_value(4);
		sensorAR=adc_value(5);
		sensorDR=adc_value(6);
		sensorDL=adc_value(7);
		adcData();
		_delay_ms(250);

		if (sensorAL||sensorAR||sensorBL||sensorBR||sensorCL||sensorCR||sensorDL||sensorDR < 50)
		{
			for (i=0; i<15;i+=1)
			{
				if (sensorAL<50 && i>5 )
				{
					roadALjam();
				}
				if (sensorBL<50 && i>5 )
				{
					roadBLjam();
				}
				if (sensorCL<50 && i>5 )
				{
					roadCLjam();
				}
				if (sensorDL<50 && i>5 )
				{
					roadDLjam();

				}
				if (sensorAR<50 && i>11)
				{
					roadARjam();
				}
				if (sensorBR<50 && i > 5 )
				{
					roadBRjam();
				}
				if (sensorCR<50 && i > 5 )
				{
					roadCRjam();
				}
				if (sensorDR<50 && i > 5 )
				{
					roadDRjam();
					i=0;
				}

				_delay_loop_2(0);
				_delay_loop_2(0);
				_delay_loop_2(0);
				_delay_loop_2(0);
			}
		}
		if(sensorAL||sensorAR||sensorBL||sensorBR||sensorCL||sensorCR||sensorDL||sensorDR > 150)
		{
			normal_sequence();
		}

	}
}

adcData()
{
	LCDClear();
	LCDWriteStringXY(0,0,"Smart Traffic System");
	LCDWriteStringXY(0,1,"Loading Channel Data");
	LCDWriteStringXY(0,2,"Loading...     %");

	LCDClear();
	LCDWriteIntXY(5,0,sensorAL,4);
	LCDWriteStringXY(0,0,"AL = ");
	LCDWriteIntXY(15,0,sensorAR,4);
	LCDWriteStringXY(10,0,"AR = ");

	LCDWriteIntXY(5,1,sensorBL,4);
	LCDWriteStringXY(0,1,"BL = ");
	LCDWriteIntXY(15,1,sensorBR,4);
	LCDWriteStringXY(10,1,"BR = ");

	LCDWriteIntXY(5,2,sensorCL,4);
	LCDWriteStringXY(0,2,"CL = ");
	LCDWriteIntXY(15,2,sensorCR,4);
	LCDWriteStringXY(10,2,"CR = ");

	LCDWriteIntXY(5,3,sensorDL,4);
	LCDWriteStringXY(0,3,"DL = ");
	LCDWriteIntXY(15,3,sensorDR,4);
	LCDWriteStringXY(10,3,"DR = ");
	_delay_loop_2(0);
	_delay_loop_2(0);
}

long_wait_jam()
{
	for (z=10;z>0;z-=1)
	{
		LCDWriteIntXY(16,1,z,4);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);
	}
}

short_wait_orange()
{
	for (z=4;z>0;z-=1)
	{
		LCDWriteIntXY(3,2,z,4);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);
	}
}

short_wait()
{
	for (z=4;z>0;z-=1)
	{
		LCDWriteIntXY(16,1,z,4);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);
	}
}
normal_wait()
{
	for (z=10;z>0;z-=1)
	{
		LCDWriteIntXY(16,1,z,4);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);
	}
}
bootCounter()
{
	for (z>0;z<101;z+=10)
	{
		LCDWriteIntXY(10,2,z,4);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
		_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);_delay_loop_2(0);
	}
}
normal_sequence()
{
	//Red leds On ...STOP
	PORTC|=(1<<PINC0);PORTC|=(1<<PINC3);PORTC|=(1<<PINC6);PORTD|=(1<<PIND1);
	LCDClear();
	LCDWriteString("RDA & RDC GETREADY");
	LCDWriteStringXY(0,1,"IN      SECONDS");
	short_wait();
	//Orange Leds On..GET READY
	PORTC|=(1<<PINC1);PORTC|=(1<<PINC7);
	short_wait_orange();
	//Red Leds
	PORTC &=~(1<<PINC0);PORTC&=~(1<<PINC6);
	//Info to Drivers
	LCDClear();
	LCDWriteStringXY(0,0,">>>ROAD A & C MOVING");
	LCDWriteStringXY(0,1,"IN STOP MODE.!!! ");
	LCDWriteStringXY(0,2,"ROAD B & ROAD D STOP");
	LCDWriteStringXY(0,3,"RDB & RDD GET READY");
	//Orange Leds Off...GO
	PORTC&=~(1<<PINC1);PORTC&=~(1<<PINC7);
	//Green Leds On...MOVE
	PORTC|=(1<<PINC2);PORTD|=(1<<PIND0);
	normal_wait();
	//Green leds Off...STOP
	PORTC&=~(1<<PINC2);PORTD&=~(1<<PIND0);

	///////////////ROAD B AND ROAD D
	//Red leds On ...STOP
	PORTC|=(1<<PINC0);PORTC|=(1<<PINC3);PORTC|=(1<<PINC6);PORTD|=(1<<PIND1);
	LCDClear();
	LCDWriteString("RDB & RDD GET READY");
	LCDWriteStringXY(0,1,"IN      SECONDS");
	short_wait();
	//Orange Leds On..GET READY
	PORTC|=(1<<PINC4);PORTD|=(1<<PIND3);
	short_wait_orange();
	//Red Leds Off.. LEAVING
	PORTC &=~(1<<PINC3);PORTD &=~(1<<PIND1);
	//Info to Drivers
	LCDClear();
	LCDWriteStringXY(0,0,">>>ROAD B & D MOVING");
	LCDWriteStringXY(0,1,"IN STOP MODE.!!! ");
	LCDWriteStringXY(0,2,"ROAD A & ROAD C STOP");
	LCDWriteStringXY(0,3,"RDA & RDC GET READY");
	//Orange Leds Off...GO
	PORTC&=~(1<<PINC4);PORTD&=~(1<<PIND3);
	//Green Leds On...MOVE
	PORTC|=(1<<PINC5);PORTD|=(1<<PIND5);
	normal_wait();
	//Green leds Off...STOP
	PORTC&=~(1<<PINC5);;PORTD&=~(1<<PIND5);
}

roadALjam()
{
	//Red leds On ...STOP
	PORTC|=(1<<PINC0);PORTC|=(1<<PINC3);PORTC|=(1<<PINC6);PORTD|=(1<<PIND1);
	LCDClear();
	LCDWriteString("ROAD AL GET READY...");
	LCDWriteStringXY(0,1,"AMBER LIGHT");
	LCDWriteStringXY(0,2,"IN        SECONDS");
	//LCDWriteIntXY(3,1,z,4);
	short_wait();
	//Orange Leds On..GET READY
	PORTC|=(1<<PINC1);
	short_wait_orange();
	//Red Leds
	PORTC &=~(1<<PINC0);
	//Orange Leds Off...GO
	PORTC&=~(1<<PINC1);
	//Info to Drivers
	LCDClear();
	//LCDWriteIntXY(3,1,z,4);
	LCDWriteStringXY(0,0,">ROAD AL MOVING ONLY");
	LCDWriteStringXY(0,1,"IN STOP MODE.!!! ");
	LCDWriteStringXY(0,2,"ROAD B , C & D STOP");
	LCDWriteStringXY(0,3,"DRIVERS GET READY>>>");
	//Green Leds On...MOVE
	PORTC|=(1<<PINC2);
	PORTC&=~(1<<PINC0);
	PORTC&=~(1<<PINC1);
	long_wait_jam();
	//Green leds Off...STOP
	PORTC&=~(1<<PINC2);

}
roadBLjam()
{
	//Red leds On ...STOP
	PORTC|=(1<<PINC0);PORTC|=(1<<PINC3);PORTC|=(1<<PINC6);PORTD|=(1<<PIND1);
	LCDClear();
	LCDWriteString("ROAD BL GET READY...");
	LCDWriteStringXY(0,1,"AMBER LIGHT");
	LCDWriteStringXY(0,2,"IN        SECONDS");
	short_wait();
	//Orange Leds On..GET READY
	PORTC|=(1<<PINC4);
	short_wait_orange();
	//Red Leds Off.. LEAVING
	PORTC &=~(1<<PINC3);
	//Info to Drivers
	LCDClear();
	LCDWriteStringXY(0,0,">ROAD BL MOVING ONLY");
	LCDWriteStringXY(0,1,"IN STOP MODE.!!! ");
	LCDWriteStringXY(0,2,"ROAD A , C & D STOP");
	LCDWriteStringXY(0,3,"DRIVERS GET READY>>>");
	//Orange Leds Off...GO
	PORTC&=~(1<<PINC4);
	//Green Leds On...MOVE
	PORTC|=(1<<PINC5);
	long_wait_jam();
	//Green leds Off...STOP
	PORTC&=~(1<<PINC5);
}

roadCLjam()
{
	//Red leds On ...STOP
	PORTC|=(1<<PINC0);PORTC|=(1<<PINC3);PORTC|=(1<<PINC6);PORTD|=(1<<PIND1);
	LCDClear();
	LCDWriteString("ROAD CL GET READY...");
	LCDWriteStringXY(0,1,"AMBER LIGHT");
	LCDWriteStringXY(0,2,"IN        SECONDS");
	short_wait();
	//Orange Leds On..GET READY
	PORTC|=(1<<PINC7);
	short_wait_orange();
	//Red Leds
	PORTC&=~(1<<PINC6);
	//Info to Drivers
	LCDClear();
	LCDWriteStringXY(0,0,">ROAD CL MOVING ONLY");
	LCDWriteStringXY(0,1,"IN STOP MODE.!!! ");
	LCDWriteStringXY(0,2,"ROAD A , B & D STOP");
	LCDWriteStringXY(0,3,"DRIVERS GET READY>>>");
	//Orange Leds Off...GO
	PORTC&=~(1<<PINC7);
	//Green Leds On...MOVE
	PORTD|=(1<<PIND0);
	long_wait_jam();
	//Green leds Off...STOP
	PORTD&=~(1<<PIND0);
}
roadDLjam()
{
	//Red leds On ...STOP
	PORTC|=(1<<PINC0);PORTC|=(1<<PINC3);PORTC|=(1<<PINC6);PORTD|=(1<<PIND1);
	LCDClear();
	LCDWriteString("ROAD DL GET READY...");
	LCDWriteStringXY(0,1,"AMBER LIGHT");
	LCDWriteStringXY(0,2,"IN        SECONDS");
	short_wait();
	//Orange Leds On..GET READY
	PORTD|=(1<<PIND3);
	short_wait_orange();
	//Red Leds Off.. LEAVING
	PORTD &=~(1<<PIND1);
	//Info to Drivers
	LCDClear();
	LCDWriteStringXY(0,0,">ROAD DL MOVING ONLY");
	LCDWriteStringXY(0,1,"IN STOP MODE.!!! ");
	LCDWriteStringXY(0,2,"ROAD A , B & C STOP");
	LCDWriteStringXY(0,3,"DRIVERS GET READY>>>");
	//Orange Leds Off...GO
	PORTD&=~(1<<PIND3);
	//Green Leds On...MOVE
	PORTD|=(1<<PIND5);
	long_wait_jam();
	//Green leds Off...STOP
	PORTD&=~(1<<PIND5);
}

roadARjam()
{
	LCDClear();
	LCDWriteStringXY(0,0,"LEVEL CROSSING!!FOR");
	LCDWriteStringXY(0,1,"ROAD A DRIVERS, STOP");
	LCDWriteStringXY(0,2,"FOR TRAIN and WAIT");
	LCDWriteStringXY(0,3,"FOR ROAD TO CLEAR");
	_delay_ms(5000);
	roadBLjam();
	roadCLjam();
	roadDLjam();
}
roadBRjam()
{
	LCDClear();
	LCDWriteStringXY(0,0,"ROAD BR IMOVABLE!!!");
	LCDWriteStringXY(0,1,"ALL DRIVERS PLZ SEEK");
	LCDWriteStringXY(0,2,"ALTENATIVE ROUTES OR");
	LCDWriteStringXY(0,3,"...WAIT ROAD TO CLEAR");
	_delay_ms(5000);
	roadALjam();
	roadBLjam();
	roadCLjam();
	roadDLjam();
	adcData();
	_delay_ms(5000);
}
roadCRjam()
{
	LCDClear();
	LCDWriteStringXY(0,0,"ROAD CR IMOVABLE!!!");
	LCDWriteStringXY(0,1,"ALL DRIVERS PLZ SEEK");
	LCDWriteStringXY(0,2,"ALTENATIVE ROUTES OR");
	LCDWriteStringXY(0,3,"...WAIT ROAD TO CLEAR");
	_delay_ms(5000);
	roadALjam();
	roadBLjam();
	roadCLjam();
	roadDLjam();
}
roadDRjam()
{
	LCDClear();
	LCDWriteStringXY(0,0,"ROAD DR IMOVABLE!!!");
	LCDWriteStringXY(0,1,"ALL DRIVERS PLZ SEEK");
	LCDWriteStringXY(0,2,"ALTENATIVE ROUTES OR");
	LCDWriteStringXY(0,3,"...WAIT ROAD TO CLEAR");
	_delay_ms(5000);
	roadALjam();
	roadBLjam();
	roadCLjam();
	roadDLjam();
}

/*void signalTest()
{
	PORTC|=(1<<PINC0);
	///////////////ROAD B AND ROAD D
	LCDClear();
	LCDWriteString("RDB & RDD GET READY");
	LCDWriteStringXY(0,1,"IN      SECONDS");
	LCDWriteIntXY(3,2,z,1);
	short_wait();
	//Orange Leds
	PORTC|=(1<<PINC1);
	short_wait_orange();
	//Red Leds
	PORTC &=~(1<<PINC0);

	LCDClear();
	LCDWriteStringXY(0,0," ROAD B & D MOVING ");
	LCDWriteStringXY(0,1,"IN STOP MODE.... ");
	LCDWriteStringXY(0,2,"ROAD A, ROAD C");
	LCDWriteStringXY(0,3,"RDA & RDC GET READY");
	//Orange Leds
	PORTC&=~(1<<PINC1);
	//Green Leds
	PORTC|=(1<<PINC2);
	normal_wait();
	PORTC&=~(1<<PINC2);

	/*clear all leds
	PORTC &=~(1<<PINC0);PORTC&=~(1<<PINC1);PORTC&=~(1<<PINC2); //RoadA
	PORTC &=~(1<<PINC3);PORTC&=~(1<<PINC4);PORTC&=~(1<<PINC5); //RoadB
	PORTC&=~(1<<PINC6);PORTC&=~(1<<PINC7);PORTD&=~(1<<PIND0);  //RoadC
	PORTD &=~(1<<PIND1);PORTD &=~(1<<PIND3);PORTD &=~(1<<PIND5);//RoadD*/

}
*/
void roadALjamforloop()
{
	//for (z=15;z>0;z-=1)
	//{
	//Red leds On ...STOP
	PORTC|=(1<<PINC0);PORTC|=(1<<PINC3);PORTC|=(1<<PINC6);PORTD|=(1<<PIND1);
	LCDClear();
	LCDWriteString("ROAD AL GET READY...");
	LCDWriteStringXY(0,1,"IN      SECONDS");
	LCDWriteIntXY(3,1,z,4);
	//if (z<10)
	//{
	short_wait();
	//Orange Leds On..GET READY
	PORTC|=(1<<PINC1);
	//}
	//if (z>=7)
	//{
	short_wait_orange();
	//Red Leds
	PORTC &=~(1<<PINC0);
	//Orange Leds Off...GO
	PORTC&=~(1<<PINC1);
	//Info to Drivers
	//}*/
	//if (z<6)
	//{
	LCDClear();
	LCDWriteIntXY(3,1,z,4);
	LCDWriteStringXY(0,0,">ROAD AL MOVING ONLY");
	LCDWriteStringXY(0,1,"IN STOP MODE.!!! ");
	LCDWriteStringXY(0,2,"ROAD B , C & D STOP");
	LCDWriteStringXY(0,3,"DRIVERS GET READY>>>");
	//Green Leds On...MOVE
	PORTC|=(1<<PINC2);
	PORTC&=~(1<<PINC0);
	PORTC&=~(1<<PINC1);
	//}
	//if (z==0)
	//{
	normal_wait();
	//Green leds Off...STOP
	PORTC&=~(1<<PINC2);
	//}
	//_delay_loop_2(0);
	//_delay_loop_2(0);
	//_delay_loop_2(0);
	//_delay_loop_2(0);
	//}
}
