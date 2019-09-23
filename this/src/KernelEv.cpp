/*
 * KernelEv.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: Xerox
 */

#include "KernelEv.h"
#include "States.h"
#include "SCHEDULE.H"
#include "Timer.h"

void dispatch();

KernelEv::KernelEv(IVTNo ivtNo) {
    value = 0;
    myPCB = PCB::running;
    IVTEntry::getIVTEntry(ivtNo)->setMyKernelEv(this);
    this->ivtNo = ivtNo;
}

KernelEv::~KernelEv() {
	signal();
	IVTEntry::getIVTEntry(ivtNo)->setMyKernelEv(0);
}

void KernelEv::wait() {
	lockingFlag
	if (PCB::running == myPCB) {
		if (value == 0) {
           myPCB->state = BLOCKED;
           unlockingFlag
           dispatch();
		} else {
		   value = 0;
		   unlockingFlag
		}
	}
}

void KernelEv::signal() {
	if (myPCB->state == BLOCKED) {
	    myPCB->state = READY;
	    Scheduler::put(myPCB);
	} else
		value = 1;
}
