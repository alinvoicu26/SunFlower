/*
 * timer.h
 *
 *  Created on: Jan 29, 2019
 *      Author: radug
 */

#ifndef TIMER_H_
#define TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
	off=0,
	started,
}states_t;

typedef enum
{
	periodic=0,
	oneshot
}timer_t1;


typedef struct
{
	void (*callback)(void *args);
	void *args;
	uint16_t tmicros;
	uint16_t init_tmicros;
	timer_t1 type;
	states_t state;

}softtimer_t;

extern void softtimer_handler(void);
extern uint8_t register_timer(void(*func)(void *args),void *args, uint16_t period_1ms, uint16_t period_init_1ms, timer_t1 type, states_t state);


#ifdef __cplusplus
}
#endif
#endif /* TIMER_H_ */
