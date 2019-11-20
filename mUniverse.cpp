//
// Created by Andy on 19.11.2019.
//


#include "mUniverse.h"
#include <chrono>


void mUniverse::Go()
{
    if (CountOfIteration <= 0)
    {
        Start();
    } else
    {
        Start(CountOfIteration);
    }
}


void mUniverse::Start(int count_itertation)
{


    auto t1 = std::chrono::steady_clock::now();

    for (int i=0; i< count_itertation; i++ )
    {
        gen->NextGen();

        if (gen->isAliveExist() == false)
        {
            break;
        }
    }

    auto t2 = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    if (CountOfIteration > 0)
    {
        gen->printSpace();
        std::cout << endl << " duration = " << duration/1000000.0 << " sec." << endl;
        getchar();
    }
}


void mUniverse::Start()
{
    for (int i=0; i< INT_MAX; i++ )
    {
        gen->NextGen();

        if (CountOfIteration < 1)
        {
            gen->printSpace();
            Sleep(100);
            if (CountOfIteration == 0)
            {
                getchar();
            }
        }

        if (gen->isAliveExist() == false)
        {
            break;
        }
    }
}