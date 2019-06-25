#include <iostream>
#include <algorithm>
#include <string>
#include "../include/mpool.h"
//#include "../include/mempool_commun.h"

int main(){

	using byte = char; // 1 byte

	int x = 300;

	mp::SLPool < 32 > p( sizeof(int) * 2000 );

	int j = 2;
	void * it = p.Allocate( 20 );//
	int * qt = (int*)p.Allocate( sizeof(int) * 5 );
	std::cout << "Cheguei aqui" << std::endl;
	qt[0] = 4000;
	qt[1] = 10000;
	qt[2] = 20000;
	std::cout << "Cheguei aqui\n\n\nd" << std::endl;
	

	//p.visualisation();
	int * qt2 = (int*)p.Allocate( sizeof(int) * 5 );

	//void * at = p.Allocate( 2 );
	//void * wt = p.Allocate( 5 );
	//void * zt = p.Allocate( 2 );

	//p.visualisation();
	std::cout << "Cheguei aqui" << std::endl;

	//void * xt = p.Allocate( 400 );
	//void * i = new( p ) int ;

	//p.visualisation();

	std::cout << "Cheguei aqui" << std::endl;

	for(int i = 0; i < 4; i++){

		std::cout << qt[i] << std::endl;
		
	}

	p.Free( qt );

	//p.visualisation();

	//qt = (int*)p.Allocate( 10 );

	//p.visualisation();

	//p.visualisation();
	//p.Free( wt );

	//p.visualisation();
	//p.Free( at );
	//delete[] i;
//
}