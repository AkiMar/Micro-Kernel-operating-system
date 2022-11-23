#include "KerSem.h"
#include "KSElem.h"
#include "kernel.h"


KernelSemElem::KernelSemElem(KernelSem* kerSem){
		this->next = 0;
		this->kerSem = kerSem;
		if( glavaKerSem == 0){
			glavaKerSem = this;
			poslKerSem = this;
		}else{
			poslKerSem->next = this;
			poslKerSem = this;
		}
	}


