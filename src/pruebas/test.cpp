#include <iostream>
#include <sdsl/wavelet_trees.hpp>
using namespace std;

typedef unsigned char uchar;


int test_support_char_gmr() {
	string letts = "aaabbaab";
	wt_gmr<> wt;
	construct_im(wt, letts, 1);
	return 0;
}

int main(int argc, const char *argv[])
{
	uchar uc[5] = "hola";
	cout << uc << endl;
	uc[3] = (char)1;
	cout << uc << endl;
	return 0;
}




