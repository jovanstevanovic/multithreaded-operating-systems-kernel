/*
 * Queue.h
 *
 *  Created on: Apr 10, 2017
 *      Author: Xerox
 */

#ifndef BCC_H_QUEQUE_H_
#define BCC_H_QUEQUE_H_

#include "PCB.h"
#include "Thread.h"

class Queue {
public:
	Queue();
	~Queue();

	void put(PCB* p);
	PCB* get();
	PCB* getWithID(ID id);
	PCB* getCopyWithID(ID id);
	inline unsigned int listsLength() const;

private:
	struct Elem {
		PCB* pcb;
		Elem* next;
		Elem(PCB* pcb, Elem* next = 0) : pcb(pcb), next(next) { }
	};
	Elem *head, *tail;
	volatile unsigned int numOfElems;
};

#endif /* BCC_H_QUEQUE_H_ */
