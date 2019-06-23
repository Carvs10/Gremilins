#ifndef MPOOL_H
#define MPOOL_H


#include <iostream>
#include <cstdio> //std::size_t
#include <cmath>  //std::ceil
#include <new>    //std::bad_alloc
//#include "mempool_commun.h"
//#include "mempool_commun.h"
//#include "../include/mempool_commun.h"

//teste




class StoragePool{


	public:

		virtual ~StoragePool(){};
		//operadores->

		virtual void* Allocate( size_t ) = 0;
		//gerar uma exceçao senao tiver a memoria solicitada

		virtual void Free( void * ) = 0;




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
			union
			{
				Block *m_next; // Pointer to next block OR..
				char  m_raw[ BLK_SIZE - sizeof( Header ) ]; // Clients raw area 
			};
			Block() : Header(), m_next( nullptr ) {/* Empty */};
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
			std::cout << m_pool[0].m_length << "\n" << m_sentinel.m_next << "\ncontrutor \n";
		}


		


		~SLPool()
		{
			delete[] m_pool;

			std::cout << "destruiu \n";
		}





		void * Allocate( size_t	mem )//Alocar um raw para o clinte usando best fit
		{

			Block *fpt = this-> m_sentinel.m_next;//ponteiro de posicao
			Block *spt = &m_sentinel;
			long int mem_blocks = std::ceil( ( mem + sizeof( Header ))/BLK_SIZE );//blocos que serao alocados

			while( fpt != nullptr ){

				if( mem_blocks <= fpt->m_length ){

					//fpt->m_length = mem_blocks;	//colocar o tamanho do espaco alocado 

					if ( mem_blocks == fpt->m_length ){//caso nao precise criar uma free area nova

						spt->m_next = fpt->m_next;//ponteiro antigo tem que ser atualizado
						fpt->m_length = mem_blocks;
						return fpt;
					}

					spt->m_next = fpt + mem_blocks //ponteiro antigo tem que ser atualizado

					spt->m_next->m_next = fpt->m_next;//colocar o ponteiro do novo no

					spt->m_next->m_length = fpt->m_length - mem_blocks; //colocar o espaco do novo no

					fpt->m_length = mem_blocks;

					//for(int i = 0; i <= mem_blocks; i++){

					//	spt	= spt->m_next;
					//}

					return fpt;

					//alocar
					//colocar o tamanho do espaco alocado v
					//colocar o espaco do novo no v
					//colocar o ponteiro do novo no v
					//ponteiro antigo tem que ser atualizado v
				}

				spt = fpt;//ponteiro aux na posicao anterior
				fpt = fpt->m_next;//avança o ponteiro

			}
			

			//bad alloc
			throw std::bad_alloc();
		}






		void Free ( void * )
		{
			std::cout << "amigo estou aqui!!!\n";
		}

};

#endif