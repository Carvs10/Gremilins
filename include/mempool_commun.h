#ifndef MEMPOOL_COMMUN_H
#define MEMPOOL_COMMUN_H

#include <iostream>
#include <cstdio>
#include "mpool.h"

struct Tag{ 
	StoragePool * pool;
};

void *operator new( size_t bytes, StoragePool & p )
{
	Tag* const tag = reinterpret_cast <Tag *> ( p.Allocate( bytes + sizeof( Tag ) ) );
	tag->pool = &p;
	// skip sizeof tag to get the raw data-block.
	return( reinterpret_cast <void *> ( tag + 1U ) );
}

void *operator new( size_t bytes )// regular new
{
	Tag* const tag = reinterpret_cast <Tag *> ( std::malloc( bytes + sizeof( Tag ) ) );
	tag->pool = nullptr;
	// skip sizeof tag to get the raw data-block.
	return( reinterpret_cast <void *> ( tag + 1U ) );
}

void operator delete( void * arg ) noexcept 
{
	// we need to subtract 1U (in fact, pointer arithmetics) because arg
	// points to the raw data (second black of information).
	// The pool id (tag) is located 'sizeof(Tag)' bytes before.
	Tag * const tag = reinterpret_cast <Tag *> ( arg ) - 1U;

	if( nullptr != tag->pool ) // Memory block belongs toa particular  GM.
		tag->pool->Realese( tag );
	else
		std::free( tag ); // Memory black belongs to a operation system.
}
#endif