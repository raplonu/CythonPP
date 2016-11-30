#pragma once

#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "Common.h"
#include "Callable.h"

template<typename T>
class Store
{
protected:
	std::vector<std::shared_ptr<T>> callables;
public:
	Store():callables(){}

	std::vector<std::string> signatureName(std::size_t pos)
	{
		return callables[pos]->signatureName();
	}

	std::string returnName(std::size_t pos)
	{
		return callables[pos]->returnName();
	}

protected:
	void add(T* element)
	{
		callables.emplace_back(element);
	}
};

template<typename T>
class NamedStore
{
protected:
	std::vector<std::shared_ptr<T>> callables;
	std::vector<std::string> names_;
public:
	NamedStore():callables(), names_(){}

	NamedStore(NamedStore<T>& namedStore):
		callables(namedStore.callables),
		names_(namedStore.names_)
	{}

	std::vector<std::string> names()
	{
		return names_;
	}

	std::vector<std::string> signatureName(std::size_t pos)
	{
		return callables[pos]->signatureName();
	}

	std::string returnName(std::size_t pos)
	{
		return callables[pos]->returnName();
	}

	void add(T* element, char const * name)
	{
		callables.emplace_back(element);
		names_.emplace_back(name);
	}
};

class GObject : public NamedStore<GMethod>
{
public:
	GObject(NamedStore<GMethod> &storeMethod):
		NamedStore<GMethod>(storeMethod)
	{}

	virtual Variant call(std::size_t pos, Variant* args) = 0;
};

template<typename T>
class Object : public GObject
{
	std::unique_ptr<T> obj;
public:
	Object(
		NamedStore<GMethod> &storeMethod, Variant obj
	):
		GObject(storeMethod),
		obj(reinterpret_cast<T*>(obj))
	{}

	virtual Variant call(std::size_t pos, Variant* args) override
	{
		return callables[pos]->call(reinterpret_cast<Variant>(obj.get()), args);
	}
};

class ConstructorManager : public Store<GConstructor>
{
public:
	ConstructorManager():Store<GConstructor>(){}

	Variant create(std::size_t pos, Variant* args)
	{
		return callables[pos]->call(args);
	}
};

class GClassStore : public ConstructorManager
{
protected:
	NamedStore<GMethod> storeMethod;
public:
	GClassStore():
		ConstructorManager(),
		storeMethod()
	{}

	void addConstructor(GConstructor* constuctor)
	{
		ConstructorManager::add(constuctor);
	}

	void addMethod(GMethod* met, char const * metName)
	{
		storeMethod.add(met, metName);
	}

	virtual GObject* call(std::size_t pos, Variant* args) = 0;

};

template<typename T>
class ClassStore : public GClassStore
{
public:
	ClassStore():GClassStore(){}

	virtual GObject* call(std::size_t pos, Variant* args) override
	{
		return new Object<T>(storeMethod, GClassStore::create(pos, args));
	}
};

class ClassManager
{
protected:
	std::vector<std::shared_ptr<GClassStore>> classesStore;
	std::vector<std::string> names_;
public:
	ClassManager():
		classesStore(),
		names_()
	{}

	void addClass(GClassStore *classStore, char const * name)
	{
		classesStore.emplace_back(classStore);
		names_.emplace_back(name);
	}

	GObject* create(std::size_t classPos, std::size_t constructorPos, Variant* args)
	{
		return classesStore[classPos]->call(constructorPos, args);
	}

	std::vector<std::string> classNames()
	{
		return names_;
	}

	std::vector<std::string> signatureName(std::size_t classPos, std::size_t constructorPos)
	{
		return classesStore[classPos]->signatureName(constructorPos);
	}

	std::string returnType(std::size_t pos)
	{
		return names_[pos];
	}
};

class FunctionManager : public NamedStore<GFunction>
{
public:
	FunctionManager():NamedStore<GFunction>(){}

	void addFunction(GFunction* fun, char const * funName)
	{
		NamedStore<GFunction>::add(fun, funName);
	}

	Variant call(std::size_t pos, Variant* args)
	{
		return callables[pos]->call(args);
	}

	std::vector<std::string> functionNames()
	{
		return NamedStore<GFunction>::names();
	}
};
