#pragma once

#include <string>
#include <map>
#include <typeinfo>

//Choose arbitrarily a type for variant ptr
using Variant=unsigned long;
using VoidRet=int;

std::string const SEPARATOR=",";
std::string const SUPERSEPARATOR=";";

union CharAccess
{
	CharAccess(int i):i(i){}
	int i;
	char c[sizeof(int)];
};

static std::string intToString(int val)
{
	std::string str;
	CharAccess ca(val);

	for(size_t i = 0; i < sizeof(int); i++)
		if(ca.c[sizeof(int)-1-i] != '\0')
			str += std::string(1, ca.c[sizeof(int)-1-i]);
	return str;
}

template <int i, int c, int... cs>
struct StrReq
{
	static std::string toString()
	{
		std::string str = intToString(c);
		str += StrReq<i - 1, cs...>::toString();
		return str;
	}
};

//Stop recursive call when all data is copied in the tuple
template<int c, int... cs>
struct StrReq<0, c, cs...>
{
	static std::string toString(){
		return intToString(c);
	}
};

//Compute the size of the recursion and start the copy
template <int c, int... cs>
struct Str
{
	static std::string toString()
	{
		return StrReq<sizeof...(cs), c, cs...>::toString();
	}
};
//Create a std::map with multiple template arguments
class Named
{
public:
	Named(std::string const name):_name(name){}
	std::string name() {return _name;}
private:
	std::string const _name;
};

template<typename base, typename... Args> struct ArgsToMap;

template <typename base, typename First, typename... Args>
struct ArgsToMap<base, First, Args...>
{
	static std::map<std::string, base*> todo()
	{
		//Get the map from recursive call
		std::map<std::string, base*> map(ArgsToMap<base, Args...>::todo());

		//Test if template param respect the constraints
		static_assert(std::is_base_of<base, First>::value, "Arg not base of arg");
		static_assert(std::is_base_of<Named, First>::value, "Arg not base of Name");

		//Create the object
		First* arg = new First();

		//Insert the object in the map
		map.insert(std::pair<std::string, base*>(arg->name(), arg));

		//Return the map
		return map;
	}
};

template<typename base>
struct ArgsToMap<base>
{
	static std::map<std::string, base*> todo()
	{
		return std::map<std::string, base*>();
	}
};

template<typename base, typename... Args>
std::map<std::string, base*> argsToMap()
{
	return ArgsToMap<base, Args...>::todo();
}

/*
 * Args to string functions
 */
template <typename... Args> struct ArgsToString;

template <typename First, typename... Args>
struct ArgsToString<First, Args...>
{
  static std::string name()
  {
    return std::string(typeid(First).name()) + SEPARATOR + ArgsToString<Args...>::name();
  }
};


template <typename Last>
struct ArgsToString<Last>
{
  static std::string name()
  {
    return std::string(typeid(Last).name());
  }
};

template <>
struct ArgsToString<>
{
  static std::string name()
  {
    return "";
  }
};

template <typename... Args>
std::string argsToString()
{
    return ArgsToString<Args...>::name();
}


/*
 * Return Object
 */
template<typename Ret> class Return;

template<typename Ret>
class Return
{
public:
	Return():returnValue(new Ret){}
	virtual ~Return(){delete returnValue;}
	Variant returnAddr(){return reinterpret_cast<Variant>(returnValue);}
	void setValue(Ret const & value){*returnValue = value;}
private:
	Ret* returnValue;
};

template<>
class Return<void>
{
public:
	Return(){}
	virtual ~Return(){}
	Variant returnAddr(){return 0;}
	void setValue(VoidRet value){}
};
