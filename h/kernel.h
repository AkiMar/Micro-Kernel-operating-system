#ifndef KERNEL_H_
#define KERNEL_H_

#include "listaPcb.h"
#include <stdio.h>
#include <dos.h>
#include "thread.h"



class PCB;
class Thread;

void interrupt timer();

extern void tick();

extern int userMain(int argc, char* argv[]);

class Kernel {

public:

	static volatile unsigned lockFlag;

	static listaPcb* listaPCB;

	static Thread* mainNit;
	static Thread* zaludnaNit;
	static PCB* mainPCB;
	static PCB* pcbZaludne;
	static volatile PCB* running;

	static volatile int cntr;
	static volatile int context_switch_on_demand;

	// za signale
	static int globalneZabraneSig[16];

};

volatile extern unsigned tbp;
volatile extern unsigned tsp;
volatile extern unsigned tss;

#define lock asm{\
	pushf;\
	cli;\
}

#define unlock asm popf

class KernelSemElem;

extern KernelSemElem* glavaKerSem;
extern KernelSemElem* poslKerSem;

// stara prekidna rutina
extern unsigned oldTimerOFF, oldTimerSEG;

// deklaracija nove prekidne rutine
void interrupt timer();

// postavlja novu prekidnu rutinu
void inic();

// vraca staru prekidnu rutinu
void restore();


// nova prekidna rutina tajmera
void interrupt timer();

void KernelLock();

void KernelUnlock();

void Signal0();

#endif /* KERNEL_H_ */
