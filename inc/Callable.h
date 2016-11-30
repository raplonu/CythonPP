#pragma once

#include <vector>
#include <string>

#include "Common.h"
#include "TemplateCall.h"

class GCallable
{
	std::vector<std::string> sigName;
	std::string retName;

public:
	GCallable(
		std::vector<std::string> && signatureName, std::string && returnName
	):
		sigName(signatureName),
		retName(returnName)
	{}

	std::vector<std::string> signatureName() const
	{
		return sigName;
	}

	std::string returnName() const
	{
		return retName;
	}
};

class GFunction : public GCallable
{
public:
	GFunction(
		std::vector<std::string> && signatureName, std::string && returnName
	):
		GCallable(std::move(signatureName), std::move(returnName))
	{}

	virtual Variant call(Variant *args) = 0;
};

template<typename Fn, Fn> class Function;

template<
	typename Ret, typename... Args,
	Ret (*Fn)(Args...)>
class Function<Ret(*)(Args...), Fn> : public GFunction
{
	Return<Ret> returnValue;
public:
	Function():
		GFunction(argsToNames<Args...>(), nameOf<Ret>()), returnValue()
	{}

	virtual Variant call(Variant *args) override
	{
		Call<Ret(*)(Args...), Fn>::call(returnValue, args);
		return returnValue;
	}
};

class GMethod : public GCallable
{
public:
	GMethod(
		std::vector<std::string> && signatureName, std::string && returnName
	):
		GCallable(std::move(signatureName), std::move(returnName))
	{}

	virtual Variant call(Variant obj, Variant *args) = 0;
};

template<typename Fn, Fn> class Method;

template<
	typename T, typename Ret, typename... Args,
	Ret (T::*Fn)(Args...)>
class Method<Ret(T::*)(Args...), Fn> : public GMethod
{
	Return<Ret> returnValue;
public:
	Method():
		GMethod(argsToNames<Args...>(), nameOf<Ret>()), returnValue()
	{}

	virtual Variant call(Variant obj, Variant *args) override
	{
		Call<Ret(T::*)(Args...), Fn>::call(returnValue, *(T*)obj, args);
		return returnValue;
	}
};

class GConstructor : public GCallable
{
public:
	GConstructor(
		std::vector<std::string> && signatureName, std::string && returnName
	):
		GCallable(std::move(signatureName), std::move(returnName))
	{}

	virtual Variant call(Variant *args) = 0;
};

template<typename T, typename... Args> class Contructor;

template<typename T, typename... Args>
class Constructor : public GConstructor
{
	Return<T*> returnValue;
public:
	Constructor():
		GConstructor(argsToNames<Args...>(), nameOf<T>()), returnValue()
	{}

	virtual Variant call(Variant *args) override
	{
		Construct<T, Args...>::call(returnValue, args);
		return returnValue;
	}
};
