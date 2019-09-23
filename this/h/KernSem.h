/*
 * KernelSem.h
 *
 *  Created on: Apr 11, 2017
 *      Author: Xerox
 */

#ifndef H_KERNSEM_H_
#define H_KERNSEM_H_

#include "Queue.h"
#include "WaitList.h"

class Queue;
class WaitingList;

class KernelSem {
public:
	KernelSem(int init=1);
	~KernelSem();

	int wait (Time maxTimeToWait);
	void signal();
	int val() const;
	void incValue();
	Queue* blocked;
	static WaitingList* wl;

private:
	volatile int value;
};

#endif /* H_KERNSEM_H_ */
