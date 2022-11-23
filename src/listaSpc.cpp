/*
 * listaSpc.cpp
 *
 *  Created on: Sep 16, 2020
 *      Author: OS1
 */

#include "listaSpc.h"
#include "pcb.h"
#include "SCHEDULE.H"

listaPcbSem::listaPcbSem():glava(0),posl(0){ }

listaPcbSem::~listaPcbSem(){
	this->posl = glava;
	while(glava != 0){
		posl = glava;
		glava = glava->next;
		delete posl;
	}
	glava = 0;
	posl = 0;
}

void listaPcbSem::dodaj(PCB* pcb, Time vremeCekanja){
	elemPcbSem* novi = new elemPcbSem(pcb,vremeCekanja);
	if( glava == 0){
		glava = novi;
		posl = novi;
		return;
	}
	if( vremeCekanja == 0){
		posl->next = novi;
		posl = posl->next;
		return;
	}else{
		novi->next = glava;
		glava = novi;
		return;
	}
}

PCB* listaPcbSem::izbaciPrvi(){
	if( glava == 0)
		return 0;
	PCB* tmp = glava->pcb;
	elemPcbSem* pok = glava;
	glava = glava->next;
	if( glava == 0)
		posl = glava;
	delete pok;
	return tmp;
}

void listaPcbSem::decVreme(){
	elemPcbSem* pok = this->glava;
	elemPcbSem* prev = this->glava;
	while( pok!= 0 && (pok->flagBlokade == 0)){
		pok->vremeCekanja--;
		if(pok->vremeCekanja == 0){
			if( pok == glava){
				glava = glava->next;
				pok->pcb->status = 0;
				pok->pcb->semaforRet = 0;
				Scheduler::put(pok->pcb);
				delete pok;
				pok = glava;
				if( glava == 0)
					posl = 0;
				continue;
			}else{
				prev->next = pok->next;
				pok->pcb->status = 0;
				pok->pcb->semaforRet = 0;
				Scheduler::put(pok->pcb);
				if( pok == posl)
					posl = prev;
				delete pok;
				pok = prev->next;
				continue;
			}
		}
		prev = pok;
		pok = pok->next;
	}
}


