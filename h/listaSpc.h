/*
 * listaSpc.h
 *
 *  Created on: Sep 16, 2020
 *      Author: OS1
 */

#ifndef LISTASPC_H_
#define LISTASPC_H_

typedef unsigned int Time;

#include "kernel.h"

class PCB;

struct elemPcbSem {
	PCB* pcb;
	elemPcbSem* next;
	unsigned int vremeCekanja;
	int flagBlokade; // ako je 1 znaci neograniceno ceka 0 ima vreme cekanja
	elemPcbSem(PCB* pcb, Time vremeCekanja){
		this->pcb = pcb;
		this->next = 0;
		this->vremeCekanja = vremeCekanja;
		this->flagBlokade = !vremeCekanja;
	}
};


class listaPcbSem {

public:
	elemPcbSem* glava;
	elemPcbSem* posl;

	listaPcbSem();
	~listaPcbSem();

	void dodaj(PCB* pcb,Time vremeCekanja );
	PCB* izbaciPrvi();

	void decVreme();

};

#endif /* LISTASPC_H_ */
