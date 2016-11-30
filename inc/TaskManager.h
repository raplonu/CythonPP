#pragma once

#include <string>
#include <map>

#include "CallableManager.h"

#define CLASSBEGIN(T)													\
{																		\
using Type_  = T;														\
char name[] = #T;														\
GClassStore* gcl = new ClassStore<Type_>();

#define CLASSEND														\
addClass(gcl, name);													\
}

#define DEFAULTCONSTRUCTOR												\
gcl->addConstructor(new Constructor<Type_>());

#define CONSTRUCTOR(args...)											\
gcl->addConstructor(new Constructor<Type_ , ##args>());

#define METHOD(fn)														\
gcl->addMethod(new Method<decltype(&Type_::fn), &Type_::fn>(), #fn);

#define FUNTION(fn)														\
addFunction(new Function<decltype(&fn), &fn>(), #fn);

class TaskManager : public ClassManager, public FunctionManager
{
public:
	TaskManager():ClassManager(), FunctionManager(){}

	std::vector<std::string> classSignatureName(std::size_t classPos, std::size_t constructorPos)
	{
		return ClassManager::signatureName(classPos, constructorPos);
	}

	std::vector<std::string> functionSignatureName(std::size_t pos)
	{
		return FunctionManager::signatureName(pos);
	}

	std::string functionReturnName(std::size_t pos)
	{
		return FunctionManager::returnName(pos);
	}
};
