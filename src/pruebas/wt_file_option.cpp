#include <iostream>
#include <string>
typedef unsigned char uchar;

using namespace std;

int main(int argc, const char *argv[])
{
	string name = "hola mundo";
	cout << "size: " << name.size() << endl;
	//cout < "capacity: " << name.capacity() << endl;
	cout << "name: " << name << endl;

	name[2] = '\0';
	cout << "size: " << name.size() << endl;
	//cout < "capacity: " << name.capacity() << endl;
	cout << "name: " << name << endl;

	return 0;
}
