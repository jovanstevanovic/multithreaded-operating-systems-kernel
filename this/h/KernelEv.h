/*
 * KernelEv.h
 *
 *  Created on: Apr 13, 2017
 *      Author: Xerox
 */

#ifndef H_KERNELEV_H_
#define H_KERNELEV_H_

#include "Event.h"
#include "IVTEntry.h"
#include "PCB.h"

class Event;
class IVTEntry;
class PCB;

typedef unsigned char IVTNo;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

    void wait  ();
    void signal();

private:
    PCB* myPCB;
    unsigned int value;
    IVTNo ivtNo;
};

#endif /* H_KERNELEV_H_ */
