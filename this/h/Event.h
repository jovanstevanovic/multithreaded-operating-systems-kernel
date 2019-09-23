/*
 * Event.h
 *
 *  Created on: Apr 13, 2017
 *      Author: Xerox
 */

#ifndef H_EVENT_H_
#define H_EVENT_H_

#include "Macro.h"

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();

    void wait  ();

protected:
    friend class KernelEv;
    void signal(); // can call KernelEv

private:
    KernelEv* myImpl;
};

#endif /* H_EVENT_H_ */
