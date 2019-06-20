#include <iostream>
#include <cstdio> //std::size_t

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
