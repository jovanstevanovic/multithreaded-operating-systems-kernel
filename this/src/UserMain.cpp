/*
 * UserMainThread.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: Xerox
 */

#include "UserMain.h"
#include "Thread.h"

int userMain(int argc, char* argv[]);
int status = 0;

UserMainThread::UserMainThread(int argc, char* argv[]) : Thread(16192), argc(argc),
		                                                  argv(argv) { }

UserMainThread::~UserMainThread() {
   waitToComplete();
}

void UserMainThread::run() {
	status = userMain(argc, argv);
}
