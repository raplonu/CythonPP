#include <iostream>

#include "../inc/TaskManager.h"

using namespace std;

int fun(int i, int j)
{
    cout << "OK" << endl;
    return i + j;
}

struct A
{
    int k;
    A(){}
    A(int k):k(k){}

    int fun(int i, int j)
    {
        cout << "OK" << endl;
        return i + j;
    }
};



struct MeTM : public TaskManager
{
    MeTM() : TaskManager()
    {
        FUNTION(fun)

        CLASSBEGIN(A)
            DEFAULTCONSTRUCTOR
            CONSTRUCTOR(int)
            METHOD(fun)
        CLASSEND
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}

#include <cstdio>
#define eprintf(format, args...) printf(format , ##args)

int main()
{

    eprintf("ok\n");
	eprintf("ok %d\n", 5);

    MeTM tm;
    cout << tm.functionNames() << endl;
    cout << tm.functionSignatureName(0) << endl;
    cout << tm.functionReturnName(0) << endl;

    int i(5), j(10), res;
    Variant* tab = new Variant[2];
    tab[0] = reinterpret_cast<Variant>(&i);
    tab[1] = reinterpret_cast<Variant>(&j);

    res = *reinterpret_cast<int*>(tm.call(0, tab));

    cout << res << endl << endl;

    cout << tm.classNames() << endl;
    cout << tm.classSignatureName(0, 0) << endl;
    cout << tm.classSignatureName(0, 1) << endl;

    GObject *o = tm.create(0, 0, 0);

    i = 1;
    j = 3;

    res = *reinterpret_cast<int*>(o->call(0, tab));
    cout << res << endl << endl;

    return 0;
}
