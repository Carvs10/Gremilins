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
//Free x
//APRESESNTAR CODIGO DEMONNSTRANDO MELHOR DESENPENHO EM RELACAO AO SO
//MEMORY VISUALISATION
//BEST FIT

// static constexpr size_t BLK_SZ = sizeof( mp::SLPool<BLK_SIZE>::Block ); //!< The block size in bytes.
// static constexpr size_t TAG_SZ = sizeof( mp::Tag ); //!< The Tag size in bytes (each reserved area has a tag).
// static constexpr size_t HEADER_SZ = sizeof( mp::SLPool<BLK_SIZE>::Header ); //!< The header size in bytes


namespace mp{
	class StoragePool{


		public:

			virtual ~StoragePool(){};
			//operadores->

			virtual void* Allocate( size_t ) = 0;
			//gerar uma exceçao senao tiver a memoria solicitada

			virtual void Free( void * ) = 0;




	};

	struct Tag
			{ 
				StoragePool * pool;
			};





	template < size_t BLK_SIZE = 16 >

	class SLPool : public StoragePool{

		public:

		static constexpr size_t HEADER_SZ = sizeof( mp::SLPool<BLK_SIZE>::Header ); 
		static constexpr size_t BLK_SZ = sizeof( mp::SLPool<BLK_SIZE>::Block ); //!< The block size in bytes.
		static constexpr size_t TAG_SZ = sizeof( mp::Tag ); //!< The Tag size in bytes (each reserved area has a tag).
		//static constexpr size_t HEADER_SZ = sizeof( mp::SLPool<BLK_SIZE>::Header ); //!< The header size in bytes


		

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

			// struct Tag
			// { 
			// 	StoragePool * pool;
			// };


		
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
				std::cout << "entrou1 \n";

				Block *fpt = this-> m_sentinel.m_next;//ponteiro de posicao
				Block *spt = &m_sentinel;
				long int mem_blocks = std::ceil( ( mem + sizeof( Header ))/BLK_SIZE );//blocos que serao alocados

				while( fpt != nullptr ){

					if( mem_blocks <= fpt->m_length ){

						std::cout << "tem espaço \n";

						if ( mem_blocks == fpt->m_length ){//caso nao precise criar uma free area nova

							std::cout << "espaco = \n";

							spt->m_next = fpt->m_next;//ponteiro antigo tem que ser atualizado

							fpt->m_length = mem_blocks;//colocar o tamanho do espaco alocado 
							//return fpt;
							return reinterpret_cast<void*> (reinterpret_cast<Header *> (fpt) + (1U));
						}

						std::cout << "espaço maior \n";

						spt->m_next = fpt + mem_blocks; //ponteiro antigo tem que ser atualizado

						spt->m_next->m_next = fpt->m_next;//colocar o ponteiro do novo no

						spt->m_next->m_length = fpt->m_length - mem_blocks; //colocar o espaco do novo no

						fpt->m_length = mem_blocks;//colocar o tamanho do espaco alocado 

						//return fpt;
						return reinterpret_cast<void*> (reinterpret_cast<Header *> (fpt) + (1U));

					}

					spt = fpt;//ponteiro aux na posicao anterior
					fpt = fpt->m_next;//avança o ponteiro

				}
				
				throw std::bad_alloc( );
			}



			void Free ( void * pt_m )
			{	
				std::cout << "entrou2 \n";

				pt_m = reinterpret_cast<Block *> (reinterpret_cast <Header *> (pt_m) - (1U)); //entender melhor como funciona o reinterpret_cast!!!

				Block * ptReserved = (Block *) pt_m; 			//memoria a ser liberada
				Block * ptPostReserved = this-> m_sentinel.m_next;	//ponteiro fast
				Block *	ptPrevReserved = &m_sentinel;				//ponteiro slow

				std::cout << "passou \n";

				while( (ptPostReserved != nullptr) or (ptPostReserved <= ptReserved) ){//deixar um dois ponteiros entre o ponteiro para a memoria a ser liberada

					std::cout << ptPostReserved << "   " << ptReserved << "  looping \n";

					if(ptPostReserved > ptReserved){

						std::cout << "passou2 \n";
						break;
					}

					ptPrevReserved = ptPostReserved;
					ptPostReserved = ptPostReserved->m_next;
				}

				std::cout << "passou3 \n";

				if( ((ptPrevReserved + ptPrevReserved->m_length) == ptReserved) and ((ptReserved->m_length + ptReserved) == ptPostReserved) ){//verificar se vai juntar duas areas livres(esquerda e direita) 

					std::cout << "igual igual \n";
					ptPrevReserved->m_next = ptPostReserved->m_next;																//formar um ponteiro unico que aponta para o proximo bloco
					ptPrevReserved->m_length = (ptPrevReserved->m_length + ptReserved->m_length + ptPostReserved->m_length);		//formar um tamanho so com os 3 blocos
				}

				else if( (ptReserved->m_length + ptReserved) == ptPostReserved ){	//verificar se a direita é livre 
					std::cout << "igual direita \n";
					ptReserved->m_next = ptPostReserved->m_next;					//juntar o tamanho da direita e o espaco novo em um unico tamanho
					ptReserved->m_length = ptReserved->m_length + ptPostReserved->m_length;	//m_next desse novo no
				} 

				else if( (ptPrevReserved + ptPrevReserved->m_length) == ptReserved ){//vreficar se a esquerda é livre
					std::cout << "igual esquerda \n";
					ptPrevReserved->m_length = ptPrevReserved->m_length + ptReserved->m_length;//juntar tamanho da esquerda com o espaço novo para o proximo bloco
				}

				else{												//verificar se nao existe areas libres adjacentes
					std::cout << "nada \n";
					ptPrevReserved->m_next = ptReserved;			//m_next vai ter que apontar para proximo bloco
					ptReserved->m_next = ptPostReserved;			//m_next vai ter que apontar para proximo bloco
				}

				std::cout << "amigo estou aqui!!!\n";
			}

	};



}

void *operator new( size_t bytes, mp::StoragePool & p )
{
	mp::Tag* const tag = reinterpret_cast <mp::Tag *> ( p.Allocate( bytes + sizeof( mp::Tag ) ) );
	tag->pool = &p;
	// skip sizeof tag to get the raw data-block.
	return( reinterpret_cast <void *> ( tag + 1U ) );
}
void *operator new[]( size_t bytes, mp::StoragePool & p )
{
	mp::Tag* const tag = reinterpret_cast <mp::Tag *> ( p.Allocate( bytes + sizeof( mp::Tag ) ) );
	tag->pool = &p;
	// skip sizeof tag to get the raw data-block.
	return( reinterpret_cast <void *> ( tag + 1U ) );
}

void *operator new( size_t bytes )// regular new
{
	mp::Tag* const tag = reinterpret_cast <mp::Tag *> ( std::malloc( bytes + sizeof( mp::Tag ) ) );
	tag->pool = nullptr;
	// skip sizeof tag to get the raw data-block.
	return( reinterpret_cast <void *> ( tag + 1U ) );
}
void *operator new[]( size_t bytes )// regular new
{
	mp::Tag* const tag = reinterpret_cast <mp::Tag *> ( std::malloc( bytes + sizeof( mp::Tag ) ) );
	tag->pool = nullptr;
	// skip sizeof tag to get the raw data-block.
	return( reinterpret_cast <void *> ( tag + 1U ) );
}
void operator delete( void * arg ) noexcept 
{	
	std::cout << "overloading enter\n";
	// we need to subtract 1U (in fact, pointer arithmetics) because arg
	// points to the raw data (second black of information).
	// The pool id (tag) is located 'sizeof(Tag)' bytes before.
	mp::Tag * const tag = reinterpret_cast <mp::Tag *> ( arg ) - 1U;

	if( nullptr != tag->pool ) // Memory block belongs toa particular  GM.
		tag->pool->Free( tag );
	else
		std::free( tag ); // Memory black belongs to a operation system.
}


void operator delete[]( void * arg ) noexcept 
{	
	std::cout << "overloading enter\n";
	// we need to subtract 1U (in fact, pointer arithmetics) because arg
	// points to the raw data (second black of information).
	// The pool id (tag) is located 'sizeof(Tag)' bytes before.
	mp::Tag * const tag = reinterpret_cast <mp::Tag *> ( arg ) - 1U;

	if( nullptr != tag->pool ) // Memory block belongs toa particular  GM.
		tag->pool->Free( tag );
	else
		std::free( tag ); // Memory black belongs to a operation system.
}



// static constexpr size_t BLK_SZ = sizeof( mp::SLPool<BLK_SIZE>::Block ); //!< The block size in bytes.
// static constexpr size_t TAG_SZ = sizeof( mp::Tag ); //!< The Tag size in bytes (each reserved area has a tag).
// static constexpr size_t HEADER_SZ = sizeof( mp::SLPool<BLK_SIZE>::Header ); //!< The header size in bytes

#endif






// void *operator new( size_t bytes, mp::StoragePool & p )
// {
// 	mp::Tag* const tag = reinterpret_cast <mp::Tag *> ( p.Allocate( bytes + sizeof( mp::Tag ) ) );
// 	tag->pool = &p;
// 	// skip sizeof tag to get the raw data-block.
// 	return( reinterpret_cast <void *> ( tag + 1U ) );
// }
// void *operator new[]( size_t bytes, mp::StoragePool & p )
// {
// 	mp::Tag* const tag = reinterpret_cast <mp::Tag *> ( p.Allocate( bytes + sizeof( mp::Tag ) ) );
// 	tag->pool = &p;
// 	// skip sizeof tag to get the raw data-block.
// 	return( reinterpret_cast <void *> ( tag + 1U ) );
// }



// void *operator new( size_t bytes )// regular new
// {
// 	mp::Tag* const tag = reinterpret_cast <mp::Tag *> ( std::malloc( bytes + sizeof( mp::Tag ) ) );
// 	tag->pool = nullptr;
// 	// skip sizeof tag to get the raw data-block.
// 	return( reinterpret_cast <void *> ( tag + 1U ) );
// }
// void *operator new[]( size_t bytes )// regular new
// {
// 	mp::Tag* const tag = reinterpret_cast <mp::Tag *> ( std::malloc( bytes + sizeof( mp::Tag ) ) );
// 	tag->pool = nullptr;
// 	// skip sizeof tag to get the raw data-block.
// 	return( reinterpret_cast <void *> ( tag + 1U ) );



// void operator delete( void * arg ) noexcept 
// {	
// 	std::cout << "overloading enter\n";
// 	// we need to subtract 1U (in fact, pointer arithmetics) because arg
// 	// points to the raw data (second black of information).
// 	// The pool id (tag) is located 'sizeof(Tag)' bytes before.
// 	mp::Tag * const tag = reinterpret_cast <mp::Tag *> ( arg ) - 1U;

// 	if( nullptr != tag->pool ) // Memory block belongs toa particular  GM.
// 		tag->pool->Free( tag );
// 	else
// 		std::free( tag ); // Memory black belongs to a operation system.
// }

// void operator delete[]( void * arg ) noexcept 
// {	
// 	std::cout << "overloading enter\n";
// 	// we need to subtract 1U (in fact, pointer arithmetics) because arg
// 	// points to the raw data (second black of information).
// 	// The pool id (tag) is located 'sizeof(Tag)' bytes before.
// 	mp::Tag * const tag = reinterpret_cast <mp::Tag *> ( arg ) - 1U;

// 	if( nullptr != tag->pool ) // Memory block belongs toa particular  GM.
// 		tag->pool->Free( tag );
// 	else
// 		std::free( tag ); // Memory black belongs to a operation system.
//}