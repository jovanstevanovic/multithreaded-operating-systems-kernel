/*
 * States.h
 *
 *  Created on: May 1, 2017
 *      Author: Xerox
 */

#ifndef H_STATES_H_
#define H_STATES_H_

enum States {  // 0-new, 1-ready, 2-running, 3-blocked, 4-finished, 5-idle, 6-waiting for signal
	NEW, READY, RUNNING, BLOCKED, FINISHED, IDLE, WAITING_FOR_SIGNAL
};

#endif /* H_STATES_H_ */
