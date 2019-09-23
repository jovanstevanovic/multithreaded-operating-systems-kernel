/*
 * Timer.h
 *
 *  Created on: Apr 10, 2017
 *      Author: Xerox
 */

#ifndef BCC_H_TIMER_H_
#define BCC_H_TIMER_H_

extern volatile int lockFlag;
extern volatile int requiredContextSwitch;

#define lock asm pushf;\
	         asm cli;
#define unlock asm popf
#define lockingFlag lockFlag = 1;
#define unlockingFlag lockFlag = 0;\
	                  if (requiredContextSwitch) {\
				          dispatch();\
			           }

void inic();
void restore();
void interrupt timer();

#endif /* BCC_H_TIMER_H_ */
