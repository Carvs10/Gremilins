#include <iostream>
#include <algorithm>
#include <string>
#include <chrono>
#include <ctime>
#include "../include/mpool.h"
//#include "../include/mempool_commun.h"

int main(){

	std::cout << "\n\n\n============================== Reprezentação do memory pool ===================================\n\n\n";

	mp::SLPool < 16 > p( sizeof(int) * 200 );// creating the pool. 
	mp::SLPool < 16 > p2( sizeof(long int) * 10000 );
		
	p.visualisation();


	int *qt = (int*)p.Allocate( sizeof(int) * 20  );

	qt[0] = 4000;
	qt[1] = 10000;
	qt[2] = 20000;
	qt[3] = qt[2] + qt[1];
	std::cout << "Valores armazenados:" << std::endl;
	for(int i = 0; i < 4; i++){
		std::cout << qt[i] << std::endl;	
	}
	std::cout << std::endl;	

	p.visualisation();//visualizar a alocação

	p.Free( qt );

	p.visualisation();//visualizar a liberação

	void * at1 = p.Allocate( 50 );
	void * at2 = p.Allocate( 200 );
	void * at3 = p.Allocate( 100 );

	p.visualisation();//visualizando as 3 alocação

	p.Free( at2 );

	p.visualisation();//

	long int * h = (long int*)p.Allocate( sizeof(long int) * 20  );
	
	const char * j = (const char*)p.Allocate( sizeof(char) * 20  );
	std::cout << "Letra armazenada:" << std::endl;
	j = "s"; 
	std::cout << j << std::endl;
	std::cout << std::endl;
	
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
	t1->a = 7032;
	t1->b = 4;
	t1->c = 30;
	p.visualisation();

	delete t1 ;

	p.visualisation();

	for(int i = 0; i < 4; i++){
		std::cout << qt[i] << std::endl;	
	}




//============================== SO x GM ===================================
	std::cout << "\n\n\n============================== SO x GM : Teste de tempo ===================================\n\n\n";
	std::cout << "\nTempo do GREMLINS:  ";


	struct stc2{
			long int a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,a1;
		};

	auto start = std::chrono::system_clock::now();
	auto end = std::chrono::system_clock::now();
	auto diff = (start - start);

	long int *  vet[ 30 ];
	int opc = 1;
	start = std::chrono::system_clock::now();//start time<<<<

	//char *  vet[ 30 ];
	if( opc == 1 ){

		for( int i= 0 ; i < 30 ; i++ )
	        {
	            vet[i] = new (p2) long int [30];
	            //strcpy( vet[i], oss.str().c_str() );
	        }
	    //for(long int i =0 ; i<500000000; i++){}

	    stc2* t2 = new (p2) stc2;

	    for( int i = 0; i < 10; i++ )
	    {
	    	stc2* t22 = new (p2) stc2;
	    }


	    delete t2;//deixando vaga

	    stc2* t3 = new (p2) stc2;//ocupando


	    stc2* t20 = new (p2) stc2;

	    for( int i = 0; i < 10; i++ )
	    {
	    	stc2* t220 = new (p2) stc2;
	    }


	    delete t20;//deixando vaga

	    stc2* t30 = new (p2) stc2;//ocupando


	    stc2* t23 = new (p2) stc2;

	    for( int i = 0; i < 10; i++ )
	    {
	    	stc2* t223 = new (p2) stc2;
	    }


	    delete t23;//deixando vaga

	    stc2* t33 = new (p2) stc2;//ocupando

	    for( int z = 0; z < 5; z++){

		    for( int i= 0 ; i < 30 ; i++ )
		        {
		            vet[i] = new (p2) long int [30];
		            //strcpy( vet[i], oss.str().c_str() );
		        }
	    }

    }
    //p2.visualisation();

    
    end = std::chrono::system_clock::now();//end time<<<<<
    diff =(end - start);
    double number = std::chrono::duration <double, std::milli> (diff).count();

    std::cout << number << "  mimlisegundos" << std::endl;


    std::cout << "\nTempo do SISTEMA OPERACIONAL:  ";


    opc = 2;

	start = std::chrono::system_clock::now();//start time<<<<

	//char *  vet[ 30 ];
if( opc == 2 ){

		for( int i= 0 ; i < 30 ; i++ )
	        {
	            vet[i] = new long int [30];
	            //strcpy( vet[i], oss.str().c_str() );
	        }
	    //for(long int i =0 ; i<500000000; i++){}

	    stc2* t2 = new stc2;

	    for( int i = 0; i < 10; i++ )
	    {
	    	stc2* t22 = new stc2;
	    }


	    delete t2;//deixando vaga

	    stc2* t3 = new stc2;//ocupando


	    stc2* t20 = new stc2;

	    for( int i = 0; i < 10; i++ )
	    {
	    	stc2* t220 = new stc2;
	    }


	    delete t20;//deixando vaga

	    stc2* t30 = new stc2;//ocupando


	    stc2* t23 = new stc2;

	    for( int i = 0; i < 10; i++ )
	    {
	    	stc2* t223 = new stc2;
	    }


	    delete t23;//deixando vaga

	    stc2* t33 = new stc2;//ocupando

	    for( int z = 0; z < 5; z++){

		    for( int i= 0 ; i < 30 ; i++ )
		        {
		            vet[i] = new long int [30];
		            //strcpy( vet[i], oss.str().c_str() );
		        }
	    }

    }

    
    end = std::chrono::system_clock::now();//end time<<<<<
    diff =(end - start);
    double number2 = std::chrono::duration <double, std::milli> (diff).count();

    std::cout << number2 << "  milisegundos\n\n" << std::endl;

}