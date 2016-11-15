#include <iostream>
#include <cstdlib>
#include <cmath>
#include <sdsl/wavelet_trees.hpp>
#include <sdsl/bp_support.hpp>
#include <sdsl/bit_vectors.hpp>
using namespace std;
using namespace sdsl;

typedef unsigned char uchar;

int binary_search(string seq, int left, int right, char alpha) {
	int mid;
	size_t comp = 0;
	while (left <= right) { 
		comp++;
		mid = (left+right)/2; 
		if (seq[mid] < alpha) left = mid+1;
		else if (seq[mid] > alpha) right = mid-1;
		else { 
			cout << "comp: " << comp << endl;
			return mid;
		}
	}
	cout << "comp: " << comp << endl;
	return -1;
}

int main(int argc, const char *argv[])
{
	string a = "abcde123456789fghijklmn0opqrstuvwxyz0opqrstuvwxyztuvwxyz0opq";
	int N = (int)a.size();
	for (size_t i=0; i<N; i++) binary_search(a, 0, N-1, a[i]);
	cout << "N: " << N << endl;
	cout << "log2(N) = " << log2(N) << endl;
	return 0;
}




