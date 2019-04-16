#include <Arduino.h>

/*
 * timer.c
 *
 *  Created on: Jan 30, 2019
 *      Author: radug
 */

#include <stdio.h>
#include "timer.h"

//volatile int num=70;//value displayed


// The loop function is called in an endless loop >>>sutimi<<<

softtimer_t timers[10]={};
void softtimer_handler(void)
{
	static uint32_t prev_micros=0;

	if ((micros()-prev_micros) >= 1000)
	{
		prev_micros=micros();
		for (uint8_t i=0;i<10;i++)
		{
			if ((timers[i].state!=off) && (timers[i].callback!=NULL))
			{
					if (timers[i].tmicros>0)
					{
						timers[i].tmicros=timers[i].tmicros-1;
					}
					else
					{
						timers[i].callback(i);
						if(timers[i].type==oneshot)
						{
							timers[i].state=off;
							timers[i].callback=NULL;
						}
						else
						{
							timers[i].tmicros=timers[i].init_tmicros;
						}
					}
			}

		}
	}

}
uint8_t register_timer(void(*func)(void *args),void *args, uint16_t period_1ms, uint16_t period_init_1ms, timer_t1 type, states_t state)
{
	for (uint8_t i=0;i<10;i++)
	{
		if (timers[i].callback==NULL)
		{
			timers[i].callback=func;
			timers[i].args = args;
			timers[i].tmicros=period_1ms;
			timers[i].init_tmicros=period_init_1ms;
			timers[i].type=type;
			timers[i].state=state;
			return i;
		}
	}
	return 255;
}
