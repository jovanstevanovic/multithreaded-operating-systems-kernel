/*
 * SignalL.h
 *
 *  Created on: May 6, 2017
 *      Author: Xerox
 */

#ifndef H_SIGNALL_H_
#define H_SIGNALL_H_

typedef int ID;
typedef unsigned SignalId;
struct Signal;

class SignalL {
public:
	SignalL();
	~SignalL();

	struct Signal {
		ID pcbID;
		ID sigID;
		Signal* next;
		Signal(ID pcbID, ID sigID, Signal* next = 0) {
			this->pcbID = pcbID;
			this->sigID = sigID;
			this->next = next;
		}
	};

	void putElem(Signal* el);
	void put(ID pcbID, ID sigID);
	Signal* get();
	Signal* getWithSignalsID(SignalId signal);
	Signal* getWithPCBsID(ID pcbID);
	inline unsigned int listsLength() const;

private:
	Signal* head, *tail;
	volatile unsigned int numOfElems;
};

#endif /* H_SIGNALL_H_ */
