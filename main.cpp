#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include "prototype.h"
#include <tuple>

using namespace std;

using  mFunc = double(*)(double);


struct myff
{
    constexpr static double val(double x)
    {
        return 2*x*x;
    }
};


struct left_boundary
{
    constexpr static double value = -1.0;
};

struct right_boundary
{
    constexpr static double value = 1.0;
};

struct steps
{
    constexpr static int value = 1000;
};

struct mSolver{

    enum GausOrder {three = 3, four = 4};

    #define A3 0.555555556, 0.888888889, 0.555555556
    #define X3 -0.774596669, 0.0, 0.774596669

    #define A4 0.347854845 , 0.652145155, 0.652145155, 0.347854845
    #define X4 -0.861136312, -0.339981044, 0.339981044, 0.861136312

    constexpr static auto calcSum = [](mFunc ptr2Fnc, double *a, double *x, int order){
        double ret = 0;
        for (int i = 0; i < order; i++)
            ret += a[i]*ptr2Fnc(x[i]);
        return ret;
    };

    constexpr static double RiemannSum(mFunc ptr2Fnc, const double A = left_boundary::value, const double B = right_boundary::value)
    {
        double steps = steps::value;
        double ret = 0, b  = 0;
        double h = (B - A)/steps;

        for (int i=0; i <= steps; ++i)
        {
            b =  A + i*h;
            ret += calcSum(ptr2Fnc, &h, &b, 1);
        }
        return ret;
    }

    constexpr static double Gauss3d(mFunc ptr2Fnc)
    {
        double ret = 0;
        constexpr auto myf = [](double x) { return x; };
        constexpr auto myf2 = [](double x) { return x*x; };
        //constexpr auto myf2 = [=](double x) { return ptr2Fnc(x); };


        constexpr auto a3 = make_tuple(A3);

        constexpr auto fff = match::multiply2(myf, myf2, a3);
        ret = fff(A3, X3);

        return ret;
    }

    constexpr static double Gauss(mFunc ptr2Fnc, enum GausOrder order)
    {
        double ret = 0;

        double a3[3] = {A3};
        double x3[3] = {X3};

        double a4[4] = {A4};
        double x4[4] = {X4};

        double *a = nullptr;
        double *x = nullptr;

        switch (order) {
            case three:
                a = a3;
                x = x3;
                break;
            case four:
                a = a4;
                x = x4;
                break;
            default:
                a = a3;
                x = x3;
                break;
        }

        ret = calcSum(ptr2Fnc, a, x, order);

        return ret;
    }
};

template <typename T,typename fmFunc, typename... Args>
constexpr double Integrator(T func, fmFunc solver, Args... args)
{
    return solver(func, args...);
}


int main(int argc, char** argv) {


    constexpr auto  f = [](double x) {return x*x;};
    constexpr auto  f2 = [](double x) {return x;};

    constexpr double res = Integrator( f , mSolver::Gauss, mSolver::three);
    cout << res << endl;


    static_assert( res > 0.666667 - 1e-6 && res < 0.666667 + 1e-6, "Error");
    //static_assert( val > 1.53481 - 1e-6 && val < 1.53481 + 1e-2, "Error");

    return 0;
}
