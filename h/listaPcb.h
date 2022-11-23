/*
 * listaPcb.h
 *
 *  Created on: Sep 15, 2020
 *      Author: OS1
 */

#ifndef LISTAPCB_H_
#define LISTAPCB_H_

class PCB;

struct elemPCB {
	PCB* pcb;
	elemPCB* next;
	elemPCB(PCB* pcb){
		this->pcb = pcb;
		this->next = 0;
	}
};


class listaPcb {

public:
	elemPCB* glava;
	elemPCB* posl;

	listaPcb();
	~listaPcb();

	void dodaj(PCB* pcb);
	PCB* izbaciPrvi();
	PCB* izbaciIdPcb(int id);
	PCB* dohvatiID(int id);
	void obrisiPCB(PCB* pcb);

};



#endif /* LISTAPCB_H_ */
