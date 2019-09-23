/*
 * IVTEntry.h
 *
 *  Created on: Apr 13, 2017
 *      Author: Xerox
 */

#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

#include "KernelEv.h"

class KernelEv;

typedef unsigned char IVTNo;
typedef void interrupt (*InterruptRoutine) (...);

class IVTEntry {
public:
	IVTEntry(IVTNo No, InterruptRoutine ir);
	~IVTEntry();

	void callOldRoutine();
	void setMyKernelEv(KernelEv* kernelEv);
	void callMyKernelSignal();
	static IVTEntry* getIVTEntry(IVTNo);
	static IVTEntry** IVT;

private:
	void initialization(InterruptRoutine irNew);
	void restore();

	IVTNo ivtNo;
	InterruptRoutine irOld;
    KernelEv* myKernelEv;
};

#endif /* H_IVTENTRY_H_ */
