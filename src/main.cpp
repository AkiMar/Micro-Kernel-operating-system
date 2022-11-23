//pretpostavljeni memorijski model: huge
#include <iostream.h>
#include <dos.h>
#include "kernel.h"
#include "thread.h"
#include "Pcb.h"
#include "KerSem.h"
#include "KernelEv.h"

KernelSemElem* glavaKerSem = 0;
KernelSemElem* poslKerSem = 0;

KernelEv* nizKernelEv[256];

unsigned oldTimerOFF = 0;
unsigned oldTimerSEG = 0;

class Zal: public Thread {
public:
	Zal():Thread(4096, 1) {
		cout << "Kreirana zaludna\n";
	}
	;
protected:
	virtual void run() {
		cout << "Zaludna Nit\n";
		while (1) {
		};
	}
};

int main(int argc, char* argv[]) {
	//za signale
	for (int var = 0; var < 16; ++var) {
		Kernel::globalneZabraneSig[var] = 0;
	}

	 Kernel::mainNit = new Zal();
	 Kernel::zaludnaNit = new Zal();

	 Kernel::mainPCB = Kernel::listaPCB->dohvatiID(0);
	 Kernel::mainPCB->status = 0;
	 Kernel::running = Kernel::mainPCB;
	 Kernel::mainPCB->timeSlice = 2;
	 Kernel::cntr = Kernel::mainPCB->timeSlice;

	 Kernel::pcbZaludne = Kernel::listaPCB->dohvatiID(1);
	 Kernel::pcbZaludne->status = 0;

	 cout <<"\nMAIN 1 deo \n";
	 if( Kernel::running->nit == Kernel::mainNit)
	 {
	 cout << "ISTO\n";
	 }else{
	 cout << "NIJE ISTO";
	 }


	cout << "USER MAIN " << Kernel::cntr << "\n";

	inic();

	int ret = userMain(argc, argv);

	//lock
	restore();
	//unlock

	cout <<" \n KRAJ MAIN\n";
	Kernel::mainPCB->status = 1;
	Kernel::pcbZaludne->status = 1;
	delete Kernel::mainNit;
	delete Kernel::zaludnaNit;
	Kernel::listaPCB->~listaPcb();
	return ret;
}

