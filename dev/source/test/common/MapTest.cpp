#include "MapTest.h"
#include "TestType.h"
#include "TestHeap.h"
#include "SampleMoveableAllocationPolicy.h"
#include "g_cont_gmap_sorted.h"
#include "g_cont_gmap.h"

using namespace std;
using namespace g;
using namespace cont;

MapTest::MapTest() {}

MapTest::~MapTest() {}

//static Gstr m_Numbers[]={ "zero" , "one" , "two" , "three" , "four" , "five" , "six" , "seven", "eight", "nine", "ten"};
static const char* m_Numbers[]={ "zero" , "one" , "two" , "three" , "four" , "five" , "six" , "seven", "eight", "nine", "ten"};

static int m_Shuffle[] = { 1 , 0 , 5 , 4 , 9 , 10 , 7 , 3 , 2 , 8 , 6 , -1 };

typedef gmap_sorted<int,Gstr> MapSorted_t;
typedef gmap<int,Gstr>        Map_t;

template < class M > struct map_tests
{
	static void fill_map ( M& aMap )
	{
		aMap.setEmpty();

		for (int i = 0; m_Shuffle[i] >= 0 ; ++i)
		{
			aMap.doAdd(m_Shuffle[i],m_Numbers[m_Shuffle[i]]);
		}
	}

	static bool readback_test ( M& aMap )
	{
		try
		{
			fill_map ( aMap );

			for ( int i = 0 ; i <= 10 ; i++ )
			{
				if ( aMap[i] != m_Numbers[i] )
				{
					return false;
				}
			}
			return true;
		}
		catch ( KeyNotFoundException& e )
		{
			cout << e.getErrorMsg() << endl;
			return false;
		}
	}

	static bool ascending_order_test ( M& aMap )
	{
		fill_map ( aMap );

		int cnt = 0;

		for ( typename M::ItConst_t it = aMap.getIterator() ; it.isIn() ; it++ )
		{
			if ( it->key() != cnt++ )
			{
				return false;
			}
		}

		return true;
	}

	static bool descending_order_test ( M& aMap )
	{
		fill_map ( aMap );

		int cnt = 10;

		for (typename M::ItConst_t it = aMap.getIterator() ; it.isIn() ; it++ )
		{
			if ( it->key() != cnt-- )
			{
				return false;
			}
		}

		return true;
	}

	static bool shuffle_order_test ( M& aMap )
	{
		fill_map ( aMap );

		int cnt = 0;

		for ( typename M::ItConst_t it = aMap.getIterator() ; it.isIn() ; it++ )
		{
			if ( it->key() != m_Shuffle[cnt++] )
			{
				return false;
			}
		}

		return true;
	}

	static bool remove_search_test ( M& aMap )
	{
		bool result = true;

		fill_map(aMap);

		aMap.doRemove(1);

		try{
			aMap.doRemove(1);
			result = false;//must be raised to pass the test
		}
      catch(KeyNotFoundException&) 
      {
         result = true;
      }
      catch (Exception& exc)
      {
         cout << "Unexpected exception of type " << exc.getTag () << endl;
         result = false;
      }
      catch ( ... )
      {
         cout << "Unknown exception" << endl;
         result = false;
      }


		result = result && aMap.isKeyContained(2);
		result = result && !aMap.isKeyContained(11);

		result = result && aMap.isValueContained("two");
		result = result && !aMap.isValueContained("eleven");

		Gstr rb;

		result = result && aMap.tryRemove(2,rb) && rb == "two";
		result = result && !aMap.tryRemove(2,rb) && rb == "two";

		result = result && aMap.tryRemove(3) && rb == "two";
		result = result && !aMap.tryRemove(3) && rb == "two";

		return result;
	}
};

bool MapTest::mExecute()
{
	{	
		MapSorted_t map_sorted_plus(comparer<int>::more,AllocationPolicyAbstract::get_FromStandardPolicy(),1,1);
		
		mAddSubResult( "Plus sorted read back test","",map_tests<MapSorted_t>::readback_test(map_sorted_plus) );
		mAddSubResult("Plus sorted ordering test","",map_tests<MapSorted_t>::descending_order_test(map_sorted_plus));
		mAddSubResult("Plus sorted remove seacrh test","",map_tests<MapSorted_t>::remove_search_test(map_sorted_plus));
	}

	mPerformHeapWalkCheck("Plus sorted" );
	
	{	
		MapSorted_t map_sorted_minus(comparer<int>::less,AllocationPolicyAbstract::get_FromStandardPolicy(),1,1);
		
		mAddSubResult( "Minus sorted read back test","",map_tests<MapSorted_t>::readback_test(map_sorted_minus) );
		mAddSubResult("Minus sorted ordering test","",map_tests<MapSorted_t>::ascending_order_test(map_sorted_minus));
		mAddSubResult("Minus sorted remove seacrh test","",map_tests<MapSorted_t>::remove_search_test(map_sorted_minus));
	}

	mPerformHeapWalkCheck("Minus sorted" );

	{	
		Map_t map_simple(AllocationPolicyAbstract::get_FromStandardPolicy(),1,1);
		
		mAddSubResult("Simple read back test","",map_tests<Map_t>::readback_test(map_simple) );
		mAddSubResult("Simple ordering test","",map_tests<Map_t>::shuffle_order_test(map_simple));
		mAddSubResult("Simple remove seacrh test","",map_tests<Map_t>::remove_search_test(map_simple));
	}

	mPerformHeapWalkCheck("Simple" );

	return true;
}

