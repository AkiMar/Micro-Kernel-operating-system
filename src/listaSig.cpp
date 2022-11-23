/*
 * listaSig.cpp
 *
 *  Created on: Sep 17, 2020
 *      Author: OS1
 */

#include "listaSig.h"
#include <DOS.H>
#include "pcb.h"
#include "kernel.h"
#include "syPntf.h"
#include "IOSTREAM.H"

listaZahteva::listaZahteva(listaSig* listaSig) :
		glava(0), posl(0) {
	this->listaSvogSig = listaSig;
}

listaZahteva::~listaZahteva() {
	this->posl = glava;
	while (glava != 0) {
		posl = glava;
		glava = glava->next;
		delete posl;
	}
	glava = 0;
	posl = 0;
}

void listaZahteva::izbaciZahtev(elemZahteva* izbaci) {
	elemZahteva* pok = glava;
	elemZahteva* q = glava;
	while (pok != 0 && (pok != izbaci)) {
		q = pok;
		pok = pok->next;
	}
	if (pok == 0) {
		return;
	}
	if (pok == glava) {
		glava = glava->next;
		if (glava == 0)
			posl = 0;
	} else {
		if (pok == posl) {
			posl = q;
			posl->next = 0;
		} else {
			q->next = pok->next;
		}
	}

	delete pok;
}

void listaZahteva::obradiSveZahteve() {
	elemZahteva* tmp = glava;
	elemZahteva* pom = glava;
	while (tmp != 0) {
		int id = tmp->id;
		cout << " ID " << id;
		if( (this->listaSvogSig->signali[tmp->id].flagBlokade == 0) && (Kernel::globalneZabraneSig[tmp->id] == 0 )){
			this->listaSvogSig->obradifunkcije(tmp->id);
			if( id == 0)
				return;
			pom = tmp;
			tmp = tmp->next;
			this->izbaciZahtev(pom);
			if( id == 0)
				return;
		}else{
			tmp = tmp->next;
		}
	}
}

void listaZahteva::dodajZahtev(SignalId id){
	if( id > 15)
		return;
	elemZahteva* novi = new elemZahteva(id);
	if (glava == 0) {
		glava = novi;
		posl = novi;
	} else {
		posl->next = novi;
		posl = posl->next;
	}
}

void listaZahteva::kopirajKontekst(listaZahteva* lista){
	this->~listaZahteva();
	elemZahteva* tmp = lista->glava;
	while( tmp != 0){
		this->listaSvogSig->dodajZahtev(tmp->id);
		tmp = tmp->next;
	}

}

//lista handlera

listaSigHand::listaSigHand() :
		glava(0), posl(0), flagBlokade(0), flagObavestenje(0) {
}

listaSigHand::~listaSigHand() {
	this->posl = glava;
	while (glava != 0) {
		posl = glava;
		glava = glava->next;
		delete posl;
	}
	glava = 0;
	posl = 0;
}

void listaSigHand::dodajSigHand(SignalHandler funkcija) {
	elemSigHand* novi = new elemSigHand(funkcija);
	if (glava == 0) {
		glava = novi;
		posl = novi;
	} else {
		posl->next = novi;
		posl = posl->next;
	}
}

void listaSigHand::izbaciSigHandSve() {
	this->~listaSigHand();
}

void listaSigHand::swap(SignalHandler hand1, SignalHandler hand2) {
	elemSigHand* pok1 = this->glava;
	elemSigHand* prev1 = 0;
	while (pok1 != 0 && (pok1->funkcija != hand1)) {
		prev1 = pok1;
		pok1 = pok1->next;
	}
	if (pok1 == 0)
		return;

	elemSigHand* pok2 = this->glava;
	elemSigHand* prev2 = 0;
	while (pok2 != 0 && (pok2->funkcija != hand2)) {
		prev2 = pok2;
		pok2 = pok2->next;
	}
	if (pok2 == 0)
		return;

	// Link previous of node1 with node2
	if (prev1 != 0)
		prev1->next = pok2;

	// Link previous of node2 with node1
	if (prev2 != 0)
		prev2->next = pok1;

	// Swap node1 and node2 by swapping their
	// next node links
	elemSigHand* temp = pok1->next;
	pok1->next = pok2->next;
	pok2->next = temp;

	// Make sure to swap head node when swapping
	// first element.
	if (prev1 == 0)
		glava = pok2;
	else if (prev2 == 0)
		glava = pok1;

	if (pok1 == posl)
		posl = pok2;
	else if (pok2 == posl)
		posl = pok1;

}

void listaSigHand::obradifunkcije() {
	elemSigHand* tmp = glava;
	while (tmp != 0) {
		(tmp->funkcija)();
		tmp = tmp->next;
	}
}

// lista niti sa signalima

listaSig::listaSig(PCB* pcb) :
		flagObavestenjeSve(0), zahtevi(this), svojPCB(pcb) {
	signali[0].dodajSigHand(Signal0);
}
;

listaSig::~listaSig() {
	for (int var = 0; var < 16; ++var) {
		signali[var].~listaSigHand();
	}
}


void listaSigHand::kopirajKontekst(listaSigHand* lista){
	this->~listaSigHand();
	elemSigHand* tmp = lista->glava;
	while( tmp != 0){
		this->dodajSigHand(tmp->funkcija);
		tmp = tmp->next;
	}
}

listaSig* listaSig::lista = 0;

void listaSig::obavestiSignal(SignalId id) {
	if (id > 15)
		return;
	signali[id].flagObavestenje = 1;
	flagObavestenjeSve = 1;
}

void listaSig::blokirajSignal(SignalId id) {
	if (id > 15)
		return;
	signali[id].flagBlokade = 1;
}

void listaSig::dodajSig(SignalId id, SignalHandler funkcija) {
	if (id > 15 || funkcija == 0)
		return;
	signali[id].dodajSigHand(funkcija);
}

void listaSig::izbaciSigHandSve(SignalId id) {
	if (id > 15)
		return;
	signali[id].izbaciSigHandSve();
}

void listaSig::oslobodiSignal(SignalId id) {
	if (id > 15)
		return;
	signali[id].flagBlokade = 0;
}

void listaSig::obradifunkcije(SignalId id) {
	if (id > 15)
		return;
	if (signali[id].flagObavestenje) {
		signali[id].obradifunkcije();
	}
}

void listaSig::dodajZahtev(SignalId id){
	if( id > 15)
		return;
	syncPrintf("dodaj0\n");
	zahtevi.dodajZahtev(id);
	signali[id].flagObavestenje = 1;
	flagObavestenjeSve = 1;
}




void listaSig::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	if( id > 15 || hand1 == 0 || hand2 == 0)
		return;
	signali[id].swap(hand1, hand2);
}


void listaSig::kopirajKontekst(listaSig* lista){
	if( lista == 0)
		return;

	this->flagObavestenjeSve = 0;
	this->zahtevi.kopirajKontekst(&lista->zahtevi);

	for (int var = 0; var < 16; ++var) {
		signali[var].kopirajKontekst(&lista->signali[var]);
	}


}















