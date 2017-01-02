#include <iostream>
using namespace std;

string multiply_str(string a, int n) {
	string b = "";
	for (size_t i=0; i<n; i++) b += a;
	return b;
}

// max_arity: return the arity of a tree.
template <class type>
size_t get_max_arity(type * ct) {
	size_t x;
	size_t max_arity;
	max_arity = ct->degree(1);
	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		if (ct->degree(x) > max_arity) {
			max_arity = ct->degree(x);
		}
	}
	return max_arity;
}

template <class type>
double get_average_arity(type * ct) {
	size_t x;
	size_t degree_node = ct->degree(1);
	double sum_arity = degree_node; 
	size_t nodes = 1;
	for (size_t i=1; i<ct->count_nodes(); i++) {
		x = ct->tree_select0(i) + 1;
		degree_node = ct->degree(x);
		if (degree_node > 0) {
			sum_arity += degree_node;
			nodes++;
		}
	}
	return sum_arity / nodes;
}


// tree_height: return the max tree height.
template <class type>
size_t get_tree_height(type * ct, size_t node=1, size_t height=0) {
	vector <size_t> height_list;
	size_t node_degree = ct->degree(node);
	size_t new_node;
	if (node_degree == 0) return height;
	else {
		height++;
		for (size_t i=0; i<node_degree; i++) {
			new_node = ct->child(node, i+1);
			height_list.push_back(get_tree_height(ct, new_node, height));
		}
	}
	return *max_element(height_list.begin(), height_list.end());
}

// average_tree_height: return the average tree height.
template <class type>
double get_tree_average_height(type * ct, double* sum_heights, double* count_nodes, size_t node=1, size_t height=0) {
	size_t node_degree = ct->degree(node);
	size_t new_node;
	if (node_degree == 0) {
		*sum_heights += height;
		(*count_nodes)++;
		return height;
	}
	else {
		height++;
		for (size_t i=0; i<node_degree; i++) {
			new_node = ct->child(node, i+1);
			get_tree_average_height(ct, sum_heights, count_nodes, new_node, height); 
		}
	}
	return *sum_heights / *count_nodes;
}

// check_balanced: return true if bp contain a balanced sequence of parentheses.
bool check_balanced(bit_vector *bp) {
	stack<char> bp_stack;
	size_t N = bp->size(); 
	for (size_t i=0; i<N; i++) {
		if ((*bp)[i] == 1u) bp_stack.push('1');
		else
			if (bp_stack.empty() == false) bp_stack.pop();
			else return false;
	}

	if (bp_stack.empty() == true) return true;
	return false;
}

int main(int argc, const char *argv[])
{
	string str, str1, str2;
	str1 = "()";
	str = "(";
	str1 += multiply_str(str, 10) + ")";
	cout << str1 << endl;
	return 0;
}
