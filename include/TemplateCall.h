#pragma once

#include <functional>
#include <tuple>

#include "Common.h"


/*
 * Structure for maping tuple to function argument
 */
template<int ...> struct seq {};
template<int N, int ...S> struct gens : gens<N - 1, N - 1, S...> {};
template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };

/*
 * Template type access for variadic args list
 */
template<std::size_t N, typename... Types>
using Type = typename std::tuple_element<N, std::tuple<Types...>>::type;

/*
 * Simple function declaration
 */
template<class Ret, class... Args, int ...S>
Ret callFun_(std::function<Ret(Args...)> &fun, Variant const * const args, seq<S...>){
	return fun(*(reinterpret_cast<Type<S, Args...>*>(args[S]))...);
}

template<class Ret, class... Args> struct CallFun;

template<class Ret, class... Args>
struct CallFun
{
	static Ret call(std::function<Ret(Args...)> &fun, Variant const * const args)
	{
		return callFun_(fun, args, typename gens<sizeof...(Args)>::type());
	}
};

template<class... Args>
struct CallFun<void, Args...>
{
	static VoidRet call(std::function<void(Args...)> &fun, Variant const * const args)
	{
		callFun_(fun, args, typename gens<sizeof...(Args)>::type());
		return 0;
	}
};

/*
 * Class method declaration
 */
template<class T, class Ret, class... Args, int ...S>
Ret callMet_(std::function<Ret(T&, Args...)> &fun, T & obj, Variant const * const args, seq<S...>){
	return fun(std::forward<T&>(obj), *(reinterpret_cast<Type<S, Args...>*>(args[S]))...);
}

template<class T, class Ret, class... Args> struct CallMet;

template<class T, class Ret, class... Args>
struct CallMet
{
	static Ret call(std::function<Ret(T&, Args...)> &fun, T & obj, Variant const * const args)
	{
		return callMet_(fun, std::forward<T&>(obj), args, typename gens<sizeof...(Args)>::type());
	}
};

template<class T, class... Args>
struct CallMet<T, void, Args...>
{
	static VoidRet call(std::function<void(T&, Args...)> &fun, T & obj, Variant const * const args)
	{
		callMet_(fun, std::forward<T&>(obj), args, typename gens<sizeof...(Args)>::type());
		return 0;
	}
};

/*
 * Class constructor declaration
 */
template<class T, class... Args, int ...S>
T* createPtr_(Variant const * const args, seq<S...>){
	return new T(*(reinterpret_cast<Type<S, Args...>*>(args[S]))...);
}

template<typename T, typename... Args> struct CreatePtr;

template<typename T, typename... Args>
struct CreatePtr
{
	static T* call(Variant const * const args)
	{
		return createPtr_<T, Args...>(args, typename gens<sizeof...(Args)>::type());
	}
};


