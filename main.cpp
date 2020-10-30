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

//        printf("\r\n shift = %i \r\n", shift);
//        cout << "col_bytes_in_word = " << col_bytes_in_word << " mSizeInOwnWord = " << mSizeInOwnWord << endl;

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
//        printf("\r\n const A = %i const B = %i  const shift = %i \r\n", constants.A, constants.B, constants.shift);

    uint8_t uelem = ((uint8_t*)&mSequence[constants.A])[constants.B];


    uint8_t ret = ( (uelem >> constants.shift) & 0b11 );
    enum Nucl rett = (enum Nucl)ret;


//        cout << "element = " << std::bitset<sizeof(uelem) * CHAR_BIT>(uelem) << endl;
//        cout << "result = " << std::bitset<sizeof(ret) * CHAR_BIT>(ret) << endl;
//        printf("\r\n RNK = %i \r\n", rett);
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
//        printf("\r\n SET const A = %i const B = %i  const shift = %i \r\n", constants.A, constants.B, constants.shift);


    uint8_t uelem = ((uint8_t*)&mSequence[constants.A])[constants.B];

    int8_t resultByte = (~(0b11 << constants.shift) & uelem) | (nucleotide << constants.shift);

    ((uint8_t*)&mSequence[constants.A])[constants.B] = resultByte;


//        cout << "element = " << std::bitset<sizeof(uelem) * CHAR_BIT>(uelem) << endl;
//        cout << "selMask = " << std::bitset<sizeof(resultByte) * CHAR_BIT>(resultByte) << endl;
//        cout << "rev selMask = " << std::bitset<sizeof(reversebits<uint8_t>( resultByte)) * CHAR_BIT>(reversebits<uint8_t>( resultByte)) << endl;

//        printFullSequence();
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
//            cout << " countOfFreeBits = " << countOfFreeBits << endl;

        if (countOfFreeBits != 0)
        {
            size_t newCountOfNuclInBytes = newSizeOfNucl / 4;
            countOfBytes = newCountOfNuclInBytes;

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
//        printFullSequence();
}

RNK::RNK()
{
    cout << "im default constructor" << endl;
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

    cout << "im universal copy constructor" << endl;
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
    cout << "im constructor" << endl;

    size_t nucl_in_word = sizeof(mVar) * 4;
    size_t newSizeInOwnWord = ceil(  (double)volumeOfNucl / nucl_in_word);

//        cout << " newSizeInOwnWord = " << newSizeInOwnWord << endl;

    fillSequence(mSequence, nucleotide, 0, newSizeInOwnWord, volumeOfNucl);
}

template<typename T>
void RNK::InnerOperatorSquadBreaks(T && item, size_t itemOfNucl) const
{

//    print_is_same<RNK*, decltype(this)>();

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

    size_t newSizeOfNucl = r1.mCountOfNucl + r2.mCountOfNucl;
    size_t newSizeInOwnWord = (newSizeOfNucl-1)/(sizeof(mVar)*4) + 1;

//    printf("\r\n this mCountOfNucl = %li, mCountOfNucl = %li \r\n newSizeOfNucl = %li newSizeInOwnWord = %li \r\n", this->mCountOfNucl, r1.mCountOfNucl, newSizeOfNucl, newSizeInOwnWord);

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
    cout << "hello im: InnerOperatorAssignment" << endl;


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
    cout << "hello im: template<typename T> RNK& RNK::operator=(T && r1)" << endl;

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


template <typename T1, typename T2>
bool RNK::InnerComprasion(T1 && t1, T2 && t2)
{
    if (t1.mCountOfNucl != t2.mCountOfNucl) return false;

    for (uint32_t i=0; i<t1.mCountOfNucl; i++)
    {
        if (t1.operator[](i).item != t2.operator[](i).item) return false;
    }

//    std::cout << "InnerIsComplementary ref = " << std::is_reference<T>::value << '\n';
    return true;

}

template<typename T, typename T2>
bool RNK::isComplementary(T && item1, T2 && item2)
{
    std::cout << "isComplementary ref = " << std::is_reference<T>::value << '\n';
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

//    std::cout << "InnerIsComplementary ref = " << std::is_reference<T>::value << '\n';
    return true;
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

//    printf("\r\n mCountOfNucl = %li  mSizeInOwnWord = %li newSizeInOwnWord = %li \r\n", mCountOfNucl , mSizeInOwnWord, newSizeInOwnWord);
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


//    printf("\r\n newSizeOfNucl = %li newSizeInOwnWord = %li \r\n ", newSizeOfNucl, newSizeInOwnWord);

}


RNK::~RNK()
{
//    cout << " hello im destructor" << endl;
    free(mSequence);
}


int main() {


    RNK testRNK = RNK(G, 32);
    testRNK.printFullSequence();


    const RNK testRNK2 = RNK(C, 32);
    testRNK2.printFullSequence();


    testRNK = testRNK;
    testRNK.printFullSequence();

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
////    testRNK2 + testRNK ;
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


    /*
    testRNK[126] = A;
    testRNK[127] = A;
    testRNK[128] = A;
    testRNK.printFullSequence();

    testRNK[129] = A;
    testRNK.printFullSequence();
*/

/*
    RNK testRNK1 = RNK(A, 4);
    RNK testRNK2 = RNK(A, 4);

    testRNK1[0] = A;
    testRNK1[1] = C;
    testRNK1[2] = G;
    testRNK1[3] = U;

    testRNK2[3] = A;
    testRNK2[2] = C;
    testRNK2[1] = G;
    testRNK2[0] = U;


    testRNK1.printFullSequence();
    testRNK2.printFullSequence();

*/

    return 0;
}
