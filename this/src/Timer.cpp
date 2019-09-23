/*
 * Timer.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: Xerox
 */

#include "Timer.h"
#include "PCB.h"
#include "KernSem.h"
#include "SCHEDULE.H"
#include "States.h"
#include <dos.h>

unsigned int tsp;
unsigned int tss;
unsigned int tbp;

volatile int counter = 20;
volatile int lockFlag = 0;
volatile int requiredContextSwitch = 0;
unsigned oldTimerOFF, oldTimerSEG;

void tick();

void interrupt timer() {
	if (!requiredContextSwitch) {
		tick();
		if(counter > 0)
		   counter--;
#ifndef BCC_BLOCK_IGNORE
    	asm int 60h;
#endif
        KernelSem::wl->updateTimes();
	}
	if ((PCB::running->timeSlice > 0 && counter == 0) || requiredContextSwitch) {
		if(lockFlag == 0) {
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tsp, sp // save sp
			mov tss, ss
			mov tbp, bp;
		}
#endif
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if (PCB::running->state == READY)
		   Scheduler::put(PCB::running);

		PCB::running = Scheduler::get();

        if (PCB::running == 0)
        	PCB::running = PCB::allPCBs->getCopyWithID(-1); // -1 is idles ID

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		counter = PCB::running->timeSlice;

#ifndef BCC_BLOCK_IGNORE
		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp;
		}
#endif

		if(PCB::PCBtoDelete != 0) {
			if (PCB::PCBtoDelete->stack != 0) {
				 delete[] PCB::PCBtoDelete->stack;
				 PCB::PCBtoDelete->stack = 0;
			}
			if(PCB::PCBtoDelete->blocked != 0) {
				delete PCB::PCBtoDelete->blocked;
				PCB::PCBtoDelete->blocked = 0;
			}
			if(PCB::PCBtoDelete->localyBlocked != 0) {
				delete[] PCB::PCBtoDelete->localyBlocked;
				PCB::PCBtoDelete->localyBlocked = 0;
			}
			if(PCB::PCBtoDelete->localyBlockedL != 0) {
				delete PCB::PCBtoDelete->localyBlockedL;
				PCB::PCBtoDelete->localyBlockedL = 0;
			}
			if(PCB::PCBtoDelete->localyMasked != 0) {
				delete[] PCB::PCBtoDelete->localyMasked;
				PCB::PCBtoDelete->localyMasked = 0;
			}
			if(PCB::PCBtoDelete->readySigL != 0) {
				delete PCB::PCBtoDelete->readySigL;
				PCB::PCBtoDelete->readySigL = 0;
			}
		    PCB::PCBtoDelete = 0;
		}
		requiredContextSwitch = 0;
		PCB::running->checkSignalList();
	    }
	}
}

void dispatch(){
#ifndef BCC_BLOCK_IGNORE
	asm pushf
	asm cli;
	requiredContextSwitch = 1;
	asm int 8h
	asm popf
#endif
}

void inic(){
#ifndef BCC_BLOCK_IGNORE
	asm{
		pushf
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		popf
	}
#endif
}

void restore(){
#ifndef BCC_BLOCK_IGNORE
	asm {
		pushf
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		popf
	}
#endif
}

