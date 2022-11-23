
#ifndef KERSEM_H_
#define KERSEM_H_

#include "listaSpc.h"
#include "KSElem.h"
#include "KerSem.h"

void timerProlazKerSem();

typedef unsigned int Time;

class KernelSem{
public:
	KernelSem(int init = 1);
	virtual ~KernelSem();

	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n);

	int val() const; // Returns the current value of the semaphore

	void decVreme();

private:
	int vrednost;
	void block(Time vremePauze);
	int unblock(int odblokiranaVrednost);      // sluzi za pdblokiranje samo 1 niti u redu
	int odblokirajNiti(int n);  //koristi se za signal prvih n niti
public:
	listaPcbSem blkNiti;

};



#endif /* KERSEM_H_ */
