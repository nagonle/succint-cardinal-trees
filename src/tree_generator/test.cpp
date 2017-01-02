#include <iostream>
#include <fstream>
using namespace std;

void write_file(ofstream& out, size_t N) {
	out << "hola";
	if (N > 10) return;
	N++;
	write_file(out, N);
}

int main(int argc, const char *argv[]) {
	ofstream out;
	out.open("test.txt");
	write_file(out, 0);
	return 0;
}
