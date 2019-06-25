#include <iostream>
#include <algorithm>
#include <string>
#include "../include/mpool.h"
//#include "../include/mempool_commun.h"

int main(){

	using byte = char; // 1 byte

	int x = 300;

	mp::SLPool < 10 > p( sizeof(int) * 200 );

	int j = 2;
	void * it = p.Allocate( 20 );//
	void * qt = p.Allocate( 20 );
	
	p.visualisation();

	void * at = p.Allocate( 40 );
	void * wt = p.Allocate( 5 );
	void * zt = p.Allocate( 2 );

	p.visualisation();

	void * xt = p.Allocate( 4 );
	void * i = new( p ) int ;

	p.visualisation();

	p.Free( qt );

	p.visualisation();
	p.Free( wt );

	p.visualisation();
	p.Free( at );
	//delete[] i;
//
}