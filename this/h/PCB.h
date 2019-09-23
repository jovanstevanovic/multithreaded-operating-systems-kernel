/*
 * PCB.h
 *
 *  Created on: Apr 10, 2017
 *      Author: Xerox
 */

#ifndef BCC_H_PCB_H_
#define BCC_H_PCB_H_

#include "Thread.h"
#include "States.h"
#include "KernSem.h"
#include "SignalL.h"
#include <dos.h>
#include "Queue.h"

class Queue;
class KernelSem;

const unsigned int numOfSignals = 16; // 0..15

class PCB {
public:
	unsigned int ss;
	unsigned int sp;
	unsigned int bp;
	Time timeSlice;
	unsigned int* stack;
	StackSize stackSize;
    States state; // 0-new, 1-ready, 2-running, 3-blocked, 4-finished, 5 - idle, 6-waiting for signal
    static PCB* running;
    Thread* myThread;
    static Queue* allPCBs;
    Queue* blocked;
    Time maxTimeToWait;
    KernelSem* myKernelSem;
    PCB* myParent;
    short int retValue;
    short int systemSendSignal;
    static PCB* PCBtoDelete;

    PCB(StackSize stackSize, Time timeSlice, Thread* myThread);
    PCB(StackSize stackSize);
	~PCB();

	ID getID() const;
	void exitThread();
	static void wrapper();
	void waitToComplete();
	void start();

// --- SIGNAL'S INTERFACE ---------------------------------------
	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	SignalHandler getHandler(SignalId signal);
	void maskSignal(SignalId signal);
	static void maskSignalGlobally(SignalId signal);
	void unmaskSignal(SignalId signal);
    static void unmaskSignalGlobally(SignalId signal);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
	static void pause();
	static short int validSignalID(SignalId signal);
	void checkSignalList();
	static void FirstSignalHandler();
// ---------------------------------------------------------------
// --- SIGNAL'S DATE STRUCTURES ----------------------------------
	       SignalHandler* sigHand;
	static short int*     globalyMasked;
	static short int*     globalyBlocked;
	       short int*     localyMasked;
	       short int*     localyBlocked;
	static SignalL**      globalyBlockedL;
	       SignalL*       localyBlockedL;
	       SignalL*       readySigL;
// ----------------------------------------------------------------

private:
	PCB();
	void nullInicGlTable();
	void nullInicLclTable();
	static unsigned int pidGen;
	const ID id;
};

void dispatch();
static void idleThread();
int userMain (int argc, char* argv[]);

#endif /* BCC_H_PCB_H_ */
