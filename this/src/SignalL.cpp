/*
 * SignalL.cpp
 *
 *  Created on: May 6, 2017
 *      Author: Xerox
 */

#include "SignalL.h"

SignalL::SignalL() {
	head = tail = 0;
	numOfElems = 0;
}

SignalL::~SignalL() {
	while(head != 0) {
		Signal* old = head;
		head = head->next;
		delete old;
	}
	head = tail = 0;
	numOfElems = 0;
}

void SignalL::put(ID pcbID, ID sigID) {
	Signal* newEl = new Signal(pcbID, sigID);
	if(head == 0)
		head = tail = newEl;
	else
		tail = tail->next = newEl;
    numOfElems++;
}

void SignalL::putElem(Signal* newEl) {
	if(head == 0)
		head = tail = newEl;
	else
		tail = tail->next = newEl;
    numOfElems++;
}

SignalL::Signal* SignalL::getWithPCBsID(ID pcbID) {
	Signal* prev = 0;
	Signal* cur = 0;
	for (cur = head; cur != 0 && cur->pcbID != pcbID; cur = cur->next)
		prev = cur;

	if (cur == 0) return 0;
	numOfElems--; // Element is found!
	if (prev == 0) {
		head = head->next;
		if (head == 0)
			tail = 0;
	} else {
	    prev->next = cur->next;
	    if (cur == tail)
		    tail = prev;
	}
	return cur;
}

SignalL::Signal* SignalL::get() {
	Signal* temp = 0;
	if (head != 0) {
		temp = head;
		head = head->next;
		if (head == 0)
			tail = 0;
		numOfElems--;
	}
	return temp;
}

SignalL::Signal* SignalL::getWithSignalsID(SignalId signal) {
	Signal* prev = 0;
	Signal* cur = 0;
	for (cur = head; cur != 0 && cur->sigID != signal; cur = cur->next)
		prev = cur;

	if (cur == 0) return 0;
	numOfElems--; // Element is found!
	if (prev == 0) {
		head = head->next;
		if (head == 0)
			tail = 0;
	} else {
	    prev->next = cur->next;
	    if (cur == tail)
		    tail = prev;
	}
	return cur;
}

unsigned int SignalL::listsLength() const {
	return numOfElems;
}




