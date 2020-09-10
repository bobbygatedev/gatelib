#pragma once

#include "g_cont_cont_with_positioner.h"
#include "g_cont_vect.h"
#include "g_cont_common.h"
#include "g_cont_AllocationPolicyAbstract.h"
#include "g_cont_private_data_pointer.h"

#ifndef NULL
#define NULL 0
#endif

#define LIST_ALLOCATOR_DELTA 16
namespace g
{
namespace cont
{
namespace priv
{
#if G_AUTOPTION_ON_GCC
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

template < class T > struct list_node
{
	bool isNodeBusy () const{ return currentIndex >= 0; }

	void release ( ) 
	{ 		
		currentIndex = G_CONT_INDEX_NOT_VALID;
	}

	int prevIndex;
	int nextIndex;
	int currentIndex;

	void setContent ( const T& aT )
	{
		new(reinterpret_cast<T*>(mContBuff))T(aT);
	}

	operator const T& ( ) const { return *(reinterpret_cast<const T*>( mContBuff )); }
	operator T& ( ) { return *(reinterpret_cast<T*>( mContBuff )); }

	void callContentDestructor ( )
	{
		( reinterpret_cast<T*> ( mContBuff ) )->~T();
	}

	// Trick for obtain list_node address from the (TRUE item )
	static list_node* nodeFromItem ( const T* aItemIndex )
	{
		static list_node* dummy = 0;
		static size_t      delta = ( size_t )&dummy->mContBuff;
		
		return ( list_node* ) ( ( char* )aItemIndex - delta );
	}

	class data_instanciable
	{
	public:
		typedef list_node Node_t;
		typedef priv::data_pointer<list_node> ListNodesDataPointer_t;

		data_instanciable():mSize(0){}
		
		~data_instanciable()
		{
			if ( mSize > 0 )
			{
				( reinterpret_cast<ListNodesDataPointer_t*> (mData) )->~ListNodesDataPointer_t();
			}
		}

		operator Node_t* ( ) { return (Node_t*)*( reinterpret_cast<ListNodesDataPointer_t*> (mData) ); }
					
		operator ListNodesDataPointer_t& ( ) { return *( reinterpret_cast<ListNodesDataPointer_t*> (mData) ); }
		operator const ListNodesDataPointer_t& ( ) { return *( reinterpret_cast<const ListNodesDataPointer_t*> (mData) ); }

		void instanciate ( AllocationPolicyAbstract* aAllocPolicyP , size_t size )
		{
			ListNodesDataPointer_t* data_p =  new (mData) ListNodesDataPointer_t ( aAllocPolicyP );

			data_p->allocate ( mSize = size , 0 );

			//set the page to not valid
			for ( size_t i = 0 ; i < mSize ; i++ )
			{
				( *data_p )[i].currentIndex = G_CONT_INDEX_NOT_VALID;
			}
		}

		size_t getSize ( ) const { return mSize; }

	private:
		char mData[sizeof(priv::data_pointer<Node_t>)];
		size_t mSize;
	};

private:
	char mContBuff[sizeof(T)];
};


template < class T > class list_node_page
{
#define G_LOCAL_CLASS "list_node_page<T>::"

public:
	list_node_page ( const list_node_page& )
	{      
      G_EXC_SET_CONTEXT ( G_LOCAL_CLASS "::list_node_page(const list_node_page& o )" );

		G_EXC_FATAL_ACTION ("Prohibited copy constructor declared for compatibility only!");
	}

	typedef vect<list_node_page> Vect_t;
	typedef list_node<T>         Node_t; 

	list_node_page() : numUsed(0),firstFreeNodeIndex(0) {	}

	typename Node_t::data_instanciable nodes;

	Node_t* getFreeNodeAndMakeBusy ( )
	{
      G_EXC_SET_CONTEXT ( "Node_t* " G_LOCAL_CLASS "::getFreeNodeAndMakeBusy ( )" );

		int pos_out = firstFreeNodeIndex;

		//if all nodes are used
		if ( ++numUsed < nodes.getSize() )
		{
			for ( int i = firstFreeNodeIndex + 1 ; i <  (int)nodes.getSize() ; i++ )
			{
				if ( !nodes[i].isNodeBusy ( ) )
				{
					firstFreeNodeIndex = i;
					goto out;
				}
			}
						
			G_EXC_FATAL_ACTION ( "First free index is not reassigned but the page is not full!" ); 
		}
		else
		{
			firstFreeNodeIndex = (int)nodes.getSize();
		}

out:
		nodes[pos_out].currentIndex = pos_out;

		return (Node_t*)nodes + pos_out;
	}

	//releases the node and update firstFreeNodeIndex
	void releaseNodeUpdateFirstFree ( int aPageNodeIndex )
	{
		nodes[aPageNodeIndex].release ( );
		numUsed--;

		if ( aPageNodeIndex < firstFreeNodeIndex )
		{
			firstFreeNodeIndex = aPageNodeIndex;
		}
	}
	
	size_t numUsed;
	int    firstFreeNodeIndex;

	int isPageBusy () const { return numUsed >= nodes.getSize(); }
#undef G_LOCAL_CLASS
};

template < class T > class list_content
{
public:
	typedef typename list_node_page<T>::Vect_t PageVector_t;
	typedef list_node<T> Node_t;   

	list_content ( 
		AllocationPolicyAbstract* aAllocPolicyP ,      //pointer to allocation policy 
		int                       aListPageRightBits , //list page size in number of bits ( page size = 1<<aListPageRightBits )
		int                       aPageVectorAllocDelta ) : 
			firstIndex (G_CONT_INDEX_NOT_VALID),
			lastIndex(G_CONT_INDEX_NOT_VALID),
			size(0),
			allocPolicyP(aAllocPolicyP),
			pages(aAllocPolicyP,aPageVectorAllocDelta),
			pageRightbits(aListPageRightBits),
			firstFreePage(-1){}

	~list_content( ) { erase ( ); } 

	int                       firstIndex;
	int                       lastIndex;
	size_t                    size;
	AllocationPolicyAbstract* allocPolicyP;
	PageVector_t              pages;
	int                       pageRightbits;
	int                       firstFreePage;

	void erase ( )
	{
		empty ( );
		pages.eraseMemory ( );
		firstFreePage = -1;
	}

	Node_t* getNodeFromIndex ( int aItemIndex )
	{
		return &( pages[ (aItemIndex>>pageRightbits) ].nodes[aItemIndex & ( (1<<pageRightbits) - 1 ) ] );
	}

	T remove ( int aItemIndex )
	{		
		Node_t* n_p    = getNodeFromIndex ( aItemIndex );
		T       result = *n_p;

		if ( aItemIndex == firstIndex )
		{
			firstIndex = n_p->nextIndex;
		}
		else if ( n_p->prevIndex != G_CONT_INDEX_NOT_VALID )
		{
			Node_t* prev_node_p = getNodeFromIndex ( n_p->prevIndex );
			prev_node_p->nextIndex = n_p->nextIndex;
		}

		if ( aItemIndex == lastIndex )
		{
			lastIndex = n_p->prevIndex;
		}
		else if ( n_p->nextIndex != G_CONT_INDEX_NOT_VALID )
		{
			Node_t* next_node_p = getNodeFromIndex ( n_p->nextIndex );
			next_node_p->prevIndex = n_p->prevIndex;
		}
		
		size--;

		int current_page = ( aItemIndex >> pageRightbits );

		pages[current_page].releaseNodeUpdateFirstFree ( aItemIndex & ( (1<<pageRightbits) - 1 ) );

		if ( current_page < firstFreePage )
		{
			firstFreePage = current_page;
		}
			
		n_p->callContentDestructor ( );

		return result;
	}

	void empty ( ) 
	{
		int node_index = firstIndex;

		while ( node_index != G_CONT_INDEX_NOT_VALID )
		{
			pages[node_index >> pageRightbits].releaseNodeUpdateFirstFree ( node_index & ( ( 1<<pageRightbits) - 1 ) );

			Node_t* node_p = getNodeFromIndex ( node_index );
			
			node_p->callContentDestructor ( );

			node_index = node_p->nextIndex;
		}

		//zeroes
		firstIndex = G_CONT_INDEX_NOT_VALID;
		lastIndex  = G_CONT_INDEX_NOT_VALID;
		size = 0;

		firstFreePage = ( pages.getSize( )>0 )?0:-1;
	}

	void appendPage ( )
	{
		size_t new_size = pages.getSize()+1;

		pages.reSize(new_size);
		pages[(int)new_size-1].nodes.instanciate ( allocPolicyP , 1<<pageRightbits );			
	}
	
	Node_t* getAndSetFreeNode ( const T& aT )
	{		
		if ( firstFreePage == -1 )//not initialised
		{
			appendPage ( );
			firstFreePage++;
		}

		int current_page_index = firstFreePage;

		list_node_page<T>& free_page_ref = pages[firstFreePage];

		Node_t* n_p = free_page_ref.getFreeNodeAndMakeBusy ( );

		//search next free page
		if ( free_page_ref.isPageBusy( ) )
		{
			//search fro a free page
			for ( ++firstFreePage ; firstFreePage < (int)pages.getSize() ; firstFreePage++ )
			{
				if ( !pages[firstFreePage].isPageBusy ( ) )
				{
					break;
				}				
			}

			if ( firstFreePage >= (int)pages.getSize ( ) )
			{
				appendPage ( );
			}
		}	

		n_p->currentIndex |= ( current_page_index << pageRightbits );
		n_p->setContent ( aT );

		return n_p;
	}

	void pushNewNodeAfter ( const T& aItem , int aIndexBeingPrevious )
	{
		Node_t* new_node_p  = getAndSetFreeNode ( aItem );
		Node_t* prev_node_p = NULL;
		Node_t* next_node_p = NULL;

		if ( aIndexBeingPrevious == G_CONT_INDEX_NOT_VALID )
		{
			//at tail
			if ( lastIndex != G_CONT_INDEX_NOT_VALID )
			{
				prev_node_p = getNodeFromIndex ( lastIndex );
			}
		}
		else
		{
			prev_node_p = getNodeFromIndex ( aIndexBeingPrevious );
			
			if ( prev_node_p->nextIndex != G_CONT_INDEX_NOT_VALID )
			{
				next_node_p = getNodeFromIndex ( prev_node_p->nextIndex );
			}
		}

		mPushNewNodeAfter ( new_node_p , prev_node_p , next_node_p );
	}

	void pushNewNodeBefore ( const T& aItem , int aIndexBeingNext )
	{
		Node_t* new_node_p  = getAndSetFreeNode ( aItem );

		Node_t* prev_node_p = NULL;
		Node_t* next_node_p = NULL;

		if ( aIndexBeingNext == G_CONT_INDEX_NOT_VALID )
		{
			//at tail
			if ( firstIndex != G_CONT_INDEX_NOT_VALID )
			{
				next_node_p = getNodeFromIndex ( firstIndex );
			}
		}
		else
		{
			next_node_p = getNodeFromIndex ( aIndexBeingNext );

			if ( next_node_p->prevIndex != G_CONT_INDEX_NOT_VALID )
			{
				prev_node_p = getNodeFromIndex ( next_node_p->prevIndex );
			}
		}

		mPushNewNodeAfter ( new_node_p , prev_node_p , next_node_p );
	}

private:
	//new_node_p->prevIndex and new_node_p->nextIndex are supposed to be initialised
	void mPushNewNodeAfter ( Node_t* new_node_p , Node_t* before_node_p , Node_t* after_node_p )
	{
		if ( after_node_p )
		{
			after_node_p->prevIndex = new_node_p->currentIndex;
			new_node_p->nextIndex = after_node_p->currentIndex;
		}
		else
		{
			//becomes tail
			new_node_p->nextIndex = G_CONT_INDEX_NOT_VALID;
			lastIndex = new_node_p->currentIndex;
		}

		if ( before_node_p )
		{
			before_node_p->nextIndex = new_node_p->currentIndex;
			new_node_p->prevIndex = before_node_p->currentIndex;
		}
		else
		{
			//becomes head
			new_node_p->prevIndex = G_CONT_INDEX_NOT_VALID;
			firstIndex = new_node_p->currentIndex;
		}

		size++;
	}
};

template < class T > class list_positioner : public positioner_abstract<T>
{
public:
	list_positioner ( list_content<T>* aLstContentP ) : lstContentP (aLstContentP) {}
	virtual ~list_positioner ( ) {}

	virtual int first ( ) const { return lstContentP->firstIndex; }
	virtual int last  ( ) const { return lstContentP->lastIndex; }

	virtual bool isIn ( int aItemIndex ) const	
	{
		return aItemIndex != G_CONT_INDEX_NOT_VALID;
	}

	virtual void forward  ( int& aItemIndex , GUint32_t inc ) const
	{ 
		if ( aItemIndex != G_CONT_INDEX_NOT_VALID )
		{
			list_node<T>* n_p = lstContentP->getNodeFromIndex(aItemIndex);

			for(GUint32_t i = 0 ; i < inc ; i++)		
			{
				if ( n_p->nextIndex != G_CONT_INDEX_NOT_VALID )
				{
					n_p = lstContentP->getNodeFromIndex(n_p->nextIndex);
				}
				else
				{
					aItemIndex = G_CONT_INDEX_NOT_VALID;
					return;
				}
			}

			aItemIndex = n_p->currentIndex;
		}
	}

	virtual T* getPtr ( int aItemIndex ) const
	{
		//T* temp = &((T&)*lstContentP->getNodeFromIndex(aItemIndex)); UNCOMMENT FOR DBG PURPOSES

		return &((T&)*lstContentP->getNodeFromIndex(aItemIndex));
	}
	
	virtual void backward ( int& aItemIndex , GUint32_t dec ) const
	{ 
		if ( aItemIndex != G_CONT_INDEX_NOT_VALID )
		{
			list_node<T>* n_p = lstContentP->getNodeFromIndex(aItemIndex);

			for(GUint32_t i = 0 ; i < dec ; i++)		
			{
				if ( n_p->prevIndex != G_CONT_INDEX_NOT_VALID )
				{
					n_p = lstContentP->getNodeFromIndex(n_p->prevIndex);
				}
				else
				{
					aItemIndex = G_CONT_INDEX_NOT_VALID;
					return;
				}
			}

			aItemIndex = n_p->currentIndex;
		}
	}
	
	list_content<T>* lstContentP;
};

#if G_AUTOPTION_ON_GCC
#	pragma GCC diagnostic pop
#endif

}//namespace priv
}//namespace cont
}//namespace g

