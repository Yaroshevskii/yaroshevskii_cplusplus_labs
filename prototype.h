//
// Created by andy on 15.04.2020.
//

#ifndef INTEGRATOR_LAB_PROTOTYPE_H
#define INTEGRATOR_LAB_PROTOTYPE_H

#include <tuple>
#include <functional>

namespace prototype
{
    template<typename RetType, typename ... Params>
    struct func_n_t {
        using type = RetType(Params...);//Тип функции
        //Этот шаблонный using будет хранить тип func_n_t с параметрами, добавленными к текущим
        template<typename ... Args>
        using add = func_n_t<RetType, Params..., Args...>;
    };


    template<::std::size_t N, typename Func, typename ... Args>
    struct function_n_t_impl
    {
        //Применяем рекурсию шаблонов.
        //На каждом шаге к инстансу шаблона func_n_t прибавляются аргументы Args
        //При этом N уменьшается на 1
        using type = typename function_n_t_impl<N - 1, typename Func::template add<Args...>, Args...>::type;
    };
    //Частичная специализация, которая остановит рекурсию
    template<typename Func, typename ... Args>
    struct function_n_t_impl<0, Func, Args...>
    {
        //Просто берем от типа Func тип нужной нам функции
        using type = typename Func::type;
    };
}

//Псевдоним, который запускает формирование нужного типа, начиная с func_n_t<RetType> (т.е. с type = RetType())
template<::std::size_t N, typename RetType, typename ... ArgsType>
using make_function_t = typename ::prototype::function_n_t_impl<N, prototype::func_n_t<RetType>, ArgsType...>::type;


namespace match
{
    using mFunc =  double(*)(double);

    template<class F1, class F2, class... Args>
    constexpr auto multiply2( F1 f1, F2 f2, std::tuple<Args...>)
    {
        return[=](Args... args, Args... args2)
        {
            return ( (f1(args)*f2(args2)) + ...);
        };
    }
}


#endif //INTEGRATOR_LAB_PROTOTYPE_H