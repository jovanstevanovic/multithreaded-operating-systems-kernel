/*
 * WaitingList.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: OS1
 */

#include "WaitList.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "Timer.h"

WaitingList::WaitingList() {
	head = tail = 0;
	numOfWaiters = 0;
}

WaitingList::~WaitingList() {
	while(head != 0) {
		Elem* old = head;
		head = head->next;
		delete old;
	}
	head = tail = 0;
	numOfWaiters = 0;
}

void WaitingList::put(PCB* p) {
	if(p == 0) return;
	Elem* cur = 0, *prev = 0, *newEl = 0;
	Time sum = 0;
    Time temp = p->maxTimeToWait;
    for(cur = head; cur != 0 && cur->pcb != 0 && (sum + cur->pcb->maxTimeToWait <= temp); cur = cur->next) {
    	prev = cur;
    	sum += cur->pcb->maxTimeToWait;
    }

    if (cur == 0 && prev == 0)
        head = tail = new Elem(p);
    else {
    	p->maxTimeToWait -= sum;
    	if(prev == 0)
    		head = newEl = new Elem(p, head);
    	else
    		if (prev == tail)
    		    tail = tail->next = new Elem(p);
   		    else
                prev->next = newEl = new Elem(p, cur);
    	if(newEl->next != 0 && newEl->next->pcb != 0)
    		newEl->next->pcb->maxTimeToWait -= newEl->pcb->maxTimeToWait;
    }
    numOfWaiters++;
}

PCB* WaitingList::get() { // No need to clear interrupt
	PCB* curr = 0;
	Elem* temp = 0;
	if (head != 0) {
		temp = head;
		curr = head->pcb;
		head = head->next;
		if (head == 0)
			tail = 0;
		delete temp;
		numOfWaiters--;
	}
	return curr;
}

unsigned int WaitingList::listsLength() const {
	return numOfWaiters;
}

void WaitingList::updateTimes() {
	if(head != 0 && head->pcb != 0) {
	    --head->pcb->maxTimeToWait;
	    while(head != 0 && head->pcb != 0 && head->pcb->maxTimeToWait == 0) {
		   PCB* formerHead = get();
		   if(formerHead != 0 && formerHead->myKernelSem != 0 && formerHead->myKernelSem->blocked != 0) {
			  formerHead->state = READY;
			  formerHead->retValue = 0;
		      formerHead->myKernelSem->blocked->getWithID(formerHead->getID());
		      formerHead->myKernelSem->incValue();
		      formerHead->myKernelSem = 0;
		      Scheduler::put(formerHead);
		   }
	    }
	}
}

PCB* WaitingList::getWithID(ID id) { // No need to clear interrupt
	Elem* prev = 0;
	Elem* cur = 0;
	PCB* pcb = 0;
	for (cur = head; cur != 0 && cur->pcb != 0 && cur->pcb->getID() != id; cur = cur->next)
		prev = cur;

	if (cur == 0) return 0;
	numOfWaiters--; // Element is found!
	pcb = cur->pcb;
	if (prev == 0) {
		head = head->next;
		if (head == 0)
			tail = 0;
		else {
			if(head->pcb != 0)
			  head->pcb->maxTimeToWait += cur->pcb->maxTimeToWait;
		}
	} else {
	    prev->next = cur->next;
	    if (cur == tail)
		   tail = prev;
	    else {
	    	if(cur->pcb != 0 && cur->next->pcb != 0)
	           cur->next->pcb->maxTimeToWait += cur->pcb->maxTimeToWait;
	    }
	}
	delete cur;
	return pcb;
}




