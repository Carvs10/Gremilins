#ifndef MPOOL_H
#define MPOOL_H


#include <iostream>
#include <cstdio> //std::size_t
#include <cmath>  //std::ceil
#include <new>    //std::bad_alloc
//#include "mempool_commun.h"
//#include "mempool_commun.h"
//#include "../include/mempool_commun.h"

//FALTA FAZER:
//CONSTRUTOR	x
//Allocate	x
//GTESTES
//Free ~
//APRESESNTAR CODIGO DEMONNSTRANDO MELHOR DESENPENHO EM RELACAO AO SO
//MEMORY VISUALISATION
//BEST FIT


namespace mp{
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
				
				this->m_sentinel.m_next = this->m_pool;//ponteiro do sentinela
				this->m_sentinel.m_length = 0;		//tamanho do sentinela

				//out
				std::cout << m_pool[0].m_length << "\n" << m_sentinel.m_next << "\ncontrutor \n";
			}

			~SLPool()
			{
				delete[] m_pool;

				std::cout << "destruiu \n";
			}





			void * Allocate( size_t	mem )//Alocar um raw para o clinte usando First-fit.
			{

				Block *fpt = this-> m_sentinel.m_next;//ponteiro de posicao
				Block *spt = &m_sentinel;
				long int mem_blocks = std::ceil( ( mem + sizeof( Header ))/BLK_SIZE );//blocos que serao alocados

				while( fpt != nullptr ){

					if( mem_blocks <= fpt->m_length ){

						
						if ( mem_blocks == fpt->m_length ){//caso nao precise criar uma free area nova

							spt->m_next = fpt->m_next;//ponteiro antigo tem que ser atualizado

							fpt->m_length = mem_blocks;//colocar o tamanho do espaco alocado 
							return fpt;
						}

						spt->m_next = fpt + mem_blocks; //ponteiro antigo tem que ser atualizado

						spt->m_next->m_next = fpt->m_next;//colocar o ponteiro do novo no

						spt->m_next->m_length = fpt->m_length - mem_blocks; //colocar o espaco do novo no

						fpt->m_length = mem_blocks;//colocar o tamanho do espaco alocado 

						return fpt;

					}

					spt = fpt;//ponteiro aux na posicao anterior
					fpt = fpt->m_next;//avança o ponteiro

				}
				
				throw std::bad_alloc( );
			}



			void Free ( void * pt_m )
			{	

				pt_m = reinterpret_cast<Block *> (reinterpret_cast <Header *> (pt_m) - (1U)); //entender melhor como funciona o reinterpret_cast!!!

				Block * ptReserved = (Block *) pt_m; 			//memoria a ser liberada
				Block * ptPostReserved = this-> m_sentinel.m_next;	//ponteiro fast
				Block *	ptPrevReserved = &m_sentinel;				//ponteiro slow

				while( (ptPostReserved != nullptr) or (ptPostReserved <= ptReserved) ){//deixar um dois ponteiros entre o ponteiro para a memoria a ser liberada

					ptPrevReserved = ptPostReserved;
					ptPostReserved = ptPostReserved->m_next;
				}

				if( ((ptPrevReserved + ptPrevReserved->m_length) == ptReserved) and ((ptReserved->m_length + ptReserved) == ptPostReserved) ){//verificar se vai juntar duas areas livres(esquerda e direita) 

					ptPrevReserved->m_next = ptPostReserved->m_next;																//formar um ponteiro unico que aponta para o proximo bloco
					ptPrevReserved->m_length = (ptPrevReserved->m_length + ptReserved->m_length + ptPostReserved->m_length);		//formar um tamanho so com os 3 blocos
				}

				else if( (ptReserved->m_length + ptReserved) == ptPostReserved ){	//verificar se a direita é livre 

					ptReserved->m_next = ptPostReserved->m_next;					//juntar o tamanho da direita e o espaco novo em um unico tamanho
					ptReserved->m_length = ptReserved->m_length + ptPostReserved->m_length;	//m_next desse novo no
				} 

				else if( (ptPrevReserved + ptPrevReserved->m_length) == ptReserved ){//vreficar se a esquerda é livre

					ptPrevReserved->m_length = ptPrevReserved->m_length + ptReserved->m_length;//juntar tamanho da esquerda com o espaço novo para o proximo bloco
				}

				else{												//verificar se nao existe areas libres adjacentes

					ptPrevReserved->m_next = ptReserved;			//m_next vai ter que apontar para proximo bloco
					ptReserved->m_next = ptPostReserved;			//m_next vai ter que apontar para proximo bloco
				}
				//verificar se vai juntar duas areas livres(esquerda e direita) v
					//formar um tamanho so com os 3 blocos	v
					//formar um ponteiro unico que aponta para o proximo bloco 	v
				//verificar se a direita é livre v
					//juntar o tamanho da direita e o espaco novo em um unico tamanho v
					//m_next desse novo no  v
				//vreficar se a esquerda é livre v
					//juntar tamanho da esquerda com o espaço novo para o proximo bloco v
					//m_next apontar para proximo bloco v
				//verificar se nao existe areas libres adjacentes	v
					//tamanho vai ser igual ao tamanho do Header v
					//m_next vai ter que apontar para proximo bloco v

				std::cout << "amigo estou aqui!!!\n";
			}

	};
}
#endif