#include <iostream>

#include "Example.h"

using namespace std;

Example::Example(char c, int size, double* vect)
{
	cout << "START CLASS INIT " << endl;

	cout << c << endl;
	cout << "size " << size << endl;

	for(int i = 0; i < size; i++)
		cout << vect[i]++ << "; ";
	cout << endl;
	cout << "CLASS SUCCEFULLY INITIALIZED" << endl<< endl;
}

Example::~Example(){}

int Example::todo(int i1, int i2, int i3){
	cout << "COMPUTE" << endl;
	cout << "i1 : " << i1 << " i2 : " << i2 << " i3 : " << i3 << endl;
	return i1 + i2 + i3;
}

void test(string name)
{
	cout << "Hello " << name << endl;Z
}
