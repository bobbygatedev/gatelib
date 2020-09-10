#include "g_common_dev.h"
#include "g_time_functions.h"

#include <stdio.h>

namespace g
{
namespace time
{
double G_LIB_ITEM get_hi_performance_timer_freq ( )
{	
   G_EXC_SET_CONTEXT("double get_hi_performance_timer_freq ( )");

#if G_AUTOPTION_ON_WINDOWS
	LARGE_INTEGER l_i;

   G_WIN_CALL ( QueryPerformanceFrequency ( &l_i ) );

	return ( double ) l_i.QuadPart;		
#else
   timespec ts;
   
   G_POSIX_CALL ( clock_getres(CLOCK_REALTIME, &ts) ); // Works on Linux

   return (double)ts.tv_sec + 1e-9 * ((double)ts.tv_nsec);
#endif
}


double G_LIB_ITEM get_hi_res_time ( )
{
   G_EXC_SET_CONTEXT("double get_hi_res_time ( )");

#if G_AUTOPTION_ON_WINDOWS

   static double hi_res_period = 1.0 / get_hi_performance_timer_freq();

	LARGE_INTEGER l_i;

   G_WIN_CALL ( QueryPerformanceCounter ( &l_i ) );

   //return l_i;
	return l_i.QuadPart * hi_res_period;
#else
   timespec ts;
   
   G_POSIX_CALL ( clock_gettime(CLOCK_REALTIME, &ts) );// Works on Linux

   return (double)ts.tv_sec + 1e-9 * ((double)ts.tv_nsec);
#endif
}

}//namespace time
}//namespace g
