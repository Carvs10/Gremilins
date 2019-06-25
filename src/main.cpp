#include <iostream>
#include <algorithm>
#include <string>
#include "../include/mpool.h"
//#include "../include/mempool_commun.h"

int main(){

	using byte = char; // 1 byte

	int x = 300;

	mp::SLPool < 10 > p( sizeof(int) * 200 );//

	int j = 2;
	void * it = p.Allocate( 2 );
	void * qt = p.Allocate( 4 );
	void * at = p.Allocate( 2 );
	void * wt = p.Allocate( 4 );
	void * zt = p.Allocate( 2 );
	void * xt = p.Allocate( 4 );
	void * i = new( p ) int ;

	p.visualisation();//

	p.Free( qt );
	p.Free( wt );
	p.Free( at );
	delete[] i;
//
}