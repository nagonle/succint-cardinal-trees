#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>

#define MIN_DEGREE 280
#define MARGIN_DEGREE 100
#define MIN_HEIGHT 4
#define MARGIN_HEIGHT 1
#define MAX_HEIGHT 5
using namespace std;

bool belong(vector<uint32_t>* v, uint32_t s) {
	for (auto i=v->begin(); i<v->end(); i++) {
		if ((*i) == s) return true;
	}
	return false;
}


void generate_symbols(vector<uint32_t>* v, size_t N) {
	uint32_t symbol;
	srand(time(NULL));
	for (size_t i=0; i<N; i++) {
		symbol = (uint32_t) (rand() % (MIN_DEGREE + MARGIN_DEGREE));
		while (belong(v, symbol) == true) {
			symbol = (uint32_t) (rand() % (MIN_DEGREE + MARGIN_DEGREE));
		}
		v->push_back(symbol);
	}
	sort(v->begin(), v->end());
}

int main(int argc, const char *argv[]) {
	vector<uint32_t> symbols;
    generate_symbols(&symbols, 10);
	for (size_t i=0; i<symbols.size(); i++) cout << symbols[i] << " "; cout << endl;

	return 0;
}
