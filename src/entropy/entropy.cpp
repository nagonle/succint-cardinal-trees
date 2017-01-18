#include <iostream>
#include <cstring>
#include <fstream>
#include <cmath>
#include <map>
using namespace std;


template <typename sym_type, typename size_type>
uint64_t read_data(string name_file, sym_type** data) {
	sym_type aux;
	size_type total_nodes, total_symbols;
	ifstream letts_f;

	name_file = name_file + ".letts";
	letts_f.open(name_file);
	letts_f.read((char*)&total_nodes, sizeof(size_type));
	total_symbols = total_nodes - 1;
	letts_f.read((char*)&aux, sizeof(sym_type));

	(*data)  = (sym_type*)malloc(sizeof(sym_type)*total_symbols);
	letts_f.read((char*)*data, sizeof(sym_type)*total_symbols);
	letts_f.close();
	return (uint64_t) total_symbols;
}

template <typename sym_type>
void create_summary(sym_type* data, uint32_t N, map<sym_type, uint32_t>* summary) {
	for (size_t i=0; i<N; i++) {
		if (summary->find(data[i]) == summary->end()) (*summary)[data[i]] = 1;
		else (*summary)[data[i]] = (*summary)[data[i]] + 1;
	}
}

template <typename sym_type>
double calculate_entropy(map<sym_type, uint32_t>* summary, uint64_t N) {
	double entropy = 0.0;
	for (auto it=summary->begin(); it!=summary->end(); it++) {
		//\sigma p(x) log2(x)(1/p(x))
		double prob = (double) it->second / N;
		entropy += prob * log2(1/prob);
	}
	return entropy;
}

int main(int argc, const char *argv[]) {
	
	uint64_t total;
	if (strcmp(argv[1], "u8") == 0) {
		uint8_t* data_u8 = NULL;
		total = read_data<uint8_t, uint32_t> (argv[2], &data_u8);

		map<uint8_t, uint32_t> summary;
		create_summary<uint8_t>(data_u8, total, &summary);

		cout << "Entropy|" << calculate_entropy<uint8_t>(&summary, total) << endl;

	} else if (strcmp(argv[1], "u32") == 0) {
		uint32_t* data_u32 = NULL;
		total = read_data<uint32_t, uint64_t> (argv[2], &data_u32);

		map<uint32_t, uint32_t> summary;
		create_summary<uint32_t>(data_u32, total, &summary);

		cout << "Entropy|" << calculate_entropy<uint32_t>(&summary, total) << endl;
	} else {
		cout << "USAGE:\n\t" << "entropy {u8, u32} {path/to/data}" << endl;
	}

	return 0;
}
