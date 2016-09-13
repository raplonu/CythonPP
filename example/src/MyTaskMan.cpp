#include "MyTaskMan.h"
#include "Example.h"

using namespace std;

MyTaskMan::MyTaskMan()
:TaskManager()
{
	//Create fake compute interface : only needed for test
	addClass<
		ConstructorManager<
			Constructor<Example, char, int, double*>
		>,
		ObjectManager<Example,
			Method<Str<'TODO'>, 	MET(Example::todo)>
		>
	>("Example");

	addFunction<
		Function<FUN(test)>
	>("test");

}

MyTaskMan::~MyTaskMan(){}
