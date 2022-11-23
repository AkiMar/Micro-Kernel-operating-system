/*
 * pcb.h
 *
 *  Created on: Sep 15, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "listaPcb.h"
#include "listaSig.h"
#include "listaNit.h"

class Thread;

class PCB {
	static int staticID;
public:
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned status;   // 0-spremna 1-zavrsila 2-blokirana 3-tek kreirana jos nije startovana

	unsigned* stack;
	int preuzmiFlag;
	int timeSlice;
	int semaforRet;
	int id;
	unsigned long stackSize;

	Thread* nit;
	listaPcb blokiraneNiti;

	static void wrapper();

	void probudiSve();
	void dodajListuCekanja(PCB* pcb);
	PCB();
	~PCB();

	//implemetacija za signale
	Thread* tataNit;
	listaSig listaSignala;
	listaNit deca;
	int Signal0Flag;
	static void obradaSignala();

};

#endif /* PCB_H_ */
