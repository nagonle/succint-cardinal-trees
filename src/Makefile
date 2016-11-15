all:
	g++ -std=c++11 -O3 -DNDEBUG -g -I ~/include -L ~/lib ct_main.cpp read_utils.cpp -o ct_main -lsdsl -ldivsufsort -ldivsufsort64 
virtual: 
	g++ -std=c++11 -O3 -DNDEBUG -g -I ~/.sdsl-lite/include -L ~/.sdsl-lite/lib ct_main.cpp read_utils.cpp -o ct_main -lsdsl -ldivsufsort -ldivsufsort64 
test:
	g++ -std=c++11 -O3 -DNDEBUG -g -I ~/.sdsl-lite/include -L ~/.sdsl-lite/lib test.cpp -o test -lsdsl -ldivsufsort -ldivsufsort64 
