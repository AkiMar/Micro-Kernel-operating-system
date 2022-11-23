/*
 * listaPcb.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: OS1
 */

#include "listaPcb.h"
#include "pcb.h"

listaPcb::listaPcb():glava(0),posl(0){ }

listaPcb::~listaPcb(){
	this->posl = glava;
	while(glava != 0){
		posl = glava;
		glava = glava->next;
		delete posl;
	}
	glava = 0;
	posl = 0;
}

void listaPcb::dodaj(PCB* pcb){
	elemPCB* novi = new elemPCB(pcb);
	if( glava == 0){
		glava = novi;
		posl = novi;
	}else{
		posl->next = novi;
		posl = posl->next;
	}
}

PCB* listaPcb::izbaciPrvi(){
	if( glava == 0)
		return 0;
	PCB* tmp = glava->pcb;
	elemPCB* pok = glava;
	glava = glava->next;
	if( glava == 0)
		posl = glava;
	delete pok;
	return tmp;
}

PCB* listaPcb::izbaciIdPcb(int id){
	if( glava == 0)
			return 0;
	elemPCB* tmp = this->glava;
	elemPCB* prev = 0;
	PCB* pcb = 0;
	while( tmp != 0){
		if( tmp->pcb->id == id)
			break;
		prev = tmp;
		tmp = tmp->next;
	}
	if( tmp == 0)
		return 0;
	if(tmp == glava){
		glava = glava->next;
		if( glava == 0)
			posl = 0;
		pcb = tmp->pcb;
		delete tmp;
		return pcb;
	}
	if( tmp == posl){
		posl = prev;
		posl->next = 0;
		pcb = tmp->pcb;
		delete tmp;
		return pcb;
	}
	prev->next = tmp->next;
	pcb = tmp->pcb;
	delete tmp;
	return pcb;
}

PCB* listaPcb::dohvatiID(int id){
		elemPCB* tmp = this->glava;
		while( tmp != 0){
			if( tmp->pcb->id == id)
				break;
			tmp = tmp->next;
		}
		if( tmp == 0)
			return 0;
		return tmp->pcb;
}

void listaPcb::obrisiPCB(PCB* pcb){
	elemPCB* pok = glava;
	elemPCB* q = glava;
		while (pok != 0 && (pok->pcb != pcb) ) {
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

