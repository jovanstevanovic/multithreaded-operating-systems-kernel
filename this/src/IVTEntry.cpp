/*
 * IVTEntry.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: Xerox
 */

#include "IVTEntry.h"
#include <dos.h>
#include "Timer.h"

IVTEntry** IVTEntry::IVT = new IVTEntry*[256];

IVTEntry::IVTEntry(IVTNo ivtNo, InterruptRoutine irNew) :ivtNo(0), irOld(0), myKernelEv(0) {
    lockingFlag
	this->ivtNo = ivtNo;
	initialization(irNew);
	IVT[ivtNo] = this;
	unlockingFlag
}

IVTEntry::~IVTEntry() {
	lockingFlag
	restore();
	IVT[ivtNo] = 0;
	ivtNo = -1;
	irOld = 0;
	myKernelEv = 0;
	unlockingFlag
}

void IVTEntry::initialization(InterruptRoutine irNew) {
#ifndef BCC_BLOCK_IGNORE
	irOld = getvect(ivtNo);
	setvect(ivtNo, irNew);
#endif
}

void IVTEntry::restore() {
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, irOld);
#endif
}

IVTEntry* IVTEntry::getIVTEntry(IVTNo ivtNo) {
	return IVTEntry::IVT[ivtNo];
}

void IVTEntry::callOldRoutine() {
	if(irOld != 0)
	   (*irOld)();
}

void IVTEntry::setMyKernelEv(KernelEv* kernelEv) {
	myKernelEv = kernelEv;
}

void IVTEntry::callMyKernelSignal() {
	if(myKernelEv != 0)
	   myKernelEv->signal();
}
