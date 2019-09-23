/*
 * Main.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: Xerox
 */

#include "PCB.h"
#include "Timer.h"
#include "UserMain.h"

extern int status;

int main(int argc, char* argv[]) {

	inic();

	PCB::running = new PCB(0); // 0 is random value

	UserMainThread* userMainThr = new UserMainThread(argc, argv);

	userMainThr->start();

	delete userMainThr;

	restore();

	return status;
}




