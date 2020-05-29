//
// Created by andy on 25.05.2020.
//

#ifndef SOLVERFORRIEMMANPROBLEMQT_RIEMMANSOLVER_H
#define SOLVERFORRIEMMANPROBLEMQT_RIEMMANSOLVER_H
#include <vector>
#include <stdint.h>
#include <math.h>

using resolve_type = float_t ;

struct fruit
{
    resolve_type D;
    resolve_type P;
    resolve_type U;
    resolve_type T;
};

struct vfruit
{
    std::vector<resolve_type> XPOS;
    std::vector<resolve_type> D;
    std::vector<resolve_type> P;
    std::vector<resolve_type> U;
    std::vector<resolve_type> T;
};

enum natural {XPOS = 4, D = 0, P = 1, U = 2, T = 3};


class RiemmanSolver
{
private:
    resolve_type GV[8];
    resolve_type CL, CR;

public:
    resolve_type PL, DL, UL;
    resolve_type PR, DR, UR;
    resolve_type G;
    resolve_type CELLS;
    resolve_type STARPOS, STOPPOS, ENDTIME;

    std::vector<resolve_type> vresolve[5];
    resolve_type gTime, dt;
    resolve_type *parametrs[12] = {&DL, &PL, &UL, &DR, &PR, &UR, &STARPOS, &STOPPOS, &dt, &CELLS, &G, &ENDTIME};
public:
    RiemmanSolver();
    void initResolve();
    void calc_GV();
    void calc_CV();
    void InitHelpVars();
    std::pair<resolve_type, resolve_type> PreFun(resolve_type, resolve_type, resolve_type, resolve_type);
    resolve_type GuessP();
    std::pair<resolve_type, resolve_type> StarPU();
    fruit SAMPLE(resolve_type, resolve_type, resolve_type);
    void vsolve();
    void LoadParametrs(size_t item, resolve_type val);
    uint16_t TimeHandler(uint16_t);

};

#endif //SOLVERFORRIEMMANPROBLEMQT_RIEMMANSOLVER_H
