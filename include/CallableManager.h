#pragma once

#include <sstream>
#include <string>
#include <map>
#include <typeinfo>
#include <iostream>

#include "Common.h"
#include "Callable.h"

class CallableManager
{
public:
	CallableManager(){}

	CallableManager(std::map<std::string, GCallable*> map):callableMap(map){}

	virtual ~CallableManager(){
		for(auto it(callableMap.begin()); it != callableMap.end(); ++it)
			delete it->second;
	}

	std::string names()
	{
		std::stringstream names;
		if(!callableMap.empty())
		{
			names << callableMap.cbegin()->first;
			for(auto it(std::next(callableMap.cbegin(), 1)); it != callableMap.cend(); ++it)
				names << SUPERSEPARATOR << it->first;
		}
		return names.str();
	}

	std::string signature(std::string name)
	{
		return callableMap.at(name)->signature();
	}
	std::string returnValue(std::string name)
	{
		return callableMap.at(name)->returnValue();
	}

	Variant call(std::string name, Variant* args)
	{
		return callableMap.at(name)->call(args);
	}

protected:
	std::map<std::string, GCallable*> callableMap;
};


class GObject : public CallableManager
{
public:
	using CallableManager::CallableManager;

	virtual ~GObject(){}

	virtual std::string objSignature(){return "";}
};


template<typename T, typename... Methods>
class Object : public GObject
{
public:
	Object(T *obj):GObject(argsToMap<GCallable, Methods...>()), obj(obj)
	{
		for(auto it(callableMap.begin()); it != callableMap.end(); ++it)
			it->second->setObj(reinterpret_cast<Variant>(obj));
	}

	virtual ~Object(){delete obj;}

	virtual std::string objSignature()
	{
		return typeid(T).name();
	}

private:
	T* obj;
};



class GObjectManager
{
public:
	virtual ~GObjectManager(){}
	virtual GObject* makeObject(Variant obj) = 0;
	virtual std::string signature() = 0;
};

template<typename T, typename... Methods>
class ObjectManager : public GObjectManager
{
public:
	virtual ~ObjectManager(){}
	virtual GObject* makeObject(Variant obj) override
	{
		return new Object<T, Methods...>(*reinterpret_cast<T**>(obj));
	}
	virtual std::string signature()
	{
		return typeid(T).name();
	}
};

template<typename... Constructor>
class ConstructorManager : public CallableManager
{
public:
	ConstructorManager(): CallableManager(argsToMap<GCallable, Constructor...>()){}
};

class ClassManager
{
public:
	ClassManager(): classMap(){}

	virtual ~ClassManager()
	{
		for(auto it(classMap.begin()); it != classMap.end(); ++it)
		{
			delete it->second.first;
			delete it->second.second;
		}
	}

	template<typename ConstructorM, typename ObjectM>
	void addClass(std::string name)//, ConstructorM coM, ObjectM meM)
	{
		CallableManager* cmPtr = new ConstructorM;
		GObjectManager* gomPtr = new ObjectM;
		classMap.insert(
			std::pair<
				std::string,
				std::pair<CallableManager*, GObjectManager*>>(
						name,
						std::pair<CallableManager*, GObjectManager*>(
							cmPtr,
							gomPtr
		)));
	}

	Variant makeObject(std::string className, std::string constructorName, Variant * args)
	{
		auto p = classMap.at(className);
		return reinterpret_cast<Variant>(p.second->makeObject(p.first->call(constructorName, args)));
	}

	std::string classNames()
	{
		std::stringstream names;
		if(!classMap.empty())
		{
			names << classMap.cbegin()->first;
			for(auto it(std::next(classMap.cbegin(), 1)); it != classMap.cend(); ++it)
				names << SUPERSEPARATOR << it->first;
		}
		return names.str();
	}

	std::string classNameById(std::string id)
	{
		std::cerr << "ID: :" << id << std::endl;
		for(auto it(classMap.begin()); it != classMap.end(); ++it)
		{
			if(it->second.second->signature() == id)
							return it->first;
		}

		return "NONE";
	}

	std::string constructorNames(std::string className)
	{
		return classMap.at(className).first->names();
	}

	std::string classSignature(std::string className, std::string constructorName)
	{
		return classMap.at(className).first->signature(constructorName);
	}
	std::string classReturnValue(std::string className, std::string constructorName)
	{
		return classMap.at(className).first->returnValue(constructorName);
	}
private:
	std::map<std::string, std::pair<CallableManager*, GObjectManager*>> classMap;
};



class FunctionManager : public CallableManager
{
public:
	FunctionManager(): CallableManager(){}

	template<typename Fn>
	void addFunction(std::string name)
	{
		GCallable *funPtr = new Fn;
		callableMap.insert(std::pair<std::string, GCallable*>(name, funPtr));
	}
};

