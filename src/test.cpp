#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/resource.h>
#include <stack>
//#include <sdsl/bit_vectors.hpp>
#include <vector>
#include <fstream>
using namespace std;
//using namespace sdsl;

typedef unsigned char uchar;


int func() {
	cout << "int: " << sizeof(int) << endl;
	cout << "unsigned int: " << sizeof(unsigned int) << endl;
	cout << "unsigned long int: " << sizeof(unsigned long int) << endl;
	cout << "size_t: " << sizeof(size_t) << endl;
	cout << "uint: " << sizeof(uint) << endl;
}

int write_files_test() {
	// Write letts.
	char letts[11] = "bcrbccrbcr";
	char zero = 0;
	uint total_nodes = 11;
	ofstream myfile("mybp.letts");
	myfile.write((char*)&total_nodes, sizeof(uint));
	myfile.write((char*)&zero, sizeof(char));
	myfile.write(letts, sizeof(char)*10);
	myfile.close();

	// Write bp sequence.
	myfile.open("mybp.ascii");
	char bp[23] = "(((()(())))(()())(()))";
	myfile.write(bp, 22);
	myfile.close();

	cout << "Done!" << endl;
	return 0;
}

void replace_null(char *seq, size_t N) {
	for (size_t i=0; i<N; i++) {
		if (seq[i] == 0) {
			seq[i] = '0';
		}
	}
}

bool exists(string seq, char c) {
	return !(seq.find(c) == string::npos);
}

double get_time (void) {
	double usertime, systime;
    struct rusage usage;

    getrusage (RUSAGE_SELF, &usage);
    usertime = (double) usage.ru_utime.tv_sec + (double) usage.ru_utime.tv_usec ;
    systime = (double) usage.ru_stime.tv_sec + (double) usage.ru_stime.tv_usec ;

    return (usertime + systime);
}


void string_func(string a) {
	a[5] = 'F';
	cout << a << endl;
}

bool check_balanced(char *bp) {
	stack<char> bp_stack;
	size_t N = strlen(bp);
	for (size_t i=0; i<N; i++) {
		if (bp[i] == '(') bp_stack.push('1');
		else
			if (bp_stack.empty() == false) bp_stack.pop();
			else return false;
	}

	if (bp_stack.empty() == true) return true;
	return false;
}

int main(int argc, const char *argv[]) {
	double a, b;
	int c = 20;
	a = 0.24; b = 20.0;
	cout << a / c << endl;
	return 0;
}










