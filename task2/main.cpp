#include <iostream>

using namespace std;


class Literal {
public:
    Literal(const double v) : _val(v) {}
    double eval() const { return _val; }
private:
    const double _val;
};

class Variable {
public:
    Variable(double& v) : _val(v) {}
    double eval() const { return _val; }
private:
    double& _val;
};

template <class ExprT> struct exprTraits
{ typedef ExprT expr_type; };

template <> struct exprTraits<double>
{ typedef Literal expr_type; };

template <> struct exprTraits<int>
{ typedef Literal expr_type; };



template <class ExprT1,class ExprT2, class BinOp>
class BinaryExpr
{
private:
    typename exprTraits<ExprT1>::expr_type _expr1;
    typename exprTraits<ExprT2>::expr_type _expr2;
    BinOp  _op;
public:
    BinaryExpr(ExprT1 e1, ExprT2 e2,BinOp op=BinOp())
            : _expr1(e1),_expr2(e2),_op(op) {}

    double eval() const
    {
        return _op(_expr1.eval(),_expr2.eval());
    }

    double eval(double d) const
    {
        return _op(_expr1.eval(d),_expr2.eval(d));
    }

};

template  <class ExprT1,class ExprT2>
BinaryExpr<ExprT1,ExprT2,plus<double> >
operator+(ExprT1 e1, ExprT2 e2)
{
    return BinaryExpr<ExprT1,ExprT2,plus<double> >(e1,e2);
}

template  <class ExprT1,class ExprT2>
BinaryExpr<ExprT1,ExprT2,multiplies<double> >
operator*(ExprT1 e1, ExprT2 e2)
{
    return BinaryExpr<ExprT1,ExprT2,multiplies<double> >(e1,e2);
}

template  <class ExprT1,class ExprT2>
BinaryExpr<ExprT1,ExprT2,minus<double> >
operator-(ExprT1 e1, ExprT2 e2)
{
    return BinaryExpr<ExprT1,ExprT2,minus<double> >(e1,e2);
}

template  <class ExprT1,class ExprT2>
BinaryExpr<ExprT1,ExprT2,divides<double> >
operator/(ExprT1 e1, ExprT2 e2)
{
    return BinaryExpr<ExprT1,ExprT2,divides<double> >(e1,e2);
}


template<class T>
class Identity
{
public:
    T eval(T d) const { return d; }
};

template <class ExprT>
double integrate (ExprT e,double from,double to,size_t n)
{
    double sum=0, step=(to-from)/n;
    for (double i=from+step/2; i<to; i+=step)
        sum += e.eval(i);
    return step*sum;
}

int main() {

    Identity<double> x;
    cout << integrate (x*x,0.0,1.0,10) << endl;

    return 0;
}