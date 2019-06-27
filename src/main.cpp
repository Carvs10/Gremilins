#include <iostream>
#include <algorithm>
#include <string>
#include "../include/mpool.h"
//#include "../include/mempool_commun.h"

int main(){

	std::string str = "ola amigo\n";

	std::cout << str;

	mp::SLPool < 16 > p( sizeof(int) * 200 );// creating the pool. 
	mp::SLPool < 16 > p2( sizeof(long int) * 500 );
		
	p.visualisation();


	int *qt = (int*)p.Allocate( sizeof(int) * 20  );

	qt[0] = 4000;
	qt[1] = 10000;
	qt[2] = 20000;
	qt[3] = qt[2] + qt[1];

	for(int i = 0; i < 4; i++){
		std::cout << qt[i] << std::endl;	
	}

	p.visualisation();//visualizar a alocação

	p.Free( qt );

	p.visualisation();//visualizar a liberação

	void * at1 = p.Allocate( 50 );
	void * at2 = p.Allocate( 200 );
	void * at3 = p.Allocate( 100 );

	p.visualisation();//visualizando as 3 alocação

	p.Free( at2 );

	p.visualisation();//

	long int * h = (long int*)p.Allocate( sizeof(long int) * 20  );;
	
	const char * j = (const char*)p.Allocate( sizeof(char) * 20  );
	j = "s"; 
	std::cout << j << std::endl;
	
	p.visualisation();

	void * xt = p.Allocate( 250 );

	p.visualisation();

	void * ppt = p.Allocate( 16 );

	p.visualisation();
	p.Free( xt );
	p.visualisation();
	//p.Free( at );

	struct stc{
			int a,b,c;
		};

	stc* t1 = new (p) stc;
	t1->a = 3;
	t1->b = 4;
	t1->c = 5;


	for(int i = 0; i < 4; i++){
		std::cout << qt[i] << std::endl;	
	}

	//void * zzt = p.Allocate( 20 );

	p.visualisation();


}