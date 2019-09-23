/*
 * Macro.h
 *
 *  Created on: Apr 14, 2017
 *      Author: Xerox
 */

#ifndef H_MACRO_H_
#define H_MACRO_H_

#include "IVTEntry.h"

class IVTEntry;
class KernelEv;

#define PREPAREENTRY(IVTNo, callOldRout)\
void interrupt interruptRoutine##IVTNo(...);\
IVTEntry newEntry##IVTNo(IVTNo, interruptRoutine##IVTNo);\
void interrupt interruptRoutine##IVTNo(...){\
	if (callOldRout == 1)\
	   IVTEntry::IVT[IVTNo]->callOldRoutine();\
IVTEntry::IVT[IVTNo]->callMyKernelSignal();\
}

#endif /* H_MACRO_H_ */
