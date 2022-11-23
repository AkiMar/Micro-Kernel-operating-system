#include "event.h"
#include "KernelEv.h"
#include "kernel.h"

Event::Event(IVTNo ivtNo)
{
	this->myImpl = new KernelEv( this, (int)ivtNo );
}

Event::~Event()
{
	KernelLock();
	delete this->myImpl;
	KernelUnlock();
}

void Event::wait()
{
	this->myImpl->wait();
}

void Event::signal()
{
	this->myImpl->signal();
}
