#include "KernelEv.h"
#include "kernel.h"
#include "SCHEDULE.H"
#include "pcb.h"

KernelEv::~KernelEv(){
	nizKernelEv[brUlaza] = 0;
}

KernelEv::KernelEv(Event * event, int brUlaza)
{
	KernelLock();
	this->event = event;
	nizKernelEv[brUlaza] = this;
	this->blkNit = (PCB*)Kernel::running;
	this->vrednost = 0;
	KernelUnlock();
}

void KernelEv::signal()
{
	KernelLock();
	if (this->vrednost == -1) {
		this->vrednost = 0;
		unblock();
	}
	else {
		this->vrednost = 1;
	}
	KernelUnlock();
}

void KernelEv::wait()
{
	if (this->blkNit != Kernel::running)
		return;
	if (this->vrednost == 0) {
		KernelLock();
		this->vrednost = -1;
		block();
		KernelUnlock();
	}
	this->vrednost = 0;
	return;
}

void KernelEv::block()
{
	Kernel::running->status = 2;
	dispatch();
}

void KernelEv::unblock()
{
	this->blkNit->status = 0;
	Scheduler::put(this->blkNit);
}
