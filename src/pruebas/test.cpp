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

template <typename T, typename U>
void sumar(T t, U u) {
	cout <<  u + t;
};


template <typename T, typename U>
void saludar(T t, U u){
	sumar<T, U>(t, u);
};



template <class type1, typename type2>
class myclass {
	public:
		type1 edad;
		type2 peso;
		myclass(){
			cout << edad << " " << peso << endl;
		}
};

int main(int argc, const char *argv[])
{
	saludar(5.2, 2);
	sumar
	return 0;
}




