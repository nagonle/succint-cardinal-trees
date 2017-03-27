#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
using namespace std;

template <typename Size_Type, typename Count_Type>
extern uint64_t read_total_nodes(char *name_file, Size_Type **letts) {
	Count_Type nodes, symbols;
	Size_Type tmp;
	ifstream in;
	in.open(name_file, ios::in | ios::binary);
	// Read total nodes
	in.read((char*)&nodes, sizeof(Count_Type));
	symbols = nodes - 1;
	// Read zero separator
	in.read((char*)&tmp, sizeof(Size_Type));
	(*letts) = (Size_Type*)malloc(sizeof(Size_Type)*symbols);
	in.read((char*)*letts, sizeof(Size_Type)*symbols);
	in.close();

	return nodes;
};

int main(int argc, char *argv[])
{
	uint32_t * array32;
	uint8_t * array8;
	int count = atoi(argv[3]);
	int bits = atoi(argv[2]);
	cout << "name file:" << argv[1] << endl;
	if (bits == 32) {
		uint64_t total_nodes = read_total_nodes<uint32_t, uint64_t>(argv[1], &array32);
		cout << "Total nodes: " << total_nodes << endl;
		for (int i = 0; i < count; i++) {
			cout << array32[i] << " ";
		}
		cout << endl;
	} 
	if (bits == 8) {
		uint32_t total_nodes = read_total_nodes<uint8_t, uint32_t>(argv[1], &array8);
		cout << "Total nodes: " << total_nodes << endl;
		for (int i = 0; i < count; i++) {
			cout << array8[i] << " ";
		}
		cout << endl;
	} 
	
	return 0;
}
