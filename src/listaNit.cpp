/*
 * listaNit.cpp
 *
 *  Created on: Sep 17, 2020
 *      Author: OS1
 */

#include "listaNit.h"
#include "thread.h"
#include "pcb.h"

listaNit::listaNit():glava(0),posl(0){ }

listaNit::~listaNit(){
	this->posl = glava;
	while(glava != 0){
		posl = glava;
		glava = glava->next;
		delete posl;
	}
	glava = 0;
	posl = 0;
}

void listaNit::dodajDete(Thread* dete){
	elemThread* novi = new elemThread(dete);
	if( glava == 0){
		glava = novi;
		posl = novi;
	}else{
		posl->next = novi;
		posl = posl->next;
	}
}


void listaNit::obrisiDete(Thread* dete){
	elemThread* pok = glava;
	elemThread* q = glava;
		while (pok != 0 && (pok->deteNit != dete) ) {
			q = pok;
			pok = pok->next;
		}
		if( pok == 0){
			return;
		}
		if (pok == glava) {
			glava = glava->next;
			if (glava == 0) posl = 0;
		}
		else {
			if (pok == posl) {
				posl = q;
				posl->next = 0;
			}
			else {
				q->next = pok->next;
			}
		}

		delete pok;
}

void listaNit::obavestiDecu(){
	elemThread* pok = glava;
	while( pok != 0){
		pok->deteNit->signal(2);
	}
}

void listaNit::deteObrisiRod(){
	elemThread* pok = glava;
		while( pok != 0){
			PCB* pcb = Kernel::listaPCB->dohvatiID(pok->deteNit->getId());
			pcb->tataNit = 0;
		}
}
