//
// Created by andy on 25.05.2020.
//
#include <iostream>
#include <tuple>
#include "RiemmanSolver.h"

void RiemmanSolver::calc_GV()
{
    GV[0] = (G - 1.0)/(2.0*G);
    GV[1] = (G + 1.0)/(2.0*G);
    GV[2] = 2.0*G/(G - 1.0);
    GV[3] = 2.0/(G - 1.0);
    GV[4] = 2.0/(G + 1.0);
    GV[5] = (G - 1.0)/(G + 1.0);
    GV[6] = (G - 1.0)/2.0;
    GV[7] = G - 1.0;
}

void RiemmanSolver::calc_CV()
{
    CL = std::sqrt(G*PL/DL);
    CR = std::sqrt(G*PR/DR);
}


void RiemmanSolver::initResolve()
{
    for (uint16_t i=0; i<=XPOS; i++ )
    {
        this->vresolve[i].clear();
    }
}

void RiemmanSolver::InitHelpVars()
{
    gTime = 0.0;
    calc_GV();
    calc_CV();
    initResolve();
}

RiemmanSolver::RiemmanSolver()
{
    G = 1.4;

    PL = 1.0;
    DL = 1.0;
    UL = 0.0;

    PR = 0.1;
    DR = 0.125;
    UR = 0.0;

    CELLS = 200;
    STARPOS = -50;
    STOPPOS = 50;
    ENDTIME = 13.0;
    dt = 0.05;

    InitHelpVars();
    initResolve();
};


void RiemmanSolver::LoadParametrs(size_t item, resolve_type val)
{
    *parametrs[item] = val;
}

uint16_t RiemmanSolver::TimeHandler(uint16_t type)
{
    uint16_t ret = 0;
    switch (type)
    {
        case 0:
            if ((ENDTIME - gTime) >= (dt - 1e-3)) {
                gTime += dt;
            } else if (std::fabs(ENDTIME - gTime) >= 1e-9) {
                ret = 1;
            }
        break;
        case 1:
            if (gTime > 0)
            {
                gTime -= dt;
            }
        break;
        case 2:
            if (gTime < ENDTIME)
            {
                gTime += dt;
            }
        break;
        case 3:
            if (gTime > 1e-6)
            {
                gTime -= dt;
            }
        break;
        case 4:
            gTime = ENDTIME;
        break;
        default:
            gTime = 0.0;
    }

    return ret;
}



std::pair<resolve_type, resolve_type> RiemmanSolver::PreFun(resolve_type P, resolve_type DK, resolve_type PK, resolve_type CK)
{
    resolve_type F, FD;
    resolve_type PRAT, QRT, AK, BK;

    if (P <= PK) {
        PRAT = P / PK;
        F = GV[3] * CK * (std::pow(PRAT,GV[0]) - 1.0);
        FD = (1.0 / (DK * CK)) * std::pow(PRAT,(-GV[1]));
    }
    else {
        AK = GV[4] / DK;
        BK = GV[5] * PK;
        QRT = std::sqrt(AK / (BK + P));
        F = (P - PK) * QRT;
        FD = (1.0 - 0.5 * (P - PK) / (BK + P)) * QRT;
    }

    return std::make_pair(F, FD);
}

resolve_type RiemmanSolver::GuessP()
{
    resolve_type zero = 0.0;
    resolve_type QUser = 2.0;
    resolve_type PM = 0.0;

    resolve_type CUP = 0.25*(DL+DR)*(CL+CR);
    resolve_type PPV = 0.5*(PL+PR)+0.5*(UL-UR)*CUP;
    PPV = std::max(PPV, zero);
    resolve_type Pmin = std::min(PL, PR);
    resolve_type Pmax = std::max(PL, PR);
    resolve_type Qmax = Pmax/Pmin;

    if (Qmax <= QUser and (Pmin <= PPV and PPV <= Pmax))
        PM = PPV;
    else
    {
        if (PPV < Pmin)
        {
            resolve_type PQ = std::pow((PL / PR), GV[0]);
            resolve_type UM = (PQ * UL / CL + UR / CR + GV[3] * (PQ - 1.0)) / (PQ / CL + 1.0 / CR);
            resolve_type PTL = 1.0 + GV[6] * (UL - UM) / CL;
            resolve_type PTR = 1.0 + GV[6] * (UM - UR) / CR;
            PM = 0.5 * (PL * std::pow(PTL, GV[2]) + PR * std::pow(PTR, GV[2]));
        }
        else
        {
            resolve_type GEL = std::sqrt((GV[4] / DL) / (GV[5] * PL + PPV));
            resolve_type GER = std::sqrt((GV[4] / DR) / (GV[5] * PR + PPV));
            PM = (GEL * PL + GER * PR - (UR - UL)) / (GEL + GER);
        }
    }
    return PM;
}

std::pair<resolve_type, resolve_type> RiemmanSolver::StarPU()
{
    resolve_type FL, FLD, FR, FRD;
    resolve_type TOLPre = 10e-6;
    uint16_t NRIter = 20;
    resolve_type P = 0.0, Change = 0.0;

    resolve_type PStart = GuessP();

    resolve_type POld = PStart;
    resolve_type UDiff = UR - UL;


    for(uint16_t i=0; i < NRIter; i++)
    {
        std::tie(FL, FLD) = PreFun(POld, DL, PL, CL);
        std::tie(FR, FRD) = PreFun(POld, DR, PR, CR);
        P = POld - (FL + FR + UDiff) / (FLD + FRD);
        Change = 2.0 * std::fabs((P - POld) / (P + POld));
        if (Change <= TOLPre) break;
        if (P < 0.0) P = TOLPre;
        POld = P;
    }

    resolve_type U = 0.5*(UL + UR + FR - FL);
    return std::make_pair(P, U);
}

fruit RiemmanSolver::SAMPLE(resolve_type PM, resolve_type UM, resolve_type S)
{
    fruit ret = {0, 0, 0};
    resolve_type D, U, P, STL, SHL, CML, SL, PML, C, STR, SHR, CMR, SR, PMR;

    if (S <= UM)
    {
        if (PM <= PL) {
            //sampling point lies to the left of the contact discontinuity
            //Left rarefaction
            SHL = UL - CL;
            if (S <= SHL)
            {
                //Sampled point is left data state
                D = DL;
                U = UL;
                P = PL;
            }
            else
            {
                CML = CL * std::pow( (PM / PL),GV[0]);
                STL = UM - CML;
                if (S > STL)
                {
                    //sampled point is star left state
                    D = DL * std::pow( (PM / PL),(1.0 / G) );
                    U = UM;
                    P = PM;
                }
                else
                {
                    //sampled point is inside left fan
                    U = GV[4] * (CL + GV[6] * UL + S);
                    C = GV[4] * (CL + GV[6] * (UL - S));
                    D = DL * std::pow( (C / CL), GV[3] );
                    P = PL * std::pow( (C / CL), GV[2] );
                }
            }
        }
        else
        {
            //left shock
            PML = PM / PL;
            SL = UL - CL * std::sqrt(GV[1] * PML + GV[0]);
            if (S <= SL)
            {
                //sampled point is left date state
                D = DL;
                U = UL;
                P = PL;
            }
            else
            {
                //sampled point is left data state
                D = DL * (PML + GV[5]) / (PML * GV[5] + 1.0);
                U = UM;
                P = PM;
            }
        }
    }
    else
    {
        //Sampling point lies to the right of the contact discontinuity
        if (PM > PR)
        {
            //right shock
            PMR = PM / PR;
            SR = UR + CR * std::sqrt(GV[1] * PMR + GV[0]);
            if (S >= SR)
            {
                //Sampled point is right data state
                D = DR;
                U = UR;
                P = PR;
            }
            else
            {
                //Sampled point is Star Right state
                D = DR * (PMR + GV[5]) / (PMR * GV[5] + 1.0);
                U = UM;
                P = PM;
            }
        }
        else
        {
            //right rarefaction
            SHR = UR + CR;
            if (S >= SHR)
            {
                //Sampled point is right data state
                D = DR;
                U = UR;
                P = PR;
            }
            else
            {
                CMR = CR * std::pow( (PM / PR), GV[0] );
                STR = UM + CMR;
                if (S <= STR)
                {
                    //Sampled point is Star Right state
                    D = DR * std::pow( (PM / PR), (1.0 / G) );
                    U = UM;
                    P = PM;
                }
                else
                {
                    //Sampled point is inside left fan
                    U = GV[4] * (-CR + GV[6] * UR + S);
                    C = GV[4] * (CR - GV[6] * (UR - S));
                    D = DR * std::pow( (C / CR), GV[3]);
                    P = PR * std::pow( (C / CR), GV[2]);
                }
            }
        }
    }
    ret.D = D;
    ret.P = P;
    ret.U = U;
    return ret;
}

void RiemmanSolver::vsolve()
{
    initResolve();
    fruit SampleRet;
    resolve_type S, X;
    resolve_type timeout = this->gTime;

    resolve_type DIAPH = (std::fabs(STOPPOS) - std::fabs(STARPOS)) / 2.0;
    resolve_type PMM, UMM;


    std::tie(PMM, UMM) = StarPU();
    resolve_type DX = (std::fabs(STOPPOS) + std::fabs(STARPOS)) / CELLS;

    for(size_t i=0; i < (size_t)CELLS; i++ )
    {
        X = ((i+1) - 0.5)*DX - std::fabs(STARPOS);
        vresolve[XPOS].push_back(X);
        S = (X - DIAPH)/timeout;
        SampleRet = SAMPLE(PMM, UMM, S);
        SampleRet.T = SampleRet.P/SampleRet.D/GV[7];

        vresolve[U].push_back(SampleRet.U);
        vresolve[D].push_back(SampleRet.D);
        vresolve[P].push_back(SampleRet.P);
        vresolve[T].push_back(SampleRet.T);

    }
}



