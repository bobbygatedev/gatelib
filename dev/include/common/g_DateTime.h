#pragma once

#include "g_common_functions.h"
#include <string>
#include <iostream>

namespace g
{
typedef long long GIntTime_t;

enum GDayOfTheWeek_t
{
	sunday = 0 , 
	monday = 1 ,
	tuesday = 2 , 
	wednesday = 3 ,
	thursday = 4 ,
	friday = 5 ,
	saturday = 6
};

enum GMonth_t
{
	january = 1  , 
	february = 2 ,
	march = 3 ,
	april = 4 , 
	may = 5 ,
	june = 6,
	july = 7 ,
	august = 8 ,
	september = 9 ,
	october = 10 ,
	november = 11 ,
	december = 12
};

class G_LIB_ITEM Date 
{
public:
	Date ( int d = 1 , int m = january , int y = 1900 ) { mDay=d; mMonth=(GMonth_t)m; mYear=y; }
	Date ( const Date& o ) { mDay = o.getDay(); mMonth = o.getMonth();mYear = o.getYear(); }

	virtual ~Date ( ){}

	int      getDay   ( ) const { return mDay; }
	GMonth_t getMonth ( ) const { return mMonth; }
	int      getYear  ( ) const { return mYear; }

	void setDay   ( int d ) { mDay = d; }
	void setMonth ( int m ) { mMonth = (GMonth_t)m; }
	void setYear  ( int y ) { mYear = y; }

	Date& operator = ( const Date& o ) { mDay = o.getDay();mMonth = o.getMonth();mYear = o.getYear(); return *this; }

	std::wstring getStringW ( const char* format = "%x" ) const { return to_Wstring(getString(format)); }
	std::string  getString  ( const char* = "%x" ) const;

	//returns 0 equal -1 d1<d2 +1 d1>d2
	static int do_Compare ( const Date& d1 , const Date& d2 );

	bool operator == ( const Date& o ) const { return (do_Compare(*this,o)==0); }
	bool operator != ( const Date& o ) const { return (do_Compare(*this,o)!=0); }
	bool operator > ( const Date& o ) const { return (do_Compare(*this,o)>0); }
	bool operator < ( const Date& o ) const { return (do_Compare(*this,o)<0); }

	static Date now ( );

private:
	int      mDay;
	GMonth_t mMonth;
	int      mYear;
	bool     mIsDaylightSavingsTime;
};

class G_LIB_ITEM Time 
{
public:		
	Time ( int h = 0 , int m = 0 , int s = 0 ){ mHour = h; mMinutes = m; mSeconds =s; }
	Time ( const Time& o ) { mHour = o.getHour() ; mMinutes = o.getMinutes(); mSeconds =o.getSeconds(); }
	virtual ~Time ( ){}
	
	Time& operator = ( const Time& o ) { mHour = o.getHour() ; mMinutes = o.getMinutes(); mSeconds =o.getSeconds(); return *this; }

	int getHour    ( ) const { return mHour; }//0:23
	int getMinutes ( ) const { return mMinutes; }
	int getSeconds ( ) const { return mSeconds; }

	void setHour    ( int h ) { mHour = h; }
	void setMinutes ( int m ) { mMinutes = m; }
	void setSeconds ( int s ) { mSeconds = s; }

	//returns 0 equal -1 d1<d2 +1 d1>d2
	static int do_Compare ( const Time& d1 , const Time& d2 );

	bool operator == ( const Time& o ) const { return (do_Compare(*this,o)==0); }
	bool operator != ( const Time& o ) const { return (do_Compare(*this,o)!=0); }
	bool operator > ( const Time& o ) const { return (do_Compare(*this,o)>0); }
	bool operator < ( const Time& o ) const { return (do_Compare(*this,o)<0); }

   //see http://www.cplusplus.com/reference/ctime/strftime/ for format values
	std::wstring getStringW ( const char* format = "%X" ) const { return to_Wstring(format); }
   //see http://www.cplusplus.com/reference/ctime/strftime/ for format values
   std::string  getString  ( const char* = "%X" ) const;

	static Time now ( );

private:
	int mHour;
	int mMinutes;
	int mSeconds;
};

//Encapsulates local time
class G_LIB_ITEM DateTime 
{
public:
	DateTime ( int d = 1 , int mo = january , int y = 1900 , int h = 0 , int mi = 0 , int s = 0 ):date(d,mo,y),time(h,mi,s){}
	DateTime ( GIntTime_t aIntTime ) { setIntTime ( aIntTime ); }
	DateTime ( const DateTime& o ):date(o.date),time(o.time){}
	virtual ~DateTime ( ){}
	
	DateTime& operator = ( const DateTime& o );

	//returns 0 equal -1 d1<d2 +1 d1>d2
	static int do_Compare ( const DateTime& d1 , const DateTime& d2 );

	bool operator == ( const DateTime& o ) const { return (do_Compare(*this,o)==0); }
	bool operator != ( const DateTime& o ) const { return (do_Compare(*this,o)!=0); }
	bool operator > ( const DateTime& o ) const { return (do_Compare(*this,o)>0); }
	bool operator < ( const DateTime& o ) const { return (do_Compare(*this,o)<0); }

	//returns integer system time (typically seconds from 1.1.1970)
	GIntTime_t getIntTime ( ) const;
	void       setIntTime ( GIntTime_t aIntTime );

	GDayOfTheWeek_t getDayOfTheWeek ( ) const;
	// 1 = for 1th January 2 for 2nd ..
	int             getDayFromYearBeginning ( ) const;

	void       setUtc   ( const DateTime& utc );
	DateTime   getUtc   ( ) const;
	
	static GIntTime_t getSecondsFromUtc ( );

	void       addSeconds ( GIntTime_t );
	void       addMinutes ( GIntTime_t minutes ) { addSeconds(minutes*60); }
	void       addHour    ( GIntTime_t hours ) { addMinutes(hours*60); }

   //see http://www.cplusplus.com/reference/ctime/strftime/ for format values
	std::wstring getStringW ( const char* format = "%c" ) const { return g::to_Wstring ( getString ( format ) ); }
   //see http://www.cplusplus.com/reference/ctime/strftime/ for format values
	std::string  getString  ( const char* format = "%c" ) const;

	bool isDaylightSavingsTime ( ) const;

	Date date;
	Time time;

	static DateTime now ( );
private:

};

inline std::basic_ostream<char>& operator<< (std::basic_ostream<char>& out , const Date& aDate )
{
	out << aDate.getString();

	return out;
}

inline std::basic_ostream<char>& operator<< (std::basic_ostream<char> &out, const Time& aTime )
{
	out << aTime.getString();

	return out;
}

inline std::basic_ostream<char>& operator<< (std::basic_ostream<char> &out, const DateTime& aDateTime )
{
	out << aDateTime.getString();

	return out;
}

inline std::basic_ostream<wchar_t>& operator<< (std::basic_ostream<wchar_t>& out , const Date& aDate )
{
	out << aDate.getStringW();

	return out;
}

inline std::basic_ostream<wchar_t>& operator<< (std::basic_ostream<wchar_t> &out, const Time& aTime )
{
	out << aTime.getStringW();

	return out;
}

inline std::basic_ostream<wchar_t>& operator<< (std::basic_ostream<wchar_t> &out, const DateTime& aDateTime )
{
	out << aDateTime.getStringW();

	return out;
}

}//namespace g

