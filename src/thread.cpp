#include "thread.h"
#include "Pcb.h"
#include "kernel.h"
#include <dos.h>
#include <IOSTREAM.H>
#include "SCHEDULE.h"

void Thread::start()
{
	if( this->myPCB->status == 0)
		return;
	KernelLock();
	this->myPCB->status = 0;
	Scheduler::put(this->myPCB);
	cout << "START\n";
	KernelUnlock();
};

void Thread::waitToComplete()
{
	if( this->myPCB->status == 1){
		return;
	}
	KernelLock();
	//cout <<"waitToComplete\n";
	this->myPCB->dodajListuCekanja((PCB*)Kernel::running);
	Kernel::running->status=2;
	dispatch();
	KernelUnlock();
};

Thread::~Thread()
{

	if( this->myPCB->Signal0Flag == 1){
		return;
	}

	lock
	cout << "Destruktor ID"<< getId() <<"\n";
	unlock

	if( this->myPCB->status != 1){
	    this->waitToComplete();
	}

	KernelLock();

	//Kernel::running->deca.deteObrisiRod();

	Kernel::listaPCB->obrisiPCB(this->myPCB);
	delete myPCB;
	KernelUnlock();
};

ID Thread::getId()
{
	return this->myPCB->id;
};

ID Thread::getRunningId()
{
	return Kernel::running->id;
};

Thread * Thread::getThreadById(ID id)
{
	KernelLock();
	Thread* nit = Kernel::listaPCB->dohvatiID(id)->nit;
	KernelUnlock();
	return nit;
};

Thread::Thread(StackSize stackSize, Time timeSlice)
{
	KernelLock();
	if( stackSize > 65535){
		stackSize = 65535;
	}
	cout << "KONSTRUKTOR\n";
	stackSize = stackSize / sizeof(unsigned);
	cout << stackSize << timeSlice ;
	unsigned* stack = new unsigned[stackSize];
	this->myPCB = new PCB();
	this->myPCB->nit = this;
	if( this != Kernel::zaludnaNit){
	      Kernel::listaPCB->dodaj(this->myPCB);
	}else{
		Kernel::pcbZaludne = this->myPCB;
	}
	stack[stackSize -1] = 0x200;
	stack[stackSize -2] = FP_SEG(PCB::wrapper);
	stack[stackSize -3] = FP_OFF(PCB::wrapper);
	myPCB->ss = FP_SEG(stack + stackSize -12);
	myPCB->sp = FP_OFF(stack + stackSize -12);
	myPCB->bp = myPCB->sp;
	stack[stackSize -12] = myPCB->bp;
	myPCB->timeSlice = timeSlice;
	myPCB->status = 3;
	myPCB->stack = stack;
	myPCB->stackSize = stackSize;
	cout << stack[stackSize -2] <<" " << stack[stackSize -3] <<" "<<myPCB->sp << " " << myPCB->ss ;
	cout <<"KRAJ KONSTRUKTORA\n";

	//za decu niti
	if( Kernel::mainPCB != 0){
		this->myPCB->listaSignala.kopirajKontekst((listaSig*)&Kernel::running->listaSignala);
		Kernel::running->deca.dodajDete(this);
	}
	KernelUnlock();
};

void dispatch()
{
	lock
	/*if( Kernel::running == Kernel::mainPCB){
			cout << "Dispatch Main nit\n";
	}
	else
	{
	cout <<" Dispatch ID:" << Thread::getRunningId() << "\n";
	}*/
	// eksplicitna promena konteksta za ovu nit
	Kernel::context_switch_on_demand = 1;
	timer();
	unlock
};

void Thread::signal(SignalId id){
	KernelLock();
	this->myPCB->listaSignala.dodajZahtev(id);
	KernelUnlock();
}

void Thread::registerHandler(SignalId id, SignalHandler handler){
	KernelLock();
	this->myPCB->listaSignala.dodajSig(id,handler);
	KernelUnlock();
}

void Thread::swap(SignalId id,SignalHandler hand1, SignalHandler hand2){
	KernelLock();
	this->myPCB->listaSignala.swap(id,hand1,hand2);
	KernelUnlock();
}

void Thread::blockSignal(SignalId id){
	KernelLock();
	this->myPCB->listaSignala.blokirajSignal(id);
	KernelUnlock();
}

void Thread::blockSignalGlobally(SignalId id){
	if(id > 15)
		return;
	KernelLock();
	Kernel::globalneZabraneSig[id] = 1;
	KernelUnlock();
}


void Thread::unblockSignal(SignalId id){
	if( id > 15)
		return;
	KernelLock();
	this->myPCB->listaSignala.oslobodiSignal(id);
	KernelUnlock();
}

void Thread::unblockSignalGlobally(SignalId id){
	if( id > 15)
		return;
	KernelLock();
	Kernel::globalneZabraneSig[id] = 0;
	KernelUnlock();
}























