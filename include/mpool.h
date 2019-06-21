#ifndef MPOOL_H
#define MPOOL_H


#include <iostream>
#include <cstdio> //std::size_t
#include "mempool_commun.h"

//teste




class StoragePool{


	public:

		virtual ~StoragePool();
		//operadores->

		virtual void* Allocate( size_t ) = 0;
		//gerar uma exceçao senao tiver a memoria solicitada

		virtual void Free( void* ) = 0;



	private:

};

template < size_t BLK_SIZE = 16 >

class SLPool : public StoragePool{

	public:

		struct Header
		{
			size_t m_length;
			Header() : m_length(0u) {/* Empty */};
		};

		struct Block : public Header 
		{
			Block *m_next; // Pointer to next block OR..
			char  m_raw[ BLK_SIZE - sizeof( Header ) ]; // Clients raw area 
		};

	private:

		unsigned int m_n_blocks; //!< Number of blocks in the list.
		Block *m_pool;           //!< Head of list.
		Block &m_sentinel;		 //!< End of the list.

	public:

		explicit SLPool( size_t );
		~SLPool();
		void * Allocate( size_t );
		void Free ( void * );

};

#endif