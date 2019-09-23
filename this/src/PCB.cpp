/*
 * PCB.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: Xerox
 */

#include "PCB.h"
#include "Queue.h"
#include "States.h"
#include "SCHEDULE.H"
#include "Timer.h"
#include "Reserved.h"
#include "SignalL.h"

PCB* PCB::running = 0;
unsigned int PCB::pidGen = 0;
Queue* PCB::allPCBs = new Queue();
PCB* PCB::PCBtoDelete = 0;

short int* PCB::globalyMasked = new short int[numOfSignals];
short int* PCB::globalyBlocked = new short int[numOfSignals];
SignalL** PCB::globalyBlockedL = new SignalL*[numOfSignals];

static void idleThread() {
	while(1) {
       // Do nothing!
      for(int i = 0; i < 30000; i++)
      	for(int j = 0; j < 30000; j++);
	}
}

// regular PCB constructor
PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThread) : id(++pidGen) {
	if(stackSize > 65536)
		stackSize = 65536; // 65 536 == 2 ^ 16 == 64KB - max stack size value
	this->timeSlice = timeSlice;
	this->stackSize = stackSize;
	this->myThread = myThread;
    blocked = new Queue();
    maxTimeToWait = 0;
    myKernelSem = 0;
    myParent = PCB::running;

    sigHand = new SignalHandler[numOfSignals];
    localyMasked = new short int[numOfSignals];
    localyBlocked = new short int[numOfSignals];
    localyBlockedL = new SignalL();
    readySigL = new SignalL();
    retValue = 0;
    systemSendSignal = 0;

    stackSize /= 2; // stackSize / 2 == length of stacks array
	bp = sp = ss = 0; // temporary value for pointers
	unsigned int* st1 = new unsigned int[stackSize];
	st1[stackSize - 1] = 0x200; // set I flag

    #ifndef BCC_BLOCK_IGNORE
	st1[stackSize - 2] = FP_SEG(PCB::wrapper);
	st1[stackSize - 3] = FP_OFF(PCB::wrapper);
    sp = bp = FP_OFF(st1 + stackSize - 12); // offset for all registers
    ss = FP_SEG(st1 + stackSize - 12);
	#endif

    stack = st1;
	this->state = NEW;
	if (PCB::allPCBs->listsLength() == 0) // creating idle thread
		PCB::allPCBs->put(new PCB());
	PCB::allPCBs->put(this);
	nullInicLclTable();
}

PCB::PCB() : id(-1) {  // PCB for IDLE thread
	this->timeSlice = defaultTimeSlice;
	this->stackSize = defaultStackSize;
	this->myThread = 0;
    blocked        = 0;
    maxTimeToWait  = 0;
    myKernelSem    = 0;
    myParent       = 0;

    sigHand        = 0;
    localyMasked   = 0;
    localyBlocked  = 0;
    localyBlockedL = 0;
    readySigL      = 0;
    retValue       = 0;
    systemSendSignal = 0;

    unsigned int lengthOfArray = defaultStackSize / 2;
	bp = sp = ss = 0; // temporary value for pointers
	unsigned* st1 = new unsigned int[lengthOfArray];
	st1[lengthOfArray - 1] = 0x200; // set I flag

	#ifndef BCC_BLOCK_IGNORE
	st1[lengthOfArray - 2] = FP_SEG(idleThread);
	st1[lengthOfArray - 3] = FP_OFF(idleThread);
	sp = bp = FP_OFF(st1 + lengthOfArray - 12); // offset for all registers
	ss = FP_SEG(st1 + lengthOfArray - 12);
    #endif

    stack = st1;
	this->state = IDLE;
}

PCB::PCB(StackSize stackSize) : id(0) { // PCB for main function
	this->timeSlice = defaultTimeSlice;
	this->stackSize = stackSize;
	this->myThread = 0;
    blocked        = 0;
    maxTimeToWait  = 0;
    myKernelSem    = 0;
    myParent       = 0;

    sigHand = new SignalHandler[numOfSignals];
    localyMasked = new short int[numOfSignals];
    localyBlocked = new short int[numOfSignals];
    localyBlockedL = new SignalL();
    readySigL = new SignalL();
    retValue = 0;
    systemSendSignal = 0;

    bp = sp = ss = 0;
    stack = 0;

	this->state = READY;
	if (PCB::allPCBs->listsLength() == 0) // creating idle thread
		PCB::allPCBs->put(new PCB());
	PCB::allPCBs->put(this);
	nullInicGlTable();
}

PCB::~PCB() {
	if (stack != 0) {
	   delete[] stack;
	   stack = 0;
	}
	if(blocked != 0) {
	   delete blocked;
	   blocked = 0;
	}
	if(localyBlocked != 0) {
	   delete[] localyBlocked;
	   localyBlocked = 0;
	}
	if(localyBlockedL != 0) {
	   delete localyBlockedL;
	   localyBlockedL = 0;
	}
	if(localyMasked != 0) {
	   delete[] localyMasked;
	   localyMasked = 0;
	}
	if(readySigL != 0) {
	   delete readySigL;
	   readySigL = 0;
	}
}

void PCB::exitThread() {
	PCB::running->state = FINISHED;
}

void PCB::wrapper() {
	PCB::running->myThread->run();
    lockingFlag
	if(PCB::running->myParent != 0) {
		PCB::running->myParent->systemSendSignal = 1;
		PCB::running->myParent->signal(1);           // 1 - finishing signal from child's to parent
    }
    PCB::running->systemSendSignal = 1;
    PCB::running->signal(2);                         // 2 - finishing signal to this object
	while(PCB::running != 0 && PCB::running->blocked != 0 && PCB::running->blocked->listsLength() > 0) {
		PCB* temp = PCB::running->blocked->get();
		temp->state = READY;
		Scheduler::put(temp);
	}
    if(PCB::running != 0)
	  PCB::running->exitThread();
	unlockingFlag
	dispatch();
}

void PCB::waitToComplete() {
	 lockingFlag
     if(this->state != NEW && this->state != FINISHED && PCB::running != this) {
    	 PCB::running->state = BLOCKED;
    	 blocked->put(PCB::running);
    	 unlockingFlag
    	 dispatch();
     } else
    	 unlockingFlag
}

void dump() {
	idleThread(); // Never used!
}

ID PCB::getID() const {
	return id;
}

void PCB::start() {
	if(this->state == NEW) {
	   this->state = READY;
	   Scheduler::put(this);
	}
}

void PCB::signal(SignalId signal) {
	if(PCB::running == this && signal == RESERVED_0)
		PCB::FirstSignalHandler();
	if((signal == RESERVED_1 && systemSendSignal == 0) || (signal == RESERVED_2 && systemSendSignal == 0))
		return;
	if(localyMasked[signal] == 1 || PCB::globalyMasked[signal] == 1)
		return;
	if(localyBlocked[signal] == 1) {
		localyBlockedL->put(this->id, signal);
		return;
	}
	if(PCB::globalyBlocked[signal] == 1) {
		PCB::globalyBlockedL[signal]->put(this->id, signal);
		return;
	}
	if(this->state == WAITING_FOR_SIGNAL) {
		this->state = READY;
        Scheduler::put(this);
	}
	if(signal == RESERVED_2 && systemSendSignal == 1) {
		if(sigHand[signal] != 0)
		  (*sigHand[signal])();
	}
    else
	    readySigL->put(this->id, signal);
	systemSendSignal = 0;
}

void PCB::registerHandler(SignalId signal, SignalHandler handler) {
	if(validSignalID(signal))
		sigHand[signal] = handler;
}

SignalHandler PCB::getHandler(SignalId signal) {
    return sigHand[signal];
}

void PCB::maskSignal(SignalId signal) {
	localyMasked[signal] = 1;
}

void PCB::maskSignalGlobally(SignalId signal) {
	PCB::globalyMasked[signal] = 1;
}

void PCB::unmaskSignal(SignalId signal) {
	localyMasked[signal] = 0;
}

void PCB::unmaskSignalGlobally(SignalId signal) {
	PCB::globalyMasked[signal] = 0;
}

void PCB::blockSignal(SignalId signal) {
	localyBlocked[signal] = 1;
}

void PCB::blockSignalGlobally(SignalId signal) {
	PCB::globalyBlocked[signal] = 1;
}

void PCB::unblockSignal(SignalId signal) {
	localyBlocked[signal] = 0;
	if(localyBlockedL == 0) return;
	SignalL::Signal* el = localyBlockedL->getWithSignalsID(signal);
	while(el != 0) {
		if(PCB::globalyBlocked[signal] == 0) {
			PCB* temp = PCB::allPCBs->getCopyWithID(el->pcbID);
			if(temp != 0 && temp->state == WAITING_FOR_SIGNAL) {
				temp->state = READY;
				Scheduler::put(temp);
		    }
	        readySigL->putElem(el);
		}
		else
		   PCB::globalyBlockedL[signal]->putElem(el);
		if(localyBlockedL->listsLength() > 0)
		   el = localyBlockedL->getWithSignalsID(signal);
		else
		   el = 0;
	}
}

void PCB::unblockSignalGlobally(SignalId signal) {
	PCB::globalyBlocked[signal] = 0;
	for(SignalL* cur = PCB::globalyBlockedL[signal]; cur != 0 && cur->listsLength() > 0;) {
	    SignalL::Signal* el = cur->get();
	    if(el != 0) {
	    	PCB* temp = PCB::allPCBs->getCopyWithID(el->pcbID);
	    	if(temp != 0) {
	    	   if(temp->localyBlocked[signal] == 0) {
	    		  if(temp->state == WAITING_FOR_SIGNAL) {
	    			  temp->state = READY;
	    			  Scheduler::put(temp);
	    		  }
	              temp->readySigL->putElem(el);
	    	   }
	    	   else
	    		  temp->localyBlockedL->putElem(el);
	    	}
	    }
	}
}

void PCB::pause() {
	  lockingFlag
	  PCB::running->state = WAITING_FOR_SIGNAL;
	  unlockingFlag
	  dispatch();
}

short int PCB::validSignalID(SignalId signal) {
	return signal != 0;
}

void PCB::checkSignalList() {
	#ifndef BCC_BLOCK_IGNORE
    asm sti
    #endif
	lockingFlag
	while(readySigL != 0 && readySigL->listsLength() > 0) {
		SignalL::Signal* el = readySigL->get();
		if(el->sigID == 0) {
          #ifndef BCC_BLOCK_IGNORE
		    asm pushf
		    asm cli
          #endif
            delete el;
          #ifndef BCC_BLOCK_IGNORE
		    asm popf
          #endif
			PCB::FirstSignalHandler();
		} else {
		   if(sigHand[el->sigID] != 0)
			  (*sigHand[el->sigID])();
           #ifndef BCC_BLOCK_IGNORE
	          asm pushf
	          asm cli
           #endif
              delete el;
           #ifndef BCC_BLOCK_IGNORE
	          asm popf
           #endif
		}
	}
    unlockingFlag
}

void PCB::nullInicGlTable() {
	for(int i = 0; i < numOfSignals; i++) {
		PCB::globalyBlocked[i] = 0;
		PCB::globalyMasked[i] = 0;
		PCB::globalyBlockedL[i] = new SignalL();
	}
}

void PCB::nullInicLclTable() {
	for(int i = 0; i < numOfSignals; i++) {
		sigHand[i] = 0;
		localyBlocked[i] = 0;
		localyMasked[i] = 0;
	}
}

void PCB::FirstSignalHandler() {
	lockingFlag
	if(PCB::running->myParent != 0) {
		PCB::running->myParent->systemSendSignal = 1;
		PCB::running->myParent->signal(1);           // 1 - finishing signal from child's to parent
    }
    PCB::running->systemSendSignal = 1;
    PCB::running->signal(2);                         // 2 - finishing signal to this object
	PCB::running->state = FINISHED;
	PCB::PCBtoDelete = PCB::running;
	while(PCB::running != 0 && PCB::running->blocked != 0 && PCB::running->blocked->listsLength() > 0) {
		PCB* temp = PCB::running->blocked->get();
		temp->state = READY;
		Scheduler::put(temp);
	}
	unlockingFlag
	dispatch();
}



