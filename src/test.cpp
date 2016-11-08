#include <iostream>
#include <set>
//#include <sdsl/bit_vectors.hpp>
//
using namespace std;
//using namespace sdsl;

typedef unsigned char uchar;

size_t vocabulary_size(string& letts) {
	size_t count = 0;
	set<char> voc;
	for (size_t i=0; i<letts.size(); i++) {
		voc.insert(letts[i]);
	}
	count = voc.size();
	return count;
}

int main() {
	char a[5] = "Hola";
	a[0] = (char)1;
	a[2] = (char)1;

	string letts2 = string(a);
	string letts = "aaabbcccdddddaaaabbbcdabdcabdcabdcabcadadbcadcabcdbcabdcadc";
	cout << vocabulary_size(letts2) << endl;
	return 0;
}




