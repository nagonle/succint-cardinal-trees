#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>
#include <sdsl/wavelet_trees.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/int_vector.hpp>
using namespace std;
using namespace sdsl;

typedef unsigned char uchar;


int main(int argc, const char *argv[])
{
	vector<size_t> height {2,2,1,3,3,3};
	cout << *max_element(height.begin(), height.end());
	return 0;
}




