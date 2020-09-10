#pragma once

#include "g_cont_ref.h"
#include "g_cont_ref_lst.h"
#include "g_cont_vect.h"
#include "g_str_lib.h"
#include "g_signal_queueDataBuffer.h"
#include <string>
/****************************************************************************************
	Defines an abstract interface to the data queue for serializable signals.
****************************************************************************************/
namespace g
{
namespace signal
{
//!Subclass for defining queue rx side.
struct QueueReceiverAbstract
{
	typedef g::cont::ref<QueueReceiverAbstract> Ref_t;

	virtual QueueDataBufferAbstract::Ref_t pop ( GTimeoutSec_t ) = 0;
};

//!Subclass for definng queue tx side.
struct QueueSenderAbstract
{
	typedef g::cont::ref<QueueSenderAbstract> Ref_t;
	
	virtual QueueDataBufferAbstract::Ref_t instanciateDataBufferRef ( ) = 0;
	virtual void                           push                     ( QueueDataBufferAbstract::Ref_t& ) = 0;
};

struct QueueSenderReceiverAbstract : QueueSenderAbstract , QueueReceiverAbstract
{
	typedef g::cont::ref<QueueSenderReceiverAbstract> Ref_t;
};


typedef g::cont::ref_lst<QueueDataBufferAbstract> DataBufferQueue_t;

struct StdSignalDataPacket : QueueDataBufferAbstract , g::cont::vect<char>
{
	StdSignalDataPacket (
			g::cont::AllocationPolicyAbstract* ap = g::cont::AllocationPolicyAbstract::get_FromStandardPolicy() ,
			int aAllocDeltaRBits = G_VECT_AL_DELTA_MASK_BITS ) : g::cont::vect<char>(ap,aAllocDeltaRBits){}

	typedef g::cont::ref<StdSignalDataPacket> Ref_t;

	virtual void  write ( const void* aData , size_t aSize )
	{
		size_t current_size = getSize();

		reSize ( current_size + aSize );

		g::cont::move_data ( (const char*) aData , (char*)(*this)+mOffset , aSize );

		mOffset += aSize;
	}

	virtual const void* read  ( void* aData , size_t aSize )
	{
		const char* current_data = ((const char*)(*this)) + mOffset;
		
		if ( aData != NULL )
		{
			g::cont::move_data<char> ( current_data , (char*) aData , aSize );			
		}

		mOffset += aSize;

		return current_data;
	}

	virtual const void* getCurrentPtr ( ) { return ((const char*)(*this)) + mOffset; }
};

struct QueueTxRxSimple : QueueSenderAbstract , QueueReceiverAbstract
{
	DataBufferQueue_t queue;

	QueueTxRxSimple(g::cont::AllocationPolicyAbstract* ap = g::cont::AllocationPolicyAbstract::get_FromStandardPolicy ( )):queue(ap){}

	QueueDataBufferAbstract::Ref_t instanciateDataBufferRef ( )	{ return g::cont::ref<StdSignalDataPacket>::g_cont_new(queue.getAllocPolicyP()); }
	QueueDataBufferAbstract::Ref_t pop ( GTimeoutSec_t ) { return queue.popHead();	}	
	void push ( QueueDataBufferAbstract::Ref_t& aDataPacket ) {	queue.pushTail(aDataPacket); }
};


}//namespace signal
}//namespace g

