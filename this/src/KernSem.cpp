/*
 * KernelSem.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: Xerox
 */

#include "KernSem.h"
#include "SCHEDULE.H"
#include "WaitList.h"
#include "Queue.h"
#include "WaitList.h"
#include <iostream.h>

WaitingList* KernelSem::wl = new WaitingList();

KernelSem::KernelSem(int value) : value(value), blocked(new Queue()) { }

KernelSem::~KernelSem() { // Erasing KernelSem
    while(blocked != 0 && blocked->listsLength() > 0)
	      signal();
     blocked = 0;
}

int KernelSem::wait (Time maxTimeToWait) {
	 PCB::running->retValue = 1;
     if (--value < 0) {
    	PCB::running->maxTimeToWait = maxTimeToWait;
    	PCB::running->state = BLOCKED;
        if (maxTimeToWait > 0) {
        	PCB::running->myKernelSem = this;
        	KernelSem::wl->put(PCB::running);
        }
        blocked->put(PCB::running);
	    dispatch();
     }
	 return PCB::running->retValue;
}

void KernelSem::signal() {
     if(value++ < 0) {
       PCB* temp = blocked->get();
       if (temp != 0) {
    	   if (temp->maxTimeToWait > 0)
    		   KernelSem::wl->getWithID(temp->getID());
    	   temp->state = READY;
    	   Scheduler::put(temp);
       }
     }
}

int KernelSem::val() const {
	return value;
}

void KernelSem::incValue() {
	value++;
}




