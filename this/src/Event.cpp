/*
 * Event.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: Xerox
 */

#include "Event.h"
#include "KernelEv.h"
#include "Timer.h"

Event::Event (IVTNo ivtNo) {
#ifndef BCC_BLOCK_IGNORE
	asm pushf
	asm cli
#endif
	myImpl = new KernelEv(ivtNo);
#ifndef BCC_BLOCK_IGNORE
	asm popf
#endif
}

Event::~Event () {
#ifndef BCC_BLOCK_IGNORE
	asm pushf
	asm cli
#endif
	if(myImpl != 0) {
	  delete myImpl;
	  myImpl = 0;
	}
#ifndef BCC_BLOCK_IGNORE
	asm popf
#endif
}

 void Event::wait() {
     myImpl->wait();
 }

void Event::signal() {
	lockingFlag
    myImpl->signal();
	unlockingFlag
}



