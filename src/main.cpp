#include <iostream>
#include <algorithm>
#include <string>
#include "../include/mpool.h"
//#include "../include/mempool_commun.h"

int main(){

	using byte = char; // 1 byte

	int x = 300;

	mp::SLPool < 16 > p( sizeof(int) * 200 );// 8 bytes ate 10 bytes

	int j = 2;
	void * it = p.Allocate( 20 );//
	//void * qt = p.Allocate( 20 );
	
	p.visualisation();

	// int * qt = (int*)p.Allocate( sizeof(int) * 50 );
	// //std::cout << "Cheguei aqui" << std::endl;
	// qt[0] = 4000;//
	// qt[1] = 10000;
	// qt[2] = 20000;

	// for(int i = 0; i < 3; i++){

	// 	std::cout << qt[i] << std::endl;
		
	// }
	//p.visualisation();

	//p.Free( qt );

	p.visualisation();

	int *qt = (int*)p.Allocate( sizeof(int) * 50  );

	qt[0] = 4000;//s
	qt[1] = 10000;
	qt[2] = 20000;

	for(int i = 0; i < 3; i++){

		std::cout << qt[i] << std::endl;
		
	}

	p.visualisation();

	//p.Free( qt );

	//p.visualisation();

	//p.visualisation();

	void * at = p.Allocate( 200 );

	p.visualisation();

	p.Free( qt );

	p.visualisation();

	void * wt = p.Allocate( 5 );
	void * zt = p.Allocate( 2 );

	p.visualisation();

	void * xt = p.Allocate( 250 );//			TEM QUE ALOCAR ALGO APOS PARA PODER DAR FREE!!
	void * i = new( p ) int ;

	p.visualisation();

	void * ppt = p.Allocate( 2 );


	p.Free( xt );

	// p.visualisation();
	//p.Free( wt );

	p.visualisation();
	//p.Free( at );
	for(int i = 0; i < 3; i++){

		std::cout << qt[i] << std::endl;
		
	}
	// qt = (int*)p.Allocate( sizeof(int) * 50 );
	// //std::cout << "Cheguei aqui" << std::endl;
	// qt[0] = 4000;//
	// qt[1] = 10000;
	// qt[2] = 20000;
	void * zzt = p.Allocate( 20 );
	// for(int i = 0; i < 3; i++){

	// 	std::cout << qt[i] << std::endl;
		
	// }

	// p.visualisation();

	// p.Free( qt );

	p.visualisation();
	//delete[] i;
	//p.Free( zzt );

}