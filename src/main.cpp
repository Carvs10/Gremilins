#include <iostream>
#include <algorithm>
#include <string>
#include "../include/mpool.h"
#include "../include/mempool_commun.h"

int main(){

	using byte = char; // 1 byte

	int x = 300;

	mp::SLPool < 10 > p( sizeof(int) * 200 );//

	void * it = p.Allocate( 2 );

	p.Free( it );

}