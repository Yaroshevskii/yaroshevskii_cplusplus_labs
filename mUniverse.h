//
// Created by Andy on 19.11.2019.
//

#ifndef TASK0_MUNIVERSE_H
#define TASK0_MUNIVERSE_H


#include "mLife.h"





class mUniverse
{
private:
    IGeneration *gen;
    int CountOfIteration;
public:
    mUniverse(mGeneration *igen,  int iterations) : CountOfIteration(iterations)
    {
        gen = static_cast<IGeneration*>(igen);
    }
    mUniverse(mLife *igen, int iterations) : CountOfIteration(iterations)
    {
        gen = static_cast<IGeneration*>(igen);
    }
    void Go();
    void Start();
    void Start(int);
};



#endif //TASK0_MUNIVERSE_H
