/*
 * Semaphor.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: Xerox
 */

#include "Semaphor.h"
#include "KernSem.h"

Semaphore::Semaphore (int initValue) {
#ifndef BCC_BLOCK_IGNORE
     asm pushf
	 asm cli
#endif
	myImpl = new KernelSem(initValue);
#ifndef BCC_BLOCK_IGNORE
     asm popf
#endif
}

Semaphore::~Semaphore () {
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

int Semaphore::wait (Time maxTimeToWait) {
#ifndef BCC_BLOCK_IGNORE
     asm pushf
	 asm cli
#endif
    int returnValue =  myImpl->wait(maxTimeToWait);
#ifndef BCC_BLOCK_IGNORE
     asm popf
#endif
	 return returnValue;
}

void Semaphore::signal() {
#ifndef BCC_BLOCK_IGNORE
     asm pushf
	 asm cli
#endif
    myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
     asm popf
#endif
}

int Semaphore::val () const {
	return myImpl->val();
}



