/*
 * UserMainThread.h
 *
 *  Created on: Apr 12, 2017
 *      Author: Xerox
 */

#ifndef H_USERMAIN_H_
#define H_USERMAIN_H_

#include "Thread.h"

class Thread;

class UserMainThread : public Thread {
public:
    UserMainThread(int argc, char* argv[]);
    ~UserMainThread();

	void run();

private:
	int argc;
    char** argv;
};

#endif /* H_USERMAIN_H_ */
