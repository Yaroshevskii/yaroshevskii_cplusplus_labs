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


template<typename F, typename T>
auto sumf(F func, T val)
{
    return val;
}


template<typename F, typename T, typename ...Args>
auto sumf(F func, T val1,  Args... values)
{
    return func(sumf(func, values...), val1);
}


template<class A, class B>
auto multiply_func(A func1, B func2)
{
    return [=](auto a, auto b) {return func1(a,b) * func2(a,b);};
}

int main(int argc, char *argv[])
{

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}

TEST(finalcheck, test1)
{
    int aa = 1, bb = 2;
    int cc = close_enough(aa, bb);
    EXPECT_EQ(cc, 0);
}

TEST(finalcheck, test2)
{
    int aa = 2;
    int bb = 2;
    int cc = close_enough(aa, bb);
    EXPECT_EQ(cc, 1);
}

TEST(finalcheck, test3)
{
    double aa = 2;
    double bb = 2;
    double cc = close_enough(aa, bb);
    EXPECT_EQ(cc, 1);
}

TEST(finalcheck, test4)
{
    double aa = 2.01;
    double bb = 2;
    double cc = close_enough(aa, bb);
    EXPECT_EQ(cc, 0);
}

TEST(finalcheck, test5)
{
    EXPECT_EQ(sum(2, 2, 1, 1), 6);
}

TEST(finalcheck, test6)
{
    EXPECT_EQ(sum(2, 2, 1, 5), 10);
}

TEST(finalcheck, test7)
{
    EXPECT_EQ(suml(5, 5, 5, 5), 20);
}

TEST(finalcheck, test8)
{
    auto f = [](auto a, auto b){return a*b;};
    EXPECT_EQ(sumf( f, 1, 2, 3, 4), 24);
}

TEST(finalcheck, test9)
{
    auto f = [](auto a, auto b){return a+b;};
    EXPECT_EQ(sumf( f, 1, 2, 3, 4), 10);
}

TEST(finalcheck, test10)
{
    auto f = [](auto a, auto b){return a-b;};
    EXPECT_EQ(sumf( f, 4, 3, 2, 1), -8);
}

TEST(finalcheck, test11)
{
    auto f = [](float a, float b){return a/b;};
    EXPECT_EQ(sumf( f, 2, 5, 2, 100), 5);
}

TEST(finalcheck, test12)
{
    auto f1 = [](auto a, auto b){return a+b;};
    auto f2 = [](auto a, auto b){return a*b;};
    auto f3 = multiply_func(f1, f2);
    EXPECT_EQ(f3(1, 2), 6);
}

TEST(finalcheck, test13)
{
    m_weak_ptr<int> gw;

    int *pp = new int(42);
    m_shared_ptr<int> msp(pp);
    gw = msp;

    EXPECT_EQ(gw.use_count(), 1);
}

TEST(finalcheck, test14)
{
    m_weak_ptr<int> gw;
    {
        int *pp = new int(42);
        m_shared_ptr<int> msp(pp);
        gw = msp;
    }

    EXPECT_EQ(gw.use_count(), 0);
}