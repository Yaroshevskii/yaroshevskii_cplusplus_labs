#include "main.h"



template<typename T>
const T reversebits(const T& in)
{
    T out = T(0);

    for (size_t i = 0; i < sizeof(T) * 8; ++i)
    {
        out <<= 1;
        out |= (in >> i) & 1;
    }

    return out;
}

template<class T1, class T2>
void print_is_same() {
    std::cout << "is same = " << std::is_same<T1, T2>() << '\n';
}

template<typename T>
RNK::reference::reference(size_t num, T && ptrMother) : nuclItem(num), motherClass(const_cast<RNK*>(ptrMother))
{
    item = motherClass->GetNucl(num);
}

RNK::reference& RNK::reference::operator=(const enum Nucl& item)
{
    motherClass->SetNucl(nuclItem, item);

    return *this;
}



RNK::reference::operator int()
{
    return item;
}


const mConstants RNK::calcConst(size_t item)
{
    const uint32_t col_bit_in_word = sizeof(mVar)*8;
    const uint32_t col_bytes_in_word = sizeof(mVar);
    const uint32_t col_bits_to_encode = 2;


    const uint8_t shift = 6 - (item%4)*col_bits_to_encode;
    const uint32_t A = (item * col_bits_to_encode) / col_bit_in_word;
    const uint32_t B = (col_bytes_in_word-1) - (item * col_bits_to_encode - (((item * col_bits_to_encode) / col_bit_in_word) * col_bit_in_word) ) / (col_bit_in_word / col_bytes_in_word);

    return mConstants(A, B, shift);
}


enum Nucl RNK::GetNucl(size_t item)
{
    if ( item > (mCountOfNucl-1) )
    {
        cout <<"get item = " << item << " return OVERFLOW" << endl;
        return E;
    }

    const mConstants constants = calcConst(item);

    uint8_t uelem = ((uint8_t*)&mSequence[constants.A])[constants.B];


    uint8_t ret = ( (uelem >> constants.shift) & 0b11 );
    enum Nucl rett = (enum Nucl)ret;

    return rett;
}


void RNK::SetNucl(size_t item, enum Nucl nucleotide)
{
    if ( item > (mCountOfNucl-1) )
    {
        cout <<"set item = " << item << " return OVERFLOW" << endl;
        return;
    }

    const mConstants constants = calcConst(item);

    uint8_t uelem = ((uint8_t*)&mSequence[constants.A])[constants.B];

    int8_t resultByte = (~(0b11 << constants.shift) & uelem) | (nucleotide << constants.shift);

    ((uint8_t*)&mSequence[constants.A])[constants.B] = resultByte;
}

    
void RNK::printFullSequence() const
{
    for(uint32_t i=0; i < mSizeInOwnWord; i++)
    {
        cout << " i = " << i << " mask = " << std::bitset<sizeof(mSequence[i]) * CHAR_BIT>(mSequence[i]) << endl;
    }
    cout << endl;
}

void RNK::fillSequence(mVar *sourcePoint, enum Nucl nucleotide, size_t startPosInWords, size_t countOfWords, size_t newSizeOfNucl)
{
    size_t countOfBytes = sizeof(mVar) * (countOfWords);

    if (mSizeInOwnWord != 0)
    {
        size_t oldCountOfBits = 2*mCountOfNucl;
        size_t countOfFreeBits = sizeof(mVar)*8*mSizeInOwnWord - oldCountOfBits;

        if (countOfFreeBits != 0)
        {
            size_t newCountOfNuclInBytes = newSizeOfNucl / 4;
            countOfBytes = newCountOfNuclInBytes == 0 ? sizeof(mVar) : newCountOfNuclInBytes;

            mVar iMask = 0, cMask = 0-4;
            for (uint16_t i = 0; i< (countOfFreeBits/2 - 1); i++)
            {
                iMask |= nucleotide;
                iMask <<= 2;
                cMask <<= 2;
            }
            iMask |= nucleotide;

            mSequence[mSizeInOwnWord-1] = (mSequence[mSizeInOwnWord-1] & cMask) | iMask;
        }
    }

    mSequence = (mVar*)realloc(sourcePoint, countOfBytes );

    mVar mask = 0;
    for (uint32_t i=0; i < (sizeof(mVar)*4) - 1; i++)
    {
        mask |= nucleotide;
        mask <<= 2;
    }
    mask |= nucleotide;

    for(uint32_t i=startPosInWords; i<countOfWords; i++)
    {
        mSequence[i] = mask;
    }

    mCountOfNucl = newSizeOfNucl;
    mSizeInOwnWord = countOfWords;
}

RNK::RNK()
{
}

template <typename T1 >
void RNK::InnerCopyConstructor(RNK & t1,  T1 && t2)
{
    t1.mSequence = (mVar*)realloc(t1.mSequence, sizeof(mVar)*t2.mSizeInOwnWord );


    t1.mSizeInOwnWord = t2.mSizeInOwnWord;
    t1.mCountOfNucl = t2.mCountOfNucl;

    for (uint32_t i=0; i< t2.mCountOfNucl; i++)
    {
        t1.operator[](i) = t2.operator[](i).item;
    }
}

template <>
void RNK::InnerCopyConstructor(RNK & t1,  RNK && t2)
{
    if (t1.mSequence == t2.mSequence) return;

    t1.mSequence = (mVar*)realloc(t1.mSequence, sizeof(mVar)*t2.mSizeInOwnWord );


    t1.mSizeInOwnWord = t2.mSizeInOwnWord;
    t1.mCountOfNucl = t2.mCountOfNucl;

    t2.mSizeInOwnWord = 0;
    t2.mCountOfNucl = 0;


    swap(t1.mSequence, t2.mSequence);
    t2.mSequence = nullptr;
}

RNK::RNK(const RNK & r1)
{
    InnerCopyConstructor(*this, r1);
}

template<typename T>
RNK::RNK(T && r1)
{
    InnerCopyConstructor(*this, forward<T>(r1) );
}


RNK::RNK(enum Nucl nucleotide, size_t volumeOfNucl)
{
    size_t nucl_in_word = sizeof(mVar) * 4;
    size_t newSizeInOwnWord = ceil(  (double)volumeOfNucl / nucl_in_word);

    fillSequence(mSequence, nucleotide, 0, newSizeInOwnWord, volumeOfNucl);
}

template<typename T>
void RNK::InnerOperatorSquadBreaks(T && item, size_t itemOfNucl) const
{
    if( (itemOfNucl+1) > mCountOfNucl)
    {

        size_t nucl_in_word = sizeof(mVar) * 4;
        size_t newSizeInOwnWord = ceil((double)(itemOfNucl+1)  / nucl_in_word);

        const_cast<RNK*>(this)->fillSequence(mSequence, U, mSizeInOwnWord, newSizeInOwnWord, (itemOfNucl+1) );
    }
}

template<typename T, typename reT>
reT RNK::InnerOperatorSquadBreaks2(T && item, size_t itemOfNucl)
{
    print_is_same<RNK*, decltype(this)>();

    if( (itemOfNucl+1) > mCountOfNucl)
    {
        size_t nucl_in_word = sizeof(mVar) * 4;
        size_t newSizeInOwnWord = ceil((double)(itemOfNucl+1)  / nucl_in_word);

        const_cast<RNK*>(this)->fillSequence(mSequence, U, mSizeInOwnWord, newSizeInOwnWord, (itemOfNucl+1) );
    }

    return reference(itemOfNucl, item);
}

RNK::reference RNK::operator[](size_t itemOfNucl)
{
    InnerOperatorSquadBreaks(this, itemOfNucl);
    return reference(itemOfNucl, this);

//    return InnerOperatorSquadBreaks2<RNK*, RNK::reference>(this, itemOfNucl);
}

const RNK::reference RNK::operator[](size_t itemOfNucl) const
{
    InnerOperatorSquadBreaks(this, itemOfNucl);
    return reference(itemOfNucl, this);

//    return InnerOperatorSquadBreaks2<const RNK*, const RNK::reference>(this, itemOfNucl);
}

template<typename T1, typename T2>
RNK& operator+(T1 && r1, T2 && r2) {

    if (r1.mSequence == r2.mSequence)
    {
        size_t oldCountNucl = r1.mCountOfNucl;

        r1.mCountOfNucl *= 2;
        r1.mSizeInOwnWord *= 2;

        r1.mSequence = (mVar*)realloc(r1.mSequence, sizeof(mVar)*r1.mSizeInOwnWord );

        for (uint32_t i=0; i< oldCountNucl; i++)
        {
            r1.operator[](oldCountNucl+i) = r1.operator[](i).item;
        }
        return r1;
    }

    size_t newSizeOfNucl = r1.mCountOfNucl + r2.mCountOfNucl;
    size_t newSizeInOwnWord = (newSizeOfNucl-1)/(sizeof(mVar)*4) + 1;

    r1.mSequence = (mVar*)realloc(r1.mSequence, sizeof(mVar)*newSizeInOwnWord );

    size_t oldCountNucl = r1.mCountOfNucl;
    r1.mCountOfNucl = newSizeOfNucl;
    r1.mSizeInOwnWord = newSizeInOwnWord;

    for (uint32_t i=0; i< r2.mCountOfNucl; i++)
    {
        r1.operator[](oldCountNucl+i) = r2.operator[](i).item;
    }

    return r1;
}

template<typename T>
RNK& RNK::InnerOperatorAssignment(RNK & r1, T && r2)
{
    r1.mSequence = (mVar*)realloc(r1.mSequence, sizeof(mVar)*r2.mSizeInOwnWord );

    r1.mSizeInOwnWord = r2.mSizeInOwnWord;
    r1.mCountOfNucl = r2.mCountOfNucl;

    for (uint32_t i=0; i< r2.mCountOfNucl; i++)
    {
        r1.operator[](i) = r2.operator[](i).item;
    }

    return r1;
}

template<typename T1>
RNK& RNK::operator=(T1 && r1)
{
    if (this->mSequence == r1.mSequence)
    {
        return *this;
    }

    return InnerOperatorAssignment(*this, forward<T1>(r1));
}

RNK& RNK::operator=( const RNK & r1)
{
    cout << "hello im: RNK& RNK::operator=( const RNK & r1)" << endl;

    return InnerOperatorAssignment(*this, r1);
}

template<typename T1, typename T2>
bool operator==(T1 && r1, T2 && r2)
{
    return RNK::InnerComprasion(forward<T1>(r1), forward<T2>(r2));
}

template<typename T1, typename T2>
bool operator!=(T1 && r1, T2 && r2)
{
    return !(RNK::InnerComprasion(forward<T1>(r1), forward<T2>(r2)));
}

template <typename T1>
RNK& RNK::InnerNot(T1 && t1)
{
    enum Nucl item_store;
    for(size_t i=0; i<t1.mCountOfNucl/2; i++)
    {
        item_store = t1.operator[](i).item;
        t1.operator[](i) = t1.operator[]((t1.mCountOfNucl-1)-i).item;
        t1.operator[]((t1.mCountOfNucl-1)-i) = item_store;
    }

    return t1;
}

template<typename T1>
RNK& operator!(T1 && r1)
{
    return RNK::InnerNot(forward<T1>(r1));
}


template <typename T1, typename T2>
bool RNK::InnerComprasion(T1 && t1, T2 && t2)
{
    if (t1.mCountOfNucl != t2.mCountOfNucl) return false;

    for (uint32_t i=0; i<t1.mCountOfNucl; i++)
    {
        if (t1.operator[](i).item != t2.operator[](i).item) return false;
    }

    return true;
}



template<typename T, typename T2>
bool RNK::isComplementary(T && item1, T2 && item2)
{
    return RNK::InnerIsComplementary(forward<T>(item1), forward<T2>(item2));
}


template<typename T, typename T2>
bool RNK::InnerIsComplementary(T && item1, T2 && item2)
{
    if (item1.mCountOfNucl != item2.mCountOfNucl) return false;

    for (uint32_t i=0, j = (item1.mCountOfNucl -1); i<item1.mCountOfNucl; i++, j--)
    {
        if (item1.operator[](i).item != item2.operator[](j).item) return false;
    }

    return true;
}


size_t RNK::capacity() const
{
    return this->mSizeInOwnWord;
}
void RNK::trim(size_t lastIndex)
{
    if ( lastIndex > (mCountOfNucl-1) )
    {
        return;
    }
    size_t newSizeOfNucl = lastIndex;
    size_t newSizeInOwnWord = (newSizeOfNucl)/(sizeof(mVar)*4) + 1;


    mCountOfNucl = newSizeOfNucl + 1;
    mSizeInOwnWord = newSizeInOwnWord;

    mSequence = (mVar*)realloc(mSequence, mSizeInOwnWord*sizeof(mVar) );
}


size_t RNK::cardinality( enum Nucl value) const
{
    size_t ret = 0;
    for (size_t i=0; i<this->mCountOfNucl; i++)
    {
        this->operator[](i).item == value ? ret++ : ret;
    }
    return ret;
}

unordered_map<enum Nucl, int, std::hash<int>> RNK::cardinality_map() const
{
    unordered_map<enum Nucl, int, std::hash<int>> ret;

    for (uint16_t i = A; i <= U; i++)
    {
//        cout << " nucl = " << (Nucl)i << " cardinality = " << this->cardinality((Nucl)i) << endl;
        ret.insert(pair((Nucl)i , this->cardinality((Nucl)i)) );
    }
    return ret;
}

void RNK::split(size_t index)
{
    if ( index > (mCountOfNucl-1) )
    {
        return;
    }
    size_t newSizeOfNucl = mCountOfNucl - (index);
    size_t newSizeInOwnWord = (newSizeOfNucl-1)/(sizeof(mVar)*4) + 1;


    for (uint32_t i=0, j = index; i<newSizeOfNucl; i++, j++)
    {
        this->operator[](i) = this->operator[](j).item;
    }


    mCountOfNucl = newSizeOfNucl ;
    mSizeInOwnWord = newSizeInOwnWord;

    mSequence = (mVar*)realloc(mSequence, mSizeInOwnWord*sizeof(mVar) );
}


RNK::~RNK()
{
    free(mSequence);
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(check, test1) {
    RNK testRNK = RNK(G, 32);
    const RNK ctestRNK = RNK(G, 32);



    EXPECT_EQ(testRNK == ctestRNK, true);
    EXPECT_EQ(testRNK == RNK(G, 32), true);
    EXPECT_EQ(testRNK != RNK(G, 31), true);

    testRNK[31] = C;
//    ctestRNK[31] = C; error
    EXPECT_EQ((testRNK[31]).item == C, true);

    EXPECT_EQ(testRNK != ctestRNK, true);
    EXPECT_EQ(ctestRNK != testRNK, true);
    EXPECT_EQ(ctestRNK != move(testRNK), true);
    EXPECT_EQ(move(ctestRNK) != move(testRNK), true);
    EXPECT_EQ(move(ctestRNK) != testRNK, true);
    EXPECT_EQ(testRNK == RNK(G, 32), false);
    EXPECT_EQ(testRNK != RNK(G, 32), true);
    EXPECT_EQ(testRNK != move(RNK(G, 32)), true);
}

TEST(check, test2) {
    RNK testRNK = RNK(G, 32);
    const RNK ctestRNK = RNK(G, 32);
    RNK copyRNK = RNK(testRNK);
    RNK copyRNK2 = RNK(ctestRNK);
    RNK copyRNK3 = move(testRNK);

    EXPECT_EQ(copyRNK == RNK(G, 32), true);
    EXPECT_EQ(copyRNK2 == RNK(G, 32), true);
    EXPECT_EQ(copyRNK3 == RNK(G, 32), true);
}

TEST(check, test3) {
    RNK testRNK = RNK(G, 32);
    RNK testRNK2 = RNK(G, 32);

    const RNK ctestRNK = RNK(G, 32);
    RNK sumRNK = testRNK + testRNK;
    RNK sumRNK2 = RNK(G, 32) + testRNK2;
    RNK sumRNK3 = testRNK2 + RNK(G, 32);


//    ctestRNK + RNK(G, 32);  error

    EXPECT_EQ(sumRNK == RNK(G, 64), true);
    EXPECT_EQ(sumRNK2 == RNK(G, 64), true);
    EXPECT_EQ(sumRNK3 == RNK(G, 64), true);

    RNK sumRNK4 = RNK();
    sumRNK4 = move(sumRNK3) + ctestRNK;
    EXPECT_EQ(sumRNK4 == RNK(G, 96), true);
}

TEST(check, test4) {
    RNK testRNK = RNK(G, 32);
    RNK testRNK2 = RNK(G, 32);

    RNK testRNK3 = RNK(G, 32);
    const RNK testRNK4 = RNK(G, 32);
    testRNK3[31] = A;

    testRNK.trim(15);
    EXPECT_EQ(testRNK.cardinality(G) == 16, true);
    EXPECT_EQ(testRNK == RNK(G, 16), true);
    EXPECT_EQ(testRNK == RNK(G, 17), false);

    testRNK2.split(1);
    EXPECT_EQ(testRNK2.cardinality(G) == 31, true);
    EXPECT_EQ(testRNK2 == RNK(G, 31), true);
    EXPECT_EQ(testRNK2 == RNK(G, 32), false);

    testRNK3 = !testRNK3;
//    !testRNK4 error
    EXPECT_EQ((testRNK3)[0].item == A, true);
    EXPECT_EQ(testRNK3.cardinality(G) == 31, true);
    EXPECT_EQ(testRNK3.cardinality(A) == 1, true);


    RNK testRNK5 = RNK(G, 32);
    testRNK5[1] = A;
    testRNK5[2] = C;
    testRNK5[3] = C;
    testRNK5[4] = U;
    testRNK5[5] = U;
    testRNK5[6] = U;
    testRNK5[7] = U;
    unordered_map<enum Nucl, int, std::hash<int>> mmap = testRNK5.cardinality_map();


    EXPECT_EQ(mmap[A], 1);
    EXPECT_EQ(mmap[C], 2);
    EXPECT_EQ(mmap[U], 4);
    EXPECT_EQ(mmap[G], 25);


    
    RNK RNK1 = RNK(G, 32);
    RNK RNK2 = RNK(G, 32);

    RNK1[1] = A;
    RNK2[30] = A;

    EXPECT_EQ(RNK::isComplementary(RNK1, RNK2), true);
}

TEST(check, test5) {
    size_t n = 10000000;
    RNK mrnk = RNK();

    for(size_t i=0; i<n; i++){
        mrnk[i] = C;
    }

    EXPECT_EQ(mrnk.cardinality(C) == n, true);
}

/*
int main() {


    RNK testRNK = RNK(G, 32);
//    testRNK.printFullSequence();


    const RNK testRNK2 = RNK(G, 32);
//    testRNK2.printFullSequence();


    testRNK[1] = U;
    testRNK.printFullSequence();
    cout << endl;
    (!testRNK).printFullSequence();

//    cout << " not comprasion = " << (testRNK != testRNK2) << endl;

//    testRNK[2] = A;
//    cout << " cardinality = " << testRNK.cardinality(G) << endl;
//    testRNK2.cardinality_map();

//    testRNK = testRNK;
//    testRNK.printFullSequence();

//    RNK copyRNK = RNK(testRNK);
//    copyRNK.printFullSequence();
//    RNK copyRNK2 = testRNK2;
//    copyRNK2.printFullSequence();
//    RNK copyRNK3 = std::move(testRNK);
//    copyRNK3.printFullSequence();



//    copyRNK.printFullSequence();

//    printf ("\r\n isCompl = %i \r\n" ,RNK::isComplementary(testRNK, RNK(G, 32)) );
//
//    RNK testRNK3 = testRNK;
//    RNK testRNK4 = testRNK2;
//
//    testRNK4 = RNK(G, 32);


//    testRNK + testRNK2;
//    testRNK2 + testRNK ;
//    testRNK + RNK(G, 32);
//    RNK(G, 32) + testRNK;


//    RNK tmp;
//    tmp = testRNK2;
//    tmp = testRNK;
//    tmp = RNK(U, 32);
//    (RNK(G, 32) = testRNK).printFullSequence();
//    (RNK(G, 1) = testRNK2).printFullSequence();
//    (RNK(G, 32) = RNK(U, 32)).printFullSequence();
//    tmp.printFullSequence();
//
//    testRNK[20] = A;

//    if (testRNK == RNK(G, 44))
//    if ( testRNK2 == testRNK)
//    if ( RNK(G, 32) == testRNK)
//    if ( RNK(C, 32) == testRNK2)
//    if ( testRNK2 == RNK(C, 32))
//    {
//        cout << " comprasion true" << endl;
//    }
//    else
//    {
//        cout << " comprasion false" << endl;
//    }


//    testRNK.trim(32);


//    testRNK.printFullSequence();

//    cout << " item = " <<  testRNK[128] << endl;
//    testRNK.printFullSequence();


//    testRNK.split(32);
//    testRNK.printFullSequence();

    return 0;
}
*/