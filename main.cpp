#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>

using namespace std;


//cell
//generation
//universe

class mCell
{
    int value;

public:
    mCell()
    {
        value = 0;
    }
    mCell& operator++ (int)
    {
        value++;
        return *this;
    }

    int getvalue() const
    {
        return value;
    }

};

class mGeneration
{
    unordered_map<int, mCell> space;
public:
    mGeneration()
    {

    }
};

int main() {

    mCell mc;

    mc++;
    mc++;
    cout << " i = " << mc.getvalue() << endl;


    return 0;
}