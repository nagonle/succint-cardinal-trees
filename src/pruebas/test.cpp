#include <iostream>
#include <cstdlib>
#include <cmath>
#include <sdsl/wavelet_trees.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/int_vector.hpp>
using namespace std;
using namespace sdsl;

typedef unsigned char uchar;


int main(int argc, const char *argv[])
{
	uint8_t letts[20] = {2,3,1,255,0,3,2,1,4,2,3,1,2,4,5,3,2,1,3,2};
	int_vector<> my(20);
	for (int i=0; i<20; i++) my[i] = letts[i];

	for (int i=0; i<20; i++) cout << my[i] << " ";

	cout << "size char: " << sizeof(char) << endl;
	cout << "size uint8_t: " << sizeof(uint8_t) << endl;

	bit_vector bp(5);
	bp = {1,0,1,0,1};
	bp_support_sada<256, 32, rank_support_v5<0>, bit_vector::select_0_type> tree(&bp);  // <- pointer to b
	cout << "tree[3]: " << tree.acess(3);
	return 0;
}




