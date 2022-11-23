#ifndef PREP_H_
#define PREP_H_

#include "IVTEntry.h"
#include <dos.h>
#include "IOSTREAM.H"

#define PREPAREENTRY(brUlaza, callOld) \
	void interrupt inter##brUlaza(...); \
	IVTEntry newEntry##brUlaza(brUlaza, inter##brUlaza); \
	void interrupt inter##brUlaza(...){ \
		newEntry##brUlaza.signal(); \
		if (callOld == 1) \
			newEntry##brUlaza.pozivStarePrekidneRutine(); \
}

#endif /* PREP_H_ */
