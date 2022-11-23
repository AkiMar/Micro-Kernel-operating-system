/*
 * kernel.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: OS1
 */

#include "kernel.h"
#include <IOSTREAM.H>
#include "syPntf.h"
#include "pcb.h"
#include "SCHEDULE.H"

volatile unsigned Kernel::lockFlag = 0;

listaPcb* Kernel::listaPCB = new listaPcb();

Thread* Kernel::mainNit = 0;
Thread* Kernel::zaludnaNit = 0;
PCB* Kernel::mainPCB = 0;
PCB* Kernel::pcbZaludne = 0;
volatile PCB* Kernel::running = 0;

volatile int Kernel::cntr = 2;
volatile int Kernel::context_switch_on_demand = 0;

int Kernel::globalneZabraneSig[16];

void KernelLock(){
	Kernel::lockFlag = 1;
}

void KernelUnlock(){
	Kernel::lockFlag = 0;
	if(Kernel::context_switch_on_demand){
		dispatch();
	}
}

volatile unsigned tbp = 0;
volatile unsigned tsp = 0;
volatile unsigned tss = 0;

extern void timerProlazKerSem();

// nova prekidna rutina tajmera
void interrupt timer() {


	if (!Kernel::context_switch_on_demand){
		Kernel::cntr--;
		asm int 60h;
		tick();
		timerProlazKerSem();
	}

	if( Kernel::running->timeSlice == 0 && Kernel::context_switch_on_demand == 0){
		//syncPrintf("%d",Thread::getRunningId());
		return;
	}
	if (Kernel::cntr <= 0 || Kernel::context_switch_on_demand) {
		if(Kernel::lockFlag == 0){
		Kernel::context_switch_on_demand = 0;
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		Kernel::running->sp = tsp;
		Kernel::running->ss = tss;
		Kernel::running->bp = tbp;

		if( Kernel::running->status == 0 && Kernel::running != Kernel::pcbZaludne ){
			//syncPrintf("PUT");
			Scheduler::put((PCB*)Kernel::running);
		}

		// scheduler
		Labela:
		do {
			Kernel::running = Scheduler::get();
			if (!(Kernel::running))
				Kernel::running = Kernel::pcbZaludne;
		} while(!Kernel::running->stack);

		tsp = Kernel::running->sp;
		tss = Kernel::running->ss;
		tbp = Kernel::running->bp;

		Kernel::cntr = Kernel::running->timeSlice;

		//syncPrintf("%d\n", Thread::getRunningId());

		asm {
			// restaurira sp
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}

		if( Kernel::running->listaSignala.flagObavestenjeSve){
			asm pushf
			asm sti
			KernelLock();
			PCB::obradaSignala();
			KernelUnlock();
			asm popf
			if(Kernel::running->Signal0Flag == 1){
				Kernel::running->nit->~Thread();
				goto Labela;
			}
		}

		}
		else{
			Kernel::context_switch_on_demand = 1;
		}
	}
};

// postavlja novu prekidnu rutinu
void inic() {
asm {
cli
push es
push ax

mov ax,0
mov es,ax // es = 0

// pamti staru rutinu
mov ax, word ptr es:0022h
mov word ptr oldTimerSEG, ax
mov ax, word ptr es:0020h
mov word ptr oldTimerOFF, ax

// postavlja novu rutinu
mov word ptr es:0022h, seg timer
mov word ptr es:0020h, offset timer

// postavlja staru rutinu na int 60h
mov ax, oldTimerSEG
mov word ptr es:0182h, ax
mov ax, oldTimerOFF
mov word ptr es:0180h, ax

pop ax
pop es
sti
}
};

// vraca staru prekidnu rutinu
void restore() {
asm {
cli
push es
push ax

mov ax,0
mov es,ax

mov ax, word ptr oldTimerSEG
mov word ptr es:0022h, ax
mov ax, word ptr oldTimerOFF
mov word ptr es:0020h, ax

pop ax
pop es
sti
}
};

void Signal0(){
	cout << "BWOOOA\n";
	Kernel::running->status = 1;
	Kernel::running->probudiSve();
	Kernel::running->deca.obavestiDecu();
	Kernel::running->Signal0Flag = 1;
}



