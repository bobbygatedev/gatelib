#include "g_DateTime.h"
#include <time.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>

using namespace std;

namespace g
{

string Date::getString ( const char* aFormat ) const
{
	DateTime date_time;

	date_time.date = *this;

	return date_time.getString ( aFormat );
}

int Date::do_Compare(const Date& d1, const Date& d2)
{
	if(d1.getYear()>d2.getYear())
	{
		return +1;
	}
	else if(d1.getYear()>d2.getYear())
	{
		return -1;
	}
	else //==
	{
		if(d1.getMonth()>d2.getMonth())
		{
			return +1;
		}
		else if(d1.getMonth()>d2.getMonth())
		{
			return -1;
		}
		else //==
		{
			if(d1.getDay()>d2.getDay())
			{
				return +1;
			}
			else if(d1.getDay()>d2.getDay())
			{
				return -1;
			}
			else //==
			{
				return 0;
			}
		}
	}
}

Date Date::now() { return DateTime::now().date; }


int Time::do_Compare(const Time& t1, const Time& t2)
{
	if(t1.getHour()>t2.getHour())
	{
		return +1;
	}
	else if(t1.getHour()>t2.getHour())
	{
		return -1;
	}
	else //==
	{
		if(t1.getMinutes()>t2.getMinutes())
		{
			return +1;
		}
		else if(t1.getMinutes()>t2.getMinutes())
		{
			return -1;
		}
		else //==
		{
			if(t1.getSeconds()>t2.getSeconds())
			{
				return +1;
			}
			else if(t1.getSeconds()>t2.getSeconds())
			{
				return -1;
			}
			else //==
			{
				return 0;
			}
		}
	}
}

string Time::getString ( const char* aFormat ) const
{
	DateTime date_time;

	date_time.time = *this;

	return date_time.getString(aFormat);
}

Time Time::now() { return DateTime::now().time; }

int DateTime::do_Compare(const DateTime& d1, const DateTime& d2)
{
	int date_compare = Date::do_Compare(d1.date,d2.date);

	if(date_compare==0)
	{
		return Time::do_Compare(d1.time,d2.time);
	}
	else
	{
		return date_compare;
	}
}

string DateTime::getString ( const char* aFormat ) const
{
	char temp[1024];
	time_t int_time = (time_t)getIntTime ();
	
	strftime ( temp , sizeof( temp ) , aFormat , localtime ( &int_time ) );

	return temp;
}

DateTime& DateTime::operator =(const DateTime& o)
{
	date = o.date;
	time = o.time;

	return *this;
}

GIntTime_t DateTime::getIntTime() const
{
	tm       temp_tm;
	DateTime temp_dt(*this);

	memset ( &temp_tm , 0x0 , sizeof(tm) );
	
	temp_tm.tm_hour = temp_dt.time.getHour();
	temp_tm.tm_min  = temp_dt.time.getMinutes();
	temp_tm.tm_sec  = temp_dt.time.getSeconds();

	temp_tm.tm_year  = temp_dt.date.getYear()-1900;
	temp_tm.tm_mon   = temp_dt.date.getMonth()-1;
	temp_tm.tm_mday  = temp_dt.date.getDay();

	temp_tm.tm_isdst = temp_dt.isDaylightSavingsTime();

	return mktime(&temp_tm);
}

bool DateTime::isDaylightSavingsTime ( ) const 
{
	tm       temp_tm;

	memset ( &temp_tm , 0x0 , sizeof(tm) );

	temp_tm.tm_year  = date.getYear()-1900;
	temp_tm.tm_mon   = date.getMonth()-1;
	temp_tm.tm_mday  = date.getDay();
	temp_tm.tm_hour  = 12;

	time_t int_time = mktime(&temp_tm);

	return ( localtime(&int_time)->tm_isdst != 0 );
}

void DateTime::setIntTime ( GIntTime_t aIntTime )
{
	time_t temp_time = (time_t)aIntTime;
	tm* local_time_p = localtime ( &temp_time );

	time.setHour    ( local_time_p->tm_hour );
	time.setMinutes ( local_time_p->tm_min );
	time.setSeconds ( local_time_p->tm_sec );
	date.setDay     ( local_time_p->tm_mday );
	date.setMonth   ( (GMonth_t)(local_time_p->tm_mon + 1 ) );
	date.setYear    ( local_time_p->tm_year + 1900 );
}

void DateTime::setUtc(const DateTime& utc)
{
	setIntTime ( utc.getIntTime ( ) + getSecondsFromUtc() );
}

DateTime DateTime::getUtc() const
{
	DateTime result (*this);

	result.addSeconds ( -getSecondsFromUtc ( ) );

	return result;
}

GIntTime_t DateTime::getSecondsFromUtc ( )
{
	time_t ts = 0;

	::time ( &ts );

	tm* utc_p     = gmtime ( &ts );
	time_t offset = ts - mktime ( utc_p );

	return offset;
}

DateTime DateTime::now()
{
	time_t ts;

	::time ( &ts );

	tm* tm_p = localtime ( &ts );

	DateTime result (tm_p->tm_mday,(GMonth_t)(tm_p->tm_mon+1),tm_p->tm_year + 1900, tm_p->tm_hour , tm_p->tm_min , tm_p->tm_sec);

	return result;
}

void DateTime::addSeconds ( GIntTime_t aSeconds )
{
	setIntTime ( getIntTime ( ) + aSeconds );
}

GDayOfTheWeek_t DateTime::getDayOfTheWeek ( ) const
{
	time_t int_time = (time_t)getIntTime();
	
	return (GDayOfTheWeek_t)localtime( &int_time )->tm_wday;
}

int DateTime::getDayFromYearBeginning ( ) const
{
	time_t int_time = (time_t)getIntTime();
	
	return (GDayOfTheWeek_t)localtime( &int_time )->tm_yday;
}

} //namespace g
