#ifndef MPOOL_H
#define MPOOL_H


#include <iostream>
#include <cstdio> //std::size_t
#include <cmath>  //std::ceil
#include <new>    //std::bad_alloc
#include <iterator>
#include <array>


//CONSTRUTOR	x
//Allocate	x
//Free x
//APRESESNTAR CODIGO DEMONNSTRANDO MELHOR DESENPENHO EM RELACAO AO SO
//MEMORY VISUALISATION x
//BEST FIT


namespace mp{
	class StoragePool{


		public:

			virtual ~StoragePool(){};
			

			virtual void* Allocate( size_t ) = 0;
			

			virtual void Free( void * ) = 0;




	};

	struct Tag
			{ 
				StoragePool * pool;
			};





	template < size_t BLK_SIZE = 16 >

	class SLPool : public StoragePool{

		public:

		
		static constexpr size_t BLK_SZ = sizeof( mp::SLPool<BLK_SIZE>::Block ); //!< The block size in bytes.
		static constexpr size_t TAG_SZ = sizeof( mp::Tag );  //!< The Tag size in bytes (each reserved area has a tag).
		static constexpr size_t HEADER_SZ = sizeof( mp::SLPool<BLK_SIZE>::Header ); //!< The header size in bytes


		

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
			//int 

		public:

			explicit SLPool( size_t mem ) : 
				m_n_blocks(  std::ceil(  static_cast<float>(mem)/BLK_SIZE  ) + 1),	//!< Store the number of blocks + sentinel block.
				m_pool( new Block[m_n_blocks] ),									//!< The block area.
				m_sentinel( m_pool[m_n_blocks - 1] )								//!< Last element of the space.
			{
				this->m_pool[0].m_length = (m_n_blocks -1);		//!< Size of the free space.
				this->m_pool[0].m_next = nullptr;				//!< Pointer to next node.
				
				this->m_sentinel.m_next = this->m_pool;	//!< pointer to first node.
				this->m_sentinel.m_length = 0;			//!< Sentinel size.

				
				//std::cout << m_pool[0].m_length << "\n" << m_sentinel.m_next << "\ncontrutor \n";
			}

			~SLPool()
			{
				delete[] m_pool;

				//std::cout << "destruiu \n";
			}





			void * Allocate( size_t	mem )				//!< Allocate a  raw using First-fit.
			{
				//std::cout << "entrou1 \n";

				Block *fpt = this-> m_sentinel.m_next;	//!< Pointer to the first node.
				Block *spt = &m_sentinel;				//!< Pointer to the sentinel
				unsigned mem_blocks = std::ceil(  static_cast<float>(mem)/BLK_SIZE );	//!< the number of blocks that will be used.

				while( fpt != nullptr ){

					if( mem_blocks <= fpt->m_length ){

						//std::cout << "tem espaço \n";

						if ( mem_blocks == fpt->m_length ){	//!< If dont need to create a new node of free blocks.

							//std::cout << "espaco = \n";

							spt->m_next = fpt->m_next;		//!< Updating the pointer to other head. 

							fpt->m_length = mem_blocks;		//!< Size of the raw space.
							
							return reinterpret_cast<void*> (reinterpret_cast<Header *> (fpt) + (1U));
						}

						//std::cout << "espaço maior \n";

						spt->m_next = fpt + mem_blocks; 					//!< Updating the old pointer.

						spt->m_next->m_next = fpt->m_next;					//!< Updating the new node pointer.

						spt->m_next->m_length = fpt->m_length - mem_blocks; //!< Updating the space of the new node.

						fpt->m_length = mem_blocks;							//!< Size of the raw space.

						return reinterpret_cast<void*> (reinterpret_cast<Header *> (fpt) + (1U));

					}

					spt = fpt;			//!< Updating the pointer to next posicion.
					fpt = fpt->m_next;	//!< Updating the pointer to next posicion.

				}
				
				throw std::bad_alloc( );
			}



			void Free ( void * pt_m )	//!< free a raw area.
			{	
				//std::cout << "entrou2 \n";

				pt_m = reinterpret_cast<Block *> (reinterpret_cast <Header *> (pt_m) - (1U));

				Block * ptReserved = (Block *) pt_m; 				//!< Pointer to raw area that will be released.
				Block * ptPostReserved = this-> m_sentinel.m_next;	//!< Pointer to position node after the raw area.
				Block *	ptPrevReserved = &m_sentinel;				//!< Pinter to node before the raw.

				//std::cout << "passou \n";

				while( (ptPostReserved != nullptr) ){		//!< Updating the pointers to left and right position.

					//std::cout << ptPostReserved << "   " << ptReserved << "  looping \n";

					if(ptPostReserved >= ptReserved){

						//std::cout << "passou2 \n";
						break;
					}

					ptPrevReserved = ptPostReserved;
					ptPostReserved = ptPostReserved->m_next;
				}

				//std::cout << "passou3 \n";

				if( ((ptPrevReserved + ptPrevReserved->m_length) == ptReserved) and ((ptReserved->m_length + ptReserved) == ptPostReserved) ){	//!< Checking if the raw area have free adjacent areas.

					//std::cout << "igual igual \n";
					ptPrevReserved->m_next = ptPostReserved->m_next;																//!< Making a uniq pointer to the next block.
					ptPrevReserved->m_length = (ptPrevReserved->m_length + ptReserved->m_length + ptPostReserved->m_length);		//!< Unite the three length spaces.
				}

				else if( (ptReserved->m_length + ptReserved) == ptPostReserved ){	//!< Checking if the right area is a free adjacent area.
					//std::cout << "igual direita \n";
					ptReserved->m_next = ptPostReserved->m_next;					//!< Making a uniq pointer to the next block.
					ptReserved->m_length = ptReserved->m_length + ptPostReserved->m_length;	//!< Unite the two length spaces.
				} 

				else if( (ptPrevReserved + ptPrevReserved->m_length) == ptReserved ){			//!< Checking if the left area is a free adjacent area.
					//std::cout << "igual esquerda \n";
					ptPrevReserved->m_length = ptPrevReserved->m_length + ptReserved->m_length;	//!< Unite the two length spaces.
				}

				else{												//!< Checking if the raw area dont have free adjacent areas.
					//std::cout << "nada \n";
					ptPrevReserved->m_next = ptReserved;			//!< Making a pointer to the next node.
					ptReserved->m_next = ptPostReserved;			
				}

				//std::cout << "amigo estou aqui!!!\n";
			}

			void visualisation()
			{

				Block * ptPostReserved = this->m_sentinel.m_next;	//!< Pointer to position node after the raw area.
				Block *	ptPrevReserved = &m_sentinel;	

				int ocup 	= 0;
				int paint = 0;
				//std::array<Block>::iterator it;

				//it = m_pool.begin();
				//auto it = this->m_pool[0];

				if( &this->m_pool[0] != ptPostReserved ){

					ocup = ptPostReserved - &this->m_pool[0];

					std::cout << " ";
					for(int i = 0; i < ocup; i++){
						std::cout << "X";
					}
					std::cout << " ";
				}

				for(int i = 0; i < ptPostReserved->m_length; i++){
					std::cout << "O";
				}
				std::cout << " ";

				ptPrevReserved = ptPostReserved;
				ptPostReserved = ptPostReserved->m_next;

				while (ptPostReserved != nullptr){

					ocup = ptPrevReserved->m_length + ocup + paint;

					//it = m_pool.begin(+ ocup);

					paint = ptPostReserved - (&this->m_pool[0] + ocup);

					for( int i = 0; i < paint; i++ ){
						std::cout << "X";
					}
					std::cout << " ";

					for( int i = 0; i < ptPostReserved->m_length; i++ ){
						std::cout << "O";
					}
					std::cout << " ";

					ptPrevReserved = ptPostReserved;
					ptPostReserved = ptPostReserved->m_next;

					if( ptPostReserved == nullptr ){
							ocup = ptPrevReserved->m_length + ocup + paint;
						if( (m_n_blocks - 1) != ocup ){

							for (int i = 0; i < ( (m_n_blocks - 1) - ocup ); ++i)
							{
								std::cout << "X";
							}
							std::cout << " ";

						}
					}


				}
			std::cout << "\n\n";
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
	//std::cout << "overloading enter\n";
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
	//std::cout << "overloading enter\n";
	// we need to subtract 1U (in fact, pointer arithmetics) because arg
	// points to the raw data (second black of information).
	// The pool id (tag) is located 'sizeof(Tag)' bytes before.
	mp::Tag * const tag = reinterpret_cast <mp::Tag *> ( arg ) - 1U;

	if( nullptr != tag->pool ) // Memory block belongs toa particular  GM.
		tag->pool->Free( tag );
	else
		std::free( tag ); // Memory black belongs to a operation system.
}

#endif

