#include <iostream>
#include <algorithm>
#include <string>
#include "../include/mpool.h"
#include "../include/mempool_commun.h"

int main(){

	int x = 300;

	mp::SLPool < 10 > p( sizeof(int) * 200 );//

	p.Allocate( 2 );

	//p.Free(ponteridaasd);

}