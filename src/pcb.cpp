/*
 * pcb.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: OS1
 */


#include "pcb.h"
#include "kernel.h"
#include "SCHEDULE.H"
#include "IOSTREAM.H"

int PCB::staticID = 0;

void PCB::wrapper() {
	KernelLock();
	cout <<"Wrapper funkciju ";
	cout << Thread::getRunningId() << "\n";
	KernelUnlock();

	Kernel::running->nit->run();

	KernelLock();
	Kernel::running->probudiSve();
	Kernel::running->status = 1;
	cout << "WRAPPER 1\n";

	//za tata nit
	if( Kernel::running->tataNit != 0)
			Kernel::running->tataNit->signal(1);

	Kernel::running->deca.obavestiDecu();

	cout << Kernel::mainPCB->status;

	KernelUnlock();
	dispatch();
}

void PCB::dodajListuCekanja(PCB* pcb){
	this->blokiraneNiti.dodaj(pcb);
}

void PCB::probudiSve(){
	while(blokiraneNiti.glava != 0){
		PCB* pcb = blokiraneNiti.izbaciPrvi();
		pcb->status = 0;
		Scheduler::put(pcb);
	}
}

PCB::PCB():listaSignala(this){
	this->id = staticID++;
	this->nit = 0;
	Signal0Flag = 0;
}

PCB::~PCB(){
	this->probudiSve();
	delete[] this->stack;
}

void PCB::obradaSignala(){

	cout << "Obrada";


	if( Kernel::running->listaSignala.flagObavestenjeSve == 0){
		KernelUnlock();
		return;
	}

	Kernel::running->listaSignala.zahtevi.obradiSveZahteve();

	if( Kernel::running->listaSignala.zahtevi.glava == 0){
		Kernel::running->listaSignala.flagObavestenjeSve = 0;
	}



}




























