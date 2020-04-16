#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include "prototype.h"

using namespace std;

using mFunc =  double(*)(double);


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

    enum GausOrder {three = 3, four = 4};

    constexpr static double Gauss(mFunc ptr2Fnc, enum GausOrder order)
    {
        double ret = 0;

        double a3[3] = {0.555555556, 0.888888889, 0.555555556};
        double x3[3] = {-0.774596669, 0.0, 0.774596669};

        double a4[4] = {0.347854845 , 0.652145155, 0.652145155, 0.347854845};
        double x4[4] = {-0.861136312, -0.339981044, 0.339981044, 0.861136312};

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

template <typename fmFunc, typename... Args>
constexpr double Integrator(mFunc func, fmFunc solver, Args... args)
{
    return solver(func, args...);
}



int main(int argc, char** argv) {

    constexpr auto  f = [](double x) {return x*x;};


    constexpr double res = Integrator( f, mSolver::Gauss, mSolver::three );
    cout << res << endl;


    static_assert( res > 0.666667 - 1e-6 && res < 0.666667 + 1e-6, "Error");
    //static_assert( res > 1.33333 - 1e-2 && res < 1.33333 + 1e-2, "Error");

    return 0;
}
