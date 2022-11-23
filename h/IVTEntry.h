
#ifndef IVTENTRY_H_
#define IVTENTRY_H_


typedef void interrupt (*pInterrupt)(...);


class IVTEntry {
public:
	IVTEntry(int brUlaza, pInterrupt noviPrekid);
	~IVTEntry();

	void signal();
	void pozivStarePrekidneRutine();

	pInterrupt oldRout;
	int brUlaza;
};



#endif /* IVTENTRY_H_ */
