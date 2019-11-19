//
// Created by Andy on 19.11.2019.
//

#include "mLife.h"
#include "mUniverse.h"


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
    for (int i=0; i< count_itertation; i++ )
    {
        gen->NextGen();

        if (gen->isAliveExist() == false)
        {
            break;
        }
    }

    if (CountOfIteration > 0)
    {
        gen->printSpace();
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