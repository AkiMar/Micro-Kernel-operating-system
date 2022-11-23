/*
 * listaSig.h
 *
 *  Created on: Sep 17, 2020
 *      Author: OS1
 */



#ifndef LISTASIG_H_
#define LISTASIG_H_

#include "kernel.h"

typedef void (*SignalHandler)();

typedef unsigned SignalId;

volatile extern unsigned lbp;
volatile extern unsigned lsp;
volatile extern unsigned lss;

class listaSig;

struct elemZahteva {
	elemZahteva* next;
	SignalId id;
	elemZahteva(SignalId id){
		this->next = 0;
		this->id = id;
	}

};

class listaZahteva {

public:

	elemZahteva* glava;
	elemZahteva* posl;
	listaSig* listaSvogSig;

	listaZahteva(listaSig* listaSig);
	~listaZahteva();

	void dodajZahtev(SignalId id);
	void obradiSveZahteve();
	void izbaciZahtev(elemZahteva* tmp);

	void kopirajKontekst(listaZahteva* lista);
};


struct elemSigHand {
	elemSigHand* next;
	SignalHandler funkcija;

	elemSigHand(SignalHandler funkcija){
		this->next = 0;
		this->funkcija = funkcija;
	}

};

class listaSigHand {

public:

	int flagBlokade; // ako je 1 znaci blokiran Signal 0 nije

	int flagObavestenje;

	elemSigHand* glava;
	elemSigHand* posl;

	listaSigHand();
	~listaSigHand();

	void dodajSigHand(SignalHandler funkcija);
	void izbaciSigHandSve();
	void swap(SignalHandler hand1, SignalHandler hand2);
	void obradifunkcije();

	void kopirajKontekst(listaSigHand* lista);

};


class listaSig {

public:

	static listaSig* lista;
	PCB* svojPCB;
	listaSig(PCB* pcb);
	~listaSig();

	void kopirajKontekst(listaSig* lista);

	void obavestiSignal(SignalId id);

	void dodajSig(SignalId id,SignalHandler funkcija);
	void izbaciSigHandSve(SignalId id);

	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blokirajSignal(SignalId id);
	void oslobodiSignal(SignalId id);

	void obradifunkcije(SignalId id);
	void dodajZahtev(SignalId id);
	listaSigHand signali[16];
	int flagObavestenjeSve;
	listaZahteva zahtevi;
};





#endif /* LISTASIG_H_ */
