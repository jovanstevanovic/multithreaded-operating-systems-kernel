/*
 * Queque.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: Xerox
 */

#include "Queue.h"
#include "PCB.h"
#include "Thread.h"
#include <iostream.h>

Queue::Queue() {
	head = tail = 0;
	numOfElems = 0;
}

void Queue::put(PCB* p) {
	if(head == 0)
		head = tail = new Elem(p);
	else
		tail = tail->next = new Elem(p);
    numOfElems++;
}

PCB* Queue::get() {
	PCB* pcb = 0;
	Elem* temp = 0;
	if (head != 0) {
		temp = head;
		pcb = head->pcb;
		head = head->next;
		if (head == 0)
			tail = 0;
		numOfElems--;
		delete temp;
	}
	return pcb;
}

PCB* Queue::getWithID(ID id) {
	Elem* prev = 0;
	Elem* cur = 0;
	PCB* pcb = 0;
	for (cur = head; cur != 0 && cur->pcb != 0 && cur->pcb->getID() != id; cur = cur->next)
		prev = cur;

	if (cur == 0) return 0;
	numOfElems--; // Element is found!
	pcb = cur->pcb;
	if (prev == 0) {
		head = head->next;
		if (head == 0)
			tail = 0;
	} else {
	    prev->next = cur->next;
	    if (cur == tail)
		    tail = prev;
	}
	delete cur;
	return pcb;
}

PCB* Queue::getCopyWithID(ID id) {
	Elem* cur = 0;
	for (cur = head; cur != 0 && cur->pcb != 0 && cur->pcb->getID() != id; cur = cur->next);
	if (cur == 0) return 0;
	else return cur->pcb;
}

Queue::~Queue() {
	while(head != 0) {
		Elem* old = head;
		head = head->next;
		delete old;
	}
	head = tail = 0;
	numOfElems = 0;
}

unsigned int Queue::listsLength() const {
	return numOfElems;
}




