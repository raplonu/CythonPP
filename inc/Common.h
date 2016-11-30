#pragma once

#include <memory>
#include <string>
#include <vector>
#include <typeinfo>

#include "type.h"

template<typename T>
std::string nameOf()
{
	return typeid(T).name();
}

/*
 * Args to string functions
 */
template <std::size_t POS, typename... Args> struct ArgsToVect;

template <std::size_t POS, typename First, typename... Args>
struct ArgsToVect<POS, First, Args...>
{
	static void names(std::vector<std::string> & vect)
	{
		vect[POS] = nameOf<First>();
		ArgsToVect<POS + 1, Args...>::names(vect);
	}

	template<typename Base>
	static void ptr(std::vector<std::unique_ptr<Base>> & vect)
	{
		//Test if template param respect the constraints
		static_assert(std::is_base_of<Base, First>::value, "Arg not base of arg");

		//Create the object
		vect[POS] = {new First()};

		//Continue recursive call
		ArgsToVect<POS + 1, Args...>::template ptr<Base>(vect);
	}
};

template <std::size_t POS>
struct ArgsToVect<POS>
{
  static void names(std::vector<std::string> & vect){}

  template<typename Base>
  static void ptr(std::vector<std::unique_ptr<Base>> & vect){}
};

template <typename... Args>
std::vector<std::string> argsToNames()
{
	std::vector<std::string> vect(sizeof...(Args));
	ArgsToVect<0, Args...>::names(vect);
    return vect;
}

template<typename Base, typename... Args>
std::vector<std::unique_ptr<Base>> argsToPtr()
{
	std::vector<std::unique_ptr<Base>> vect(sizeof...(Args));
	ArgsToVect<0, Args...>::template ptr<Base>(vect);
	return vect;
}


/*
 * Return Object
 */
template<typename Ret> class Return;

template<typename Ret>
class Return
{
	std::unique_ptr<Ret> returnValue;
public:
	Return():returnValue(new Ret){}

	virtual ~Return(){}

	Variant addr() const {return reinterpret_cast<Variant>(returnValue.get());}
	operator Variant() const {return addr();}

	void set(Ret const & value){*returnValue = value;}
	void operator=(Ret const & value){set(value);}
};

template<>
class Return<void>
{
public:
	Return(){}

	virtual ~Return(){}

	Variant addr()const{return 0;}
	operator Variant() const {return addr();}
};
