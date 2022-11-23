#include "Semaphor.h"
#include "KerSem.h"
#include "kernel.h"

Semaphore::Semaphore(int init)
{
	this->myImpl = new KernelSem(init);
}

Semaphore::~Semaphore()
{
	KernelLock();
	delete this->myImpl;
	KernelUnlock();
}

int Semaphore::wait(Time maxTimeToWait)
{
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n)
{
	return myImpl->signal(n);
}

int Semaphore::val() const
{
	return myImpl->val();
}


