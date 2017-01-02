#include <iostream>
#include <fstream>

/*
#define MIN_DEGREE 900
#define MARGIN_DEGREE 124
#define MIN_HEIGHT 30
#define MARGIN_HEIGHT 15
#define MAX_HEIGHT 50
*/
#define MIN_DEGREE 900
#define MARGIN_DEGREE 100
#define MIN_HEIGHT 3
#define MARGIN_HEIGHT 1
#define MAX_HEIGHT 5
using namespace std;

string multiply_str(string a, size_t n) {
	string b = "";
	for (size_t i=0; i<n; i++) b += a;
	return b;
}

void gen(string& bp, size_t current_height) {
	if (current_height  == MIN_HEIGHT) {
		current_height += rand() % MARGIN_HEIGHT;
	}
	current_height++;
	string bp_tmp = "";
	if (current_height > MAX_HEIGHT) {
		bp += ")";
		return;
	}
	else {
		size_t degree = rand() % MARGIN_DEGREE + MIN_DEGREE;
		bp_tmp = multiply_str("(", degree) + ")";
		bp += bp_tmp; 
		for (size_t i=0; i<degree; i++) {
			gen(bp, current_height);
		}
	}
}

bool check_bp(string bp) {
	size_t open = 0;
	size_t close = 0;
	for (size_t i=0; i<bp.size(); i++) {
		if (bp[i] == '(') open++;
		else close++;
	}
	cout << "nodes: " << open << endl;
	return open == close;
}

void write_bp(string name_file, string bp) {
	ofstream out_file;
	out_file.open(name_file);
	out_file << bp;
	out_file.close();
}

void random_tree_to_file(string& bp, size_t current_height, ofstream& out_file) {

	if (current_height  == MIN_HEIGHT) {
		current_height += rand() % MARGIN_HEIGHT + 1;
	}
	current_height++;
	string bp_tmp = "";
	if (current_height > MAX_HEIGHT) {
		out_file << ")";
		return;
	}
	else {
		size_t degree = rand() % MARGIN_DEGREE + MIN_DEGREE;
		bp_tmp = multiply_str("(", degree) + ")";
		out_file << bp_tmp;
		for (size_t i=0; i<degree; i++) {
			random_tree_to_file(bp, current_height, out_file);
		}
	}
	//out_file.close();
}

int main(int argc, const char *argv[])
{
	//ofstream out_file;
	//out_file.open("giant_random_tree.ascii");

	string bp = "(";
	gen(bp, 1);
	if (check_bp(bp) == true) 	write_bp("giant_random_tree.ascii", bp);
	//random_tree_to_file(bp, 1, out_file);
	return 0;
}
