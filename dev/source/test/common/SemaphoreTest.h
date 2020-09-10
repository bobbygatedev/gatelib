/*
 * SemaphoreTest.h
 *
 *  Created on: 17/set/2014
 *      Author: ripamontie
 */

#ifndef SEMAPHORETEST_H_
#define SEMAPHORETEST_H_

#include "TestAbstract.h"

class SemaphoreTest: public TestAbstract {
public:
	SemaphoreTest();
	virtual ~SemaphoreTest();

	virtual g::cont::Gstr getName ( ) const { return "Semaphore Test"; }
	virtual g::cont::Gstr getDescription ( ) const { return "Tests Semaphore features."; }
protected:
	virtual bool mExecute ( );
};

#endif /* SEMAPHORETEST_H_ */
