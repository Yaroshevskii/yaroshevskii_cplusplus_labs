//
// Created by andy on 10/13/20.
//

#ifndef RNK_MAIN_H
#define RNK_MAIN_H


#include <iostream>
#include <string>
#include <future>
#include <bitset>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <type_traits>
#include <unordered_map>

using namespace std;
using namespace std::literals;


enum Nucl {A = 0, G = 1 , C = 2 , U = 3, E = 4};

using mVar = uint64_t;


struct mConstants{
    const uint32_t A, B;
    uint8_t shift;
    mConstants(const uint32_t itemA, const uint32_t itemB, const uint8_t itemShift): A(itemA), B(itemB), shift(itemShift){}
};


class RNK {
    mVar *mSequence = nullptr;
    size_t mSizeInOwnWord = 0;
    size_t mCountOfNucl = 0;


    const mConstants calcConst(size_t item);
    enum Nucl GetNucl(size_t item);
    void SetNucl(size_t item, enum Nucl nucleotide);
    void fillSequence(mVar *sourcePoint, enum Nucl nucleotide, size_t startPosInWords, size_t countOfWords, size_t newSizeOfNucl);

    template<typename T, typename T2>
    static bool InnerIsComplementary(T && item1, T2 && item2);

    template<typename T>
    void InnerOperatorSquadBreaks(T && item, size_t itemOfNucl) const;

    template<typename T, typename reT>
    reT InnerOperatorSquadBreaks2(T && item, size_t itemOfNucl);

    template<typename T>
    RNK& InnerOperatorAssignment(RNK & r1, T && r2);

    template <typename T1, typename T2 >
    static bool InnerComprasion(T1&& t1, T2&& t2);

    template <typename T1>
    static RNK& InnerNot(T1 && t1);

    template <typename T1>
    static void InnerCopyConstructor(RNK & t1, T1 && t2);
public:

    class reference
    {
        size_t nuclItem = 0;
        RNK *motherClass = NULL;
    public:
        enum Nucl item;
        template<typename T>
        explicit reference(size_t num, T && ptrMother);

        reference& operator=(const enum Nucl & item);
        reference& operator=(const enum Nucl& item) const = delete;
        operator int();
    };

    RNK(enum Nucl nucleotide, size_t volumeOfNucl);
    RNK();

    template<typename T>
    RNK(T && r1);

    RNK(const RNK & r1);

    template<typename T, typename T2>
    static bool isComplementary(T && item1, T2 && item2);

    void trim(size_t lastIndex);
    void split(size_t index);
    [[nodiscard]] size_t cardinality( enum Nucl value) const;
    [[nodiscard]] unordered_map<enum Nucl, int, std::hash<int>> cardinality_map() const;

    template<typename T>
    RNK& operator+(T && r1) const  = delete;

    template<typename T1, typename T2>
    friend RNK& operator+ (T1 && r1, T2 && r2);

    reference operator[](size_t itemOfNucl);
    const reference operator[](size_t itemOfNucl) const;

    template<typename T1>
    RNK& operator=( T1 && r1);

    RNK& operator=( const RNK & r1);

    template<typename T, typename T2>
    T2 operator=(T && r1) const = delete;

    template<typename T1, typename T2>
    friend bool operator==(T1 && r1, T2 && r2);

    template<typename T1, typename T2>
    friend bool operator!=(T1 && r1, T2 && r2);

    template<typename T1>
    friend RNK& operator!(T1 && r1);

    RNK& operator!() const = delete;

    void printFullSequence() const;

    ~RNK();
};


class DNK
{
    RNK res = RNK();
public:
    DNK(RNK &item1, RNK &item2)
    {
        if (RNK::isComplementary(item1, item2))
        {
            res = item1 + item2;
        }
    }
};


#endif //RNK_MAIN_H
