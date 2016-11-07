#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
using namespace std;

typedef unsigned char uchar;

void read_bp(char name_file[], char** bp, size_t nodes) {
	ifstream in(name_file, ios::binary);
	size_t size_block = nodes * 2;
	(*bp) = (char*)malloc(size_block);
	in.read((char*)*bp, size_block);
	in.close();
}

int main(int argc, const char *argv[])
{
	char name_file[] = "/data/LZ78/DNA/dnaCustom.letts";
	uint total_nodes = 30136373;

	string test_array;
	char *bp;
	read_bp(name_file, &bp, total_nodes);

	cout << "[DEBUG] sizeof(bp): " << sizeof(*bp) << endl;
	test_array = string(bp);
	for (size_t i=0; i<10; i++) cout << test_array[i] << " "; cout << endl;
	for (size_t i=30136362; i<30136372; i++) cout << test_array[i] << " "; cout << endl;
	cout << bp[30136375] << endl;
	free(bp);
	return 0;
}

int test_string(int argc, const char *argv[])
{
	string array = "hola mundo";
	cout << "array capacity: " << array.capacity() << endl;
	cout << "array size: " << array.size() << endl;

	array[1] = '\0';

	cout << "array: " << array << endl;
	cout << "array capacity: " << array.capacity() << endl;
	cout << "array size: " << array.size() << endl;
	return 0;
}
