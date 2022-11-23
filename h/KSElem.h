
#ifndef KSELEM_H_
#define KSELEM_H_

class KernelSem;

class KernelSemElem {
public:
	KernelSemElem* next;
	KernelSem* kerSem;

	KernelSemElem(KernelSem* kerSem);

};




#endif /* KSELEM_H_ */
