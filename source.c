
#include <avr/io.h>    // for ports
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "C:\Users\geozu\Documents\Arduino\DriverLibHW\LCD.h"
#include "C:\Users\geozu\Documents\Arduino\DriverLibHW\Wait.h"
#include "C:\Users\geozu\Documents\Arduino\DriverLibHW\LCDCfg.h"


uint8_t sec = 0;
uint8_t min = 0;
uint8_t hour = 0;
volatile uint8_t output = 0;
char buff[NUM_ROWS + 1];

void menu();

// enable overflow interrupt TIMSK1 |= 0x01; 
 // enable global interrupts sei(); 
ISR(TIMER1_OVF_vect) {	// handle interrupt 	
	TCNT1 = -62500;
	sec++;
	output = 1;

}


int main(void)
{
	DDRD = 0x00;
	PORTD = 0xFF;
	cli();
	// enable overflow interrupt 
	TIMSK1 |= 0x01;
	// enable global interrupts 
	TCNT1 = -62500;   //start value 63536   
	TCCR1A = 0x0;   //normal operation mode   
	TCCR1B = 0x04;   //internal clock(16 MHz),prescaler 8,normal mode,start timer   


	LCDInit(); // Initialisieren des LCD
	LCDClear(); // Loeschen des LCD
	char buffsec[NUM_ROWS + 1];
	char buffmin[NUM_ROWS + 1];
	char buffh[NUM_ROWS + 1];






	LCDChar(2, 10, ':');
	LCDChar(2, 7, ':');
	sei();
	while (1)
	{
		LCDString(1, 5, "Uhrzeit:");

		sprintf(buff, "%02d:%02d:%02d ", hour, min, sec);


		
		if (output == 1) {
			LCDString(2, 5, buff);
			output = 0;
		}
		if (sec == 60)
		{
			sec = 0;
			min++;
			if (min == 60)
			{
				min = 0;
				hour++;
				if (hour == 24)
				{
					hour = 0;
				}
			}

		}
		if (((~PIND & 0x04) >> 2))
		{
			cli();
			menu();
			WaitMs(20);
			sei();
		}
	}

}

void menu()
{
	LCDClear();

	int taster3 = 0;
	int taster2 = 0;

	sec = 0;
	min = 0;
	hour = 0;

	sprintf(buff, "%02d:%02d:%02d ", hour, min, sec);
	WaitMs(1500);
	LCDString(1, 1, "Stunden eingeben ");
	LCDString(2, 5, buff);

	while (!((~PIND & 0x04) >> 2))
	{
		if ((~PIND & 0x08) >> 3)
		{
			if (taster3 != 1)
			{
				hour++;
				if (hour == 24)
				{
					hour = 0;
				}
				sprintf(buff, "%02d:%02d:%02d ", hour, min, sec);
				LCDString(2, 5, buff);
				taster3 = 1;
			}

		}
		else
		{
			taster3 = 0;
		}
	}
	WaitMs(1000);


	LCDString(1, 1, "Minuten eingeben ");
	while (!((~PIND & 0x04) >> 2))
	{
		if ((~PIND & 0x08) >> 3)
		{
			if (taster3 != 1)
			{
				min++;
				if (min == 60)
				{
					min = 0;
				}
				sprintf(buff, "%02d:%02d:%02d ", hour, min, sec);
				LCDString(2, 5, buff);
				taster3 = 1;
			}

		}
		else
		{
			taster3 = 0;
		}
	}

	LCDClear();
}


 
