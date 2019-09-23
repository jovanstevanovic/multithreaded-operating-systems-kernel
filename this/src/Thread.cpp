/*
 * Thread.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: Xerox
 */

#include "Thread.h"
#include "PCB.h"
#include "Timer.h"
#include "SCHEDULE.H"

Thread::Thread(StackSize stackSize, Time timeSlice) {
#ifndef BCC_BLOCK_IGNORE
     asm pushf
	 asm cli
#endif
    myPCB = new PCB(stackSize, timeSlice, this);
#ifndef BCC_BLOCK_IGNORE
     asm popf
#endif
}

ID Thread::getId() {
	lockingFlag
	ID id =  myPCB->getID();
	unlockingFlag
	return id;
}

ID Thread::getRunningId() {
	lockingFlag
	ID id = PCB::running->getID();
	unlockingFlag
	return id;
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

Thread* Thread::getThreadById(ID id) {
	lockingFlag
	PCB* temp = PCB::allPCBs->getCopyWithID(id);
	if (temp == 0) {
		unlockingFlag
		return 0;
	}
	else {
		unlockingFlag
		return temp->myThread;
	}
}

void Thread::start() {
	lockingFlag
	myPCB->start();
	unlockingFlag
}

Thread::~Thread() {
	waitToComplete();
	lockingFlag
	if(myPCB != 0) {
	  PCB::allPCBs->getWithID(myPCB->getID());
#ifndef BCC_BLOCK_IGNORE
     asm pushf
	 asm cli
#endif
	  delete myPCB;
#ifndef BCC_BLOCK_IGNORE
     asm popf
#endif
	  myPCB = 0;
	}
	unlockingFlag
}

void Thread::signal(SignalId signal) {
	lockingFlag
	myPCB->signal(signal);
	unlockingFlag
}


void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	lockingFlag
	myPCB->registerHandler(signal, handler);
	unlockingFlag
}

SignalHandler Thread::getHandler(SignalId signal) {
	lockingFlag
	SignalHandler sh =  myPCB->getHandler(signal);
	unlockingFlag
	return sh;
}

void Thread::maskSignal(SignalId signal) {
	lockingFlag
	myPCB->maskSignal(signal);
	unlockingFlag
}

void Thread::maskSignalGlobally(SignalId signal) {
	lockingFlag
	PCB::maskSignalGlobally(signal);
	unlockingFlag
}

void Thread::unmaskSignal(SignalId signal) {
	lockingFlag
	myPCB->unmaskSignal(signal);
	unlockingFlag
}

void Thread::unmaskSignalGlobally(SignalId signal) {
	lockingFlag
	PCB::unmaskSignalGlobally(signal);
	unlockingFlag
}

void Thread::blockSignal(SignalId signal) {
	lockingFlag
	myPCB->blockSignal(signal);
	unlockingFlag
}

void Thread::blockSignalGlobally(SignalId signal) {
	lockingFlag
	PCB::blockSignalGlobally(signal);
	unlockingFlag
}

void Thread::unblockSignal(SignalId signal) {
	lockingFlag
	myPCB->unblockSignal(signal);
	unlockingFlag
}

void Thread::unblockSignalGlobally(SignalId signal) {
	lockingFlag
	PCB::unblockSignalGlobally(signal);
	unlockingFlag
}

void Thread::pause() {
	PCB::pause();
}

