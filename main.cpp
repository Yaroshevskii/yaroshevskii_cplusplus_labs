#include <iostream>
#include <gtest/gtest.h>
#include <math.h>
#include <functional>
#include "mWeakPtr.h"
using namespace std;


template<class T>
bool close_enough(T a, T b, typename std::enable_if<std::is_integral<T>::value >::type* = 0)
{
    return a == b;
}

template<class T>
bool close_enough(T a, T b, typename std::enable_if<std::is_floating_point<T>::value >::type* = 0)
{
    float eps = 1e-5;
    return fabs(a - b) < eps;
}


template<typename ...Args>
auto sum(Args... values)
{
    return ((values) + ...);
}


auto suml()
{
    return 0;
}


template<typename T, typename ...Args>
auto suml(T val, Args... values)
{
    return val + suml(values...);
}

template<typename F>
auto sumf(F func)
{
    return 0;
}

template<typename F, typename T, typename ...Args>
auto sumf(F func, T val1, T val2, Args... values)
{
    return func(val1, val2) + sumf(func, values...);
}


template<class A, class B>
auto multiply_func(A func1, B func2)
{
    return [=](auto a, auto b) {return func1(a,b) * func2(a,b);};
}


m_weak_ptr<int> gw;

void f()
{
    auto spt = gw.lock();
    if (spt.get())
    {
        std::cout << *spt.get() << "\n";
    } else
    {
        std::cout << "gw is expired\n";
    }
}

int main()
{
    float a = 10.0, b = 11.0;
    int aa = 1, bb = 2;

    cout << close_enough(a, b) << endl;
    cout << close_enough(aa, bb) << endl;

    cout << sum(2, 2, 1, 1) << endl;

    cout << suml(1, 2, 3, 4) << endl;

    cout << sumf( [](auto a, auto b){return a*b;} , 1,2, 3, 4) << endl;

    auto f1 = [](auto a, auto b){return a+b;};
    auto f2 = [](auto a, auto b){return a*b;};
    auto f3 = multiply_func(f1, f2);
    cout << f3(1, 2) << endl;


    {
        int *pp = new int(42);
        m_shared_ptr<int> msp(pp);
        gw = msp;

        f();
    }
    f();


    return 0;
}
