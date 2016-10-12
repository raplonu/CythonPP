#pragma once

#include <string>

#include "Common.h"
#include "TemplateCall.h"

class GCallable
{
public:
	virtual ~GCallable(){}
	virtual Variant call(Variant *args) = 0;
	virtual std::string signature() = 0;
	virtual std::string returnValue() = 0;
	virtual void setObj(Variant obj){(void)obj;}
protected:
	virtual void internCall(Variant *args) = 0;
};

template<typename Ret, typename... Args>
class Callable : public GCallable
{
public:
	virtual ~Callable(){}

	virtual Variant call(Variant *args) override
	{
		internCall(args);
		return returnVal.returnAddr();
	}
	virtual std::string signature() override
	{
		return argsToString<Args...>();
	}
	virtual std::string returnValue() override
	{
		return argsToString<Ret>();
	}
protected:
	Return<Ret> returnVal;
};

template<typename Fn, Fn> class Function;

template<
	typename Ret, typename... Args,
	Ret (*Fn)(Args...)>
class Function<Ret(*)(Args...), Fn> : public Callable<Ret, Args...>
{
public:
	Function():fun(Fn){}
	virtual ~Function(){}
protected:
	virtual void internCall(Variant *args) override
	{
		this->returnVal.setValue(CallFun<Ret, Args...>::call(fun, args));
	}
private:
	std::function<Ret(Args...)> fun;
};

template<typename Str, typename Fn, Fn> class Method;

template<typename Str,
	typename T, typename Ret, typename... Args,
	Ret (T::*Fn)(Args...)>
class Method<Str, Ret(T::*)(Args...), Fn> : public Callable<Ret, Args...>, public Named
{
public:
	Method():Callable<Ret, Args...>(), Named(Str::toString()), obj(nullptr), fun(Fn){}
	virtual ~Method(){}
	virtual void setObj(Variant obj)
	{
		this->obj = reinterpret_cast<T*>(obj);
	}
protected:
	virtual void internCall(Variant *args) override
	{
		this->returnVal.setValue(CallMet<T, Ret, Args...>::call(fun, *obj, args));
	}
private:
	T *obj;
	std::function<Ret(T&, Args...)> fun;
};

template<typename T, typename... Args> class Contructor;

template<typename T, typename... Args>
class Constructor : public Callable<T*, Args...>, public Named
{
public:
	Constructor():Callable<T*, Args...>(), Named(argsToString<Args...>()){}
	virtual ~Constructor(){}
protected:
	virtual void internCall(Variant *args) override
	{
		this->returnVal.setValue(CreatePtr<T, Args...>::call(args));
	}
};
