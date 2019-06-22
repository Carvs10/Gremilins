#ifndef MPOOL_H
#define MPOOL_H


#include <iostream>
#include <cstdio> //std::size_t
#include <cmath>  //std::ceil
//#include "mempool_commun.h"
//#include "../include/mempool_commun.h"

//teste




class StoragePool{


	public:

		virtual ~StoragePool(){};
		//operadores->

		//virtual void* Allocate( size_t ) = 0;
		//gerar uma exceçao senao tiver a memoria solicitada

		//virtual void Free( void* ) = 0;




};

template < size_t BLK_SIZE = 16 >

class SLPool : public StoragePool{

	public:

		struct Header
		{
			size_t m_length;
			Header() : m_length(0u) {};
		};
	
		struct Block : public Header 
		{
			union
			{
				Block *m_next; // Pointer to next block OR..
				char  m_raw[ BLK_SIZE - sizeof( Header ) ]; // Clients raw area 
			};
			Block() : Header(), m_next( nullptr ) {};
		};
	
	private:

		unsigned int m_n_blocks; //!< Number of blocks in the list.
		Block *m_pool;           //!< Head of list.
		Block &m_sentinel;		 //!< End of the list.        

	public:

		explicit SLPool( size_t mem ) : 
			m_n_blocks(  std::ceil( ( mem + sizeof( Header ))/BLK_SIZE ) + 1 ),//pega o numero de blocks + sentinela
			m_pool{ new Block[m_n_blocks] },//aloca vetor de blocks 
			m_sentinel{ m_pool[m_n_blocks - 1] }//ultimo elemento da lista alocada
		{
			this->m_pool[0].m_length = (m_n_blocks -1);//tamanho do espaço livre
			this->m_pool[0].m_next = nullptr;//ponteiro para proximo node que nao existe
			
			this->m_sentinel.m_next = this->m_pool;
			this->m_sentinel.m_length = 0;

			//out
			std::cout << m_pool[0].m_length << m_sentinel.m_next << "\ncontrutor \n";
		}


		


		~SLPool()
		{
			delete[] m_pool;

			std::cout << "destruiu \n";
		}

		//void * Allocate( size_t );
		//void Free ( void * );

};

#endif