#pragma once

#include <string>

class Example {
public:
	Example(char c, int size, double* vect);

	virtual ~Example();

	int todo(int i1, int i2, int i3);
};

void test(std::string name);
