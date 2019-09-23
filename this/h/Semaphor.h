/*
 * Semaphor.h
 *
 *  Created on: Apr 11, 2017
 *      Author: Xerox
 */

#ifndef H_SEMAPHOR_H_
#define H_SEMAPHOR_H_

class KernelSem;

typedef unsigned int Time;

class Semaphore {
public:
	Semaphore (int init = 1);
	virtual ~Semaphore ();

    virtual int wait (Time maxTimeToWait);
    virtual void signal();
    int val () const;  // Returns the current value of the semaphore

private:
    KernelSem* myImpl;
};

#endif /* H_SEMAPHOR_H_ */
