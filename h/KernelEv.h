
#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "kernel.h"

class Event;

class KernelEv {
public:
	KernelEv(Event* event, int brUlaza);
	~KernelEv();

	void signal();
	void wait();
	void block();
	void unblock();
protected:
	Event* event;
	int vrednost;
	int brUlaza;
	PCB* blkNit;
};

extern KernelEv* nizKernelEv[256];


#endif /* KERNELEV_H_ */
