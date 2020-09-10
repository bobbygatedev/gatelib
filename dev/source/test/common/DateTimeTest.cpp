/*
 * DateTimeTest.cpp
 *
 *  Created on: 30/gen/2014
 *      Author: Bobby Gate
 */

#include "DateTimeTest.h"
#include "g_DateTime.h"

using namespace std;
using namespace g;
using namespace cont;
using namespace alg;

DateTimeTest::DateTimeTest() {}

DateTimeTest::~DateTimeTest() {}

bool DateTimeTest::mExecute()
{
	DateTime date_time;

	date_time= DateTime(26,june,1974,1,30,35);

	cout << endl << date_time << endl;

	GIntTime_t int_time = date_time.getIntTime();

	DateTime date_time2(int_time);

	mAddSubResult ( "My Birthsday day of week test." , "" , ( wednesday == date_time.getDayOfTheWeek ( ) ) );
	mAddSubResult ( "My Birthsday is daylight saving." , "" , date_time.isDaylightSavingsTime ( ) );

	date_time.addHour ( 24 );

	mAddSubResult ( "My Birthsday is day after." , "" , ( date_time == DateTime(27,june,1974,1,30,35) ) );

	return true;
}
