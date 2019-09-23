/*
 * Thread.h
 *
 *  Created on: Apr 10, 2017
 *      Author: Xerox
 */

#ifndef BCC_H_THREAD_H_
#define BCC_H_THREAD_H_

class PCB;

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time;
const Time defaultTimeSlice = 2;
typedef int ID;
typedef void (*SignalHandler)();
typedef unsigned SignalId;

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

// --- SIGNAL ---------------------------------------------------
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
// --------------------------------------------------------------

protected:
	friend class PCB;
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() { }

private:
	PCB* myPCB;
};

void dispatch();

#endif /* BCC_H_THREAD_H_ */
