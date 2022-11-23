/*
 * listaNit.h
 *
 *  Created on: Sep 17, 2020
 *      Author: OS1
 */

#ifndef LISTANIT_H_
#define LISTANIT_H_

class Thread;

struct elemThread {
	Thread* deteNit;
	elemThread* next;
	elemThread(Thread* dete){
		this->deteNit = dete;
		this->next = 0;
	}
};


class listaNit {

public:
	elemThread* glava;
	elemThread* posl;

	listaNit();
	~listaNit();

	void dodajDete(Thread* dete);
	void obrisiDete(Thread* dete);
	void obavestiDecu();

	void deteObrisiRod();

};





#endif /* LISTANIT_H_ */
