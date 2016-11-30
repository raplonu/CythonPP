#pragma once

#include <memory>
#include <functional>
#include <tuple>

#include "Common.h"


/*
 * Structure for maping tuple to function argument
 */
template<int...> struct seq {};
template<int N, int... S> struct gens : gens<N - 1, N - 1, S...> {};
template<int... S> struct gens<0, S...>{ typedef seq<S...> type; };

/*
 * Template type access for variadic args list
 */
template<std::size_t N, typename... Types>
using Type = typename std::tuple_element<N, std::tuple<Types...>>::type;

/*
 * General call structure declaration
 */
 template<typename Fn, Fn> struct Call;

/*
 * Simple function with return value declaration
 */
template<
	typename Ret, typename... Args,
 	Ret (*Fn)(Args...)>
class Call<Ret(*)(Args...), Fn>
{
	template<int... S>
	static Ret call_(Variant const * args, seq<S...>)
	{
		(void)args;
		return Fn(*(reinterpret_cast<Type<S, Args...>*>(args[S]))...);
 	}

public:
 	static void call(Return<Ret> & ret, Variant const * args)
 	{
 		ret = call_(args, typename gens<sizeof...(Args)>::type());
 	}
};

/*
 * Simple function with no return value declaration
 */
template<
 	typename... Args,
 	void (*Fn)(Args...)>
class Call<void(*)(Args...), Fn>
{
 	template<int... S>
 	static void call_(Variant const * args, seq<S...>)
 	{
 		(void)args;
 		return Fn(*(reinterpret_cast<Type<S, Args...>*>(args[S]))...);
 	}

public:
 	static void call(Return<void> & ret, Variant const * args)
 	{
 		call_(args, typename gens<sizeof...(Args)>::type());
 	}
};

/*
 * Method with return value declaration
 */
template<
  	typename T, typename Ret, typename... Args,
  	Ret (T::*Fn)(Args...)>
class Call<Ret(T::*)(Args...), Fn>
{
  	template<int... S>
  	static Ret call_(T & obj, Variant const * args, seq<S...>)
  	{
  		(void)args;
  		return (obj.*Fn)(*(reinterpret_cast<Type<S, Args...>*>(args[S]))...);
  	}

public:
  	static void call(Return<Ret> & ret, T & obj, Variant const * args)
  	{
  		ret = call_(obj, args, typename gens<sizeof...(Args)>::type());
  	}
};

/*
 * Method with no return value declaration
 */
 template<
  	typename T, typename... Args,
  	void (T::*Fn)(Args...)>
class Call<void(T::*)(Args...), Fn>
{
  	template<int... S>
  	static void call_(T & obj, Variant const * args, seq<S...>)
  	{
  		(void)args;
  		(obj.*Fn)(*(reinterpret_cast<Type<S, Args...>*>(args[S]))...);
  	}

public:
  	static void call(Return<void> & ret, T & obj, Variant const * args)
  	{
  		call_(obj, args, typename gens<sizeof...(Args)>::type());
  	}
};

/*
 * Class constructor declaration
 */
template<
  	typename T, typename... Args>
class Construct
{
  	template<int... S>
  	static T* call_(Variant const * args, seq<S...>)
  	{
  		(void)args;
  		return new T(*(reinterpret_cast<Type<S, Args...>*>(args[S]))...);
  	}

public:
  	static void call(Return<T*> & ret, Variant const * args)
  	{
  		ret = call_(args, typename gens<sizeof...(Args)>::type());
  	}
};
