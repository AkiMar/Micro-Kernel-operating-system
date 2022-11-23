#include "IVTEntry.h"
#include "KernelEv.h"
#include <dos.h>
#include "kernel.h"

IVTEntry::IVTEntry(int brUlaza, pInterrupt noviPrekid)
{
	lock
	this->brUlaza = brUlaza;
	this->oldRout = getvect(brUlaza);
	setvect(brUlaza, noviPrekid);
	unlock
}

IVTEntry::~IVTEntry()
{
	lock
	setvect(this->brUlaza, this->oldRout);
	unlock;
}

void IVTEntry::signal()
{
	KernelLock();
	nizKernelEv[this->brUlaza]->signal();
	KernelUnlock();
}

void IVTEntry::pozivStarePrekidneRutine()
{
	KernelLock();
	this->oldRout();
	KernelUnlock();
}
