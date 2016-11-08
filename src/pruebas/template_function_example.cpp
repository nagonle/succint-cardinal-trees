#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;


int main(int argc, const char *argv[])
{
	char array[100000000];
	for (size_t i=0; i<100000000; i++) array[i] = rand() % 27 + 65;

	for (size_t i=0; i<10; i++) cout << array[i] << " "; cout << endl;
	for (size_t i=99999990; i<100000000; i++) cout << array[i] << " "; cout << endl;
	return 0;
}
