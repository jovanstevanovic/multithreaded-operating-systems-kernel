/*
 * WaitingList.h
 *
 *  Created on: Apr 12, 2017
 *      Author: Xerox
 */

#ifndef H_WAITLIST_H_
#define H_WAITLIST_H_

#include "PCB.h"

class PCB;

typedef unsigned int Time;

class WaitingList {
public:
		WaitingList();
		~WaitingList();

		void put(PCB* p);
		PCB* get();
		PCB* getWithID(ID id);
		void updateTimes();
		unsigned int listsLength() const;

private:
	struct Elem {
		PCB* pcb;
		Elem* next;
		Elem(PCB* pcb, Elem* next = 0) : pcb(pcb), next(next) { }
	};
		Elem *head, *tail;
		volatile unsigned int numOfWaiters;
};

#endif /* H_WAITLIST_H_ */
