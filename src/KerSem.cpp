
#include "kernel.h"
#include "KerSem.h"
#include "KSElem.h"
#include "thread.h"
#include <IOSTREAM.H>
#include "syPntf.h"
#include "SCHEDULE.H"
#include "pcb.h"


KernelSem::KernelSem(int init){
	KernelLock();
	this->vrednost = init;
	new KernelSemElem(this);
	KernelUnlock();
}

KernelSem::~KernelSem()
{
	KernelLock();
	blkNiti.~listaPcbSem();
	KernelUnlock();

}

int KernelSem::wait(Time vreme){
	int ret = 1;
	if( this->vrednost-- <=0 ) {
		KernelLock();
		this->block(vreme);
		KernelUnlock();
		ret = Kernel::running->semaforRet;
	}else{
		ret = 1;
	}
	return ret;

}

int KernelSem::signal(int n){
	if (n < 0) return n;

	int ret = 0;
	KernelLock();
	if( n == 0){
		if( this->vrednost++ < 0) {
			ret = unblock(1);
		}
	}else{
		this->vrednost++;
		ret = odblokirajNiti(n);
	}
	KernelUnlock();
	return ret;

}


int KernelSem::val() const
{
	return this->vrednost;
}

void KernelSem::decVreme()
{
	blkNiti.decVreme();
}

void KernelSem::block(Time vremePauze){
	blkNiti.dodaj((PCB*)Kernel::running, vremePauze);
	Kernel::running->status = 2;
	dispatch();
}


int KernelSem::unblock(int odblokiranaVrednost){
	if( blkNiti.glava == 0)
		return 0;

	PCB* pcb = blkNiti.izbaciPrvi();
	pcb->status = 0;
	pcb->semaforRet = odblokiranaVrednost;
	Scheduler::put(pcb);

	return 1;
}


int KernelSem::odblokirajNiti(int n){
	if( this->vrednost + n > 0){
		n = 0 - this->vrednost;
	}
	if( n <= 0 ){
		return 0;
	}
	int i = 0;
	int tmp = 0;
	for( i = 0; i < n; i++) {
		tmp = this->unblock(1);
		if( tmp == 0)
			break;
	}
	return i;
}

void timerProlazKerSem()
{
	KernelSemElem* tmp = glavaKerSem;
	while (tmp != 0) {
		tmp->kerSem->decVreme();
		tmp = tmp->next;
	}
}

